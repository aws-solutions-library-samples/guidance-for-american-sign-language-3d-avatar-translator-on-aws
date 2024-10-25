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

// Represents internal settings; note: a struct could be used instead and a mutex
// is encouraged for value synchronization needs
//
namespace ASLMetaHuman::Config {

class FInternalSettings {
public:
    static float GetAnimationSpinlockSeconds() {
        return AnimationSpinlockSeconds;
    }
    static FString GetFixedTextToSign() {
        return FixedTextToSign;
    }
    static float GetHideMessageSynchronizationMultiplier() {
        return HideMessageSynchronizationMultiplier;
    }
    static bool GetIgnoreSQS() {
        return IgnoreSQS;
    }
    static bool GetOnlySignFixedText() {
        return OnlySignFixedText;
    }
    static bool GetPurgeQueuesOnStartup() {
        return PurgeQueuesOnStartup;
    }
    static FString GetSQSActionQueueName() {
        return SQSActionQueueName;
    }
    static FString GetSQSTranslationQueueName() {
        return SQSTranslationQueueName;
    }
    static float GetSQSSpinlockSeconds() {
        return SQSSpinlockSeconds;
    }
    static void SetAnimationSpinlockSeconds(const float Value) {
        AnimationSpinlockSeconds = Value;
    }
    static void SetFixedTextToSign(const FString& Value) {
        FixedTextToSign = Value;
    }
    static void SetHideMessageSynchronizationMultiplier(const float Value) {
        HideMessageSynchronizationMultiplier = Value;
    }
    static void SetIgnoreSQS(const bool Value) {
        IgnoreSQS = Value;
    }
    static void SetOnlySignFixedText(const bool Value) {
        OnlySignFixedText = Value;
    }
    static void SetPurgeQueuesOnStartup(const bool Value) {
        PurgeQueuesOnStartup = Value;
    }
    static void SetSQSActionQueueName(const FString & Value) {
        SQSActionQueueName = Value;
    }
    static void SetSQSTranslationQueueName(const FString & Value) {
        SQSTranslationQueueName = Value;
    }
    static void SetSQSSpinlockSeconds(const float Value) {
        SQSSpinlockSeconds = Value;
    }

private:
    FInternalSettings();
    static inline float AnimationSpinlockSeconds = 0.05;
    static inline FString FixedTextToSign = "";
    static inline float HideMessageSynchronizationMultiplier = 2.0;
    static inline bool IgnoreSQS = false;
    static inline bool OnlySignFixedText = false;
    static inline bool PurgeQueuesOnStartup = false;
    static inline float SQSSpinlockSeconds = 1.0;
    static inline FString SQSActionQueueName = "";
    static inline FString SQSTranslationQueueName = "";
};
}