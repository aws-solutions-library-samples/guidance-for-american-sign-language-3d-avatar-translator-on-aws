/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: MIT-0
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// Provides utility functions that wrap the UE and Windows API's
//

#include "UnrealAPI.h"
#include "Config/GlobalState.h"
#include "Config/InternalSettings.h"

// Note: Windows API-specific and ordering sensitive
//
#include <Windows/PreWindowsApi.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <Windows/PostWindowsApi.h>

#include <Components/SkeletalMeshComponent.h>
#include <Engine.h>
#include <Engine/UserInterfaceSettings.h>
#include <SlateBasics.h>

using ASLMetaHuman::Config::FGlobalState;
using ASLMetaHuman::Config::FInternalSettings;
using ASLMetaHuman::Utilities::UnrealAPI;

namespace {
constexpr int32 HighZOrder = 9999;
constexpr float RenderThreadSpinlockSeconds = 0.05;
constexpr bool WantNoMessages = false;
}

/* The following methods are Windows-specific
 */

// Performance: useful to periodically call this to avoid private memory overallocation (Windows API-specific)
//
void UnrealAPI::ClearMemory() {
    ::EmptyWorkingSet(GetCurrentProcess());
}

// Returns the name of the current process (this binary)
//
void UnrealAPI::GetModuleName(FString & ModuleName) {
    wchar_t ModuleNameInternal[MAX_PATH];
    GetModuleBaseNameW(GetCurrentProcess(), nullptr, ModuleNameInternal, MAX_PATH);
    ModuleName = ModuleNameInternal;
}

// Returns true if a process by name (ProcessName) has multiple instances running; false otherwise
//
bool UnrealAPI::IsProcessRunningMultipleTimes(const FString & ProcessName) {
    const wchar_t * ProcessNameInternal = *ProcessName;
    unsigned int Count = 0;
    PROCESSENTRY32W ProcessEntry;
    ProcessEntry.dwSize = sizeof(PROCESSENTRY32W);
    auto * const Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32FirstW(Snapshot, &ProcessEntry))
        while (Process32NextW(Snapshot, &ProcessEntry))
            if (_wcsicmp(ProcessEntry.szExeFile, ProcessNameInternal) == 0) {
                Count++;
            }
    CloseHandle(Snapshot);
    return Count > 1;
}

// Returns the amount of occurrences of Character in InString
//
unsigned int UnrealAPI::CountCharOccurrences(const FString & InString, const char Character) {
    return std::ranges::count(std::string(TCHAR_TO_UTF8(*InString)), Character);
}

