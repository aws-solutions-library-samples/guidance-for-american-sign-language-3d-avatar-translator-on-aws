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

// This implementation drives the main ASL Demo workflow
//

#include "ASLMetaHumanDemo.h"
#include "ASLAlgorithms.h"
#include "ASLMetaHumanAction.h"
#include "ASLMetaHumanSentenceAction.h"
#include "Config/GlobalState.h"
#include "Config/InternalSettings.h"
#include "Config/UISettings.h"
#include "Config/UserSettings.h"
#include "Utilities/UnrealAPI.h"

#include <Animation/AnimSequence.h>
#include <Components/SkyAtmosphereComponent.h>
#include <Kismet/KismetMathLibrary.h>

using ASLMetaHuman::Config::FGlobalState;
using ASLMetaHuman::Config::FInternalSettings;
using ASLMetaHuman::Config::FUISettings;
using ASLMetaHuman::Config::FUserSettings;
using ASLMetaHuman::Core::ASLAlgorithms;
using ASLMetaHuman::Core::ASLMetaHumanAction;
using ASLMetaHuman::Core::ASLMetaHumanAnimateSentenceAction;
using ASLMetaHuman::Core::ASLMetaHumanDemo;
using ASLMetaHuman::Utilities::UnrealAPI;

namespace {
// Blueprint Actor-specific naming convention (for MetaHuman)
// Provides access to Actor's Skeletal Mesh Components
// Notice the (compiled) BP paths ending in _C
//
const auto BPActorObjectPathFormat {TEXT("Blueprint'/Game/MetaHumans/{0}/BP_{0}.BP_{0}_C'")};
// Materials related to the background static plane
// Consider exposing/configuring these (hardcoded) Content paths into an outside configuration file (ASLMetaHuman.ini)
//
const auto BackgroundMaterialPath {TEXT("Material'/Game/Custom/BackgroundMaterial.BackgroundMaterial'")};
const auto DefaultBackgroundMaterialPath {TEXT("/Material'/Game/Custom/AWS-reInvent-Logo_Mat.AWS-reInvent-Logo_Mat'")};
// Animation sequence path (from Content directory)
//
const auto ASLAnimationPath {TEXT("/Game/ASL_Animations")};
// Filename convention for animation sequences, where the ASL Sign word(s) follow an underscore and are separated by
// underscores
//
const auto AnimationNamePrefix {TEXT("Anim_")};
constexpr auto AnimationNameWordDelimiter {'_'};
// Important: links to a material's texture's parameter name, which needs to match in order to change that texture!
//
const auto TextureImageParameterName {FName("Image")};
// Timing specific settings
//
constexpr int SQSShutdownWaitTimeSeconds {2};
constexpr float UpdateMessageDurationSeconds {2.0f};
constexpr float RunTestActionDelaySeconds {2.0f};
// Conventions for distinguishing similar signs (can extend more broadly)
//
const FString & LetterIAsSubject {"I"};
const FString & LetterIAsAlphabetSymbol {"_I"};
const FString & AnimationNameWordDelimiterStr {"_"};
const FString & AnimationNameWordSpaceStr {" "};
// Consider changing plane names (for HUD and background plane) to more meaningful identifiers
// Warning: ensure that these objects exist, else initialization will fail - check the scan of scene objects.
//
const FString & BackgroundPlaneName {"Plane_2"};
// Action Update HUD Messages
//
const FString & AnimateSentenceMessage {"ANIMATE SENTENCE"};
const FString & ChangingAvatarMessage {"CHANGING AVATAR"};
const FString & ChangingBackgroundMessage {"CHANGING BACKGROUND"};
const FString & ChangingSignRateMessage {"CHANGING SIGN RATE"};
const FString & StoppingAnimationMessage {"STOPPING ANIMATION"};
// Sentence-related HUD Status Messages
//
const auto SentenceOutputFormat {TEXT("GenAI simplification: {0}")};
const auto ASLOutputFormat {TEXT("GenAI ASL: {0}")};
const auto SentimentPromptMessageFormat {TEXT("Sentiment: {0}")};
const FString & NegativeSentimentMessage {"negative :/"};
const FString & PositiveSentimentMessage {"positive ^_^"};
const FString & MixedSentimentMessage {"mixed (o-o)"};
const FString & NeutralSentimentMessage {"neutral(-)"};
const FString & ShockedSentimentMessage {"shocked (!)"};
// UI settings - note: these could migrate into a config file (ASLMetaHuman.ini)
//
constexpr int SentimentHorizontalLocation = 50;
constexpr int SentimentFontSize = 40;
constexpr float StatusHorizontalProportion = 0.75;
constexpr int StatusVerticalOffset = -50;
}

// Early initialization: note: should only have one instance of this demo active!
// - Populate animation sequences for ASL signs into cache
// - Setup SQS request background worker thread
//
bool ASLMetaHumanDemo::Init() {
    if ((! InitInternalUEObjectReferences()) || (! InitUEObjectsAndEnvironment())) {
        return false;
    }
    UnrealAPI::GetViewportSize(ViewportSize, true);
    DisplayVersion();
    InitAnimations();
    if (! FInternalSettings::GetIgnoreSQS()) {
        InitSQSBackgroundWorker();
    }
    if (FInternalSettings::GetOnlySignFixedText()) {
        // Self-test mode: animate a fixed phrase (decoupled from AWS)
        //
        AnimateSentence(FInternalSettings::GetFixedTextToSign(), FInternalSettings::GetFixedTextToSign());
    }
    return true;
}

