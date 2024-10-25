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

// High-level interface to ASLMetaHuman project
// - Contains startup/shutdown entry points, UE callback handlers
// - Starts AWS SDK initialization
//
// Reminders:
// - Ensure that your system has the following environment variables set prior to command line invocation:
//      AWS_ACCESS_KEY_ID, AWS_SECRET_ACCESS_KEY, AWS_SESSION_TOKEN
//  (values should match the AWS environment targeted with appropriate (unexpired) values)
// - Consider modifying the time duration in which AWS_SESSION_TOKEN expires
// - Perform clean builds (in case of inconsistencies/crashing during shader cooking operations)
// - Cook the project in case StartupModule() isn't reached; else an exception in WinMain() may be raised
// - If you encounter obscure threading-related crashes, try adjusting AnimationSpinlockSeconds = 0.2 (in ASLMetaHuman.ini).
//

#include "ASLMetaHuman.h"
#include "Core/ASLMetaHumanDemo.h"

#include <Engine/World.h>
#include <Misc/CoreDelegates.h>

// Convenience mechanism for debugging needs
//
#define WANT_BREAKPOINT_AT_STARTUP false
#define LOCTEXT_NAMESPACE "FASLMetaHuman"
DEFINE_LOG_CATEGORY(LogASLMetaHuman);

using ASLMetaHuman::AwsMemoryManagerWrapper;
using ASLMetaHuman::FASLMetaHumanModule;
using ASLMetaHuman::Core::ASLMetaHumanDemo;

namespace {
const FString & BreakpointHitMessage {"Breakpoint Hit!"};
}

//*******************************************************************
// AWS SDK-specific memory allocators/handling
//*******************************************************************

void * AwsMemoryManagerWrapper::AllocateMemory(std::size_t BlockSize, std::size_t, const char *) {
    return ::operator new(BlockSize);
}

void AwsMemoryManagerWrapper::FreeMemory(void * MemoryPtr) {
    return ::operator delete(MemoryPtr);
}

FASLMetaHumanModule::FASLMetaHumanModule(): AwsSDKOptionsInternalPtr(new Aws::SDKOptions),DemoInstancePtr(nullptr) {
}

FASLMetaHumanModule::~FASLMetaHumanModule() {
    delete static_cast<Aws::SDKOptions *>(AwsSDKOptionsInternalPtr);
    AwsSDKOptionsInternalPtr = nullptr;
}

// Main entry point - calls high-level routines
//
void FASLMetaHumanModule::StartupModule() {
#if WANT_BREAKPOINT_AT_STARTUP
    FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Yes, FText::FromString(BreakpointHitMessage));
#endif
    if (! InitExternalConfig())
        return;
    RegisterUECallbackHandlers();
    InitAwsSDK();
}

// Main shutdown point (from UE)
//
void FASLMetaHumanModule::ShutdownModule() {
    // Re-attempts AWS SDK cleanup in case of prior failure during OnEnginePreExit()
    //
    ShutdownAwsSDK();
}

// Read and process external configuration - returns true if it processes successfully
//
bool FASLMetaHumanModule::InitExternalConfig() {
    UConfigStore::GetInstance(ConfigStore);
    return ConfigStore->IsValidLowLevelFast() && ConfigStore->ReloadConfig();
}

// AWS SDK Initialization
//
void FASLMetaHumanModule::InitAwsSDK() {
    const auto AwsSdkOptions = static_cast<Aws::SDKOptions *>(AwsSDKOptionsInternalPtr);
    AwsSdkOptions->memoryManagementOptions.memoryManager = &AwsMemoryManager;
    Aws::InitAPI(*AwsSdkOptions);
    AwsSDKInitialized = true;
}

void FASLMetaHumanModule::ShutdownAwsSDK() {
    if (! AwsSDKInitialized) {
        return;
    }
    AwsSDKInitialized = false;
    Aws::ShutdownAPI(*static_cast<Aws::SDKOptions *>(AwsSDKOptionsInternalPtr));
}

// Register UE-specific callbacks - activated based on the Engine's state change
//
void FASLMetaHumanModule::RegisterUECallbackHandlers() {
    FCoreDelegates::OnFEngineLoopInitComplete.AddRaw(this, &FASLMetaHumanModule::OnEngineLoopInitComplete);
    FCoreDelegates::OnEnginePreExit.AddRaw(this, &FASLMetaHumanModule::OnEnginePreExit);
}

// De-register UE-specific callbacks - ensure these are matched to their registered callbacks
//
void FASLMetaHumanModule::DeregisterUECallbackHandlers() const {
    FCoreDelegates::OnFEngineLoopInitComplete.RemoveAll(this);
    FCoreDelegates::OnEnginePreExit.RemoveAll(this);
}

// Spawns detached demo singleton (intended to be long-running, hosts the main demo)
//
void FASLMetaHumanModule::OnEngineLoopInitComplete() {
    DemoInstancePtr = ASLMetaHumanDemo::GetInstance();
}

// Clean up the demo singleton, start AWS SDK shutdown
//
void FASLMetaHumanModule::OnEnginePreExit() {
    if (nullptr != DemoInstancePtr) {
        DemoInstancePtr->Shutdown();
        ShutdownAwsSDK();
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE(FASLMetaHumanModule, ASLMetaHuman, "ASLMetaHuman");