// Converts UTexture2DDynamic into UTexture2D (static texture), making it usable for UMaterial objects.
// Reference: https://forums.unrealengine.com/t/how-do-i-convert-a-utexture2ddynamic-to-utexture2d/404375/2
// Note: this approach could be made more efficient by using other mechanisms (since the current approach 
// could incur significant time slice from the Game Thread, which would produce cause delays)
//
void UnrealAPI::ConvertTexture(const UTexture2DDynamic * DynamicTexture, TWeakObjectPtr<UTexture2D> & StaticTexture) {
    StaticTexture = UTexture2D::CreateTransient(DynamicTexture->SizeX, DynamicTexture->SizeY, DynamicTexture->Format);
    StaticTexture->AddToRoot();
    bool RenderDataComplete = false;
    // Exclusive access to RHI resource
    //
    const auto Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&StaticTexture, DynamicTexture, &RenderDataComplete]() {
                ENQUEUE_RENDER_COMMAND(FConvertTextureCommand)
                ([&DynamicTexture, &StaticTexture, &RenderDataComplete](FRHICommandListImmediate & RHICmdList) {
                    RHICmdList.BlockUntilGPUIdle();
                    // Get the texture data from DynamicTexture in a synchronized fashion and copy it into StaticTexture
                    //
                    const auto TextureResource = const_cast<FTextureResource *>(DynamicTexture->GetResource());
                    uint32 DestStride {0};
                    const FColor * SrcDataPtr = static_cast<FColor *>(RHILockTexture2D(
                            TextureResource->GetTexture2DRHI(), 0, EResourceLockMode::RLM_ReadOnly, DestStride, false));
                    FColor * DestDataPtr = static_cast<FColor *>(
                            StaticTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
                    // Warning: there are concerns about texture data truncation/overflow in large payloads (which may be
                    // connected with the incoming data itself and how it's being handled in the core UE code)
                    // Review the SIZE_T and int32 storage.
                    //
                    FMemory::Memcpy(
                            DestDataPtr, SrcDataPtr, DynamicTexture->SizeX * DynamicTexture->SizeY * sizeof(FColor));
                    StaticTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
                    RHIUnlockTexture2D(TextureResource->GetTexture2DRHI(), 0, false);
                    RenderDataComplete = true;
                });
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    Task->Wait();
    while (! RenderDataComplete) {
        FPlatformProcess::Sleep(RenderThreadSpinlockSeconds);
    }
    // Release access to RHI resource and make the static texture be usable in UE
    //
    const auto TaskUpdate = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&StaticTexture]() {
                FlushRenderingCommands();
                StaticTexture->UpdateResource();
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    TaskUpdate->Wait();
}

// Wrapper for downloading a texture
// Accepts a signed URL that refers to the texture (image file); if download is successful, asynchronously
// invokes ExternalDownloadTextureDelegate and provides it a dynamic texture
//
void UnrealAPI::DownloadTexture(const FString & SignedUrl,
        const TDelegate<void(const UTexture2DDynamic *)> & ExternalDownloadTextureDelegate) {
    if (! UUnrealAPIInstance->IsValidLowLevelFast()) {
        UUnrealAPIInstance = NewObject<UUnrealAPI>();
    }
    if (! UUnrealAPIInstance->IsValidLowLevelFast()) {
        return;
    }
    UUnrealAPIInstance->DownloadTexture(SignedUrl, ExternalDownloadTextureDelegate);
}

// Provides access to a material given its path. Returns true if that material can be found; false otherwise.
//
bool UnrealAPI::GetMaterial(const FString & MaterialPath, TWeakObjectPtr<UMaterialInterface> & Material) {
    Material = Cast<UMaterialInterface>(
            StaticLoadObject(UMaterial::StaticClass(), nullptr, *MaterialPath, nullptr, LOAD_None, nullptr));
    return nullptr != Material;
}

// Provides access to only one of an Actor's specific Skeletal Mesh Components (requested by its name).
// Returns true if there's a matching Skeletal Mesh Component (by name); false otherwise.
// Note: Skeletal Mesh Components are read-only (r/o) in terms of making changes to their transforms.
//
bool UnrealAPI::GetSkeletalMeshComponent(const FString & SkeletalMeshName,
        const AActor * Actor,
        TWeakObjectPtr<USkeletalMeshComponent> & SkeletalMeshComponentPtr) {
    if ((nullptr == Actor) || (! IsValid(Actor))) {
        return false;
    }
    TArray<USkeletalMeshComponent *> SkeletalMeshComponents;
    Actor->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);
    for (const auto SkeletalMeshComponent: SkeletalMeshComponents) {
        const auto SkeletalMesh = SkeletalMeshComponent->GetSkeletalMeshAsset();
        if (nullptr != SkeletalMesh) {
            const auto SkeletonPtr = SkeletalMesh->GetSkeleton();
            // Ensure correct values populated for the transform
            //
            SkeletonPtr->UpdateReferencePoseFromMesh(SkeletalMesh);
            const auto CurrentName = SkeletalMesh->GetName();
            if (CurrentName == SkeletalMeshName) {
                SkeletalMeshComponentPtr = SkeletalMeshComponent;
                return true;
            }
        }
    }
    return false;
}

