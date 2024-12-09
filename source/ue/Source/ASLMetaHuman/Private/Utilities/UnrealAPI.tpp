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

// Provides access to templated utility functions that wrap the UE API
//

#pragma once

#include "AssetRegistry/AssetRegistryModule.h"

using ASLMetaHuman::Utilities::UnrealAPI;

namespace {
const FName AssetRegistryName {"AssetRegistry"};
const FString CompiledActorSuffix {"_C"};
const FString DuplicateActorSuffix {"_1"};
const FString UnderscoreActorSuffix {"_"};
}

// Provides a requested asset of type matching TypeOfAsset, given an asset path. Returns true if there's a valid
// unboxing/conversion to that asset; false otherwise (includes case where AssetRegistry is still loading assets).
//
// WARNING: If you can't find an asset programmatically (but you can see it in a matching Content path folder):
// - check DefaultGame.ini for bCookAll=True (is it set to false?) and:
//      +DirectoriesToAlwaysStageAsUFS=(Path="FolderWithStuff")
//      +DirectoriesToAlwaysCook=(Path="/Game/FolderWithStuff")
// - check if asset was cooked
// - invoke "Fix Up Redirectors"" on the ""Content Browser" folder
// - check the cooking log
// - consider the stage that the asset is being loaded
// - check for similar assets/or all assets (including any misspellings)
// - check for a "_C" suffix (this may be missing if there's a compiled BP at runtime)
//
template <class TypeOfAsset>
bool UnrealAPI::GetAsset(const FString & PathToAsset, TWeakObjectPtr<TypeOfAsset> & AssetPtr) {
    const auto & AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryName);
    if (AssetRegistryModule.GetRegistry().IsLoadingAssets()) {
        return false;
    }
    FAssetData AssetData;
    AssetRegistryModule.TryGetAssetByObjectPath(PathToAsset, AssetData);
    if (AssetData.IsValid()) {
        const auto ClassType = AssetData.GetClass();
        if ((nullptr != ClassType) && (ClassType->IsChildOf<TypeOfAsset>())) {
            AssetPtr = Cast<TypeOfAsset>(AssetData.GetAsset());
            return true;
        }
    }
    return false;
}

// Provides assets of type matching TypeOfAsset, which are within an asset path. Returns true if there's a
// valid asset path with assets detected; false otherwise.
//
template <class TypeOfAsset>
bool UnrealAPI::GetAssets(const FString & PathToAsset, TArray<TWeakObjectPtr<TypeOfAsset>> & AssetsPtr) {
    AssetsPtr.Reset();
    UObjectLibrary * ObjectLibrary = UObjectLibrary::CreateLibrary(TypeOfAsset::StaticClass(), false, GIsEditor);
    ObjectLibrary->AddToRoot();
    ObjectLibrary->LoadAssetDataFromPath(PathToAsset);
    ObjectLibrary->LoadAssetsFromAssetData();
    TArray<FAssetData> AssetDataList;
    ObjectLibrary->GetAssetDataList(AssetDataList);
    const unsigned int NumAssets = static_cast<unsigned int>(AssetDataList.Num());
    if (! NumAssets) {
        return false;
    }
    AssetsPtr.Reserve(NumAssets);
    for (unsigned int i = 0; i < NumAssets; i++) {
        TWeakObjectPtr<TypeOfAsset> AssetPtr;
        GetAsset(AssetDataList[i].GetObjectPathString(), AssetPtr);
        AssetsPtr.Add(AssetPtr);
    }
    return true;
}

// Attempts to hide all actors of type TypeOfActor, offering to hide Blueprint-specific actors
//
template <class TypeOfActor>
void UnrealAPI::HideAllActors(const UWorld * World, const bool BlueprintsOnly) {
    if ((nullptr == World) || (! World->IsValidLowLevelFast())) {
        return;
    }
    const auto Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&World, BlueprintsOnly]() {
                for (TActorIterator<TypeOfActor> ActorIterator(World); ActorIterator; ++ActorIterator) {
                    TypeOfActor * Actor = Cast<TypeOfActor>(*ActorIterator);
                    if ((nullptr != Actor) && (Actor->IsValidLowLevelFast())) {
                        if (BlueprintsOnly) {
                            if (Actor->GetClass()->IsInBlueprint()) {
                                Actor->SetActorHiddenInGame(true);
                            }
                        } else {
                            Actor->SetActorHiddenInGame(true);
                        }
                    }
                }
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    Task->Wait();
}

