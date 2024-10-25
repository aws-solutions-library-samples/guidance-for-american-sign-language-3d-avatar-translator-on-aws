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

// Provides routines for decoding overall backing field data from a raw payload form
// into Action objects. Determines a general action type and makes its data accessible.
//

#include "ASLMetaHumanAction.h"

#include <JsonObjectConverter.h>

using ASLMetaHuman::Core::ASLMetaHumanAction;

 
namespace {
const auto & MetaHumanActionTypeEnumName = TEXT("EASLMetaHumanActionType");
}

ASLMetaHumanAction::ASLMetaHumanAction(const FString & JsonMessagePayload) {
    InitAction(JsonMessagePayload);
}

// Deserializes the JSON Payload into a corresponding backing structure (referenced internally).
// Returns true if the JSON payload was valid and could be decoded; false otherwise.
// Note: missing fields in payload aren't an issue - they'll be defaulted.
// Note: the JSON Payload needs to have all string-based fields and values be double-quoted!
//
bool ASLMetaHumanAction::InitAction(const FString & JsonMessagePayload) {
    FASLMetaHumanActionPayload ActionBackingStructure;
    if (! FJsonObjectConverter::JsonObjectStringToUStruct<FASLMetaHumanActionPayload>(
                JsonMessagePayload, &ActionBackingStructure, 0, 0)) {
        return false;
    }
    ActionType = EASLMetaHumanActionType::NONE;
    const auto ActionTypeEnum = FindFirstObjectSafe<UEnum>(MetaHumanActionTypeEnumName);
    if (ActionTypeEnum) {
        const auto & Index = ActionTypeEnum->GetIndexByNameString(ActionBackingStructure.Action);
        if (Index >= 0) {
            ActionType = static_cast<EASLMetaHumanActionType>(static_cast<uint8>(Index));
        }
    }
    ActionData = ActionBackingStructure.Data;
    ActionKeywordArgs = ActionBackingStructure.kwargs;
    return true;
}

/* The following methods retrieve an Action's field values
*/

EASLMetaHumanActionType ASLMetaHumanAction::GetActionType() const {
    return ActionType;
}

void ASLMetaHumanAction::GetActionData(FString & Data) const {
    Data = ActionData;
}

void ASLMetaHumanAction::GetActionKeywordArgs(TMap<FString, FString> & Data) const {
    Data = ActionKeywordArgs;
}

FString ASLMetaHumanAction::GetActionKeywordArg(const FString & Key) const {
    const auto FoundValue = ActionKeywordArgs.Find(Key);
    if (nullptr != FoundValue) {
        return *FoundValue;
    }
    return "";
}