// Returns the viewport size, factoring in whether it's DPI-scaled
// Note: viewport size!=resolution; viewport size => screen space that the game occupies
//
void UnrealAPI::GetViewportSize(FVector2D & ViewportSize, bool UseDPIScale) {
    TWeakObjectPtr<UWorld> WorldPtr;
    if (! GetWorld(WorldPtr)) {
        return;
    }
    ViewportSize = WorldPtr->GetGameViewport()->Viewport->GetSizeXY();
    if (UseDPIScale) {
        const float DPIScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())
                                       ->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
        if (0.0f < DPIScale) {
            ViewportSize.X = FGenericPlatformMath::FloorToInt(static_cast<float>(ViewportSize.X) / DPIScale);
            ViewportSize.Y = FGenericPlatformMath::FloorToInt(static_cast<float>(ViewportSize.Y) / DPIScale);
        }
    }
}

// Provides access to the active world context. Returns whether that context is valid/determinable.
//
bool UnrealAPI::GetWorld(TWeakObjectPtr<UWorld> & WorldPtr) {
    if (nullptr == GEngine) {
        return false;
    }
    UWorld * World = GEngine->GetWorldContexts()[0].World();
    if ((nullptr == World) || (! World->IsValidLowLevelFast())) {
        return false;
    }
    WorldPtr = World;
    return true;
}

// Attempts to play an animation sequence for a skeletal mesh component at a speed (Rate) and start time (Start)
//
void UnrealAPI::PlayAnimation(USkeletalMeshComponent & SkeletalMeshComponent,
        UAnimSequence & AnimSequence,
        const float Rate,
        const float Start) {
    FScopeLock ScopeLock(&MutexPlayAnimation);
    SkeletalMeshComponent.Stop();
    SkeletalMeshComponent.PlayAnimation(&AnimSequence, false);
    // Note: non-obvious - have to apply these settings after PlayAnimation() in order for them to take effect.
    //
    SkeletalMeshComponent.SetPlayRate(Rate);
    SkeletalMeshComponent.SetPosition(Start);
}

// Configure first player camera - assign to a specific position (LocationOffset) at a specific rotation
// (RotationOffset) in world space, and apply a specific angle for field of view (FieldOfView). Returns ture if this
// operation succeeds; false otherwise.
//
bool UnrealAPI::SetFirstPlayerCameraView(const float FieldOfView,
        const FVector & LocationOffset,
        const FVector & RotationOffset) {
    TWeakObjectPtr<UWorld> WorldPtr;
    if (! GetWorld(WorldPtr)) {
        return false;
    }
    const auto FirstPlayerController = WorldPtr->GetFirstPlayerController();
    if (nullptr == FirstPlayerController) {
        return false;
    }
    const auto FirstPlayerCameraManager = FirstPlayerController->PlayerCameraManager;
    if (nullptr == FirstPlayerCameraManager) {
        return false;
    }
    FirstPlayerCameraManager->SetFOV(FieldOfView);
    const FVector & CurrentLocation = FirstPlayerController->K2_GetActorLocation();
    FirstPlayerController->ClientSetLocation(CurrentLocation + LocationOffset, FRotator::MakeFromEuler(RotationOffset));
    return true;
}

/* The following methods provide support for on screen messages in release mode (similar to GEngine->AddOnScreenDebugMessage())
*/

// Helper method to support a mechanism (TriggerHideMessage) that will hide a text block when the provided
// time period expires (or prior if TriggerHideMessage changes its reference/signal value to true).
//
void UnrealAPI::PerformTimedMessageDisappearance(const float DurationSeconds,
        FThreadSafeBool & TriggerHideMessage,
        TSharedRef<STextBlock> & TextBlock) {
    FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&TriggerHideMessage, DurationSeconds, TextBlock]() {
                for (float i = 0.0f; i < DurationSeconds; i += FInternalSettings::GetAnimationSpinlockSeconds()) {
                    if (FGlobalState::IsAborting()) {
                        return;
                    }
                    // Note: this addresses cases where messages are hidden when TriggerHideMessage is out of scope
                    //
                    if (DurationSeconds == FLT_MAX && TriggerHideMessage) {
                        break;
                    }
                    FPlatformProcess::Sleep(FInternalSettings::GetAnimationSpinlockSeconds());
                }
                HideWidget<STextBlock>(TextBlock);
            },
            TStatId(), nullptr, ENamedThreads::AnyThread);
}

