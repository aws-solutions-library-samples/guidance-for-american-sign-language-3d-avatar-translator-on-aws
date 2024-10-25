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

#include "ASLMetaHumanActionPayload.h"

// Provides routines for decoding overall backing field data from a raw payload form
// into Action objects. Determines a general action type and makes its data accessible.
//

namespace ASLMetaHuman::Core {

class ASLMetaHumanAction {
public:
    explicit ASLMetaHumanAction(const FString & JsonMessagePayload);
    EASLMetaHumanActionType GetActionType() const;
    void GetActionData(FString & Data) const;
    void GetActionKeywordArgs(TMap<FString, FString> & Data) const;
    FString GetActionKeywordArg(const FString & Key) const;

private:
    ASLMetaHumanAction() = default;
    bool InitAction(const FString & JsonMessagePayload);
    EASLMetaHumanActionType ActionType {EASLMetaHumanActionType::NONE};
    FString ActionData {""};
    TMap<FString, FString> ActionKeywordArgs {};
};
}