// Supports pre-program exit cleanup process
//
void ASLMetaHumanDemo::Shutdown() {
    // This will trigger a thread-safe abort that's periodically checked elsewhere (see IsAborting() call sites)
    //
    FGlobalState::Abort();
    if (IsInitialized) {
        if (nullptr != SkeletalMeshBodyComponentInternalPtr) {
            // Stop playing the ongoing animation (if any)
            //
            SkeletalMeshBodyComponentInternalPtr->Stop();
        }
        // Shutdown SQS (even if blocked)
        //
        if (nullptr != SQSWorkerTaskPtr) {
            SQSWorkerTaskPtr->TryAbandonTask();
            SQSWorkerTaskPtr->WaitCompletionWithTimeout(SQSShutdownWaitTimeSeconds);
            SQSWorkerTaskPtr.Reset();
        }
        if (nullptr != DemoInstancePtr) {
            FPlatformProcess::Sleep(FInternalSettings::GetAnimationSpinlockSeconds());    
            DemoInstancePtr.Reset();
        }
    }
}

// Sets up the relationships between known ASL signs/tokens and their animation sequences (including play duration),
// and the number of words that they represent
//
void ASLMetaHumanDemo::InitAnimations() {
    AnimationSequenceByTokenMap = new TMap<FString, TWeakObjectPtr<UAnimSequence>>();
    AnimationSequenceLengthByTokenMap = new TMap<FString, float>();
    TranslatableTokensByWordCount = new TMap<unsigned int, TArray<FString>>();
    InitAnimationSequences(ASLAnimationPath);
}

// Sets up a background thread to check for (and act on) SQS requests - including sentence animation.
//
void ASLMetaHumanDemo::InitSQSBackgroundWorker() {
    SQSWorkerTaskPtr = MakeUnique<FAsyncTask<FAsynchronousSqsWorker>>(
            ActionHandlerDelegate.CreateRaw(this, &ASLMetaHumanDemo::ActionHandler));
    SQSWorkerTaskPtr->StartBackgroundTask();
}

// Displays the version string in the HUD
//
void ASLMetaHumanDemo::DisplayVersion() {
    GeneralStatusPosition = FVector2D(UKismetMathLibrary::FFloor(ViewportSize.X * StatusHorizontalProportion),
            ViewportSize.Y + StatusVerticalOffset);
    UnrealAPI::ShowMessage(FGlobalState::GetVersionString(), UpdateMessageDurationSeconds, FontPtr.Get(),
            FUISettings::GetFontSize(), GeneralStatusPosition, FColor::Red);
}

// Redirects Action requests (based on their type) from SQS to handling logic after unpacking details of those requests
// Note: called via delegate in SQS Worker (FAsynchronousSqsWorker::ProcessMessage()) to process an action
// Consider enhancing with a polymorphic implementation (v.s. switch/case) to cleanly scale to more request types
//
void ASLMetaHumanDemo::ActionHandler(const ASLMetaHumanAction & Action) {
    FString ActionData;
    Action.GetActionData(ActionData);
    switch (Action.GetActionType()) {
        case EASLMetaHumanActionType::ANIMATE_SENTENCE: {
            // Periodic optimization for stability in case of gradual memory leakage that's external to this project
            //
            UnrealAPI::ClearMemory();
            FString ASLTense = "";
            ASLMetaHumanAnimateSentenceAction::GetASLTense(Action, ASLTense);
            FString ASLText;
            ASLMetaHumanAnimateSentenceAction::GetASLText(Action, ASLText);
            const auto & Sentiment = ASLMetaHumanAnimateSentenceAction::GetSentiment(Action);
            // Assumes an 'inappropriate' sentence was constructed, ignores its tense, displays warning message instead.
            //
            if (EASLMetaHumanSentimentType::SHOCKED == Sentiment) {
                ASLTense.Empty();
                ASLText = ActionData;
            }
            AnimateSentence(ActionData, FString::Format(TEXT("{0} {1}"), TArray<FStringFormatArg>({ASLTense, ASLText})),
                    Sentiment, true);
            break;
        }
        case EASLMetaHumanActionType::CHANGE_AVATAR:
            SwitchAvatar(ActionData, true);
            break;
        case EASLMetaHumanActionType::CHANGE_BACKGROUND:
            AssignBackgroundTexture(ActionData, true);
            break;
        case EASLMetaHumanActionType::CHANGE_SIGN_RATE:
            ChangeSignRate(FCString::Atof(*ActionData), true);
            break;
        case EASLMetaHumanActionType::STOP_ALL_ANIMATIONS:
            StopAllAnimations(true);
            break;
        default:
            break;
    }
}

