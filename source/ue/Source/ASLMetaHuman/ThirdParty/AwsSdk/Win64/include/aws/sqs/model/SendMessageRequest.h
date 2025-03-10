﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/sqs/SQS_EXPORTS.h>
#include <aws/sqs/SQSRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSMap.h>
#include <aws/sqs/model/MessageAttributeValue.h>
#include <aws/sqs/model/MessageSystemAttributeNameForSends.h>
#include <aws/sqs/model/MessageSystemAttributeValue.h>
#include <utility>

namespace Aws
{
namespace SQS
{
namespace Model
{

  /**
   * <p/><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/sqs-2012-11-05/SendMessageRequest">AWS
   * API Reference</a></p>
   */
  class SendMessageRequest : public SQSRequest
  {
  public:
    AWS_SQS_API SendMessageRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "SendMessage"; }

    AWS_SQS_API Aws::String SerializePayload() const override;

  protected:
    AWS_SQS_API void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>The URL of the Amazon SQS queue to which a message is sent.</p> <p>Queue URLs
     * and names are case-sensitive.</p>
     */
    inline const Aws::String& GetQueueUrl() const{ return m_queueUrl; }

    /**
     * <p>The URL of the Amazon SQS queue to which a message is sent.</p> <p>Queue URLs
     * and names are case-sensitive.</p>
     */
    inline bool QueueUrlHasBeenSet() const { return m_queueUrlHasBeenSet; }

    /**
     * <p>The URL of the Amazon SQS queue to which a message is sent.</p> <p>Queue URLs
     * and names are case-sensitive.</p>
     */
    inline void SetQueueUrl(const Aws::String& value) { m_queueUrlHasBeenSet = true; m_queueUrl = value; }

    /**
     * <p>The URL of the Amazon SQS queue to which a message is sent.</p> <p>Queue URLs
     * and names are case-sensitive.</p>
     */
    inline void SetQueueUrl(Aws::String&& value) { m_queueUrlHasBeenSet = true; m_queueUrl = std::move(value); }

    /**
     * <p>The URL of the Amazon SQS queue to which a message is sent.</p> <p>Queue URLs
     * and names are case-sensitive.</p>
     */
    inline void SetQueueUrl(const char* value) { m_queueUrlHasBeenSet = true; m_queueUrl.assign(value); }

    /**
     * <p>The URL of the Amazon SQS queue to which a message is sent.</p> <p>Queue URLs
     * and names are case-sensitive.</p>
     */
    inline SendMessageRequest& WithQueueUrl(const Aws::String& value) { SetQueueUrl(value); return *this;}

    /**
     * <p>The URL of the Amazon SQS queue to which a message is sent.</p> <p>Queue URLs
     * and names are case-sensitive.</p>
     */
    inline SendMessageRequest& WithQueueUrl(Aws::String&& value) { SetQueueUrl(std::move(value)); return *this;}

    /**
     * <p>The URL of the Amazon SQS queue to which a message is sent.</p> <p>Queue URLs
     * and names are case-sensitive.</p>
     */
    inline SendMessageRequest& WithQueueUrl(const char* value) { SetQueueUrl(value); return *this;}


    /**
     * <p>The message to send. The minimum size is one character. The maximum size is
     * 256 KiB.</p>  <p>A message can include only XML, JSON, and
     * unformatted text. The following Unicode characters are allowed:</p> <p>
     * <code>#x9</code> | <code>#xA</code> | <code>#xD</code> | <code>#x20</code> to
     * <code>#xD7FF</code> | <code>#xE000</code> to <code>#xFFFD</code> |
     * <code>#x10000</code> to <code>#x10FFFF</code> </p> <p>Any characters not
     * included in this list will be rejected. For more information, see the <a
     * href="http://www.w3.org/TR/REC-xml/#charsets">W3C specification for
     * characters</a>.</p> 
     */
    inline const Aws::String& GetMessageBody() const{ return m_messageBody; }

    /**
     * <p>The message to send. The minimum size is one character. The maximum size is
     * 256 KiB.</p>  <p>A message can include only XML, JSON, and
     * unformatted text. The following Unicode characters are allowed:</p> <p>
     * <code>#x9</code> | <code>#xA</code> | <code>#xD</code> | <code>#x20</code> to
     * <code>#xD7FF</code> | <code>#xE000</code> to <code>#xFFFD</code> |
     * <code>#x10000</code> to <code>#x10FFFF</code> </p> <p>Any characters not
     * included in this list will be rejected. For more information, see the <a
     * href="http://www.w3.org/TR/REC-xml/#charsets">W3C specification for
     * characters</a>.</p> 
     */
    inline bool MessageBodyHasBeenSet() const { return m_messageBodyHasBeenSet; }

    /**
     * <p>The message to send. The minimum size is one character. The maximum size is
     * 256 KiB.</p>  <p>A message can include only XML, JSON, and
     * unformatted text. The following Unicode characters are allowed:</p> <p>
     * <code>#x9</code> | <code>#xA</code> | <code>#xD</code> | <code>#x20</code> to
     * <code>#xD7FF</code> | <code>#xE000</code> to <code>#xFFFD</code> |
     * <code>#x10000</code> to <code>#x10FFFF</code> </p> <p>Any characters not
     * included in this list will be rejected. For more information, see the <a
     * href="http://www.w3.org/TR/REC-xml/#charsets">W3C specification for
     * characters</a>.</p> 
     */
    inline void SetMessageBody(const Aws::String& value) { m_messageBodyHasBeenSet = true; m_messageBody = value; }

    /**
     * <p>The message to send. The minimum size is one character. The maximum size is
     * 256 KiB.</p>  <p>A message can include only XML, JSON, and
     * unformatted text. The following Unicode characters are allowed:</p> <p>
     * <code>#x9</code> | <code>#xA</code> | <code>#xD</code> | <code>#x20</code> to
     * <code>#xD7FF</code> | <code>#xE000</code> to <code>#xFFFD</code> |
     * <code>#x10000</code> to <code>#x10FFFF</code> </p> <p>Any characters not
     * included in this list will be rejected. For more information, see the <a
     * href="http://www.w3.org/TR/REC-xml/#charsets">W3C specification for
     * characters</a>.</p> 
     */
    inline void SetMessageBody(Aws::String&& value) { m_messageBodyHasBeenSet = true; m_messageBody = std::move(value); }

