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

// Represents a SQS Background Worker (separate thread) that will accept requests for processing.
//

#include "AsynchronousSQSWorker.h"
#include "Config/GlobalState.h"
#include "Config/InternalSettings.h"
#include "Utilities/UnrealAPI.h"

#include <aws/core/client/DefaultRetryStrategy.h>
#include <aws/sqs/model/DeleteMessageRequest.h>
#include <aws/sqs/model/GetQueueUrlRequest.h>
#include <aws/sqs/model/PurgeQueueRequest.h>
#include <aws/sqs/model/ReceiveMessageRequest.h>

using ASLMetaHuman::Config::FGlobalState;
using ASLMetaHuman::Config::FInternalSettings;
using ASLMetaHuman::Core::FAsynchronousSqsWorker;
using ASLMetaHuman::Utilities::UnrealAPI;

namespace {
constexpr unsigned int MaxMessagesToReceive = 1;
// For resiliency parameters
//
constexpr float RequestTimeoutMs = 2 * 1000.0f;
constexpr int RequestRetries = 1;
// Console status-related messages
//
constexpr auto & ErrorDeletingFromQueueFormatted = TEXT("Error: failed to delete message from queue: %s ");
constexpr auto & ErrorInitializationFailed = TEXT("Init Async SQS worker failed");
constexpr auto & ErrorQueueUrlFormatted = TEXT("Failed to get SQS Queue Url: %s");
constexpr auto & ErrorPurgingQueueFormatted = TEXT("Error failed to purge queue: %s ");
constexpr auto & ErrorReceivingMessageFormatted = TEXT("Error receiving message from queue: %s ");
constexpr auto & InfoMessageDeleted = TEXT("Message deleted");
constexpr auto & InfoMessageReceivedFormatted = TEXT("Message received from SQS Queue: %s");
constexpr auto & InfoNoMessageReceived = TEXT("No messages received from queue");
constexpr auto & InfoQueuePurged = TEXT("Queue purged");
constexpr auto & InfoQueueUrlFormatted = TEXT("Queue Url: %s");
}

// Note: maintains an ActionHandler callback for received actions
//
FAsynchronousSqsWorker::FAsynchronousSqsWorker(
        const TDelegate<void(const ASLMetaHumanAction &)> & ExternalActionHandlerDelegate):
            ActionHandlerDelegate {ExternalActionHandlerDelegate} {
}

// Aids basic shutdown - triggered from ASLMetaHumanDemo::Shutdown() / Reset()
//
FAsynchronousSqsWorker::~FAsynchronousSqsWorker() {
    if (nullptr != AwsSQSClient) {
        AwsSQSClient->ShutdownSdkClient(nullptr);
        AwsSQSClient.Reset();
    }
}

// Main entry point / busy-wait work loop for this background SQS Worker - triggered via StartBackgroundTask()
// Retrieves queued action requests, prioritizes them.
//
void FAsynchronousSqsWorker::DoWork() {
    if (! InitAwsClient()) {
        UE_LOG(LogTemp, Log, ErrorInitializationFailed);
        return;
    }
    if (FInternalSettings::GetPurgeQueuesOnStartup()) {
        if (! ClearQueue(ImmediateActionQueueUrl) && ClearQueue(TranslationActionQueueUrl)) {
            return;
        }
    }
    while (! FGlobalState::IsAborting()) {
        FPlatformProcess::Sleep(FInternalSettings::GetSQSSpinlockSeconds());
        if (! FGlobalState::IsAborting()) {
            // Prioritize queued actions that require immediate processing
            //
            RequestNextQueuedMessage(ImmediateActionQueueUrl, true);
            if (IsReadyForNextTranslationMessage()) {
                // Each queued translations is considered long-running (shouldn't interrupt)
                //           
                RequestNextQueuedMessage(TranslationActionQueueUrl);
            }
        }
    }
}

// Configures and instantiates the SQS Client. Also establishes its Queue URL's.
// Returns true if the SQS Queue URL's could be determined; false otherwise.
//
bool FAsynchronousSqsWorker::InitAwsClient() {
    Aws::Client::ClientConfiguration ClientConfig;

    // Resiliency parameters
    //
    std::shared_ptr<Aws::Client::RetryStrategy> RetryStrategy;
    RetryStrategy.reset(new Aws::Client::DefaultRetryStrategy(RequestRetries, 0));
    ClientConfig.retryStrategy = RetryStrategy;
    ClientConfig.connectTimeoutMs = RequestTimeoutMs;
    ClientConfig.httpRequestTimeoutMs = RequestTimeoutMs;
    ClientConfig.requestTimeoutMs = RequestTimeoutMs;
    ClientConfig.tcpKeepAliveIntervalMs = RequestTimeoutMs;

    AwsSQSClient = MakeUnique<Aws::SQS::SQSClient>(ClientConfig);

    return GetQueueUrl(UnrealAPI::FStringToAwsString(FInternalSettings::GetSQSActionQueueName()), ImmediateActionQueueUrl)
            && GetQueueUrl(UnrealAPI::FStringToAwsString(FInternalSettings::GetSQSTranslationQueueName()),
                    TranslationActionQueueUrl);
}