// Initializes map structures (below) in relation to discoverable ASL animation sequences (found in AnimationPath).
// - AnimationSequenceByTokenMap: ASL sign labels -> Animation sequence
// - AnimationSequenceLengthByTokenMap: ASL sign labels -> Animation sequence length
// - TranslatableTokensByWordCount: number of words -> array[ASL sign labels that have that amount of words]
//
// Warning: ensure that your animation sequences are cooked and that nothing is preventing them from being
// cooked (including DefaultGame.ini files)!
// Consider memory and compute efficiency - an on-demand MoCap/Live Link data streaming could be used instead
// to produce similar animation data, which would need to be re-targeted to a skeleton.
//
void ASLMetaHumanDemo::InitAnimationSequences(const FString & AnimationPath) {
    TArray<TWeakObjectPtr<UAnimSequence>> AnimationSequencesPtr;
    if (UnrealAPI::GetAssets<UAnimSequence>(AnimationPath, AnimationSequencesPtr)) {
        const unsigned int NumSequences = static_cast<unsigned int>(AnimationSequencesPtr.Num());
        AnimationSequenceByTokenMap->Reserve(NumSequences);
        AnimationSequenceLengthByTokenMap->Reserve(NumSequences);
        // For each animation sequence, populate the related map structures
        //
        for (unsigned int i = 0; i < NumSequences; i++) {
            const auto & AnimSequencePtr = AnimationSequencesPtr[i];
            const FString & AnimationName = AnimSequencePtr->GetName().Replace(AnimationNamePrefix, TEXT("")).ToUpper();
            AnimSequencePtr->AddToRoot();
            AnimSequencePtr.Get()->AddToRoot();
            AnimationSequenceByTokenMap->Add(AnimationName, AnimSequencePtr);
            AnimationSequenceLengthByTokenMap->Add(AnimationName, AnimSequencePtr->GetPlayLength());
            auto OccurrenceCount = UnrealAPI::CountCharOccurrences(AnimationName, AnimationNameWordDelimiter);
            // Treat a whole word as 'more substitutable' in preference (higher score) than a single letter
            // (fingerspelling) Ignore _I (special case). Consider having a cleaner representation mechanism for signs
            // such as context indicators (i.e. I (noun), I (letter)) versus an underscore.
            //
            if ((! AnimationName.StartsWith(AnimationNameWordDelimiterStr)) && (AnimationName.Len() > 1)) {
                OccurrenceCount++;
            }
            const auto TokensArrayPtr = TranslatableTokensByWordCount->Find(OccurrenceCount);
            if (! TokensArrayPtr) {
                TranslatableTokensByWordCount->Add(OccurrenceCount, TArray {AnimationName});
            } else {
                TokensArrayPtr->Add(AnimationName);
            }
        }
        // Expecting word counts to be sorted descending (will later substitute ASL signs/tokens by largest to smallest
        // word counts)
        //
        TranslatableTokensByWordCount->KeySort([](const unsigned int A, const unsigned int B) {
            return A > B;
        });
    }
}

// Initializes basic objects (font, avatar w/components, HUD, background plane), sets visibility of preferred avatar.
// Returns true if they are initialized; false otherwise.
//
bool ASLMetaHumanDemo::InitInternalUEObjectReferences() {
    if (! UnrealAPI::GetAsset<UFont>(FUISettings::GetFontPath(), FontPtr)) {
        return false;
    }
    FontPtr->AddToRoot();
    TWeakObjectPtr<UWorld> WorldPtr;
    if (! UnrealAPI::GetWorld(WorldPtr)) {
        return false;
    }
    UnrealAPI::HideAllActors<AActor>(WorldPtr.Get(), true);
    if (! SwitchAvatar(FUserSettings::GetAvatarName())) {
        return false;
    }
    if (! UnrealAPI::GetActorByName<AStaticMeshActor>(BackgroundPlaneName, WorldPtr.Get(), PlaneActorPtr)) {
        return false;
    }
    PlaneActorPtr->AddToRoot();
    return true;
}

// Adjusts UI and related state tracking to a default state (to accept new requests, clear UI elements)
//
void ASLMetaHumanDemo::ResetToBeginState() {
    SetCancellingState(false);
    SetReadyToAnimateNextToken(true);
    SetReadyToAnimateNextSentence(true);
    FAsynchronousSqsWorker::SetReadyForNextTranslateMessage(true);
    HideSimplifiedSentenceTrigger.AtomicSet(true);
    HideASLSentenceTrigger.AtomicSet(true);
    HideTokenTextTrigger.AtomicSet(true);
    HideTokenComponentTextTrigger.AtomicSet(true);
    HideSentimentTrigger.AtomicSet(true);
    // Reset the generated background to a default texture
    //
    auto BackgroundStaticMeshComponent = PlaneActorPtr->GetStaticMeshComponent();
    if ((nullptr == BackgroundStaticMeshComponent) || (! BackgroundStaticMeshComponent->IsValidLowLevelFast())) {
        return;
    }

    if ((nullptr == DefaultBackgroundMaterialInterfacePtr)
            || (! DefaultBackgroundMaterialInterfacePtr->IsValidLowLevelFast())) {

        const auto Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
                [&]() {
                    if (! UnrealAPI::GetMaterial(
                                DefaultBackgroundMaterialPath, DefaultBackgroundMaterialInterfacePtr)) {
                        return;
                    }
                },
                TStatId(), nullptr, ENamedThreads::GameThread);
        Task->Wait();
    }
    if ((nullptr != DefaultBackgroundMaterialInterfacePtr)
            && (DefaultBackgroundMaterialInterfacePtr->IsValidLowLevelFast())) {
        const auto Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
                [&]() {
                    BackgroundStaticMeshComponent->SetMaterial(0, DefaultBackgroundMaterialInterfacePtr->GetMaterial());
                },
                TStatId(), nullptr, ENamedThreads::GameThread);
        Task->Wait();
    }
}