// Provides the first found top-level Actor (ActorPtr) given a name of that Actor. Returns true if there's a valid
// unboxing/conversion to that Actor; false otherwise.
//
template <class TypeOfActor>
bool UnrealAPI::GetActorByName(const FString & ActorName,
        const UWorld * World,
        TWeakObjectPtr<TypeOfActor> & ActorPtr) {
    TArray<AActor *> FoundActors;
    const auto Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&World, &FoundActors]() {
                UGameplayStatics::GetAllActorsOfClass(World, TypeOfActor::StaticClass(), FoundActors);
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    Task->Wait();
    for (const auto & Actor: FoundActors) {
        const FString & TActorName = Actor->GetHumanReadableName();
        if (ActorName == TActorName) {
            ActorPtr = Cast<TypeOfActor>(Actor);
            return nullptr != ActorPtr;
        }
    }
    return false;
}

// Provides a requested top-level Actor (ActorPtr) of type TypeOfActor given an object path to that Actor.
// Returns true if there's a valid unboxing/conversion to that Actor; false otherwise. Note: considers compiled
// Actor objects as being equivalent to the requested Actor object and initializes it.
//
template <class TypeOfActor>
bool UnrealAPI::GetActorByPath(const FString & ObjectPathToActor,
        const UWorld * World,
        TWeakObjectPtr<TypeOfActor> & ActorPtr) {
    const auto ActorClass = TSoftClassPtr<TypeOfActor>(FSoftObjectPath(ObjectPathToActor));
    if ((nullptr == ActorClass) || (! ActorClass.IsValid())) {
        return false;
    }
    if ((nullptr == World) || (! World->IsValidLowLevelFast())) {
        return false;
    }
    const auto LoadedAsset = ActorClass.LoadSynchronous();
    if ((nullptr == LoadedAsset) || (! LoadedAsset->IsValidLowLevelFast())) {
        return false;
    }
    TArray<AActor *> FoundActors;
    const auto Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [&World, &LoadedAsset, &FoundActors]() {
                UGameplayStatics::GetAllActorsOfClass(World, LoadedAsset, FoundActors);
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    Task->Wait();
    const FString & UActorName = ActorClass->GetName();
    for (const auto & Actor: FoundActors) {
        const FString & TActorName = Actor->GetActorNameOrLabel();
        // Note: this is a workaround to find a matching actor accommodates compiled BP actors.
        // Note: you may find that there are ActorName_C_<numbers> designations for the desired actor.
        // Warning: you can still have actor name clashes; regex is recommended.
        //
        if ((TActorName == UActorName) || (TActorName + CompiledActorSuffix == UActorName) || (TActorName == UActorName + DuplicateActorSuffix)
                || (TActorName.StartsWith(UActorName + UnderscoreActorSuffix))) {
            ActorPtr = Cast<TypeOfActor>(Actor);
            return nullptr != ActorPtr;
        }
    }
    return false;
}

// Helper method to hide a specific type (TypeOfWidget) of Slate Widget.
//
template <class TypeOfWidget>
void UnrealAPI::HideWidget(const TSharedRef<TypeOfWidget> & Widget) {
    const auto& Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [Widget]() {
                TWeakObjectPtr<UWorld> WorldPtrInternal;
                if (! GetWorld(WorldPtrInternal)) {
                    return;
                }
                const auto ViewportClientInternal = WorldPtrInternal->GetGameViewport();
                if (nullptr == ViewportClientInternal) {
                    return;
                }
                // Note: likely to lead to GC processing
                //
                ViewportClientInternal->RemoveViewportWidgetForPlayer(
                        WorldPtrInternal->GetFirstLocalPlayerFromController(), Widget);
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    Task->Wait();
}