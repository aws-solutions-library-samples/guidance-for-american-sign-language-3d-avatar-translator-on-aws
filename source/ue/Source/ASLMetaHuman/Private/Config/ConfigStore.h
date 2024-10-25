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

#include "ConfigStore.generated.h"

UCLASS(Config = CONFIG_FILENAME)
class ASLMETAHUMAN_API UConfigStore: public UObject {
    GENERATED_BODY()

public:
    UConfigStore(UConfigStore & Other) = delete;
    static void GetInstance(TWeakObjectPtr<UConfigStore> & ConfigStore);
    bool ReloadConfig();

private:
    UConfigStore() = default;
    virtual ~UConfigStore() override {
    }
    void ApplyConfig() const;
    static void GetConfigFilePath(FString & Path);
    bool InitConfig();
    void InitInternalConfig(const FString & ConfigFilePath);
    void InitUIConfig(const FString & ConfigFilePath);
    void InitUserConfig(const FString & ConfigFilePath);

    // Keep one instance
    //
    static TWeakObjectPtr<UConfigStore> Instance;

    // Below are configuration items that map to ASLMetaHumans.ini. Ensure that the naming of properties
    // (including casing) matches entries in that file. Reminder: need lowercase "b"-prefix for bool vars
    // to be recognized.
    //
    UPROPERTY(Config, GlobalConfig)
    bool bFlipHands;
    UPROPERTY(Config, GlobalConfig)
    bool bHideAtmosphere;
    UPROPERTY(Config, GlobalConfig)
    bool bHideBackgroundPlane;
    UPROPERTY(Config, GlobalConfig)
    bool bHideSkyLight;
    UPROPERTY(Config, GlobalConfig)
    bool bHideSpotLight;
    UPROPERTY(Config, GlobalConfig)
    bool bIgnoreSQS;
    UPROPERTY(Config, GlobalConfig)
    bool bOnlySignFixedText;
    UPROPERTY(Config, GlobalConfig)
    bool bPurgeQueuesOnStartup;
    UPROPERTY(Config, GlobalConfig)
    float AnimationSpinlockSeconds;
    UPROPERTY(Config, GlobalConfig)
    FVector2D ASLTextPosition;
    UPROPERTY(Config, GlobalConfig)
    FString AvatarName;
    UPROPERTY(Config, GlobalConfig)
    FVector AvatarLocation;
    UPROPERTY(Config, GlobalConfig)
    FVector AvatarRotation;
    UPROPERTY(Config, GlobalConfig)
    FVector BackgroundImagePlaneScale;
    UPROPERTY(Config, GlobalConfig)
    FVector BackgroundImagePlaneLocationOffset;
    UPROPERTY(Config, GlobalConfig)
    FVector BackgroundImagePlaneRotationOffset;
    UPROPERTY(Config, GlobalConfig)
    FColor BackgroundLightingColor;
    UPROPERTY(Config, GlobalConfig)
    float BackgroundLightingIntensity;
    UPROPERTY(Config, GlobalConfig)
    float CameraFOV;
    UPROPERTY(Config, GlobalConfig)
    FVector CameraLocationOffset;
    UPROPERTY(Config, GlobalConfig)
    FVector CameraRotationOffset;
    UPROPERTY(Config, GlobalConfig)
    FString FixedTextToSign;
    UPROPERTY(Config, GlobalConfig)
    FString FontPath;
    UPROPERTY(Config, GlobalConfig)
    int FontSize;
    UPROPERTY(Config, GlobalConfig)
    float HideMessageSynchronizationMultiplier;
    UPROPERTY(Config, GlobalConfig)
    FVector2D LetterPosition;
    UPROPERTY(Config, GlobalConfig)
    float PlayStartOffset;
    UPROPERTY(Config, GlobalConfig)
    float PlayEndOffset;
    UPROPERTY(Config, GlobalConfig)
    float PlayRate;
    UPROPERTY(Config, GlobalConfig)
    FVector2D SentencePosition;
    UPROPERTY(Config, GlobalConfig)
    int SignFontSize;
    UPROPERTY(Config, GlobalConfig)
    FString SQSActionQueueName;
    UPROPERTY(Config, GlobalConfig)
    FString SQSTranslationQueueName;
    UPROPERTY(Config, GlobalConfig)
    float SQSSpinlockSeconds;
    UPROPERTY(Config, GlobalConfig)
    FVector2D TokenPosition;
    UPROPERTY(Config, GlobalConfig)
    bool UseEntireBackgroundForImages;
    UPROPERTY(Config, GlobalConfig)
    float WordTransitionDelay;
};