// Stop any existing animation in progress to avoid conflicts with those animations. Prior active actor can be GC'd.
//
void ASLMetaHumanDemo::StopAllAnimations(const bool Verbose) {
    SetReadyToAnimateNextSentence(false);
    if (Verbose) {
        UnrealAPI::ShowMessage(StoppingAnimationMessage, UpdateMessageDurationSeconds, FontPtr.Get(),
                FUISettings::GetFontSize(), GeneralStatusPosition, FColor::Red);
    }
    FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&]() {
                SetCancellingState(true);
                FPlatformProcess::Sleep(1.0f);
                ResetToBeginState();
                FFunctionGraphTask::CreateAndDispatchWhenReady(
                        [&]() {
                            if ((nullptr != SkeletalMeshBodyComponentInternalPtr)
                                    && SkeletalMeshBodyComponentInternalPtr->IsValidLowLevel()) { 
                                FPlatformProcess::Sleep(FInternalSettings::GetAnimationSpinlockSeconds()
                                        * FInternalSettings::GetHideMessageSynchronizationMultiplier());
                                SkeletalMeshBodyComponentInternalPtr->Stop();
                            }
                        },
                        TStatId(), nullptr, ENamedThreads::GameThread);
            },
            TStatId(), nullptr, ENamedThreads::AnyThread);
}

// Displays a HUD message containing a simplified English sentence and its ASL text approximation beneath.
// Note: message will be cleared externally.
//
void ASLMetaHumanDemo::DisplaySentencePairs(const FString & Sentence, const FString & ASLText) {
    FVector2D MessagePosition;
    FUISettings::GetSentencePosition(MessagePosition);
    FVector2D ASLTextPosition;
    FUISettings::GetASLTextPosition(ASLTextPosition);
    const FString & Message = FString::Format(SentenceOutputFormat, TArray<FStringFormatArg>({Sentence}));
    const FString & ASLMessage = FString::Format(ASLOutputFormat, TArray<FStringFormatArg>({ASLText}));
    HideSimplifiedSentenceTrigger.AtomicSet(false);
    HideASLSentenceTrigger.AtomicSet(false);
    HideSentimentTrigger.AtomicSet(false);
    UnrealAPI::ShowMessage(Message, FLT_MAX, FontPtr.Get(), FUISettings::GetFontSize(), MessagePosition,
            HideSimplifiedSentenceTrigger, FColor::Green);
    UnrealAPI::ShowMessage(ASLMessage, FLT_MAX, FontPtr.Get(), FUISettings::GetFontSize(), ASLTextPosition,
            HideASLSentenceTrigger, FColor::Purple);
}

// Displays a HUD message containing an ASL Sign/Token. Note: message will be cleared externally.
//
void ASLMetaHumanDemo::DisplayToken(const FString & Token) {
    FString TokenOutput = FString::Format(TEXT("Token: {0}"), TArray<FStringFormatArg>({Token}));
    TokenOutput = TokenOutput.Replace(*AnimationNameWordDelimiterStr, *AnimationNameWordSpaceStr);
    FVector2D Position;
    FUISettings::GetTokenPosition(Position);
    HideTokenTextTrigger.AtomicSet(false);
    UnrealAPI::ShowMessage(TokenOutput, FLT_MAX, FontPtr.Get(), FUISettings::GetFontSize(), Position,
            HideTokenTextTrigger, FColor::Yellow);
}

// Displays a HUD message containing an ASL Sign/Token's component (a subset of the token that's possibly one or more words or one letter).
// Note: message will be cleared based on the animation duration of the token.
//
void ASLMetaHumanDemo::DisplayTokenComponent(const FString & Token) {
    FVector2D Position;
    FUISettings::GetLetterPosition(Position);
    HideTokenComponentTextTrigger.AtomicSet(false);
    UnrealAPI::ShowMessage(Token, GetAnimationDuration(Token), FontPtr.Get(), FUISettings::GetSignFontSize(),
            Position, HideTokenComponentTextTrigger, FColor::Red);
}

// Displays a HUD message containing a color-colored message with emoji (based on SentimentType)
// Note: Message will be cleared externally. Consider externally configuring this message's placement.
//
void ASLMetaHumanDemo::DisplaySentiment(const EASLMetaHumanSentimentType SentimentType) {
    FString Message;
    FColor Color;
    switch (SentimentType) {
        case EASLMetaHumanSentimentType::NEGATIVE:
            Message = NegativeSentimentMessage;
            Color = FColor::Red;
            break;
        case EASLMetaHumanSentimentType::POSITIVE:
            Message = PositiveSentimentMessage;
            Color = FColor::Green;
            break;
        case EASLMetaHumanSentimentType::MIXED:
            Message = MixedSentimentMessage;
            Color = FColor::Blue;
            break;
        case EASLMetaHumanSentimentType::NEUTRAL:
            Message = NeutralSentimentMessage;
            Color = FColor::Cyan;
            break;
        case EASLMetaHumanSentimentType::SHOCKED:
            Message = ShockedSentimentMessage;
            Color = FColor::Red;
            break;
        default:
            return;
    }
    UnrealAPI::ShowMessage(FString::Format(SentimentPromptMessageFormat, TArray<FStringFormatArg>({Message})), FLT_MAX,
            FontPtr.Get(), SentimentFontSize, FVector2D(SentimentHorizontalLocation, ViewportSize.Y / 2),
            HideSentimentTrigger, Color);
}