    /**
     * <p>The message to send. The minimum size is one character. The maximum size is
     * 256 KiB.</p>  <p>A message can include only XML, JSON, and
     * unformatted text. The following Unicode characters are allowed:</p> <p>
     * <code>#x9</code> | <code>#xA</code> | <code>#xD</code> | <code>#x20</code> to
     * <code>#xD7FF</code> | <code>#xE000</code> to <code>#xFFFD</code> |
     * <code>#x10000</code> to <code>#x10FFFF</code> </p> <p>Any characters not
     * included in this list will be rejected. For more information, see the <a
     * href="http://www.w3.org/TR/REC-xml/#charsets">W3C specification for
     * characters</a>.</p> 
     */
    inline void SetMessageBody(const char* value) { m_messageBodyHasBeenSet = true; m_messageBody.assign(value); }

    /**
     * <p>The message to send. The minimum size is one character. The maximum size is
     * 256 KiB.</p>  <p>A message can include only XML, JSON, and
     * unformatted text. The following Unicode characters are allowed:</p> <p>
     * <code>#x9</code> | <code>#xA</code> | <code>#xD</code> | <code>#x20</code> to
     * <code>#xD7FF</code> | <code>#xE000</code> to <code>#xFFFD</code> |
     * <code>#x10000</code> to <code>#x10FFFF</code> </p> <p>Any characters not
     * included in this list will be rejected. For more information, see the <a
     * href="http://www.w3.org/TR/REC-xml/#charsets">W3C specification for
     * characters</a>.</p> 
     */
    inline SendMessageRequest& WithMessageBody(const Aws::String& value) { SetMessageBody(value); return *this;}

    /**
     * <p>The message to send. The minimum size is one character. The maximum size is
     * 256 KiB.</p>  <p>A message can include only XML, JSON, and
     * unformatted text. The following Unicode characters are allowed:</p> <p>
     * <code>#x9</code> | <code>#xA</code> | <code>#xD</code> | <code>#x20</code> to
     * <code>#xD7FF</code> | <code>#xE000</code> to <code>#xFFFD</code> |
     * <code>#x10000</code> to <code>#x10FFFF</code> </p> <p>Any characters not
     * included in this list will be rejected. For more information, see the <a
     * href="http://www.w3.org/TR/REC-xml/#charsets">W3C specification for
     * characters</a>.</p> 
     */
    inline SendMessageRequest& WithMessageBody(Aws::String&& value) { SetMessageBody(std::move(value)); return *this;}

    /**
     * <p>The message to send. The minimum size is one character. The maximum size is
     * 256 KiB.</p>  <p>A message can include only XML, JSON, and
     * unformatted text. The following Unicode characters are allowed:</p> <p>
     * <code>#x9</code> | <code>#xA</code> | <code>#xD</code> | <code>#x20</code> to
     * <code>#xD7FF</code> | <code>#xE000</code> to <code>#xFFFD</code> |
     * <code>#x10000</code> to <code>#x10FFFF</code> </p> <p>Any characters not
     * included in this list will be rejected. For more information, see the <a
     * href="http://www.w3.org/TR/REC-xml/#charsets">W3C specification for
     * characters</a>.</p> 
     */
    inline SendMessageRequest& WithMessageBody(const char* value) { SetMessageBody(value); return *this;}


    /**
     * <p> The length of time, in seconds, for which to delay a specific message. Valid
     * values: 0 to 900. Maximum: 15 minutes. Messages with a positive
     * <code>DelaySeconds</code> value become available for processing after the delay
     * period is finished. If you don't specify a value, the default value for the
     * queue applies. </p>  <p>When you set <code>FifoQueue</code>, you can't set
     * <code>DelaySeconds</code> per message. You can set this parameter only on a
     * queue level.</p> 
     */
    inline int GetDelaySeconds() const{ return m_delaySeconds; }

    /**
     * <p> The length of time, in seconds, for which to delay a specific message. Valid
     * values: 0 to 900. Maximum: 15 minutes. Messages with a positive
     * <code>DelaySeconds</code> value become available for processing after the delay
     * period is finished. If you don't specify a value, the default value for the
     * queue applies. </p>  <p>When you set <code>FifoQueue</code>, you can't set
     * <code>DelaySeconds</code> per message. You can set this parameter only on a
     * queue level.</p> 
     */
    inline bool DelaySecondsHasBeenSet() const { return m_delaySecondsHasBeenSet; }

    /**
     * <p> The length of time, in seconds, for which to delay a specific message. Valid
     * values: 0 to 900. Maximum: 15 minutes. Messages with a positive
     * <code>DelaySeconds</code> value become available for processing after the delay
     * period is finished. If you don't specify a value, the default value for the
     * queue applies. </p>  <p>When you set <code>FifoQueue</code>, you can't set
     * <code>DelaySeconds</code> per message. You can set this parameter only on a
     * queue level.</p> 
     */
    inline void SetDelaySeconds(int value) { m_delaySecondsHasBeenSet = true; m_delaySeconds = value; }

    /**
     * <p> The length of time, in seconds, for which to delay a specific message. Valid
     * values: 0 to 900. Maximum: 15 minutes. Messages with a positive
     * <code>DelaySeconds</code> value become available for processing after the delay
     * period is finished. If you don't specify a value, the default value for the
     * queue applies. </p>  <p>When you set <code>FifoQueue</code>, you can't set
     * <code>DelaySeconds</code> per message. You can set this parameter only on a
     * queue level.</p> 
     */
    inline SendMessageRequest& WithDelaySeconds(int value) { SetDelaySeconds(value); return *this;}


    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline const Aws::Map<Aws::String, MessageAttributeValue>& GetMessageAttributes() const{ return m_messageAttributes; }

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline bool MessageAttributesHasBeenSet() const { return m_messageAttributesHasBeenSet; }

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline void SetMessageAttributes(const Aws::Map<Aws::String, MessageAttributeValue>& value) { m_messageAttributesHasBeenSet = true; m_messageAttributes = value; }

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline void SetMessageAttributes(Aws::Map<Aws::String, MessageAttributeValue>&& value) { m_messageAttributesHasBeenSet = true; m_messageAttributes = std::move(value); }

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline SendMessageRequest& WithMessageAttributes(const Aws::Map<Aws::String, MessageAttributeValue>& value) { SetMessageAttributes(value); return *this;}

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline SendMessageRequest& WithMessageAttributes(Aws::Map<Aws::String, MessageAttributeValue>&& value) { SetMessageAttributes(std::move(value)); return *this;}

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline SendMessageRequest& AddMessageAttributes(const Aws::String& key, const MessageAttributeValue& value) { m_messageAttributesHasBeenSet = true; m_messageAttributes.emplace(key, value); return *this; }

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline SendMessageRequest& AddMessageAttributes(Aws::String&& key, const MessageAttributeValue& value) { m_messageAttributesHasBeenSet = true; m_messageAttributes.emplace(std::move(key), value); return *this; }

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline SendMessageRequest& AddMessageAttributes(const Aws::String& key, MessageAttributeValue&& value) { m_messageAttributesHasBeenSet = true; m_messageAttributes.emplace(key, std::move(value)); return *this; }

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline SendMessageRequest& AddMessageAttributes(Aws::String&& key, MessageAttributeValue&& value) { m_messageAttributesHasBeenSet = true; m_messageAttributes.emplace(std::move(key), std::move(value)); return *this; }

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline SendMessageRequest& AddMessageAttributes(const char* key, MessageAttributeValue&& value) { m_messageAttributesHasBeenSet = true; m_messageAttributes.emplace(key, std::move(value)); return *this; }

