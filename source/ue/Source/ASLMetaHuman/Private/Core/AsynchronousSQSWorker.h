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

// Represents a SQS Background Worker (separate thread) that will accept requests for processing.
//

#include <ModelingTaskTypes.h>

#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>

namespace ASLMetaHuman::Core {

class FAsynchronousSqsWorker: public UE::Geometry::FAbortableBackgroundTask {

public:
    explicit FAsynchronousSqsWorker(const TDelegate<void(const ASLMetaHumanAction &)> & ExternalActionHandlerDelegate);
    ~FAsynchronousSqsWorker();

    void DoWork();

    bool InitAwsClient();

    FORCEINLINE TStatId GetStatId() const {
        RETURN_QUICK_DECLARE_CYCLE_STAT(FAsynchronousSQSWorker, STATGROUP_ThreadPoolAsyncTasks);
    }

    static bool IsReadyForNextTranslationMessage() {
        FScopeLock ScopeLock(&MutexReadyForNextTranslationMessage);
        return ReadyForNextMessage;
    }

    static void SetReadyForNextTranslateMessage(const bool State) {
        FScopeLock ScopeLock(&MutexReadyForNextTranslationMessage);
        ReadyForNextMessage = State;
    }

private:
    bool GetQueueUrl(const Aws::String & QueueName, Aws::String & QueueUrl) const;
    bool RequestNextQueuedMessage(const Aws::String & QueueUrl, const bool WantOnDemandMessage = false) const;
    bool ClearQueue(const Aws::String & QueueUrl) const;
    bool DeleteQueuedMessage(const Aws::String & QueueUrl, const Aws::String & MessageReceiptHandle) const;
    void ProcessMessage(const FString & Message) const;

    TUniquePtr<Aws::SQS::SQSClient> AwsSQSClient;

    Aws::String ImmediateActionQueueUrl;
    Aws::String TranslationActionQueueUrl;
    std::map<const Aws::String, const Aws::String> AvailableQueues;
    TDelegate<void(const ASLMetaHumanAction &)> ActionHandlerDelegate;

    // Tracks shutdown request state and message retrieval readiness
    //
    static inline bool AbortActive = false;
    static inline bool ReadyForNextMessage = true;
    static inline FCriticalSection MutexAbortActive;
    static inline FCriticalSection MutexReadyForNextTranslationMessage;
};
}