// Adjusts the speed (by SignRate) by which the active Skeleton animates
//
void ASLMetaHumanDemo::ChangeSignRate(const float SignRate, const bool Verbose) {
    if (Verbose) {
        UnrealAPI::ShowMessage(ChangingSignRateMessage, UpdateMessageDurationSeconds, FontPtr.Get(),
                FUISettings::GetFontSize(), FVector2D(0, 0), FColor::Red);
    }
    FUserSettings::SetPlayRate(SignRate);
    if ((nullptr != SkeletalMeshBodyComponentInternalPtr)
            && SkeletalMeshBodyComponentInternalPtr->IsValidLowLevelFast()) {
        const auto Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
                [&, SignRate]() {
                    SkeletalMeshBodyComponentInternalPtr->SetPlayRate(SignRate);
                },
                TStatId(), nullptr, ENamedThreads::GameThread);
        Task->Wait();
    }
}

// Callback function that assigns a Dynamic Texture (which is then converted to a Static Texture) to a
// material (dynamic material instance) that's assigned to a static mesh representing a background image plane.
//
void ASLMetaHumanDemo::OnAssign2DTextureToBackground(const UTexture2DDynamic * DynamicTexture) {
    if (nullptr == DynamicTexture) {
        return;
    }
    if ((nullptr == PlaneActorPtr) || (! PlaneActorPtr->IsValidLowLevelFast())) {
        return;
    }
    auto BackgroundStaticMeshComponent = PlaneActorPtr->GetStaticMeshComponent();
    if ((nullptr == BackgroundStaticMeshComponent) || (! BackgroundStaticMeshComponent->IsValidLowLevelFast())) {
        return;
    }
    // Must have a Static Texture-equivalent copy to apply to a Material
    //
    TWeakObjectPtr<UTexture2D> StaticTexture;
    UnrealAPI::ConvertTexture(DynamicTexture, StaticTexture);
    const auto Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&]() {
                if (nullptr == DynamicBackgroundMaterialInstancePtr) {
                    DynamicBackgroundMaterialInstancePtr = UMaterialInstanceDynamic::Create(
                            DynamicBackgroundMaterialInterfacePtr.Get(), BackgroundStaticMeshComponent);
                }
                if (nullptr != DynamicBackgroundMaterialInstancePtr) {
                    // Note: the texture parameter name (in the material) needs to be set to TextureImageParameterName.
                    // You can modify the material corresponding to BackgroundMaterialPath in the editor mode to capture
                    // this texture parameter name (must match!). Otherwise, there will be no effect in terms of texture
                    // changes.
                    //
                    DynamicBackgroundMaterialInstancePtr->SetTextureParameterValue(
                            TextureImageParameterName, StaticTexture.Get());
                    BackgroundStaticMeshComponent->SetMaterial(0, DynamicBackgroundMaterialInstancePtr.Get());
                }
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    Task->Wait();
}

// High-level action that wraps the download process for the texture pointed at by SignedUrl.
// The OnAssign2DTextureToBackground() callback will be triggered, once downloading completes.
//
void ASLMetaHumanDemo::AssignBackgroundTexture(const FString & SignedUrl, const bool Verbose) {
    if (Verbose) {
        UnrealAPI::ShowMessage(ChangingBackgroundMessage, UpdateMessageDurationSeconds, FontPtr.Get(),
                FUISettings::GetFontSize(), GeneralStatusPosition, FColor::Red);
    }
    UnrealAPI::DownloadTexture(
            SignedUrl, DownloadTextureDelegate.CreateRaw(this, &ASLMetaHumanDemo::OnAssign2DTextureToBackground));
}

