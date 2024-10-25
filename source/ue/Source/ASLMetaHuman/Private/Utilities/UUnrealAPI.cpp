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

// Provides access to utility functions that are UClass-specific
//

#include "UUnrealAPI.h"

 /* Forwarding callback functions to non-UClass's
*/
void UUnrealAPI::OnAssign2DTexture(UTexture2DDynamic * Texture) {
    DownloadTextureDelegate.ExecuteIfBound(Texture);
}

void UUnrealAPI::OnAssign2DTextureFail(UTexture2DDynamic *) {
    DownloadTextureDelegate.ExecuteIfBound(nullptr);
}

// Wrapper for downloading a texture (to forwarding to non-UClass's)
// Accepts a signed URL that refers to the texture (image file); if download is successful, asynchronously
// invokes ExternalDownloadTextureDelegate and provides it a dynamic texture
//
void UUnrealAPI::DownloadTexture(const FString & SignedUrl, const TDelegate<void(const UTexture2DDynamic *)> & ExternalDownloadTextureDelegate) {
    DownloadTextureDelegate = ExternalDownloadTextureDelegate;
    if (! DownloadTaskPtr->IsValidLowLevelFast()) {
        DownloadTaskPtr = NewObject<UAsyncTaskDownloadImage>();}
        DownloadTaskPtr->OnSuccess.AddUniqueDynamic(this, &UUnrealAPI::OnAssign2DTexture);
        DownloadTaskPtr->OnFail.AddUniqueDynamic(this, &UUnrealAPI::OnAssign2DTextureFail);
    if (! DownloadTaskPtr->IsValidLowLevelFast()) {
        return;
    }
    DownloadTaskPtr->Start(SignedUrl);
}