// Helper method to produce a STextBlock Widget using the provided parameters, which include
// font object support, font size, location to display to widget, font color (text and highlighting).
//
void UnrealAPI::MakeTextBlock(const FString & Message,
        const UFont * FontPtr,
        const unsigned int FontSize,
        const FVector2D & Location,
        const FColor & TextColor,
        const FColor & TextHighlightColor,
        TSharedPtr<STextBlock> & TextBlock) {
    const auto SlateFontPtr = new FSlateFontInfo(FontPtr, FontSize);
    const auto TextStyle = new FTextBlockStyle();
    TextStyle->SetColorAndOpacity(FLinearColor(TextColor));
    TextStyle->SetHighlightColor(FLinearColor(TextHighlightColor));
    TextStyle->SetFont(*SlateFontPtr);
    SAssignNew(TextBlock, STextBlock);
    FSlateRenderTransform RenderTransform;
    RenderTransform.SetTranslation(UE::Math::TVector2<float>(Location.X, Location.Y));
    TextBlock->SetRenderTransform(RenderTransform);
    TextBlock->SetText(FText::FromString(Message));
    TextBlock->SetTextStyle(TextStyle);
}

// Wrapper Method
//
void UnrealAPI::ShowMessage(const FString & Message,
        const float DurationSeconds,
        const UFont * FontPtr,
        const unsigned int FontSize,
        const FVector2D & Location,
        const FColor & TextColor,
        const FColor & TextHighlightColor) {        
    FThreadSafeBool TriggerHide = false;
    ShowMessage(Message, DurationSeconds, FontPtr, FontSize, Location, TriggerHide, TextColor, TextHighlightColor);
}

// Displays a visual message on screen for a duration of time (no limit for FLT_MAX) using a provided
// font and related parameters (including: font size, XY 2D canvas location, text color and highlight color).
// Includes a mechanism (TriggerHideMessage) to hide its corresponding message no matter how long that message
// was specified to be shown. This is useful when the time to display a message is not known in advance.
// Accepts calls from any thread. Similar in function to: GEngine->AddOnScreenDebugMessage().
//
void UnrealAPI::ShowMessage(const FString & Message,
        const float DurationSeconds,
        const UFont * FontPtr,
        const unsigned int FontSize,
        const FVector2D & Location,
        FThreadSafeBool & TriggerHideMessage,
        const FColor & TextColor,
        const FColor & TextHighlightColor) {
    if (WantNoMessages || Message.IsEmpty() || (nullptr == FontPtr)) {
        return;
    }
    FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&TriggerHideMessage, Message, DurationSeconds, FontPtr, FontSize, TextColor, TextHighlightColor,
                    Location]() {
                TWeakObjectPtr<UWorld> WorldPtr;
                if (FGlobalState::IsAborting() || (! GetWorld(WorldPtr))) {
                    return;
                }
                TSharedPtr<STextBlock> TextBlockShared;
                MakeTextBlock(Message, FontPtr, FontSize, Location, TextColor, TextHighlightColor, TextBlockShared);
                auto TextBlock = TextBlockShared.ToSharedRef();
                const auto ViewportClient = WorldPtr->GetGameViewport();
                if (nullptr == ViewportClient) {
                    return;
                }
                ViewportClient->AddViewportWidgetForPlayer(
                        WorldPtr->GetFirstLocalPlayerFromController(), TextBlock, HighZOrder);
                PerformTimedMessageDisappearance(DurationSeconds, TriggerHideMessage, TextBlock);
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
}