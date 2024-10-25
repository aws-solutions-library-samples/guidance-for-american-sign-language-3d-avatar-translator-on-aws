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

// ASL-specific algorithms for parsing ASL-processed sentences into tokens (ideally, this logic would be decoupled/applied
// in an outside component, with finalized ASL tokens and other relevant information provided as input to this project)
//

#include "ASLAlgorithms.h"

using ASLMetaHuman::Core::ASLAlgorithms;

namespace {
constexpr auto AnimationNameWordDelimiterText {TEXT("_")};
const auto RegexValidWordCharacters {TEXT("[a-zA-Z\\w]+")};
}

// Determines the appropriate sequence of ASL tokens (signs) that can be used to represent an ASL sentence, removing non-alphabetic,
// non-whitespace characters. A provided dictionary indexes the word counts per ASL token. It is assumed that all invalid ASL words
// (including conjunctions, prepositions) were already removed, and certain word suffixes (including plural endings) were removed.
// 
// Note: a token may consist of multiple words if there is a multi-word translation represented by one sign / animation sequence.
// Tokens are produced by examining the largest amount of words available to substitute as one animation sequence, down to the
// shortest length whole word, down to individual letters (if there isn't an animation sequence that matches an ASL sign).
//
// Note: this isn't a true full-featured ASL translation algorithm, and it may suffer from token ties/word overlaps,
// but it's a starting point. A thesaurus approach could enhance interpretation further, though sentence context/structure
// information is also needed.
//
void ASLAlgorithms::GetSignTokensFromSentence(const TMap<unsigned int, TArray<FString>> * TranslatableTokensByWordCount,
        const FString & ASLSentence,
        TArray<FString> & Tokens) {

    Tokens.Reset();

    if (nullptr == TranslatableTokensByWordCount) {
        return;
    }

    // Break ASL sentence into words
    //
    TArray<FString> OneWordTokens;
    const FRegexPattern RegexPattern(RegexValidWordCharacters);
    FRegexMatcher RegexMatcher(RegexPattern, ASLSentence);
    while (RegexMatcher.FindNext()) {
        OneWordTokens.Add(RegexMatcher.GetCaptureGroup(0));
    }
    
    const FString & CombinedSentence = FString::Join(OneWordTokens, AnimationNameWordDelimiterText).ToUpper();
    // Divide And Conquer - where there are left and right join unions on the ASL tokens....
    //
    RecursiveGetTokens(TranslatableTokensByWordCount, CombinedSentence, Tokens);
}

// Recursively determines (via left and right partitions) a sequential list of ASL tokens through
// multi-word substitutions for one ASL expression. Single-word substitutions are done elsewhere.
//
void ASLAlgorithms::RecursiveGetTokens(const TMap<unsigned int, TArray<FString>> * TranslatableTokensByWordCount,
        const FString & Sentence,
        TArray<FString> & Tokens) {

    // Base cases: empty sentence or already broken into one word (no multi-words to substitute)
    //
    if (Sentence.IsEmpty()) {
        return;
    }
    if (Sentence.Find(AnimationNameWordDelimiterText) < 0) {
        FString ProcessedToken;
        Tokens.Add(Sentence);
        return;
    }

    // Attempt maximal to minimal token substitutions (i.e. examine multiple words matching to one ASL translation,
    // down to two words per ASL translation - one-word translations should be direct). WordCounts is assumed
    // to be sorted in descending order (largest to smallest).
    //
    // Note: the current approach can be made more efficient, but keeping it illustrate.
    //
    TArray<unsigned int> WordCounts;
    TranslatableTokensByWordCount->GetKeys(WordCounts);
    for (const auto WordCount: WordCounts) {
        if (WordCount <= 1) {
            break;
        }
        // Attempt one substitution at a time, break into left and right branches recursively, combine.
        // Note: these searches may become inefficient as phase size increases - there are more efficient techniques.
        //
        const auto TokensArrayPtr = TranslatableTokensByWordCount->Find(WordCount);
        if (nullptr != TokensArrayPtr) {
            for (const auto Token: *TokensArrayPtr) {
                const int PositionOfToken = Sentence.Find(Token);
                if (PositionOfToken >= 0) {
                    const FString & LeftString = Sentence.Mid(0, PositionOfToken - 1);
                    const FString & RightString = Sentence.Mid(PositionOfToken + Token.Len() + 1);
                    const FString & MiddleString = Token;
                    RecursiveGetTokens(TranslatableTokensByWordCount, LeftString, Tokens);
                    Tokens.Add(MiddleString);
                    RecursiveGetTokens(TranslatableTokensByWordCount, RightString, Tokens);
                    // Did not fail substitution of multi-word token - terminate
                    //
                    return;
                }
            }
        }
    }

    // Failed to substitute multi-word token case; therefore, add individual words that weren't yet translated:
    //
    if (Sentence.Find(AnimationNameWordDelimiterText) > 0) {
        TArray<FString> RemainderArray;
        Sentence.ParseIntoArray(RemainderArray, AnimationNameWordDelimiterText);
        for (const auto & Token: RemainderArray) {
            Tokens.Add(Token);
        }
    }
}