// Changes the Avatar that's currently active (internally) and visually applies that changes.
// Initializes the configured Blueprint Actor (Avatar by name) and its Skeletal Mesh Component objects.
// Both require persistence in order to produce animations at different time frames.
// Returns true if the Avatar name was valid and could be initialized (and is possible the same avatar); false otherwise
//
bool ASLMetaHumanDemo::SwitchAvatar(const FString & AvatarName, const bool Verbose) {
    TWeakObjectPtr<UWorld> WorldPtr;
    if (! UnrealAPI::GetWorld(WorldPtr)) {
        return false;
    }
    // Changing to the existing active avatar (if any) is unnecessary
    //
    if ((nullptr != BPActorInternalPtr) && BPActorInternalPtr->IsValidLowLevel()) {
        if (BPActorInternalPtr->GetName() == AvatarName) {
            return true;
        }
    }
    // Get a reference to the latest user-specified avatar. Bail if it's an invalid avatar,
    // and maintain the existing assigned actor.
    //
    TWeakObjectPtr<AActor> BPActorNewPtr;
    const auto & BPActorObjectPath = FString::Format(BPActorObjectPathFormat, TArray<FStringFormatArg>({AvatarName}));
    if (! UnrealAPI::GetActorByPath(BPActorObjectPath, WorldPtr.Get(), BPActorNewPtr)) {
        return false;
    }
    // Fully transition the Actor in the GUI thread to prevent crashes; it's also possible to stop the
    // prior animation via StopAllAnimations() for stability needs.
    //
    const auto & TransitionActorTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&]() {
                if (nullptr != BPActorInternalPtr) {
                    BPActorInternalPtr->SetActorHiddenInGame(true);
                }
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    TransitionActorTask->Wait();
    if (Verbose) {
        UnrealAPI::ShowMessage(ChangingAvatarMessage, UpdateMessageDurationSeconds, FontPtr.Get(),
                FUISettings::GetFontSize(), GeneralStatusPosition, FColor::Red);
    }
    // Visually, only show the avatar of interest - where other BP-based avatars have been hidden.
    //
    SwapWithActiveAvatar(BPActorNewPtr);
    SkeletalMeshBodyComponentInternalPtr = Cast<USkeletalMeshComponent>(
            BPActorInternalPtr->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
    return SkeletalMeshBodyComponentInternalPtr->IsValidLowLevel();
}

// Updates the active avatar and adjusts its location and orientation to match corresponding preferences.
//
void ASLMetaHumanDemo::SwapWithActiveAvatar(const TWeakObjectPtr<AActor> & BPActorNewPtr) {
    BPActorInternalPtr = BPActorNewPtr;
    const auto & ActorSetupTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&]() {
                // Flip Actor
                //
                if (FUserSettings::GetFlipHands()) {
                    BPActorInternalPtr->SetActorScale3D(FVector(-1, 1, 1));
                }
                // Place Actor in same location
                //
                FVector Location, Rotation;
                FUserSettings::GetAvatarLocation(Location);
                FUserSettings::GetAvatarRotation(Rotation);
                BPActorInternalPtr->SetActorLocationAndRotation(Location, FQuat::MakeFromEuler(Rotation));
                BPActorInternalPtr->SetActorHiddenInGame(false);
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    ActorSetupTask->Wait();
}

// Initializes the visual environment appearance of the world environment using externally-configurable settings.
// Includes setting up the camera view, applying background colors, accessing referenced materials, lighting tweaks.
// Returns true if the basic environment, all referenced materials, camera view and general environment were
// initialized; false otherwise.
//
bool ASLMetaHumanDemo::InitUEObjectsAndEnvironment() {
    TWeakObjectPtr<UWorld> WorldPtr;
    if (! UnrealAPI::GetWorld(WorldPtr)) {
        return false;
    }
    // Background image planes
    //
    PlaneActorPtr->SetActorHiddenInGame(FUserSettings::GetHideBackgroundPlane());
    FVector PlaneLocationOffset;
    FUISettings::GetBackgroundImagePlaneLocationOffset(PlaneLocationOffset);
    FVector PlaneRotationOffset;
    FUISettings::GetBackgroundImagePlaneRotationOffset(PlaneRotationOffset);
    FVector PlaneScale;
    FUISettings::GetBackgroundImagePlaneScale(PlaneScale);
    // Other background and lighting elements
    //
    for (TActorIterator<ASkyAtmosphere> ActorIterator(WorldPtr.Get()); ActorIterator; ++ActorIterator) {
        const auto Actor = Cast<ASkyAtmosphere>(*ActorIterator);
        if ((nullptr != Actor) && (Actor->IsValidLowLevelFast())) {
            Actor->SetActorHiddenInGame(FUserSettings::GetHideAtmosphere());
        }
    }
    for (TActorIterator<ASkyLight> ActorIterator(WorldPtr.Get()); ActorIterator; ++ActorIterator) {
        const auto Actor = Cast<ASkyLight>(*ActorIterator);
        if ((nullptr != Actor) && (Actor->IsValidLowLevelFast())) {
            const auto LightComponent = Actor->GetLightComponent();
            Actor->SetActorHiddenInGame(FUserSettings::GetHideSkyLight());
        }
    }
    for (TActorIterator<ASpotLight> ActorIterator(WorldPtr.Get()); ActorIterator; ++ActorIterator) {
        const auto Actor = Cast<ASpotLight>(*ActorIterator);
        if ((nullptr != Actor) && (Actor->IsValidLowLevelFast())) {
            const auto LightComponent = Actor->GetLightComponent();
            Actor->SetActorHiddenInGame(FUserSettings::GetHideSpotLight());
        }
    }
    // Background Lighting Support (Directional/Sunlight)
    //
    FColor BackgroundLightingColor;
    FUserSettings::GetBackgroundLightingColor(BackgroundLightingColor);
    for (TActorIterator<ADirectionalLight> ActorIterator(WorldPtr.Get()); ActorIterator; ++ActorIterator) {
        const auto Actor = Cast<ADirectionalLight>(*ActorIterator);
        if ((nullptr != Actor) && (Actor->IsValidLowLevelFast())) {
            const auto LightComponent = Actor->GetLightComponent();
            LightComponent->SetLightColor(BackgroundLightingColor);
            LightComponent->SetIntensity(FUserSettings::GetBackgroundLightingIntensity());
        }
    }
    // Camera view adjustments (useful for optimally covering a canvas with the avatar for special displays)
    //
    FVector LocationOffset;
    FUserSettings::GetCameraLocationOffset(LocationOffset);
    FVector RotationOffset;
    FUserSettings::GetCameraRotationOffset(RotationOffset);
    if (! UnrealAPI::SetFirstPlayerCameraView(FUserSettings::GetCameraFOV(), LocationOffset, RotationOffset)) {
        return false;
    }
    if (! UnrealAPI::GetMaterial(BackgroundMaterialPath, DynamicBackgroundMaterialInterfacePtr)) {
        return false;
    }
    if (! UnrealAPI::GetMaterial(DefaultBackgroundMaterialPath, DefaultBackgroundMaterialInterfacePtr)) {
        return false;
    }
    return true;
}

// High-level routine for HUD to display a simplified English phrase (Sentence), its corresponding (approximated)
// ASL text representation (ASLText), and inferred sentiment. ASLText will be broken into ASL tokens (signs), which will
// be passed (one at a time) to a lower-level routine for visual rendition of individual ASL signs. Co-ordinates the
// time between animating a sentence and receiving another sentence to animate (since UE returns asynchronously when it
// is requested to play animations).
//
void ASLMetaHumanDemo::AnimateSentence(const FString & Sentence,
        const FString & ASLText,
        const EASLMetaHumanSentimentType Sentiment,
        const bool Verbose) {
    const auto & Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&, Sentence, ASLText, Sentiment, Verbose]() {
                // Sentence in progress? Don't conflict with existing animation
                //
                while (! IsReadyToAnimateNextSentence()) {
                    FPlatformProcess::Sleep(FInternalSettings::GetAnimationSpinlockSeconds());
                }
                if (Verbose) {
                    UnrealAPI::ShowMessage(AnimateSentenceMessage, UpdateMessageDurationSeconds, FontPtr.Get(),
                            FUISettings::GetFontSize(), GeneralStatusPosition, FColor::Red);
                }
                if (EASLMetaHumanSentimentType::NONE != Sentiment) {
                    DisplaySentiment(Sentiment);
                }
                SetReadyToAnimateNextSentence(false);
                DisplaySentencePairs(Sentence, ASLText);
                // Determine the ASL Signs/tokens to animate, where they'll be animated in sequence.
                //
                TArray<FString> Tokens;
                ASLAlgorithms::GetSignTokensFromSentence(TranslatableTokensByWordCount, ASLText, Tokens);
                unsigned int i = 0;
                const int NumTokens = Tokens.Num();
                for (const auto & Token: Tokens) {
                    while (! IsReadyToAnimateNextToken()) {
                        if (FGlobalState::IsAborting() || IsCancelling()) {
                            HideSimplifiedSentenceTrigger.AtomicSet(true);
                            HideASLSentenceTrigger.AtomicSet(true);
                            return;
                        }
                        FPlatformProcess::Sleep(FInternalSettings::GetAnimationSpinlockSeconds());
                    }
                    // Note: overall animation completion time is only known when the last token is processed (in
                    // AnimateToken()'s thread).
                    //
                    if (! AnimateToken(Token.ToUpper(), (NumTokens == i + 1))) {
                        return;
                    }
                    i++;
                }
            },
            TStatId(), nullptr, ENamedThreads::AnyThread);
}