    /**
     * <p>Each message attribute consists of a <code>Name</code>, <code>Type</code>,
     * and <code>Value</code>. For more information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-message-metadata.html#sqs-message-attributes">Amazon
     * SQS message attributes</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     */
    inline SendMessageRequest& AddMessageAttributes(const char* key, const MessageAttributeValue& value) { m_messageAttributesHasBeenSet = true; m_messageAttributes.emplace(key, value); return *this; }


    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline const Aws::Map<MessageSystemAttributeNameForSends, MessageSystemAttributeValue>& GetMessageSystemAttributes() const{ return m_messageSystemAttributes; }

    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline bool MessageSystemAttributesHasBeenSet() const { return m_messageSystemAttributesHasBeenSet; }

    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline void SetMessageSystemAttributes(const Aws::Map<MessageSystemAttributeNameForSends, MessageSystemAttributeValue>& value) { m_messageSystemAttributesHasBeenSet = true; m_messageSystemAttributes = value; }

    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline void SetMessageSystemAttributes(Aws::Map<MessageSystemAttributeNameForSends, MessageSystemAttributeValue>&& value) { m_messageSystemAttributesHasBeenSet = true; m_messageSystemAttributes = std::move(value); }

    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline SendMessageRequest& WithMessageSystemAttributes(const Aws::Map<MessageSystemAttributeNameForSends, MessageSystemAttributeValue>& value) { SetMessageSystemAttributes(value); return *this;}

    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline SendMessageRequest& WithMessageSystemAttributes(Aws::Map<MessageSystemAttributeNameForSends, MessageSystemAttributeValue>&& value) { SetMessageSystemAttributes(std::move(value)); return *this;}

    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline SendMessageRequest& AddMessageSystemAttributes(const MessageSystemAttributeNameForSends& key, const MessageSystemAttributeValue& value) { m_messageSystemAttributesHasBeenSet = true; m_messageSystemAttributes.emplace(key, value); return *this; }

    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline SendMessageRequest& AddMessageSystemAttributes(MessageSystemAttributeNameForSends&& key, const MessageSystemAttributeValue& value) { m_messageSystemAttributesHasBeenSet = true; m_messageSystemAttributes.emplace(std::move(key), value); return *this; }

    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline SendMessageRequest& AddMessageSystemAttributes(const MessageSystemAttributeNameForSends& key, MessageSystemAttributeValue&& value) { m_messageSystemAttributesHasBeenSet = true; m_messageSystemAttributes.emplace(key, std::move(value)); return *this; }

