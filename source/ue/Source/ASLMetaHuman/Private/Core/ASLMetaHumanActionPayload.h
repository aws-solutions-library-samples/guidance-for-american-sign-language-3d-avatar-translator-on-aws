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

#include <ASLMetaHumanActionPayload.generated.h>

// Payload backing structure definition
// Note: FString's tend to be preferred for encoding data - can always be decoded to other types later
//

USTRUCT()
struct FASLMetaHumanActionPayload {
    GENERATED_BODY()
    // Contains action type - converts into EASLMetaHumanActionType
    //
    UPROPERTY()
    FString Action;

    // Contains primary-level data
    //
    UPROPERTY()
    FString Data;

    // Contains extended data - generic - may be another JSON payload itself (within)
    //
    UPROPERTY()
    TMap<FString, FString> kwargs;
};

// Backing-field enums that can be used across various payloads (including kwargs components)
//

// Defines the action that should be performed
//
UENUM()
enum class EASLMetaHumanActionType : uint8 {
    NONE,
    ANIMATE_SENTENCE,
    CHANGE_AVATAR,
    CHANGE_BACKGROUND,
    CHANGE_SIGN_RATE,
    STOP_ALL_ANIMATIONS,
};

// Defines the sentiment of a sentence
//
UENUM()
enum class EASLMetaHumanSentimentType : uint8 {
    NONE,
    POSITIVE,
    NEGATIVE,
    NEUTRAL,
    MIXED,
    SHOCKED
};

// Defines the tense of a sentence
// (note: for more advanced cases, it's possible to have multiple tenses and time adverbials etc.)
//
UENUM()
enum class EASLMetaHumanTenseType : uint8 {
    PAST,
    PRESENT,
    FUTURE
};

// Defines the ASL tense of a sentence
// (note: for more advanced cases, it's possible to have multiple tenses and time adverbials etc.)
//
UENUM()
enum class EASLMetaHumanASLTenseType : uint8 {
    FINISH,
    NOW,
    NEXT
};