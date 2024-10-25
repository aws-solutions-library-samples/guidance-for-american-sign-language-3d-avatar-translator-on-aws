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

#include <Engine/World.h>
#include <Modules/ModuleManager.h>

#include "Config/ConfigStore.h"
#include "Core/ASLMetaHumanDemo.h"

using ASLMetaHuman::Core::ASLMetaHumanDemo;

DECLARE_LOG_CATEGORY_EXTERN(LogASLMetaHuman, Log, All);

namespace ASLMetaHuman {

/**
 * Memory manager wrapper to enable the AWS SDK to use UE's memory management
 * (FMemory support via overloaded new/delete operators by default) for dynamic allocations
 */
class AwsMemoryManagerWrapper: public Aws::Utils::Memory::MemorySystemInterface {
public:
    virtual void * AllocateMemory(std::size_t BlockSize, std::size_t Alignment, const char * AllocationTag = nullptr) override;
    virtual void FreeMemory(void * MemoryPtr) override;
    virtual void Begin() override {
    }
    virtual void End() override {
    }
};

/**
 * Manages critical runtime resources including loading and unloading of the AWS SDK libraries during
   module startup and shutdown.
 *
 * @see IModuleInterface for further details
 */
class FASLMetaHumanModule: public IModuleInterface {
public:
    FASLMetaHumanModule();
    virtual ~FASLMetaHumanModule();

    /**
     * Called right after the module DLL has been loaded and the module object has been created
     * Load dependent modules here, and they will be guaranteed to be available during ShutdownModule.
     * i.e. FModuleManager::Get().LoadModuleChecked(TEXT("ModuleName"));
     */
    virtual void StartupModule() override;

    /**
     * Called before the module is unloaded, right before the module object is destroyed.
     * During normal shutdown, this is called in reverse order that modules finish StartupModule().
     * This means that, as long as a module references dependent modules in it's StartupModule(), it
     * can safely reference those dependencies in ShutdownModule() as well.
     */
    virtual void ShutdownModule() override;

    virtual bool IsGameModule() const override {
        return true;
    }

private:
    // AWS SDK-specific
    //
    void InitAwsSDK();
    bool InitExternalConfig();
    void ShutdownAwsSDK();

    // Organize callback registration
    //
    void RegisterUECallbackHandlers();
    void DeregisterUECallbackHandlers() const;

    // UE state-specific callbacks
    //
    void OnEngineLoopInitComplete();
    void OnEnginePreExit();

    /**
     * Reference to Aws::SDKOptions is needed for passing the same instance to Aws::InitAPI() and Aws::ShutdownAPI()
     */
    void * AwsSDKOptionsInternalPtr {nullptr};
    bool AwsSDKInitialized {false};
    AwsMemoryManagerWrapper AwsMemoryManager;

    // Long-lasting access to the high-level demo logic
    //
    ASLMetaHumanDemo * DemoInstancePtr;

    // Accesses externally-defined settings
    //
    TWeakObjectPtr<UConfigStore> ConfigStore;
};
}