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

// Supports main ASL Demo workflow
//

// Note: Making this class a UClass is possible, though there are some concerns about object lifetime.
// Containers including (maps) could have UAnimationSequence's be GC'd even under strategies like:
// PROPERTY() decorating, AddRoot(), TArray copies, TWeakObjectPtr usage.
//

#include <Animation/AnimSequence.h>
#include <Engine.h>

#include "ASLMetaHumanAction.h"
#include "AsynchronousSQSWorker.h"

#include <Tools/ControlRigPose.h>

namespace ASLMetaHuman::Core {

class ASLMetaHumanDemo {
public:
    void Shutdown();
    // This provides long-lasting access to the high-level demo logic
    //
    static ASLMetaHumanDemo * GetInstance() {
        if (nullptr == DemoInstancePtr) {
            // MakeUnique() workaround: since TUniquePtr() doesn't have access to constructor!
            //
            DemoInstancePtr = TUniquePtr<ASLMetaHumanDemo>(new ASLMetaHumanDemo());
            DemoInstancePtr->IsInitialized = DemoInstancePtr->Init();
        }
        return DemoInstancePtr.Get();
    }

private:
    ASLMetaHumanDemo() = default;
    ASLMetaHumanDemo & operator=(const ASLMetaHumanDemo &) = delete;
    ASLMetaHumanDemo(ASLMetaHumanDemo const &) = delete;

    // Returns whether animation pipeline cancellation was initiated
    //
    static bool IsCancelling() {
        FScopeLock ScopeLock(&MutexCancellationActive);
        return CancellationActive;
    }

    // Returns false if there's an animation in progress; true otherwise
    //
    static bool IsReadyToAnimateNextSentence() {
        FScopeLock ScopeLock(&MutexReadyToAnimateNextSentence);
        return ReadyToAnimateNextSentence;
    }

    // Returns false if there is an animation in progress; true otherwise
    //
    static bool IsReadyToAnimateNextToken() {
        FScopeLock ScopeLock(&MutexReadyToAnimateNextToken);
        return ReadyToAnimateNextToken;
    }

    // Updates the cancellation state of the animation pipeline
    //
    static void SetCancellingState(const bool State) {
        FScopeLock ScopeLock(&MutexCancellationActive);
        CancellationActive = State;
    }

    // Allows another sentence to be processed if State==true; else current animation is not interrupted
    //
    static void SetReadyToAnimateNextSentence(const bool State) {
        FScopeLock ScopeLock(&MutexReadyToAnimateNextSentence);
        ReadyToAnimateNextSentence = State;
    }

    // Allows another animation to be processed if State==true; else current animation is not interrupted
    //
    static void SetReadyToAnimateNextToken(const bool State) {
        FScopeLock ScopeLock(&MutexReadyToAnimateNextToken);
        ReadyToAnimateNextToken = State;
    }

    void ActionHandler(const ASLMetaHumanAction & Action);
    bool AnimateIndividualLettersForToken(const FString & Token);
    void AnimateSentence(const FString & Sentence,
            const FString & ASLText,
            const EASLMetaHumanSentimentType Sentiment = EASLMetaHumanSentimentType::NONE,
            const bool Verbose = false);
    bool AnimateSequence(const FString & Token, const float Rate, const float StartPosition);
    bool AnimateToken(const FString & Token, const bool FinalToken = false);
    void AssignBackgroundTexture(const FString & SignedUrl, const bool Verbose = false);
    void ChangeSignRate(const float SignRate, const bool Verbose = false);
    void DisplaySentencePairs(const FString & Sentence, const FString & ASLText);
    void DisplaySentiment(const EASLMetaHumanSentimentType SentimentType);
    void DisplayToken(const FString & Token);
    void DisplayTokenComponent(const FString & Token);
    void DisplayVersion();
    float GetAnimationDuration(const FString & Token);
    bool Init();
    void InitAnimations();
    void InitAnimationSequences(const FString & AnimationPath);
    bool InitInternalUEObjectReferences();
    void InitSQSBackgroundWorker();
    bool InitUEObjectsAndEnvironment();
    void OnAssign2DTextureToBackground(const UTexture2DDynamic * DynamicTexture);
    void RunInternalTestAction(const FString & JsonPayload);
    void ResetToBeginState();
    void StopAllAnimations(const bool Verbose = false);
    void SwapWithActiveAvatar(const TWeakObjectPtr<AActor> & BPActorNewPtr);
    bool SwitchAvatar(const FString & AvatarName, const bool Verbose = false);