// Lower-level routine to take one ASL sign/token and request its animation (either by word(s) or letter-by-letter
// - if lacking an ASL translation or known animation). Co-ordinates the time for being ready to animate another token.
// Returns false if there was an animation sequence referencing issue or if the animation had to be aborted; true
// otherwise.
//
bool ASLMetaHumanDemo::AnimateToken(const FString & Token, const bool FinalToken) {
    // Note: this will indirectly affect AsynchronousSQSWorker - triggering it to pause!
    //
    SetReadyToAnimateNextToken(false);
    const auto & Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&, Token, FinalToken]() {
                DisplayToken(Token);
                auto DelaySeconds = FUserSettings::GetWordTransitionDelay();
                for (float i = 0.0f; i < DelaySeconds; i += FInternalSettings::GetAnimationSpinlockSeconds()) {
                    if (FGlobalState::IsAborting() || IsCancelling()) {
                        HideTokenTextTrigger.AtomicSet(true);
                        return false;
                    }
                    FPlatformProcess::Sleep(FInternalSettings::GetAnimationSpinlockSeconds());
                }
                // Determine whether one or more whole words or one word's individual letters (due to lack of an ASL
                // translation knowledge) are animated. Wait for each animation to complete.
                //
                if (AnimationSequenceByTokenMap->Find(Token)) {
                    AnimateSequence(Token, FUserSettings::GetPlayRate(), 0.0f);
                    DelaySeconds = GetAnimationDuration(Token);
                    for (float i = 0.0f; i < DelaySeconds; i += FInternalSettings::GetAnimationSpinlockSeconds()) {
                        if (FGlobalState::IsAborting() || IsCancelling()) {
                            HideTokenTextTrigger.AtomicSet(true);
                            return false;
                        }
                        FPlatformProcess::Sleep(FInternalSettings::GetAnimationSpinlockSeconds());
                    }
                } else {
                    // Whole word(s) translation was not found
                    //
                    if (! AnimateIndividualLettersForToken(Token)) {
                        HideTokenTextTrigger.AtomicSet(true);
                        return false;
                    }
                }
                HideTokenTextTrigger.AtomicSet(true);
                if (FinalToken) {
                    ResetToBeginState();
                }
                // Warning: consider this as a 'timing workaround' to allow time for the token hiding thread to hide its
                // token/avoid collision. FScopeLock and conditional variables for synchronization are likely a
                // better/more direct approach. Note: need to fairly provide GUI thread time slice for animation playing
                // v.s. general HUD message displaying Word transition delay will also impact performance - if too low,
                // there is a risk of hangs (or uncleared text).
                //
                FPlatformProcess::Sleep(FInternalSettings::GetAnimationSpinlockSeconds()
                        * FInternalSettings::GetHideMessageSynchronizationMultiplier());
                SetReadyToAnimateNextToken(true);
                return true;
            },
            TStatId(), nullptr, ENamedThreads::AnyThread);
    return true;
}

