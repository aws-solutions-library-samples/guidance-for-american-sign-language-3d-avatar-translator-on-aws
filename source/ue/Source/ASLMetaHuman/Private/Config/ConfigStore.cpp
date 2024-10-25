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

// Provides a configuration interface to change program-wide settings via configuration file
//

#include "Config/ConfigStore.h"
#include "Config/GlobalState.h"
#include "Config/InternalSettings.h"
#include "Config/UISettings.h"
#include "Config/UserSettings.h"

#include <Misc/Paths.h>

using ASLMetaHuman::Config::FGlobalState;
using ASLMetaHuman::Config::FInternalSettings;
using ASLMetaHuman::Config::FUISettings;
using ASLMetaHuman::Config::FUserSettings;

// Keep one persistent configuration store
//
TWeakObjectPtr<UConfigStore> UConfigStore::Instance {nullptr};

// Fields and section names to parse in the configuration file (filename is in CONFIG_FILENAME)
//
namespace {
const TCHAR * ANIMATION_SPINLOCK_SECONDS_FIELD = TEXT("AnimationSpinlockSeconds");
const TCHAR * ASL_TEXT_POSITION_FIELD = TEXT("ASLTextPosition");
const TCHAR * AVATAR_NAME_FIELD = TEXT("AvatarName");
const TCHAR * AVATAR_LOCATION_FIELD = TEXT("AvatarLocation");
const TCHAR * AVATAR_ROTATION_FIELD = TEXT("AvatarRotation");
const TCHAR * BACKGROUND_IMAGE_PLANE_LOCATION_OFFSET_FIELD = TEXT("BackgroundImagePlaneLocationOffset");
const TCHAR * BACKGROUND_IMAGE_PLANE_ROTATION_OFFSET_FIELD = TEXT("BackgroundImagePlaneRotationOffset");
const TCHAR * BACKGROUND_IMAGE_PLANE_SCALE_FIELD = TEXT("BackgroundImagePlaneScale");
const TCHAR * BACKGROUND_LIGHTING_COLOR_FIELD = TEXT("BackgroundLightingColor");
const TCHAR * BACKGROUND_LIGHTING_INTENSITY_FIELD = TEXT("BackgroundLightingIntensity");
const TCHAR * CAMERA_FOV_FIELD = TEXT("CameraFOV");
const TCHAR * CAMERA_LOCATION_OFFSET_FIELD = TEXT("CameraLocationOffset");
const TCHAR * CAMERA_ROTATION_OFFSET_FIELD = TEXT("CameraRotationOffset");
// Main configuration filename (expected in the Config directory - both in Shipping and non-Shipping releases)
//
const TCHAR * CONFIG_FILENAME = TEXT("ASLMetaHuman.ini");
const TCHAR * CONFIG_FILE_INTERNAL_SECTION_NAME = TEXT("/Script/ASLMetaHuman.Internal");
const TCHAR * CONFIG_FILE_UI_SECTION_NAME = TEXT("/Script/ASLMetaHuman.UI");
const TCHAR * CONFIG_FILE_USER_SECTION_NAME = TEXT("/Script/ASLMetaHuman.User");
const TCHAR * FIXED_TEXT_TO_SIGN_FIELD = TEXT("FixedTextToSign");
const TCHAR * FLIP_HANDS_FIELD = TEXT("bFlipHands");
const TCHAR * FONT_PATH_FIELD = TEXT("FontPath");
const TCHAR * FONT_SIZE_FIELD = TEXT("FontSize");
const TCHAR * HIDE_ATMOSPHERE_FIELD = TEXT("bHideAtmosphere");
const TCHAR * HIDE_BACKGROUND_PLANE_FIELD = TEXT("bHideBackgroundPlane");
const TCHAR * HIDE_MESSAGE_SYNCHRONIZATION_MULTIPLIER_FIELD = TEXT("HideMessageSynchronizationMultiplier");
const TCHAR * HIDE_SKYLIGHT_FIELD = TEXT("bHideSkyLight");
const TCHAR * HIDE_SPOTLIGHT_FIELD = TEXT("bHideSpotLight");
const TCHAR * IGNORE_SQS_FIELD = TEXT("bIgnoreSQS");
const TCHAR * LETTER_POSITION_FIELD = TEXT("LetterPosition");
const TCHAR * ONLY_SIGN_FIXED_TEXT_FIELD = TEXT("bOnlySignFixedText");
const TCHAR * PLAY_START_OFFSET_FIELD = TEXT("PlayStartOffset");
const TCHAR * PLAY_END_OFFSET_FIELD = TEXT("PlayEndOffset");
const TCHAR * PLAY_RATE_FIELD = TEXT("PlayRate");
const TCHAR * PURGE_QUEUES_ON_STARTUP = TEXT("bPurgeQueuesOnStartup");
const TCHAR * SENTENCE_POSITION_FIELD = TEXT("SentencePosition");
const TCHAR * SIGN_FONT_SIZE_FIELD = TEXT("SignFontSize");
const TCHAR * SQS_ACTION_QUEUE_NAME_FIELD = TEXT("SQSActionQueueName");
const TCHAR * SQS_TRANSLATION_QUEUE_NAME_FIELD = TEXT("SQSTranslationQueueName");
const TCHAR * SQS_SPINLOCK_SECONDS_FIELD = TEXT("SQSSpinlockSeconds");
const TCHAR * TOKEN_POSITION_FIELD = TEXT("TokenPosition");
const TCHAR * USE_ENTIRE_BACKGROUND_FOR_IMAGES_FIELD = TEXT("UseEntireBackgroundForImages");
const TCHAR * WORD_TRANSITION_DELAY_FIELD = TEXT("WordTransitionDelay");
const FString & MissingConfigErrorMessage {"Error: configuration file is missing."};
}