    // Tracks state: shutdown requests and readiness to process next word token - with related protection
    //
    static inline bool ReadyToAnimateNextToken = true;
    static inline FCriticalSection MutexReadyToAnimateNextToken;
    static inline bool ReadyToAnimateNextSentence = true;
    static inline FCriticalSection MutexReadyToAnimateNextSentence;

    // Tracks state of animation pipeline cancellation requests
    //
    static inline bool CancellationActive = false;
    static inline FCriticalSection MutexCancellationActive;

    static inline bool IsInitialized = false;

    static inline TUniquePtr<ASLMetaHumanDemo> DemoInstancePtr;

    // For external classes (SQS handler) to invoke actions
    //
    TDelegate<void(const ASLMetaHumanAction &)> ActionHandlerDelegate;

    // For external classes (UE download handler) to invoke actions
    //
    TDelegate<void(const UTexture2DDynamic *)> DownloadTextureDelegate;

    // Blueprint-based Actor object access - MetaHuman actor
    //
    TWeakObjectPtr<AActor> BPActorInternalPtr;

    // Access to skeleton of animating character or blueprint.
    // Note: "Body" is referring to hand/fingers component in the MetaHumans rig.
    //
    TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshBodyComponentInternalPtr;

    // Background Plane Actor (for changing backgrounds) and Heads Up Display Actor
    //
    TWeakObjectPtr<AStaticMeshActor> PlaneActorPtr;
    //TWeakObjectPtr<AStaticMeshActor> PlaneHUDActorPtr;

    // Stores references to animation sequences by character and whole word (if it is defined)
    // Note: this likely won't scale well for a complete ASL alphabet and should instead
    // consider contextual information/ASL rules while feeding in skeleton motion data associated
    //
    TMap<FString, TWeakObjectPtr<UAnimSequence>> * AnimationSequenceByTokenMap;

    // Pairwise-related to AnimationSequenceByTokenMap keys - the length in seconds of an animation sequence
    //
    TMap<FString, float> * AnimationSequenceLengthByTokenMap;

    // Tracks individual ASL translations by the number of words that they contain
    //
    TMap<unsigned int, TArray<FString>> * TranslatableTokensByWordCount;

    // Holds background static mesh component materials (for changing backgrounds)
    //
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicBackgroundMaterialInstancePtr;
    TWeakObjectPtr<UMaterialInterface> DynamicBackgroundMaterialInterfacePtr;
    TWeakObjectPtr<UMaterialInterface> DefaultBackgroundMaterialInterfacePtr;

    // Background worker that receives ASL requests
    //
    TUniquePtr<FAsyncTask<FAsynchronousSqsWorker>> SQSWorkerTaskPtr;

    // Font preserving to prevent crashes (and having to resolve font many times)
    //
    TWeakObjectPtr<UFont> FontPtr;

    FVector2D ViewportSize;

    // X,Y position for status update text
    //
    FVector2D GeneralStatusPosition;

    // On-screen message hiding triggers to avoid having to pre-compute animation durations.
    // Changing their value will trigger HUD/UI element visibility changes. Consider packing into a structure.
    //
    FThreadSafeBool HideSimplifiedSentenceTrigger = false;
    FThreadSafeBool HideASLSentenceTrigger = false;
    FThreadSafeBool HideTokenTextTrigger = false;
    FThreadSafeBool HideTokenComponentTextTrigger = false;
    FThreadSafeBool HideSentimentTrigger = false;
};
}