    /**
     * <p>The message system attribute to send. Each message system attribute consists
     * of a <code>Name</code>, <code>Type</code>, and <code>Value</code>.</p>
     *  <ul> <li> <p>Currently, the only supported message system attribute
     * is <code>AWSTraceHeader</code>. Its type must be <code>String</code> and its
     * value must be a correctly formatted X-Ray trace header string.</p> </li> <li>
     * <p>The size of a message system attribute doesn't count towards the total size
     * of a message.</p> </li> </ul> 
     */
    inline SendMessageRequest& AddMessageSystemAttributes(MessageSystemAttributeNameForSends&& key, MessageSystemAttributeValue&& value) { m_messageSystemAttributesHasBeenSet = true; m_messageSystemAttributes.emplace(std::move(key), std::move(value)); return *this; }


    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * token used for deduplication of sent messages. If a message with a particular
     * <code>MessageDeduplicationId</code> is sent successfully, any messages sent with
     * the same <code>MessageDeduplicationId</code> are accepted successfully but
     * aren't delivered during the 5-minute deduplication interval. For more
     * information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-queues-exactly-once-processing.html">
     * Exactly-once processing</a> in the <i>Amazon SQS Developer Guide</i>.</p> <ul>
     * <li> <p>Every message must have a unique
     * <code>MessageDeduplicationId</code>,</p> <ul> <li> <p>You may provide a
     * <code>MessageDeduplicationId</code> explicitly.</p> </li> <li> <p>If you aren't
     * able to provide a <code>MessageDeduplicationId</code> and you enable
     * <code>ContentBasedDeduplication</code> for your queue, Amazon SQS uses a SHA-256
     * hash to generate the <code>MessageDeduplicationId</code> using the body of the
     * message (but not the attributes of the message). </p> </li> <li> <p>If you don't
     * provide a <code>MessageDeduplicationId</code> and the queue doesn't have
     * <code>ContentBasedDeduplication</code> set, the action fails with an error.</p>
     * </li> <li> <p>If the queue has <code>ContentBasedDeduplication</code> set, your
     * <code>MessageDeduplicationId</code> overrides the generated one.</p> </li> </ul>
     * </li> <li> <p>When <code>ContentBasedDeduplication</code> is in effect, messages
     * with identical content sent within the deduplication interval are treated as
     * duplicates and only one copy of the message is delivered.</p> </li> <li> <p>If
     * you send one message with <code>ContentBasedDeduplication</code> enabled and
     * then another message with a <code>MessageDeduplicationId</code> that is the same
     * as the one generated for the first <code>MessageDeduplicationId</code>, the two
     * messages are treated as duplicates and only one copy of the message is
     * delivered. </p> </li> </ul>  <p>The <code>MessageDeduplicationId</code> is
     * available to the consumer of the message (this can be useful for troubleshooting
     * delivery issues).</p> <p>If a message is sent successfully but the
     * acknowledgement is lost and the message is resent with the same
     * <code>MessageDeduplicationId</code> after the deduplication interval, Amazon SQS
     * can't detect duplicate messages.</p> <p>Amazon SQS continues to keep track of
     * the message deduplication ID even after the message is received and deleted.</p>
     *  <p>The maximum length of <code>MessageDeduplicationId</code> is 128
     * characters. <code>MessageDeduplicationId</code> can contain alphanumeric
     * characters (<code>a-z</code>, <code>A-Z</code>, <code>0-9</code>) and
     * punctuation (<code>!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~</code>).</p>
     * <p>For best practices of using <code>MessageDeduplicationId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagededuplicationid-property.html">Using
     * the MessageDeduplicationId Property</a> in the <i>Amazon SQS Developer
     * Guide</i>.</p>
     */
    inline const Aws::String& GetMessageDeduplicationId() const{ return m_messageDeduplicationId; }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * token used for deduplication of sent messages. If a message with a particular
     * <code>MessageDeduplicationId</code> is sent successfully, any messages sent with
     * the same <code>MessageDeduplicationId</code> are accepted successfully but
     * aren't delivered during the 5-minute deduplication interval. For more
     * information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-queues-exactly-once-processing.html">
     * Exactly-once processing</a> in the <i>Amazon SQS Developer Guide</i>.</p> <ul>
     * <li> <p>Every message must have a unique
     * <code>MessageDeduplicationId</code>,</p> <ul> <li> <p>You may provide a
     * <code>MessageDeduplicationId</code> explicitly.</p> </li> <li> <p>If you aren't
     * able to provide a <code>MessageDeduplicationId</code> and you enable
     * <code>ContentBasedDeduplication</code> for your queue, Amazon SQS uses a SHA-256
     * hash to generate the <code>MessageDeduplicationId</code> using the body of the
     * message (but not the attributes of the message). </p> </li> <li> <p>If you don't
     * provide a <code>MessageDeduplicationId</code> and the queue doesn't have
     * <code>ContentBasedDeduplication</code> set, the action fails with an error.</p>
     * </li> <li> <p>If the queue has <code>ContentBasedDeduplication</code> set, your
     * <code>MessageDeduplicationId</code> overrides the generated one.</p> </li> </ul>
     * </li> <li> <p>When <code>ContentBasedDeduplication</code> is in effect, messages
     * with identical content sent within the deduplication interval are treated as
     * duplicates and only one copy of the message is delivered.</p> </li> <li> <p>If
     * you send one message with <code>ContentBasedDeduplication</code> enabled and
     * then another message with a <code>MessageDeduplicationId</code> that is the same
     * as the one generated for the first <code>MessageDeduplicationId</code>, the two
     * messages are treated as duplicates and only one copy of the message is
     * delivered. </p> </li> </ul>  <p>The <code>MessageDeduplicationId</code> is
     * available to the consumer of the message (this can be useful for troubleshooting
     * delivery issues).</p> <p>If a message is sent successfully but the
     * acknowledgement is lost and the message is resent with the same
     * <code>MessageDeduplicationId</code> after the deduplication interval, Amazon SQS
     * can't detect duplicate messages.</p> <p>Amazon SQS continues to keep track of
     * the message deduplication ID even after the message is received and deleted.</p>
     *  <p>The maximum length of <code>MessageDeduplicationId</code> is 128
     * characters. <code>MessageDeduplicationId</code> can contain alphanumeric
     * characters (<code>a-z</code>, <code>A-Z</code>, <code>0-9</code>) and
     * punctuation (<code>!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~</code>).</p>
     * <p>For best practices of using <code>MessageDeduplicationId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagededuplicationid-property.html">Using
     * the MessageDeduplicationId Property</a> in the <i>Amazon SQS Developer
     * Guide</i>.</p>
     */
    inline bool MessageDeduplicationIdHasBeenSet() const { return m_messageDeduplicationIdHasBeenSet; }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * token used for deduplication of sent messages. If a message with a particular
     * <code>MessageDeduplicationId</code> is sent successfully, any messages sent with
     * the same <code>MessageDeduplicationId</code> are accepted successfully but
     * aren't delivered during the 5-minute deduplication interval. For more
     * information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-queues-exactly-once-processing.html">
     * Exactly-once processing</a> in the <i>Amazon SQS Developer Guide</i>.</p> <ul>
     * <li> <p>Every message must have a unique
     * <code>MessageDeduplicationId</code>,</p> <ul> <li> <p>You may provide a
     * <code>MessageDeduplicationId</code> explicitly.</p> </li> <li> <p>If you aren't
     * able to provide a <code>MessageDeduplicationId</code> and you enable
     * <code>ContentBasedDeduplication</code> for your queue, Amazon SQS uses a SHA-256
     * hash to generate the <code>MessageDeduplicationId</code> using the body of the
     * message (but not the attributes of the message). </p> </li> <li> <p>If you don't
     * provide a <code>MessageDeduplicationId</code> and the queue doesn't have
     * <code>ContentBasedDeduplication</code> set, the action fails with an error.</p>
     * </li> <li> <p>If the queue has <code>ContentBasedDeduplication</code> set, your
     * <code>MessageDeduplicationId</code> overrides the generated one.</p> </li> </ul>
     * </li> <li> <p>When <code>ContentBasedDeduplication</code> is in effect, messages
     * with identical content sent within the deduplication interval are treated as
     * duplicates and only one copy of the message is delivered.</p> </li> <li> <p>If
     * you send one message with <code>ContentBasedDeduplication</code> enabled and
     * then another message with a <code>MessageDeduplicationId</code> that is the same
     * as the one generated for the first <code>MessageDeduplicationId</code>, the two
     * messages are treated as duplicates and only one copy of the message is
     * delivered. </p> </li> </ul>  <p>The <code>MessageDeduplicationId</code> is
     * available to the consumer of the message (this can be useful for troubleshooting
     * delivery issues).</p> <p>If a message is sent successfully but the
     * acknowledgement is lost and the message is resent with the same
     * <code>MessageDeduplicationId</code> after the deduplication interval, Amazon SQS
     * can't detect duplicate messages.</p> <p>Amazon SQS continues to keep track of
     * the message deduplication ID even after the message is received and deleted.</p>
     *  <p>The maximum length of <code>MessageDeduplicationId</code> is 128
     * characters. <code>MessageDeduplicationId</code> can contain alphanumeric
     * characters (<code>a-z</code>, <code>A-Z</code>, <code>0-9</code>) and
     * punctuation (<code>!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~</code>).</p>
     * <p>For best practices of using <code>MessageDeduplicationId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagededuplicationid-property.html">Using
     * the MessageDeduplicationId Property</a> in the <i>Amazon SQS Developer
     * Guide</i>.</p>
     */
    inline void SetMessageDeduplicationId(const Aws::String& value) { m_messageDeduplicationIdHasBeenSet = true; m_messageDeduplicationId = value; }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * token used for deduplication of sent messages. If a message with a particular
     * <code>MessageDeduplicationId</code> is sent successfully, any messages sent with
     * the same <code>MessageDeduplicationId</code> are accepted successfully but
     * aren't delivered during the 5-minute deduplication interval. For more
     * information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-queues-exactly-once-processing.html">
     * Exactly-once processing</a> in the <i>Amazon SQS Developer Guide</i>.</p> <ul>
     * <li> <p>Every message must have a unique
     * <code>MessageDeduplicationId</code>,</p> <ul> <li> <p>You may provide a
     * <code>MessageDeduplicationId</code> explicitly.</p> </li> <li> <p>If you aren't
     * able to provide a <code>MessageDeduplicationId</code> and you enable
     * <code>ContentBasedDeduplication</code> for your queue, Amazon SQS uses a SHA-256
     * hash to generate the <code>MessageDeduplicationId</code> using the body of the
     * message (but not the attributes of the message). </p> </li> <li> <p>If you don't
     * provide a <code>MessageDeduplicationId</code> and the queue doesn't have
     * <code>ContentBasedDeduplication</code> set, the action fails with an error.</p>
     * </li> <li> <p>If the queue has <code>ContentBasedDeduplication</code> set, your
     * <code>MessageDeduplicationId</code> overrides the generated one.</p> </li> </ul>
     * </li> <li> <p>When <code>ContentBasedDeduplication</code> is in effect, messages
     * with identical content sent within the deduplication interval are treated as
     * duplicates and only one copy of the message is delivered.</p> </li> <li> <p>If
     * you send one message with <code>ContentBasedDeduplication</code> enabled and
     * then another message with a <code>MessageDeduplicationId</code> that is the same
     * as the one generated for the first <code>MessageDeduplicationId</code>, the two
     * messages are treated as duplicates and only one copy of the message is
     * delivered. </p> </li> </ul>  <p>The <code>MessageDeduplicationId</code> is
     * available to the consumer of the message (this can be useful for troubleshooting
     * delivery issues).</p> <p>If a message is sent successfully but the
     * acknowledgement is lost and the message is resent with the same
     * <code>MessageDeduplicationId</code> after the deduplication interval, Amazon SQS
     * can't detect duplicate messages.</p> <p>Amazon SQS continues to keep track of
     * the message deduplication ID even after the message is received and deleted.</p>
     *  <p>The maximum length of <code>MessageDeduplicationId</code> is 128
     * characters. <code>MessageDeduplicationId</code> can contain alphanumeric
     * characters (<code>a-z</code>, <code>A-Z</code>, <code>0-9</code>) and
     * punctuation (<code>!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~</code>).</p>
     * <p>For best practices of using <code>MessageDeduplicationId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagededuplicationid-property.html">Using
     * the MessageDeduplicationId Property</a> in the <i>Amazon SQS Developer
     * Guide</i>.</p>
     */
    inline void SetMessageDeduplicationId(Aws::String&& value) { m_messageDeduplicationIdHasBeenSet = true; m_messageDeduplicationId = std::move(value); }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * token used for deduplication of sent messages. If a message with a particular
     * <code>MessageDeduplicationId</code> is sent successfully, any messages sent with
     * the same <code>MessageDeduplicationId</code> are accepted successfully but
     * aren't delivered during the 5-minute deduplication interval. For more
     * information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-queues-exactly-once-processing.html">
     * Exactly-once processing</a> in the <i>Amazon SQS Developer Guide</i>.</p> <ul>
     * <li> <p>Every message must have a unique
     * <code>MessageDeduplicationId</code>,</p> <ul> <li> <p>You may provide a
     * <code>MessageDeduplicationId</code> explicitly.</p> </li> <li> <p>If you aren't
     * able to provide a <code>MessageDeduplicationId</code> and you enable
     * <code>ContentBasedDeduplication</code> for your queue, Amazon SQS uses a SHA-256
     * hash to generate the <code>MessageDeduplicationId</code> using the body of the
     * message (but not the attributes of the message). </p> </li> <li> <p>If you don't
     * provide a <code>MessageDeduplicationId</code> and the queue doesn't have
     * <code>ContentBasedDeduplication</code> set, the action fails with an error.</p>
     * </li> <li> <p>If the queue has <code>ContentBasedDeduplication</code> set, your
     * <code>MessageDeduplicationId</code> overrides the generated one.</p> </li> </ul>
     * </li> <li> <p>When <code>ContentBasedDeduplication</code> is in effect, messages
     * with identical content sent within the deduplication interval are treated as
     * duplicates and only one copy of the message is delivered.</p> </li> <li> <p>If
     * you send one message with <code>ContentBasedDeduplication</code> enabled and
     * then another message with a <code>MessageDeduplicationId</code> that is the same
     * as the one generated for the first <code>MessageDeduplicationId</code>, the two
     * messages are treated as duplicates and only one copy of the message is
     * delivered. </p> </li> </ul>  <p>The <code>MessageDeduplicationId</code> is
     * available to the consumer of the message (this can be useful for troubleshooting
     * delivery issues).</p> <p>If a message is sent successfully but the
     * acknowledgement is lost and the message is resent with the same
     * <code>MessageDeduplicationId</code> after the deduplication interval, Amazon SQS
     * can't detect duplicate messages.</p> <p>Amazon SQS continues to keep track of
     * the message deduplication ID even after the message is received and deleted.</p>
     *  <p>The maximum length of <code>MessageDeduplicationId</code> is 128
     * characters. <code>MessageDeduplicationId</code> can contain alphanumeric
     * characters (<code>a-z</code>, <code>A-Z</code>, <code>0-9</code>) and
     * punctuation (<code>!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~</code>).</p>
     * <p>For best practices of using <code>MessageDeduplicationId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagededuplicationid-property.html">Using
     * the MessageDeduplicationId Property</a> in the <i>Amazon SQS Developer
     * Guide</i>.</p>
     */
    inline void SetMessageDeduplicationId(const char* value) { m_messageDeduplicationIdHasBeenSet = true; m_messageDeduplicationId.assign(value); }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * token used for deduplication of sent messages. If a message with a particular
     * <code>MessageDeduplicationId</code> is sent successfully, any messages sent with
     * the same <code>MessageDeduplicationId</code> are accepted successfully but
     * aren't delivered during the 5-minute deduplication interval. For more
     * information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-queues-exactly-once-processing.html">
     * Exactly-once processing</a> in the <i>Amazon SQS Developer Guide</i>.</p> <ul>
     * <li> <p>Every message must have a unique
     * <code>MessageDeduplicationId</code>,</p> <ul> <li> <p>You may provide a
     * <code>MessageDeduplicationId</code> explicitly.</p> </li> <li> <p>If you aren't
     * able to provide a <code>MessageDeduplicationId</code> and you enable
     * <code>ContentBasedDeduplication</code> for your queue, Amazon SQS uses a SHA-256
     * hash to generate the <code>MessageDeduplicationId</code> using the body of the
     * message (but not the attributes of the message). </p> </li> <li> <p>If you don't
     * provide a <code>MessageDeduplicationId</code> and the queue doesn't have
     * <code>ContentBasedDeduplication</code> set, the action fails with an error.</p>
     * </li> <li> <p>If the queue has <code>ContentBasedDeduplication</code> set, your
     * <code>MessageDeduplicationId</code> overrides the generated one.</p> </li> </ul>
     * </li> <li> <p>When <code>ContentBasedDeduplication</code> is in effect, messages
     * with identical content sent within the deduplication interval are treated as
     * duplicates and only one copy of the message is delivered.</p> </li> <li> <p>If
     * you send one message with <code>ContentBasedDeduplication</code> enabled and
     * then another message with a <code>MessageDeduplicationId</code> that is the same
     * as the one generated for the first <code>MessageDeduplicationId</code>, the two
     * messages are treated as duplicates and only one copy of the message is
     * delivered. </p> </li> </ul>  <p>The <code>MessageDeduplicationId</code> is
     * available to the consumer of the message (this can be useful for troubleshooting
     * delivery issues).</p> <p>If a message is sent successfully but the
     * acknowledgement is lost and the message is resent with the same
     * <code>MessageDeduplicationId</code> after the deduplication interval, Amazon SQS
     * can't detect duplicate messages.</p> <p>Amazon SQS continues to keep track of
     * the message deduplication ID even after the message is received and deleted.</p>
     *  <p>The maximum length of <code>MessageDeduplicationId</code> is 128
     * characters. <code>MessageDeduplicationId</code> can contain alphanumeric
     * characters (<code>a-z</code>, <code>A-Z</code>, <code>0-9</code>) and
     * punctuation (<code>!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~</code>).</p>
     * <p>For best practices of using <code>MessageDeduplicationId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagededuplicationid-property.html">Using
     * the MessageDeduplicationId Property</a> in the <i>Amazon SQS Developer
     * Guide</i>.</p>
     */
    inline SendMessageRequest& WithMessageDeduplicationId(const Aws::String& value) { SetMessageDeduplicationId(value); return *this;}

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * token used for deduplication of sent messages. If a message with a particular
     * <code>MessageDeduplicationId</code> is sent successfully, any messages sent with
     * the same <code>MessageDeduplicationId</code> are accepted successfully but
     * aren't delivered during the 5-minute deduplication interval. For more
     * information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-queues-exactly-once-processing.html">
     * Exactly-once processing</a> in the <i>Amazon SQS Developer Guide</i>.</p> <ul>
     * <li> <p>Every message must have a unique
     * <code>MessageDeduplicationId</code>,</p> <ul> <li> <p>You may provide a
     * <code>MessageDeduplicationId</code> explicitly.</p> </li> <li> <p>If you aren't
     * able to provide a <code>MessageDeduplicationId</code> and you enable
     * <code>ContentBasedDeduplication</code> for your queue, Amazon SQS uses a SHA-256
     * hash to generate the <code>MessageDeduplicationId</code> using the body of the
     * message (but not the attributes of the message). </p> </li> <li> <p>If you don't
     * provide a <code>MessageDeduplicationId</code> and the queue doesn't have
     * <code>ContentBasedDeduplication</code> set, the action fails with an error.</p>
     * </li> <li> <p>If the queue has <code>ContentBasedDeduplication</code> set, your
     * <code>MessageDeduplicationId</code> overrides the generated one.</p> </li> </ul>
     * </li> <li> <p>When <code>ContentBasedDeduplication</code> is in effect, messages
     * with identical content sent within the deduplication interval are treated as
     * duplicates and only one copy of the message is delivered.</p> </li> <li> <p>If
     * you send one message with <code>ContentBasedDeduplication</code> enabled and
     * then another message with a <code>MessageDeduplicationId</code> that is the same
     * as the one generated for the first <code>MessageDeduplicationId</code>, the two
     * messages are treated as duplicates and only one copy of the message is
     * delivered. </p> </li> </ul>  <p>The <code>MessageDeduplicationId</code> is
     * available to the consumer of the message (this can be useful for troubleshooting
     * delivery issues).</p> <p>If a message is sent successfully but the
     * acknowledgement is lost and the message is resent with the same
     * <code>MessageDeduplicationId</code> after the deduplication interval, Amazon SQS
     * can't detect duplicate messages.</p> <p>Amazon SQS continues to keep track of
     * the message deduplication ID even after the message is received and deleted.</p>
     *  <p>The maximum length of <code>MessageDeduplicationId</code> is 128
     * characters. <code>MessageDeduplicationId</code> can contain alphanumeric
     * characters (<code>a-z</code>, <code>A-Z</code>, <code>0-9</code>) and
     * punctuation (<code>!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~</code>).</p>
     * <p>For best practices of using <code>MessageDeduplicationId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagededuplicationid-property.html">Using
     * the MessageDeduplicationId Property</a> in the <i>Amazon SQS Developer
     * Guide</i>.</p>
     */
    inline SendMessageRequest& WithMessageDeduplicationId(Aws::String&& value) { SetMessageDeduplicationId(std::move(value)); return *this;}

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * token used for deduplication of sent messages. If a message with a particular
     * <code>MessageDeduplicationId</code> is sent successfully, any messages sent with
     * the same <code>MessageDeduplicationId</code> are accepted successfully but
     * aren't delivered during the 5-minute deduplication interval. For more
     * information, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-queues-exactly-once-processing.html">
     * Exactly-once processing</a> in the <i>Amazon SQS Developer Guide</i>.</p> <ul>
     * <li> <p>Every message must have a unique
     * <code>MessageDeduplicationId</code>,</p> <ul> <li> <p>You may provide a
     * <code>MessageDeduplicationId</code> explicitly.</p> </li> <li> <p>If you aren't
     * able to provide a <code>MessageDeduplicationId</code> and you enable
     * <code>ContentBasedDeduplication</code> for your queue, Amazon SQS uses a SHA-256
     * hash to generate the <code>MessageDeduplicationId</code> using the body of the
     * message (but not the attributes of the message). </p> </li> <li> <p>If you don't
     * provide a <code>MessageDeduplicationId</code> and the queue doesn't have
     * <code>ContentBasedDeduplication</code> set, the action fails with an error.</p>
     * </li> <li> <p>If the queue has <code>ContentBasedDeduplication</code> set, your
     * <code>MessageDeduplicationId</code> overrides the generated one.</p> </li> </ul>
     * </li> <li> <p>When <code>ContentBasedDeduplication</code> is in effect, messages
     * with identical content sent within the deduplication interval are treated as
     * duplicates and only one copy of the message is delivered.</p> </li> <li> <p>If
     * you send one message with <code>ContentBasedDeduplication</code> enabled and
     * then another message with a <code>MessageDeduplicationId</code> that is the same
     * as the one generated for the first <code>MessageDeduplicationId</code>, the two
     * messages are treated as duplicates and only one copy of the message is
     * delivered. </p> </li> </ul>  <p>The <code>MessageDeduplicationId</code> is
     * available to the consumer of the message (this can be useful for troubleshooting
     * delivery issues).</p> <p>If a message is sent successfully but the
     * acknowledgement is lost and the message is resent with the same
     * <code>MessageDeduplicationId</code> after the deduplication interval, Amazon SQS
     * can't detect duplicate messages.</p> <p>Amazon SQS continues to keep track of
     * the message deduplication ID even after the message is received and deleted.</p>
     *  <p>The maximum length of <code>MessageDeduplicationId</code> is 128
     * characters. <code>MessageDeduplicationId</code> can contain alphanumeric
     * characters (<code>a-z</code>, <code>A-Z</code>, <code>0-9</code>) and
     * punctuation (<code>!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~</code>).</p>
     * <p>For best practices of using <code>MessageDeduplicationId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagededuplicationid-property.html">Using
     * the MessageDeduplicationId Property</a> in the <i>Amazon SQS Developer
     * Guide</i>.</p>
     */
    inline SendMessageRequest& WithMessageDeduplicationId(const char* value) { SetMessageDeduplicationId(value); return *this;}


    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * tag that specifies that a message belongs to a specific message group. Messages
     * that belong to the same message group are processed in a FIFO manner (however,
     * messages in different message groups might be processed out of order). To
     * interleave multiple ordered streams within a single queue, use
     * <code>MessageGroupId</code> values (for example, session data for multiple
     * users). In this scenario, multiple consumers can process the queue, but the
     * session data of each user is processed in a FIFO fashion.</p> <ul> <li> <p>You
     * must associate a non-empty <code>MessageGroupId</code> with a message. If you
     * don't provide a <code>MessageGroupId</code>, the action fails.</p> </li> <li>
     * <p> <code>ReceiveMessage</code> might return messages with multiple
     * <code>MessageGroupId</code> values. For each <code>MessageGroupId</code>, the
     * messages are sorted by time sent. The caller can't specify a
     * <code>MessageGroupId</code>.</p> </li> </ul> <p>The length of
     * <code>MessageGroupId</code> is 128 characters. Valid values: alphanumeric
     * characters and punctuation
     * <code>(!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~)</code>.</p> <p>For best
     * practices of using <code>MessageGroupId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagegroupid-property.html">Using
     * the MessageGroupId Property</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     *  <p> <code>MessageGroupId</code> is required for FIFO queues. You
     * can't use it for Standard queues.</p> 
     */
    inline const Aws::String& GetMessageGroupId() const{ return m_messageGroupId; }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * tag that specifies that a message belongs to a specific message group. Messages
     * that belong to the same message group are processed in a FIFO manner (however,
     * messages in different message groups might be processed out of order). To
     * interleave multiple ordered streams within a single queue, use
     * <code>MessageGroupId</code> values (for example, session data for multiple
     * users). In this scenario, multiple consumers can process the queue, but the
     * session data of each user is processed in a FIFO fashion.</p> <ul> <li> <p>You
     * must associate a non-empty <code>MessageGroupId</code> with a message. If you
     * don't provide a <code>MessageGroupId</code>, the action fails.</p> </li> <li>
     * <p> <code>ReceiveMessage</code> might return messages with multiple
     * <code>MessageGroupId</code> values. For each <code>MessageGroupId</code>, the
     * messages are sorted by time sent. The caller can't specify a
     * <code>MessageGroupId</code>.</p> </li> </ul> <p>The length of
     * <code>MessageGroupId</code> is 128 characters. Valid values: alphanumeric
     * characters and punctuation
     * <code>(!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~)</code>.</p> <p>For best
     * practices of using <code>MessageGroupId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagegroupid-property.html">Using
     * the MessageGroupId Property</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     *  <p> <code>MessageGroupId</code> is required for FIFO queues. You
     * can't use it for Standard queues.</p> 
     */
    inline bool MessageGroupIdHasBeenSet() const { return m_messageGroupIdHasBeenSet; }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * tag that specifies that a message belongs to a specific message group. Messages
     * that belong to the same message group are processed in a FIFO manner (however,
     * messages in different message groups might be processed out of order). To
     * interleave multiple ordered streams within a single queue, use
     * <code>MessageGroupId</code> values (for example, session data for multiple
     * users). In this scenario, multiple consumers can process the queue, but the
     * session data of each user is processed in a FIFO fashion.</p> <ul> <li> <p>You
     * must associate a non-empty <code>MessageGroupId</code> with a message. If you
     * don't provide a <code>MessageGroupId</code>, the action fails.</p> </li> <li>
     * <p> <code>ReceiveMessage</code> might return messages with multiple
     * <code>MessageGroupId</code> values. For each <code>MessageGroupId</code>, the
     * messages are sorted by time sent. The caller can't specify a
     * <code>MessageGroupId</code>.</p> </li> </ul> <p>The length of
     * <code>MessageGroupId</code> is 128 characters. Valid values: alphanumeric
     * characters and punctuation
     * <code>(!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~)</code>.</p> <p>For best
     * practices of using <code>MessageGroupId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagegroupid-property.html">Using
     * the MessageGroupId Property</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     *  <p> <code>MessageGroupId</code> is required for FIFO queues. You
     * can't use it for Standard queues.</p> 
     */
    inline void SetMessageGroupId(const Aws::String& value) { m_messageGroupIdHasBeenSet = true; m_messageGroupId = value; }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * tag that specifies that a message belongs to a specific message group. Messages
     * that belong to the same message group are processed in a FIFO manner (however,
     * messages in different message groups might be processed out of order). To
     * interleave multiple ordered streams within a single queue, use
     * <code>MessageGroupId</code> values (for example, session data for multiple
     * users). In this scenario, multiple consumers can process the queue, but the
     * session data of each user is processed in a FIFO fashion.</p> <ul> <li> <p>You
     * must associate a non-empty <code>MessageGroupId</code> with a message. If you
     * don't provide a <code>MessageGroupId</code>, the action fails.</p> </li> <li>
     * <p> <code>ReceiveMessage</code> might return messages with multiple
     * <code>MessageGroupId</code> values. For each <code>MessageGroupId</code>, the
     * messages are sorted by time sent. The caller can't specify a
     * <code>MessageGroupId</code>.</p> </li> </ul> <p>The length of
     * <code>MessageGroupId</code> is 128 characters. Valid values: alphanumeric
     * characters and punctuation
     * <code>(!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~)</code>.</p> <p>For best
     * practices of using <code>MessageGroupId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagegroupid-property.html">Using
     * the MessageGroupId Property</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     *  <p> <code>MessageGroupId</code> is required for FIFO queues. You
     * can't use it for Standard queues.</p> 
     */
    inline void SetMessageGroupId(Aws::String&& value) { m_messageGroupIdHasBeenSet = true; m_messageGroupId = std::move(value); }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * tag that specifies that a message belongs to a specific message group. Messages
     * that belong to the same message group are processed in a FIFO manner (however,
     * messages in different message groups might be processed out of order). To
     * interleave multiple ordered streams within a single queue, use
     * <code>MessageGroupId</code> values (for example, session data for multiple
     * users). In this scenario, multiple consumers can process the queue, but the
     * session data of each user is processed in a FIFO fashion.</p> <ul> <li> <p>You
     * must associate a non-empty <code>MessageGroupId</code> with a message. If you
     * don't provide a <code>MessageGroupId</code>, the action fails.</p> </li> <li>
     * <p> <code>ReceiveMessage</code> might return messages with multiple
     * <code>MessageGroupId</code> values. For each <code>MessageGroupId</code>, the
     * messages are sorted by time sent. The caller can't specify a
     * <code>MessageGroupId</code>.</p> </li> </ul> <p>The length of
     * <code>MessageGroupId</code> is 128 characters. Valid values: alphanumeric
     * characters and punctuation
     * <code>(!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~)</code>.</p> <p>For best
     * practices of using <code>MessageGroupId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagegroupid-property.html">Using
     * the MessageGroupId Property</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     *  <p> <code>MessageGroupId</code> is required for FIFO queues. You
     * can't use it for Standard queues.</p> 
     */
    inline void SetMessageGroupId(const char* value) { m_messageGroupIdHasBeenSet = true; m_messageGroupId.assign(value); }

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * tag that specifies that a message belongs to a specific message group. Messages
     * that belong to the same message group are processed in a FIFO manner (however,
     * messages in different message groups might be processed out of order). To
     * interleave multiple ordered streams within a single queue, use
     * <code>MessageGroupId</code> values (for example, session data for multiple
     * users). In this scenario, multiple consumers can process the queue, but the
     * session data of each user is processed in a FIFO fashion.</p> <ul> <li> <p>You
     * must associate a non-empty <code>MessageGroupId</code> with a message. If you
     * don't provide a <code>MessageGroupId</code>, the action fails.</p> </li> <li>
     * <p> <code>ReceiveMessage</code> might return messages with multiple
     * <code>MessageGroupId</code> values. For each <code>MessageGroupId</code>, the
     * messages are sorted by time sent. The caller can't specify a
     * <code>MessageGroupId</code>.</p> </li> </ul> <p>The length of
     * <code>MessageGroupId</code> is 128 characters. Valid values: alphanumeric
     * characters and punctuation
     * <code>(!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~)</code>.</p> <p>For best
     * practices of using <code>MessageGroupId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagegroupid-property.html">Using
     * the MessageGroupId Property</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     *  <p> <code>MessageGroupId</code> is required for FIFO queues. You
     * can't use it for Standard queues.</p> 
     */
    inline SendMessageRequest& WithMessageGroupId(const Aws::String& value) { SetMessageGroupId(value); return *this;}

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * tag that specifies that a message belongs to a specific message group. Messages
     * that belong to the same message group are processed in a FIFO manner (however,
     * messages in different message groups might be processed out of order). To
     * interleave multiple ordered streams within a single queue, use
     * <code>MessageGroupId</code> values (for example, session data for multiple
     * users). In this scenario, multiple consumers can process the queue, but the
     * session data of each user is processed in a FIFO fashion.</p> <ul> <li> <p>You
     * must associate a non-empty <code>MessageGroupId</code> with a message. If you
     * don't provide a <code>MessageGroupId</code>, the action fails.</p> </li> <li>
     * <p> <code>ReceiveMessage</code> might return messages with multiple
     * <code>MessageGroupId</code> values. For each <code>MessageGroupId</code>, the
     * messages are sorted by time sent. The caller can't specify a
     * <code>MessageGroupId</code>.</p> </li> </ul> <p>The length of
     * <code>MessageGroupId</code> is 128 characters. Valid values: alphanumeric
     * characters and punctuation
     * <code>(!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~)</code>.</p> <p>For best
     * practices of using <code>MessageGroupId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagegroupid-property.html">Using
     * the MessageGroupId Property</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     *  <p> <code>MessageGroupId</code> is required for FIFO queues. You
     * can't use it for Standard queues.</p> 
     */
    inline SendMessageRequest& WithMessageGroupId(Aws::String&& value) { SetMessageGroupId(std::move(value)); return *this;}

