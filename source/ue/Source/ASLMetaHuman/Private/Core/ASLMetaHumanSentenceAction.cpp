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
#include "ASLMetaHumanSentenceAction.h"

using ASLMetaHuman::Core::ASLMetaHumanAnimateSentenceAction;

// Provides routines for decoding the backing field data related to sentences
//

namespace {
const auto & MetaHumanSentimentTypeEnumName = TEXT("EASLMetaHumanSentimentType");
// Backing fields
//
const auto & ASLTextFieldName = TEXT("asl_text");
const auto & SentimentFieldName = TEXT("sentiment");
const auto & TenseFieldName = TEXT("tense");
const auto & TensePresent = FString("present");
const auto & TensePast = FString("past");
const auto & TenseFuture = FString("future");
const auto & ASLTensePresent = FString("now");
const auto & ASLTensePast = FString("finish");
const auto & ASLTenseFuture = FString("next");
}

// Returns the sentiment enum value corresponding to an Action object; else a value of EASLMetaHumanSentimentType::NONE
//
EASLMetaHumanSentimentType ASLMetaHumanAnimateSentenceAction::GetSentiment(const ASLMetaHumanAction & Action) {
    auto SentimentType = EASLMetaHumanSentimentType::NONE;
    const auto & SentimentValue = Action.GetActionKeywordArg(SentimentFieldName);
    const auto SentimentTypeEnum = FindFirstObjectSafe<UEnum>(MetaHumanSentimentTypeEnumName);
    if (SentimentTypeEnum) {
        const auto & Index = SentimentTypeEnum->GetIndexByNameString(SentimentValue);
        if (Index >= 0) {
            SentimentType = static_cast<EASLMetaHumanSentimentType>(static_cast<uint8>(Index));
        }
    }
    return SentimentType;
}

// Returns the ASL sentence's tense value corresponding to an Action object; possibly an empty value if omitted.
// Note: for more complicated cases, there could be multiple tenses in a sentence, time adverbials, etc.
// Consider using enums to represent tenses and map them, or feed in ASL tense directly (from external processing)
//
void ASLMetaHumanAnimateSentenceAction::GetASLTense(const ASLMetaHumanAction & Action, FString& Tense) {
    Tense.Empty();
    Tense = Action.GetActionKeywordArg(TenseFieldName);
    if (Tense.ToLower().Equals(TensePast)) {
        Tense = ASLTensePast;
    } else if (Tense.ToLower().Equals(TensePresent)) {
        Tense = ASLTensePresent;
    } else if (Tense.ToLower().Equals(TenseFuture)) {
        Tense = ASLTenseFuture;
    }
    Tense = Tense.ToUpper();
}

// Returns the ASL-formed text value corresponding to an Action object; possibly an empty value if omitted.
// Note: this could potentially become a gloss format that would need parsing.
// Note: most of the ASL text representation/formation (including algorithms) should be performed outside / be decoupled
//
void ASLMetaHumanAnimateSentenceAction::GetASLText(const ASLMetaHumanAction & Action, FString & Text) {
    Text.Empty();
    Text = Action.GetActionKeywordArg(ASLTextFieldName);
}