// Gets the SQS Queue URL that matches a given queue name.
// Returns true if the SQS Queue URL could be determined; false otherwise.
//
bool FAsynchronousSqsWorker::GetQueueUrl(const Aws::String & QueueName, Aws::String & QueueUrl) const {
    Aws::SQS::Model::GetQueueUrlRequest QueueRequest;
    QueueRequest.SetQueueName(QueueName);
    const Aws::SQS::Model::GetQueueUrlOutcome Outcome = AwsSQSClient->GetQueueUrl(QueueRequest);
    if (FGlobalState::IsAborting()) {
        return false;
    }
    if (Outcome.IsSuccess()) {
        QueueUrl = Outcome.GetResult().GetQueueUrl();
        UE_LOG(LogTemp, Log, InfoQueueUrlFormatted, *UnrealAPI::AwsStringToFString(QueueName));
    } else {
        const auto & ErrorMessage = UnrealAPI::AwsStringToFString(Outcome.GetError().GetMessage());
        UE_LOG(LogTemp, Error, ErrorQueueUrlFormatted, *ErrorMessage);
    }
    return Outcome.IsSuccess();
}

// Attempts to request the next (FIFO-based) queued message if one is available
// Only one message is processed at a time; future messages are delayed until the current message is processed.
// If there's an error before message processing, then the next message won't be delayed
// Returns ture if retrieving the next message was successful; false otherwise
//
bool FAsynchronousSqsWorker::RequestNextQueuedMessage(const Aws::String & QueueUrl,
        const bool WantOnDemandMessage) const {
    // Note: the 'ready for next translate message' state will be re-enabled from outside of this class
    // (i.e. only accept a new sentence to translate when the existing animation sequence finishes playing - determined outside)
    // On-demand (non-translate) messages will always be handled without delay
    //
    if (! WantOnDemandMessage) {
        SetReadyForNextTranslateMessage(false);
    }
    Aws::SQS::Model::ReceiveMessageRequest MessageRequest;
    MessageRequest.SetQueueUrl(QueueUrl);
    MessageRequest.SetMaxNumberOfMessages(MaxMessagesToReceive);
    // Note: messages can be sent via a lambda function response, triggered through various AWS services -
    // in order to reach this SQS FIFO queue.
    //
    const Aws::SQS::Model::ReceiveMessageOutcome Outcome = AwsSQSClient->ReceiveMessage(MessageRequest);
    if (Outcome.IsSuccess()) {
        const Aws::Vector<Aws::SQS::Model::Message> & Messages = Outcome.GetResult().GetMessages();
        if (! Messages.empty()) {
            const Aws::SQS::Model::Message & NewMessage = Messages[0];
            const auto & MessageBody = UnrealAPI::AwsStringToFString(NewMessage.GetBody());
            UE_LOG(LogTemp, Log, InfoMessageReceivedFormatted, *MessageBody);
            // Message doesn't need to remain in the queue - we have it
            // Consider adding additional handling in case a message fails to remove from its queue
            //
            bool Status = DeleteQueuedMessage(QueueUrl, NewMessage.GetReceiptHandle());
            ProcessMessage(MessageBody);
        } else {
            UE_LOG(LogTemp, Log, InfoNoMessageReceived);
            if (! WantOnDemandMessage) {
                SetReadyForNextTranslateMessage(true);
            }
        }
    } else {
        const auto & ErrorMessage = UnrealAPI::AwsStringToFString(Outcome.GetError().GetMessage());
        UE_LOG(LogTemp, Error, ErrorReceivingMessageFormatted, *ErrorMessage);
        if (! WantOnDemandMessage) {
            SetReadyForNextTranslateMessage(true);
        }
    }
    return Outcome.IsSuccess();
}

// High-level method to decode a raw incoming generic action request (which was parsed from a SQS message) and
// then forward its decoded action to a handler that will invoke the logic for that requested action
//
void FAsynchronousSqsWorker::ProcessMessage(const FString & Message) const {
    const auto & Action = ASLMetaHumanAction(Message);
    ActionHandlerDelegate.ExecuteIfBound(Action);
}

// Deletes a particular queued message (MessageReceiptHandle) from a queue and returns the success value of that operation.
//
bool FAsynchronousSqsWorker::DeleteQueuedMessage(const Aws::String & QueueUrl,
        const Aws::String & MessageReceiptHandle) const {
    Aws::SQS::Model::DeleteMessageRequest MessageRequest;
    MessageRequest.SetQueueUrl(QueueUrl);
    MessageRequest.SetReceiptHandle(MessageReceiptHandle);
    const Aws::SQS::Model::DeleteMessageOutcome Outcome = AwsSQSClient->DeleteMessage(MessageRequest);
    if (Outcome.IsSuccess()) {
        UE_LOG(LogTemp, Log, InfoMessageDeleted);
    } else {
        const auto & ErrorMessage = UnrealAPI::AwsStringToFString(Outcome.GetError().GetMessage());
        UE_LOG(LogTemp, Error, ErrorDeletingFromQueueFormatted, *ErrorMessage);
    }
    return Outcome.IsSuccess();
}

// Purges the provided configured queue of its messages. Returns the success value of that operation.
// Note: if the queue isn't purging its messages, then check its configuration:
// i.e. "Error failed to purge queue: Only one PurgeQueue operation on <Queue> is allowed every <X> seconds."
//
bool FAsynchronousSqsWorker::ClearQueue(const Aws::String & QueueUrl) const {
    Aws::SQS::Model::PurgeQueueRequest QueueRequest;
    QueueRequest.SetQueueUrl(QueueUrl);
    const Aws::SQS::Model::PurgeQueueOutcome Outcome = AwsSQSClient->PurgeQueue(QueueRequest);
    if (Outcome.IsSuccess()) {
        UE_LOG(LogTemp, Log, InfoQueuePurged);
    } else {
        const auto & ErrorMessage = UnrealAPI::AwsStringToFString(Outcome.GetError().GetMessage());
        UE_LOG(LogTemp, Error, ErrorPurgingQueueFormatted, *ErrorMessage);
    }
    return Outcome.IsSuccess();
}