    /**
     * <p>This parameter applies only to FIFO (first-in-first-out) queues.</p> <p>The
     * tag that specifies that a message belongs to a specific message group. Messages
     * that belong to the same message group are processed in a FIFO manner (however,
     * messages in different message groups might be processed out of order). To
     * interleave multiple ordered streams within a single queue, use
     * <code>MessageGroupId</code> values (for example, session data for multiple
     * users). In this scenario, multiple consumers can process the queue, but the
     * session data of each user is processed in a FIFO fashion.</p> <ul> <li> <p>You
     * must associate a non-empty <code>MessageGroupId</code> with a message. If you
     * don't provide a <code>MessageGroupId</code>, the action fails.</p> </li> <li>
     * <p> <code>ReceiveMessage</code> might return messages with multiple
     * <code>MessageGroupId</code> values. For each <code>MessageGroupId</code>, the
     * messages are sorted by time sent. The caller can't specify a
     * <code>MessageGroupId</code>.</p> </li> </ul> <p>The length of
     * <code>MessageGroupId</code> is 128 characters. Valid values: alphanumeric
     * characters and punctuation
     * <code>(!"#$%&amp;'()*+,-./:;&lt;=&gt;?@[\]^_`{|}~)</code>.</p> <p>For best
     * practices of using <code>MessageGroupId</code>, see <a
     * href="https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/using-messagegroupid-property.html">Using
     * the MessageGroupId Property</a> in the <i>Amazon SQS Developer Guide</i>.</p>
     *  <p> <code>MessageGroupId</code> is required for FIFO queues. You
     * can't use it for Standard queues.</p> 
     */
    inline SendMessageRequest& WithMessageGroupId(const char* value) { SetMessageGroupId(value); return *this;}

  private:

    Aws::String m_queueUrl;
    bool m_queueUrlHasBeenSet = false;

    Aws::String m_messageBody;
    bool m_messageBodyHasBeenSet = false;

    int m_delaySeconds;
    bool m_delaySecondsHasBeenSet = false;

    Aws::Map<Aws::String, MessageAttributeValue> m_messageAttributes;
    bool m_messageAttributesHasBeenSet = false;

    Aws::Map<MessageSystemAttributeNameForSends, MessageSystemAttributeValue> m_messageSystemAttributes;
    bool m_messageSystemAttributesHasBeenSet = false;

    Aws::String m_messageDeduplicationId;
    bool m_messageDeduplicationIdHasBeenSet = false;

    Aws::String m_messageGroupId;
    bool m_messageGroupIdHasBeenSet = false;
  };

} // namespace Model
} // namespace SQS
} // namespace Aws