// Enforces one instance of the configuration store
//
void UConfigStore::GetInstance(TWeakObjectPtr<UConfigStore> & ConfigStore) {
    if (nullptr == Instance) {
        ConfigStore = NewObject<UConfigStore>();
    }
}

// Provides configuration file path
//
void UConfigStore::GetConfigFilePath(FString & Path) {
    Path = FPaths::SourceConfigDir().Append(CONFIG_FILENAME);
}

// Apply already read configuration properties - need to keep these in sync; structs are an alternative.
//
void UConfigStore::ApplyConfig() const {
    FInternalSettings::SetAnimationSpinlockSeconds(AnimationSpinlockSeconds);
    FUISettings::SetASLTextPosition(ASLTextPosition);
    FUserSettings::SetAvatarName(AvatarName);
    FUserSettings::SetAvatarLocation(AvatarLocation);
    FUserSettings::SetAvatarRotation(AvatarRotation);
    FUISettings::SetBackgroundImagePlaneLocationOffset(BackgroundImagePlaneLocationOffset);
    FUISettings::SetBackgroundImagePlaneRotationOffset(BackgroundImagePlaneRotationOffset);
    FUISettings::SetBackgroundImagePlaneScale(BackgroundImagePlaneScale);
    FUserSettings::SetBackgroundLightingColor(BackgroundLightingColor);
    FUserSettings::SetBackgroundLightingIntensity(BackgroundLightingIntensity);
    FUserSettings::SetCameraFOV(CameraFOV);
    FUserSettings::SetCameraLocationOffset(CameraLocationOffset);
    FUserSettings::SetCameraRotationOffset(CameraRotationOffset);
    FInternalSettings::SetFixedTextToSign(FixedTextToSign);
    FUserSettings::SetFlipHands(bFlipHands);
    FUISettings::SetFontPath(FontPath);
    FUISettings::SetFontSize(FontSize);
    FUserSettings::SetHideAtmosphere(bHideAtmosphere);
    FUserSettings::SetHideBackgroundPlane(bHideBackgroundPlane);
    FUserSettings::SetHideSkyLight(bHideSkyLight);
    FUserSettings::SetHideSpotLight(bHideSpotLight);
    FInternalSettings::SetHideMessageSynchronizationMultiplier(HideMessageSynchronizationMultiplier);
    FInternalSettings::SetIgnoreSQS(bIgnoreSQS);
    FUISettings::SetLetterPosition(LetterPosition);
    FInternalSettings::SetOnlySignFixedText(bOnlySignFixedText);
    FUserSettings::SetPlayStartOffset(PlayStartOffset);
    FUserSettings::SetPlayEndOffset(PlayEndOffset);
    FUserSettings::SetPlayRate(PlayRate);
    FInternalSettings::SetPurgeQueuesOnStartup(bPurgeQueuesOnStartup);
    FUISettings::SetSentencePosition(SentencePosition);
    FUISettings::SetSignFontSize(SignFontSize);
    FInternalSettings::SetSQSActionQueueName(SQSActionQueueName);
    FInternalSettings::SetSQSTranslationQueueName(SQSTranslationQueueName);
    FInternalSettings::SetSQSSpinlockSeconds(SQSSpinlockSeconds);
    FUISettings::SetTokenPosition(TokenPosition);
    FUISettings::SetUseEntireBackgroundForImages(UseEntireBackgroundForImages);
    FUserSettings::SetWordTransitionDelay(WordTransitionDelay);
}

