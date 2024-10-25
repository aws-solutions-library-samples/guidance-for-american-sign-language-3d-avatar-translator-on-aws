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

// Represents general user settings; note: a struct could be used instead and a mutex
// is encouraged for value synchronization needs
//
namespace ASLMetaHuman::Config {

class FUserSettings {
public:
    static FString GetAvatarName() {
        return AvatarName;
    }
    static void GetAvatarLocation(FVector & Value) {
        Value = AvatarLocation;
    }
    static void GetAvatarRotation(FVector & Value) {
        Value = AvatarRotation;
    }
    static void GetBackgroundLightingColor(FColor & Value) {
        Value = BackgroundLightingColor;
    }
    static float GetBackgroundLightingIntensity() {
        return BackgroundLightingIntensity;
    }
    static float GetBackgroundLightingRadius() {
        return BackgroundLightingRadius;
    }
    static float GetCameraFOV() {
        return CameraFOV;
    }
    static void GetCameraLocationOffset(FVector & Value) {
        Value = CameraLocationOffset;
    }
    static void GetCameraRotationOffset(FVector & Value) {
        Value = CameraRotationOffset;
    }
    static float GetFlipHands() {
        return FlipHands;
    }
    static bool GetHideAtmosphere() {
        return HideAtmosphere;
    }
    static bool GetHideBackgroundPlane() {
        return HideBackgroundPlane;
    }
    static bool GetHideSkyLight() {
        return HideSkyLight;
    }
    static bool GetHideSpotLight() {
        return HideSpotLight;
    }
    static float GetPlayEndOffset() {
        return PlayEndOffset;
    }
    static float GetPlayRate() {
        return PlayRate;
    }
    static float GetPlayStartOffset() {
        return PlayStartOffset;
    }
    static float GetWordTransitionDelay() {
        return WordTransitionDelaySeconds;
    }
    static void SetAvatarName(const FString & Value) {
        AvatarName = Value;
    }
    static void SetAvatarLocation(const FVector & Value) {
        AvatarLocation = Value;
    }
    static void SetAvatarRotation(const FVector & Value) {
        AvatarRotation = Value;
    }
    static void SetBackgroundLightingColor(const FColor & Value) {
        BackgroundLightingColor = Value;
    }
    static void SetBackgroundLightingIntensity(const float Value) {
        BackgroundLightingIntensity = Value;
    }
    static void SetBackgroundLightingRadius(const float Value) {
        BackgroundLightingRadius = Value;
    }
    static void SetCameraFOV(const float Value) {
        CameraFOV = Value;
    }
    static void SetCameraLocationOffset(const FVector & Value) {
        CameraLocationOffset = Value;
    }
    static void SetCameraRotationOffset(const FVector & Value) {
        CameraRotationOffset = Value;
    }
    static void SetFlipHands(const bool Value) {
        FlipHands = Value;
    }
    static void SetHideAtmosphere(const bool Value) {
        HideAtmosphere = Value;
    }
    static void SetHideBackgroundPlane(const bool Value) {
        HideBackgroundPlane = Value;
    }
    static void SetHideSkyLight(const bool Value) {
        HideSkyLight = Value;
    }
    static void SetHideSpotLight(const bool Value) {
        HideSpotLight = Value;
    }
    static void SetPlayEndOffset(const float Value) {
        PlayEndOffset = Value;
    }
    static void SetPlayRate(const float Value) {
        PlayRate = Value;
    }
    static void SetPlayStartOffset(const float Value) {
        PlayStartOffset = Value;
    }
    static void SetWordTransitionDelay(const float Value) {
        WordTransitionDelaySeconds = Value;
    }

private:
    FUserSettings();
    static inline FString AvatarName = "";
    static inline FVector AvatarLocation = FVector(130, -20, 20);
    static inline FVector AvatarRotation = FVector(0, 0, 90);
    static inline FColor BackgroundLightingColor = FColor(0, 0, 0);
    static inline float BackgroundLightingIntensity = 0.0f;
    static inline float BackgroundLightingRadius = 0.0f;
    static inline float CameraFOV = 100.0f;
    static inline FVector CameraLocationOffset = FVector(0, 0, 0);
    static inline FVector CameraRotationOffset = FVector(0, 0, 0);
    static inline bool FlipHands = true;
    static inline bool HideAtmosphere = false;
    static inline bool HideBackgroundPlane = true;
    static inline bool HideSkyLight = false;
    static inline bool HideSpotLight = false;
    static inline float PlayStartOffset = 0.8f;
    static inline float PlayEndOffset = 0.8f;
    static inline float PlayRate = 3.0f;
    static inline float WordTransitionDelaySeconds = 1.2f;
};
}