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

// Represents user interface settings; note: a struct could be used instead and a mutex
// is encouraged for value synchronization needs
//
namespace ASLMetaHuman::Config {

class FUISettings {
public:
    static FString GetFontPath() {
        return FontPath;
    }
    static unsigned int GetFontSize() {
        return FontSize;
    }
    static void GetASLTextPosition(FVector2D & Value) {
        Value = ASLTextPosition;
    }
    static void GetBackgroundImagePlaneLocationOffset(FVector & Value) {
        Value = BackgroundImagePlaneLocationOffset;
    }
    static void GetBackgroundImagePlaneRotationOffset(FVector & Value) {
        Value = BackgroundImagePlaneRotationOffset;
    }
    static void GetBackgroundImagePlaneScale(FVector & Value) {
        Value = BackgroundImagePlaneScale;
    }
    static void GetLetterPosition(FVector2D & Value) {
        Value = LetterPosition;
    }
    static void GetSentencePosition(FVector2D & Value) {
        Value = SentencePosition;
    }
    static unsigned int GetSignFontSize() {
        return SignFontSize;
    }
    static void GetTokenPosition(FVector2D & Value) {
        Value = TokenPosition;
    }
    static bool GetUseEntireBackgroundForImages() {
        return UseEntireBackgroundForImages;
    }
    static void SetBackgroundImagePlaneLocationOffset(const FVector & Value) {
        BackgroundImagePlaneLocationOffset = Value;
    }
    static void SetBackgroundImagePlaneRotationOffset(const FVector & Value) {
        BackgroundImagePlaneRotationOffset = Value;
    }
    static void SetBackgroundImagePlaneScale(const FVector & Value) {
        BackgroundImagePlaneScale = Value;
    }
    static void SetFontPath(const FString & Value) {
        FontPath = Value;
    }
    static void SetFontSize(const unsigned int Value) {
        FontSize = Value;
    }
    static void SetSignFontSize(const unsigned int Value) {
        SignFontSize = Value;
    }
    static void SetLetterPosition(const FVector2D & Value) {
        LetterPosition = Value;
    }
    static void SetASLTextPosition(const FVector2D & Value) {
        ASLTextPosition = Value;
    }
    static void SetSentencePosition(const FVector2D & Value) {
        SentencePosition = Value;
    }
    static void SetTokenPosition(const FVector2D & Value) {
        TokenPosition = Value;
    }
    static void SetUseEntireBackgroundForImages(const bool Value) {
        UseEntireBackgroundForImages = Value;
    }

private:
    FUISettings();
    static inline FString FontPath {""};
    static inline unsigned int FontSize {14};
    static inline unsigned int SignFontSize {14};
    static inline FVector BackgroundImagePlaneScale {1.25, 0.75, 1.0};
    static inline FVector BackgroundImagePlaneLocationOffset {200, 200, 200};
    static inline FVector BackgroundImagePlaneRotationOffset {200, 200, 200};
    static inline FVector2D ASLTextPosition {300, 300};
    static inline FVector2D LetterPosition {300, 300};
    static inline FVector2D SentencePosition {100, 100};
    static inline FVector2D TokenPosition {200, 200};
    static inline bool UseEntireBackgroundForImages {false};
};
}