// Lower-level routine to animate individual ASL letter-by-letter signs/tokens derived from an input token (Token).
// Each individual token is passed (one at a time) to a lower-level routine for animation playing.
//
bool ASLMetaHumanDemo::AnimateIndividualLettersForToken(const FString & Token) {
    const unsigned int TokenLength = Token.Len();
    for (unsigned int i = 0; i < TokenLength; i++) {
        if (FGlobalState::IsAborting() || IsCancelling()) {
            return false;
        }
        // Note: corner case representation for letter 'I'... 'I' => Noun-based, '_I' => Alphabet-based.
        // Consider having a cleaner representation mechanism for signs such as context indicators versus an underscore.
        //
        FString Letter = FString(1, &Token[i]);
        if (Letter == LetterIAsSubject) {
            // Reference again as an alphabetical letter
            //
            Letter = FString(LetterIAsAlphabetSymbol);
        }
        if (AnimationSequenceByTokenMap->Find(Letter)) {
            const float StartPosition = i == 0 ? 0.0f : FUserSettings::GetPlayStartOffset();
            AnimateSequence(Letter, FUserSettings::GetPlayRate(), StartPosition);
            const float DelaySeconds = GetAnimationDuration(Letter);
            for (float j = 0.0f; j < DelaySeconds; j += FInternalSettings::GetAnimationSpinlockSeconds()) {
                if (FGlobalState::IsAborting() || IsCancelling()) {
                    return false;
                }
                FPlatformProcess::Sleep(FInternalSettings::GetAnimationSpinlockSeconds());
            }
            HideTokenComponentTextTrigger.AtomicSet(true);
        }
    }
    HideTokenTextTrigger.AtomicSet(true);
    return true;
}

// Lowest-level animation processing routine - cross-references the animation sequence for a given ASL sign/token
// (Token) and requests that UE plays that animation. Applies that animation to the internal SkeletalMeshComponent at a
// specified play speed (Rate) and start position (StartPosition). Note: the UE API for playing animations will return
// asynchronously. Returns false if the requested animation couldn't be found; true otherwise.
//
bool ASLMetaHumanDemo::AnimateSequence(const FString & Token, const float Rate, const float StartPosition) {
    const auto AnimSequencePtr = AnimationSequenceByTokenMap->Find(Token);
    if (nullptr == AnimSequencePtr) {
        return false;
    }
    auto & AnimationSequence = **AnimSequencePtr;
    const auto & Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&, Rate, StartPosition, Token]() {
                if (FGlobalState::IsAborting() || IsCancelling()) {
                    return;
                }
                // Important: avoid GC-related disappearance of TWeakObjectPtr<UAnimSequence>'s in the TMap - via
                // TUniquePtr instantiation
                //
                if (nullptr == &AnimationSequence) {
                    return;
                }
                // Consider having a cleaner representation for signs such as context indicators versus an underscore.
                //
                FString TokenCopy(Token);
                if (LetterIAsAlphabetSymbol == TokenCopy) {
                    TokenCopy = LetterIAsSubject;
                }
                TokenCopy = TokenCopy.Replace(*AnimationNameWordDelimiterStr, *AnimationNameWordSpaceStr);
                DisplayTokenComponent(TokenCopy);
                if (FGlobalState::IsAborting() || IsCancelling()) {
                    return;
                }
                UnrealAPI::PlayAnimation(
                        *SkeletalMeshBodyComponentInternalPtr.Get(), AnimationSequence, Rate, StartPosition);
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    return true;
}

// Returns the animation duration corresponding to the specific ASL sign/token provided. A value of 0.0f is returned
// if the animation duration wasn't found.
//
float ASLMetaHumanDemo::GetAnimationDuration(const FString & Token) {
    if (nullptr == AnimationSequenceLengthByTokenMap) {
        return 0.0f;
    }
    const auto DurationPtr = AnimationSequenceLengthByTokenMap->Find(Token);
    if (nullptr == DurationPtr) {
        return 0.0f;
    }
    return ((*DurationPtr) - FUserSettings::GetPlayStartOffset() - FUserSettings::GetPlayEndOffset())
            / FUserSettings::GetPlayRate();
}

// A QA/testbed-related method to perform actions in a controlled manner given a JSON payload
//
void ASLMetaHumanDemo::RunInternalTestAction(const FString & JsonPayload) {
    if (FGlobalState::IsAborting()) {
        return;
    }
    // Include a sufficient delay to prevent issues with screen text clearing/overlapping.
    //
    FPlatformProcess::Sleep(RunTestActionDelaySeconds);
    if (FGlobalState::IsAborting()) {
        return;
    }
    const auto Action = new ASLMetaHumanAction(JsonPayload);
    ActionHandler(*Action);
}