// Populates settings found in CONFIG_FILENAME (.ini file) into UProperty objects (via GConfig interface).
// Supports packaged releases, where CONFIG_FILENAME is an external sidecar file; not intended to be embedded
//
// Warning: to avoid incorrect/unexpected configuration file state,
// check for these files:
// - Saved\Config\WindowsNoEditor\<CONFIG_FILENAME>
// - Saved\Cooked\WindowsNoEditor\<Project>\Config\<CONFIG_FILENAME>
//  -Package\Windows\<Project>\Config\<CONFIG_FILENAME>
//
// Consider symlinking to <CONFIG_FILENAME> from project root's config folder
//
bool UConfigStore::InitConfig() {
    if (nullptr == GConfig) {
        return false;
    }
    const FString & ConfigFilePath =
            FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() + TEXT("/") + CONFIG_FILENAME);
    if (! FPaths::FileExists(ConfigFilePath)) {
        FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Yes, FText::FromString(MissingConfigErrorMessage));
        return false;
    }
    // Populate individual configuration sections; consider refactoring into separate configuration stores
    //
    InitUserConfig(ConfigFilePath);
    InitInternalConfig(ConfigFilePath);
    InitUIConfig(ConfigFilePath);
    return true;
}

// Refreshes the active configuration
//
bool UConfigStore::ReloadConfig() {
    if (InitConfig()) {
        ApplyConfig();
        return true;
    }
    return false;
}

// Populates end user-related configuration fields from the supplied configuration file
//
void UConfigStore::InitUserConfig(const FString & ConfigFilePath) {
    const TCHAR * SectionName = CONFIG_FILE_USER_SECTION_NAME;
    GConfig->GetString(SectionName, AVATAR_NAME_FIELD, AvatarName, ConfigFilePath);
    GConfig->GetVector(SectionName, AVATAR_LOCATION_FIELD, AvatarLocation, ConfigFilePath);
    GConfig->GetVector(SectionName, AVATAR_ROTATION_FIELD, AvatarRotation, ConfigFilePath);
    GConfig->GetColor(SectionName, BACKGROUND_LIGHTING_COLOR_FIELD, BackgroundLightingColor, ConfigFilePath);
    GConfig->GetFloat(SectionName, BACKGROUND_LIGHTING_INTENSITY_FIELD, BackgroundLightingIntensity, ConfigFilePath);
    GConfig->GetFloat(SectionName, CAMERA_FOV_FIELD, CameraFOV, ConfigFilePath);
    GConfig->GetVector(SectionName, CAMERA_LOCATION_OFFSET_FIELD, CameraLocationOffset, ConfigFilePath);
    GConfig->GetVector(SectionName, CAMERA_ROTATION_OFFSET_FIELD, CameraRotationOffset, ConfigFilePath);
    GConfig->GetBool(SectionName, FLIP_HANDS_FIELD, bFlipHands, ConfigFilePath);
    GConfig->GetBool(SectionName, HIDE_ATMOSPHERE_FIELD, bHideAtmosphere, ConfigFilePath);
    GConfig->GetBool(SectionName, HIDE_BACKGROUND_PLANE_FIELD, bHideBackgroundPlane, ConfigFilePath);
    GConfig->GetBool(SectionName, HIDE_SKYLIGHT_FIELD, bHideSkyLight, ConfigFilePath);
    GConfig->GetBool(SectionName, HIDE_SPOTLIGHT_FIELD, bHideSpotLight, ConfigFilePath);
    GConfig->GetFloat(SectionName, PLAY_START_OFFSET_FIELD, PlayStartOffset, ConfigFilePath);
    GConfig->GetFloat(SectionName, PLAY_END_OFFSET_FIELD, PlayEndOffset, ConfigFilePath);
    GConfig->GetFloat(SectionName, PLAY_RATE_FIELD, PlayRate, ConfigFilePath);
    GConfig->GetFloat(SectionName, WORD_TRANSITION_DELAY_FIELD, WordTransitionDelay, ConfigFilePath);
}

