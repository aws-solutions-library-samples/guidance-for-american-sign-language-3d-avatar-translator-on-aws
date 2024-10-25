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
#pragma once
#include "Utilities/UUnrealAPI.h"

namespace ASLMetaHuman::Utilities {

class UnrealAPI {
private:
    UnrealAPI() = default;
    template <class TypeOfWidget>
    static void HideWidget(const TSharedRef<TypeOfWidget> & Widget);
    static void MakeTextBlock(const FString & Message,
            const UFont * FontPtr,
            unsigned FontSize,
            const FVector2D & Location,
            const FColor & TextColor,
            const FColor & TextHighlightColor,
            TSharedPtr<STextBlock> & TextBlock);
    static void PerformTimedMessageDisappearance(const float DurationSeconds,
            bool & TriggerHideMessage,
            TSharedRef<STextBlock> & TextBlock);
    // Assist with UClass-specific utility method access
    //
    static inline TWeakObjectPtr<UUnrealAPI> UUnrealAPIInstance = nullptr;

public:
    template <class TypeOfActor = AActor>
    static bool GetActorByName(const FString & ActorName, const UWorld * World, TWeakObjectPtr<TypeOfActor> & ActorPtr);
    template <class TypeOfAsset>
    static bool GetAsset(const FString & PathToAsset, TWeakObjectPtr<TypeOfAsset> & AssetPtr);
    template <class TypeOfAsset>
    static bool GetAssets(const FString & PathToAsset, TArray<TWeakObjectPtr<TypeOfAsset>> & AssetsPtr);
    template <class TypeOfActor = AActor>
    static bool GetActorByPath(const FString & ObjectPathToActor,
            const UWorld * World,
            TWeakObjectPtr<TypeOfActor> & ActorPtr);
    static FString AwsStringToFString(const Aws::String & AwsString) {
        return FString(UTF8_TO_TCHAR(AwsString.c_str()));
    }
    static Aws::String FStringToAwsString(const FString & InStr) {
        return TCHAR_TO_UTF8(*InStr);
    }
    static unsigned int CountCharOccurrences(const FString & InString, const char Character);
    static void ClearMemory();
    static void ConvertTexture(const UTexture2DDynamic * DynamicTexture, TWeakObjectPtr<UTexture2D> & StaticTexture);
    static bool GetMaterial(const FString & MaterialPath, TWeakObjectPtr<UMaterialInterface> & Material);
    static void DownloadTexture(const FString & SignedUrl,
            const TDelegate<void(const UTexture2DDynamic *)> & ExternalDownloadTextureDelegate);
    static void GetModuleName(FString & ModuleName);
    static bool GetSkeletalMeshComponent(const FString & SkeletalMeshName,
            const AActor * ActorPtr,
            TWeakObjectPtr<USkeletalMeshComponent> & SkeletalMeshComponentPtr);
    static void GetViewportSize(FVector2D & ViewportSize, bool UseDPIScale);
    static bool GetWorld(TWeakObjectPtr<UWorld> & WorldPtr);
    template <class TypeOfActor>
    static void HideAllActors(const UWorld * World, const bool BlueprintsOnly);
    static bool IsProcessRunningMultipleTimes(const FString & ProcessName);
    static void PlayAnimation(USkeletalMeshComponent & SkeletalMeshComponent,
            UAnimSequence & AnimSequence,
            const float Rate,
            const float Start);
    static bool SetFirstPlayerCameraView(const float FieldOfView,
            const FVector & LocationOffset,
            const FVector & RotationOffset);
    static void ShowMessage(const FString & Message,
            const float DurationSeconds,
            const UFont * FontPtr,
            const unsigned int FontSize,
            const FVector2D & Location,
            const FColor & TextColor = FColor::White,
            const FColor & TextHighlightColor = FColor::Black);
    static void ShowMessage(const FString & Message,
            const float DurationSeconds,
            const UFont * FontPtr,
            const unsigned int FontSize,
            const FVector2D & Location,
            bool & TriggerHideMessage,
            const FColor & TextColor = FColor::White,
            const FColor & TextHighlightColor = FColor::Black);
    static inline FCriticalSection MutexPlayAnimation;
};
}

#include "UnrealAPI.tpp"