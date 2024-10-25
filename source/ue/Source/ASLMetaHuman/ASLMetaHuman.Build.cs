/*
// * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// * SPDX-License-Identifier: MIT-0
// *
// * Permission is hereby granted, free of charge, to any person obtaining a copy of this
// * software and associated documentation files (the "Software"), to deal in the Software
// * without restriction, including without limitation the rights to use, copy, modify,
// * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// * permit persons to whom the Software is furnished to do so.
// *
// * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// */

using System;
using System.Collections.Generic;
using System.IO;

using UnrealBuildTool;

public class ASLMetaHuman : ModuleRules
{
    private readonly List<string> LibraryNames = new() {
        "aws-c-common",
        "aws-cpp-sdk-core",
        "aws-cpp-sdk-sqs",
        "aws-crt-cpp"
    };

    public ASLMetaHuman(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        MinCpuArchX64 = MinimumCpuArchitectureX64.AVX2;
        bEnableBufferSecurityChecks = true;
        bEnableExceptions = true;
        IWYUSupport = IWYUSupport.Full;
        bPrecompile = true;
        bUseRTTI = false;
        bUseUnity = false;
        MinFilesUsingPrecompiledHeaderOverride = 1;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrecompileForTargets = PrecompileTargetsType.Any;
        PrivatePCHHeaderFile = "Private/ASLMetaHuman.PCH.h";
        OptimizeCode = Target.Configuration != UnrealTargetConfiguration.Shipping ? CodeOptimization.Never : CodeOptimization.Always;

        // The AWS SDK relies on some identifiers being undefined for
        // correct behavior (not the same as being defined with value
        // 0). The unreal build tool treats warnings as errors, so
        // rather than disabling that behavior (strict is good), we
        // can just disable the warnings for undefined identifiers.
        // This needs to be done in all modules that depend directly
        // on the "AwsSdk" module.
        //
        bEnableUndefinedIdentifierWarnings = false;

        PublicDependencyModuleNames.AddRange(new string[] {
            "ControlRig",
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Json",
            "JsonUtilities",
            "Projects",
            "RenderCore",
            "RHI"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "ControlRig",
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "Json",
            "JsonUtilities",
            "ModelingOperators",
            "UMG",
        });

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new[] {
                "UnrealEd"
            });
        }

        // Dynamically linking to the SDK requires us to define the
        // USE_IMPORT_EXPORT symbol for all build targets using the
        // SDK. Source: https://github.com/aws/aws-sdk-cpp/blob/main/Docs/SDK_usage_guide.md#build-defines
        //
        PublicDefinitions.Add("USE_IMPORT_EXPORT");
        PublicDefinitions.Add("AWS_CRT_CPP_USE_IMPORT_EXPORT");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDefinitions.Add("USE_WINDOWS_DLL_SEMANTICS");
        }

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty", "AwsSdk", Target.Platform.ToString(), "include"));

        LibraryNames.ForEach(AddLibrary);
    }

    private void AddLibrary(string LibraryName)
    {
        var Platform = Target.Platform.ToString();
        var LibraryPath = Path.Combine(ModuleDirectory, "ThirdParty", "AwsSdk", Platform);

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Add the library with symbols required by the linker (.lib for dynamic libraries on windows).
            //
            PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "lib", LibraryName + ".lib"));

            // Stage the library along with the target, so it can be loaded at runtime.
            //
            RuntimeDependencies.Add(
                "$(BinaryOutputDir)/" + LibraryName + ".dll",
                Path.Combine(LibraryPath, "bin", LibraryName + ".dll")
            );
        }
        else
        {
            throw new PlatformNotSupportedException(
                "Platform " + Platform + " is not supported by the AwsSdk Module."
            );
        }
    }
}