// Populates internal-related configuration fields from the supplied configuration file
//
void UConfigStore::InitInternalConfig(const FString & ConfigFilePath) {
    const TCHAR * SectionName = CONFIG_FILE_INTERNAL_SECTION_NAME;
    GConfig->GetFloat(SectionName, ANIMATION_SPINLOCK_SECONDS_FIELD, AnimationSpinlockSeconds, ConfigFilePath);
    GConfig->GetString(SectionName, FIXED_TEXT_TO_SIGN_FIELD, FixedTextToSign, ConfigFilePath);
    GConfig->GetFloat(SectionName, HIDE_MESSAGE_SYNCHRONIZATION_MULTIPLIER_FIELD, HideMessageSynchronizationMultiplier,
            ConfigFilePath);
    GConfig->GetBool(SectionName, IGNORE_SQS_FIELD, bIgnoreSQS, ConfigFilePath);
    GConfig->GetBool(SectionName, ONLY_SIGN_FIXED_TEXT_FIELD, bOnlySignFixedText, ConfigFilePath);
    GConfig->GetBool(SectionName, PURGE_QUEUES_ON_STARTUP, bPurgeQueuesOnStartup, ConfigFilePath);
    GConfig->GetString(SectionName, SQS_ACTION_QUEUE_NAME_FIELD, SQSActionQueueName, ConfigFilePath);
    GConfig->GetString(SectionName, SQS_TRANSLATION_QUEUE_NAME_FIELD, SQSTranslationQueueName, ConfigFilePath);
    GConfig->GetFloat(SectionName, SQS_SPINLOCK_SECONDS_FIELD, SQSSpinlockSeconds, ConfigFilePath);
}

// Populates UI-related configuration fields from the supplied configuration file
//
void UConfigStore::InitUIConfig(const FString & ConfigFilePath) {
    const TCHAR * SectionName = CONFIG_FILE_UI_SECTION_NAME;
    GConfig->GetVector2D(SectionName, ASL_TEXT_POSITION_FIELD, ASLTextPosition, ConfigFilePath);
    GConfig->GetVector(SectionName, BACKGROUND_IMAGE_PLANE_LOCATION_OFFSET_FIELD, BackgroundImagePlaneLocationOffset, ConfigFilePath);
    GConfig->GetVector(SectionName, BACKGROUND_IMAGE_PLANE_ROTATION_OFFSET_FIELD, BackgroundImagePlaneRotationOffset, ConfigFilePath);
    GConfig->GetVector(SectionName, BACKGROUND_IMAGE_PLANE_SCALE_FIELD, BackgroundImagePlaneScale, ConfigFilePath);
    GConfig->GetString(SectionName, FONT_PATH_FIELD, FontPath, ConfigFilePath);
    GConfig->GetInt(SectionName, FONT_SIZE_FIELD, FontSize, ConfigFilePath);
    GConfig->GetVector2D(SectionName, LETTER_POSITION_FIELD, LetterPosition, ConfigFilePath);
    GConfig->GetVector2D(SectionName, SENTENCE_POSITION_FIELD, SentencePosition, ConfigFilePath);
    GConfig->GetInt(SectionName, SIGN_FONT_SIZE_FIELD, SignFontSize, ConfigFilePath);
    GConfig->GetVector2D(SectionName, TOKEN_POSITION_FIELD, TokenPosition, ConfigFilePath);
    GConfig->GetBool(SectionName, USE_ENTIRE_BACKGROUND_FOR_IMAGES_FIELD, UseEntireBackgroundForImages, ConfigFilePath);
}