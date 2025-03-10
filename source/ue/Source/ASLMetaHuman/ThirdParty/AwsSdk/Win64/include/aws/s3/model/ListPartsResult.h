﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/s3/S3_EXPORTS.h>
#include <aws/core/utils/DateTime.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/s3/model/Initiator.h>
#include <aws/s3/model/Owner.h>
#include <aws/s3/model/StorageClass.h>
#include <aws/s3/model/RequestCharged.h>
#include <aws/s3/model/ChecksumAlgorithm.h>
#include <aws/s3/model/Part.h>
#include <utility>

namespace Aws
{
template<typename RESULT_TYPE>
class AmazonWebServiceResult;

namespace Utils
{
namespace Xml
{
  class XmlDocument;
} // namespace Xml
} // namespace Utils
namespace S3
{
namespace Model
{
  class ListPartsResult
  {
  public:
    AWS_S3_API ListPartsResult();
    AWS_S3_API ListPartsResult(const Aws::AmazonWebServiceResult<Aws::Utils::Xml::XmlDocument>& result);
    AWS_S3_API ListPartsResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Xml::XmlDocument>& result);


    /**
     * <p>If the bucket has a lifecycle rule configured with an action to abort
     * incomplete multipart uploads and the prefix in the lifecycle rule matches the
     * object name in the request, then the response includes this header indicating
     * when the initiated multipart upload will become eligible for abort operation.
     * For more information, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/dev/mpuoverview.html#mpu-abort-incomplete-mpu-lifecycle-config">Aborting
     * Incomplete Multipart Uploads Using a Bucket Lifecycle Configuration</a>.</p>
     * <p>The response will also include the <code>x-amz-abort-rule-id</code> header
     * that will provide the ID of the lifecycle configuration rule that defines this
     * action.</p>
     */
    inline const Aws::Utils::DateTime& GetAbortDate() const{ return m_abortDate; }

    /**
     * <p>If the bucket has a lifecycle rule configured with an action to abort
     * incomplete multipart uploads and the prefix in the lifecycle rule matches the
     * object name in the request, then the response includes this header indicating
     * when the initiated multipart upload will become eligible for abort operation.
     * For more information, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/dev/mpuoverview.html#mpu-abort-incomplete-mpu-lifecycle-config">Aborting
     * Incomplete Multipart Uploads Using a Bucket Lifecycle Configuration</a>.</p>
     * <p>The response will also include the <code>x-amz-abort-rule-id</code> header
     * that will provide the ID of the lifecycle configuration rule that defines this
     * action.</p>
     */
    inline void SetAbortDate(const Aws::Utils::DateTime& value) { m_abortDate = value; }

    /**
     * <p>If the bucket has a lifecycle rule configured with an action to abort
     * incomplete multipart uploads and the prefix in the lifecycle rule matches the
     * object name in the request, then the response includes this header indicating
     * when the initiated multipart upload will become eligible for abort operation.
     * For more information, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/dev/mpuoverview.html#mpu-abort-incomplete-mpu-lifecycle-config">Aborting
     * Incomplete Multipart Uploads Using a Bucket Lifecycle Configuration</a>.</p>
     * <p>The response will also include the <code>x-amz-abort-rule-id</code> header
     * that will provide the ID of the lifecycle configuration rule that defines this
     * action.</p>
     */
    inline void SetAbortDate(Aws::Utils::DateTime&& value) { m_abortDate = std::move(value); }

    /**
     * <p>If the bucket has a lifecycle rule configured with an action to abort
     * incomplete multipart uploads and the prefix in the lifecycle rule matches the
     * object name in the request, then the response includes this header indicating
     * when the initiated multipart upload will become eligible for abort operation.
     * For more information, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/dev/mpuoverview.html#mpu-abort-incomplete-mpu-lifecycle-config">Aborting
     * Incomplete Multipart Uploads Using a Bucket Lifecycle Configuration</a>.</p>
     * <p>The response will also include the <code>x-amz-abort-rule-id</code> header
     * that will provide the ID of the lifecycle configuration rule that defines this
     * action.</p>
     */
    inline ListPartsResult& WithAbortDate(const Aws::Utils::DateTime& value) { SetAbortDate(value); return *this;}

    /**
     * <p>If the bucket has a lifecycle rule configured with an action to abort
     * incomplete multipart uploads and the prefix in the lifecycle rule matches the
     * object name in the request, then the response includes this header indicating
     * when the initiated multipart upload will become eligible for abort operation.
     * For more information, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/dev/mpuoverview.html#mpu-abort-incomplete-mpu-lifecycle-config">Aborting
     * Incomplete Multipart Uploads Using a Bucket Lifecycle Configuration</a>.</p>
     * <p>The response will also include the <code>x-amz-abort-rule-id</code> header
     * that will provide the ID of the lifecycle configuration rule that defines this
     * action.</p>
     */
    inline ListPartsResult& WithAbortDate(Aws::Utils::DateTime&& value) { SetAbortDate(std::move(value)); return *this;}


    /**
     * <p>This header is returned along with the <code>x-amz-abort-date</code> header.
     * It identifies applicable lifecycle configuration rule that defines the action to
     * abort incomplete multipart uploads.</p>
     */
    inline const Aws::String& GetAbortRuleId() const{ return m_abortRuleId; }

    /**
     * <p>This header is returned along with the <code>x-amz-abort-date</code> header.
     * It identifies applicable lifecycle configuration rule that defines the action to
     * abort incomplete multipart uploads.</p>
     */
    inline void SetAbortRuleId(const Aws::String& value) { m_abortRuleId = value; }

    /**
     * <p>This header is returned along with the <code>x-amz-abort-date</code> header.
     * It identifies applicable lifecycle configuration rule that defines the action to
     * abort incomplete multipart uploads.</p>
     */
    inline void SetAbortRuleId(Aws::String&& value) { m_abortRuleId = std::move(value); }

    /**
     * <p>This header is returned along with the <code>x-amz-abort-date</code> header.
     * It identifies applicable lifecycle configuration rule that defines the action to
     * abort incomplete multipart uploads.</p>
     */
    inline void SetAbortRuleId(const char* value) { m_abortRuleId.assign(value); }

    /**
     * <p>This header is returned along with the <code>x-amz-abort-date</code> header.
     * It identifies applicable lifecycle configuration rule that defines the action to
     * abort incomplete multipart uploads.</p>
     */
    inline ListPartsResult& WithAbortRuleId(const Aws::String& value) { SetAbortRuleId(value); return *this;}

    /**
     * <p>This header is returned along with the <code>x-amz-abort-date</code> header.
     * It identifies applicable lifecycle configuration rule that defines the action to
     * abort incomplete multipart uploads.</p>
     */
    inline ListPartsResult& WithAbortRuleId(Aws::String&& value) { SetAbortRuleId(std::move(value)); return *this;}

    /**
     * <p>This header is returned along with the <code>x-amz-abort-date</code> header.
     * It identifies applicable lifecycle configuration rule that defines the action to
     * abort incomplete multipart uploads.</p>
     */
    inline ListPartsResult& WithAbortRuleId(const char* value) { SetAbortRuleId(value); return *this;}


    /**
     * <p>The name of the bucket to which the multipart upload was initiated. Does not
     * return the access point ARN or access point alias if used.</p>
     */
    inline const Aws::String& GetBucket() const{ return m_bucket; }

    /**
     * <p>The name of the bucket to which the multipart upload was initiated. Does not
     * return the access point ARN or access point alias if used.</p>
     */
    inline void SetBucket(const Aws::String& value) { m_bucket = value; }

    /**
     * <p>The name of the bucket to which the multipart upload was initiated. Does not
     * return the access point ARN or access point alias if used.</p>
     */
    inline void SetBucket(Aws::String&& value) { m_bucket = std::move(value); }

    /**
     * <p>The name of the bucket to which the multipart upload was initiated. Does not
     * return the access point ARN or access point alias if used.</p>
     */
    inline void SetBucket(const char* value) { m_bucket.assign(value); }

    /**
     * <p>The name of the bucket to which the multipart upload was initiated. Does not
     * return the access point ARN or access point alias if used.</p>
     */
    inline ListPartsResult& WithBucket(const Aws::String& value) { SetBucket(value); return *this;}

    /**
     * <p>The name of the bucket to which the multipart upload was initiated. Does not
     * return the access point ARN or access point alias if used.</p>
     */
    inline ListPartsResult& WithBucket(Aws::String&& value) { SetBucket(std::move(value)); return *this;}

    /**
     * <p>The name of the bucket to which the multipart upload was initiated. Does not
     * return the access point ARN or access point alias if used.</p>
     */
    inline ListPartsResult& WithBucket(const char* value) { SetBucket(value); return *this;}


    /**
     * <p>Object key for which the multipart upload was initiated.</p>
     */
    inline const Aws::String& GetKey() const{ return m_key; }

    /**
     * <p>Object key for which the multipart upload was initiated.</p>
     */
    inline void SetKey(const Aws::String& value) { m_key = value; }

    /**
     * <p>Object key for which the multipart upload was initiated.</p>
     */
    inline void SetKey(Aws::String&& value) { m_key = std::move(value); }

    /**
     * <p>Object key for which the multipart upload was initiated.</p>
     */
    inline void SetKey(const char* value) { m_key.assign(value); }

    /**
     * <p>Object key for which the multipart upload was initiated.</p>
     */
    inline ListPartsResult& WithKey(const Aws::String& value) { SetKey(value); return *this;}

    /**
     * <p>Object key for which the multipart upload was initiated.</p>
     */
    inline ListPartsResult& WithKey(Aws::String&& value) { SetKey(std::move(value)); return *this;}

    /**
     * <p>Object key for which the multipart upload was initiated.</p>
     */
    inline ListPartsResult& WithKey(const char* value) { SetKey(value); return *this;}


    /**
     * <p>Upload ID identifying the multipart upload whose parts are being listed.</p>
     */
    inline const Aws::String& GetUploadId() const{ return m_uploadId; }

    /**
     * <p>Upload ID identifying the multipart upload whose parts are being listed.</p>
     */
    inline void SetUploadId(const Aws::String& value) { m_uploadId = value; }

    /**
     * <p>Upload ID identifying the multipart upload whose parts are being listed.</p>
     */
    inline void SetUploadId(Aws::String&& value) { m_uploadId = std::move(value); }

    /**
     * <p>Upload ID identifying the multipart upload whose parts are being listed.</p>
     */
    inline void SetUploadId(const char* value) { m_uploadId.assign(value); }

    /**
     * <p>Upload ID identifying the multipart upload whose parts are being listed.</p>
     */
    inline ListPartsResult& WithUploadId(const Aws::String& value) { SetUploadId(value); return *this;}

    /**
     * <p>Upload ID identifying the multipart upload whose parts are being listed.</p>
     */
    inline ListPartsResult& WithUploadId(Aws::String&& value) { SetUploadId(std::move(value)); return *this;}

    /**
     * <p>Upload ID identifying the multipart upload whose parts are being listed.</p>
     */
    inline ListPartsResult& WithUploadId(const char* value) { SetUploadId(value); return *this;}


    /**
     * <p>When a list is truncated, this element specifies the last part in the list,
     * as well as the value to use for the part-number-marker request parameter in a
     * subsequent request.</p>
     */
    inline int GetPartNumberMarker() const{ return m_partNumberMarker; }

    /**
     * <p>When a list is truncated, this element specifies the last part in the list,
     * as well as the value to use for the part-number-marker request parameter in a
     * subsequent request.</p>
     */
    inline void SetPartNumberMarker(int value) { m_partNumberMarker = value; }

    /**
     * <p>When a list is truncated, this element specifies the last part in the list,
     * as well as the value to use for the part-number-marker request parameter in a
     * subsequent request.</p>
     */
    inline ListPartsResult& WithPartNumberMarker(int value) { SetPartNumberMarker(value); return *this;}


    /**
     * <p>When a list is truncated, this element specifies the last part in the list,
     * as well as the value to use for the <code>part-number-marker</code> request
     * parameter in a subsequent request.</p>
     */
    inline int GetNextPartNumberMarker() const{ return m_nextPartNumberMarker; }

    /**
     * <p>When a list is truncated, this element specifies the last part in the list,
     * as well as the value to use for the <code>part-number-marker</code> request
     * parameter in a subsequent request.</p>
     */
    inline void SetNextPartNumberMarker(int value) { m_nextPartNumberMarker = value; }

    /**
     * <p>When a list is truncated, this element specifies the last part in the list,
     * as well as the value to use for the <code>part-number-marker</code> request
     * parameter in a subsequent request.</p>
     */
    inline ListPartsResult& WithNextPartNumberMarker(int value) { SetNextPartNumberMarker(value); return *this;}


    /**
     * <p>Maximum number of parts that were allowed in the response.</p>
     */
    inline int GetMaxParts() const{ return m_maxParts; }

    /**
     * <p>Maximum number of parts that were allowed in the response.</p>
     */
    inline void SetMaxParts(int value) { m_maxParts = value; }

    /**
     * <p>Maximum number of parts that were allowed in the response.</p>
     */
    inline ListPartsResult& WithMaxParts(int value) { SetMaxParts(value); return *this;}


    /**
     * <p> Indicates whether the returned list of parts is truncated. A true value
     * indicates that the list was truncated. A list can be truncated if the number of
     * parts exceeds the limit returned in the MaxParts element.</p>
     */
    inline bool GetIsTruncated() const{ return m_isTruncated; }

    /**
     * <p> Indicates whether the returned list of parts is truncated. A true value
     * indicates that the list was truncated. A list can be truncated if the number of
     * parts exceeds the limit returned in the MaxParts element.</p>
     */
    inline void SetIsTruncated(bool value) { m_isTruncated = value; }

    /**
     * <p> Indicates whether the returned list of parts is truncated. A true value
     * indicates that the list was truncated. A list can be truncated if the number of
     * parts exceeds the limit returned in the MaxParts element.</p>
     */
    inline ListPartsResult& WithIsTruncated(bool value) { SetIsTruncated(value); return *this;}


    /**
     * <p> Container for elements related to a particular part. A response can contain
     * zero or more <code>Part</code> elements.</p>
     */
    inline const Aws::Vector<Part>& GetParts() const{ return m_parts; }

    /**
     * <p> Container for elements related to a particular part. A response can contain
     * zero or more <code>Part</code> elements.</p>
     */
    inline void SetParts(const Aws::Vector<Part>& value) { m_parts = value; }

    /**
     * <p> Container for elements related to a particular part. A response can contain
     * zero or more <code>Part</code> elements.</p>
     */
    inline void SetParts(Aws::Vector<Part>&& value) { m_parts = std::move(value); }

    /**
     * <p> Container for elements related to a particular part. A response can contain
     * zero or more <code>Part</code> elements.</p>
     */
    inline ListPartsResult& WithParts(const Aws::Vector<Part>& value) { SetParts(value); return *this;}

    /**
     * <p> Container for elements related to a particular part. A response can contain
     * zero or more <code>Part</code> elements.</p>
     */
    inline ListPartsResult& WithParts(Aws::Vector<Part>&& value) { SetParts(std::move(value)); return *this;}

    /**
     * <p> Container for elements related to a particular part. A response can contain
     * zero or more <code>Part</code> elements.</p>
     */
    inline ListPartsResult& AddParts(const Part& value) { m_parts.push_back(value); return *this; }

    /**
     * <p> Container for elements related to a particular part. A response can contain
     * zero or more <code>Part</code> elements.</p>
     */
    inline ListPartsResult& AddParts(Part&& value) { m_parts.push_back(std::move(value)); return *this; }


    /**
     * <p>Container element that identifies who initiated the multipart upload. If the
     * initiator is an Amazon Web Services account, this element provides the same
     * information as the <code>Owner</code> element. If the initiator is an IAM User,
     * this element provides the user ARN and display name.</p>
     */
    inline const Initiator& GetInitiator() const{ return m_initiator; }

    /**
     * <p>Container element that identifies who initiated the multipart upload. If the
     * initiator is an Amazon Web Services account, this element provides the same
     * information as the <code>Owner</code> element. If the initiator is an IAM User,
     * this element provides the user ARN and display name.</p>
     */
    inline void SetInitiator(const Initiator& value) { m_initiator = value; }

    /**
     * <p>Container element that identifies who initiated the multipart upload. If the
     * initiator is an Amazon Web Services account, this element provides the same
     * information as the <code>Owner</code> element. If the initiator is an IAM User,
     * this element provides the user ARN and display name.</p>
     */
    inline void SetInitiator(Initiator&& value) { m_initiator = std::move(value); }

    /**
     * <p>Container element that identifies who initiated the multipart upload. If the
     * initiator is an Amazon Web Services account, this element provides the same
     * information as the <code>Owner</code> element. If the initiator is an IAM User,
     * this element provides the user ARN and display name.</p>
     */
    inline ListPartsResult& WithInitiator(const Initiator& value) { SetInitiator(value); return *this;}

    /**
     * <p>Container element that identifies who initiated the multipart upload. If the
     * initiator is an Amazon Web Services account, this element provides the same
     * information as the <code>Owner</code> element. If the initiator is an IAM User,
     * this element provides the user ARN and display name.</p>
     */
    inline ListPartsResult& WithInitiator(Initiator&& value) { SetInitiator(std::move(value)); return *this;}


    /**
     * <p> Container element that identifies the object owner, after the object is
     * created. If multipart upload is initiated by an IAM user, this element provides
     * the parent account ID and display name.</p>
     */
    inline const Owner& GetOwner() const{ return m_owner; }

    /**
     * <p> Container element that identifies the object owner, after the object is
     * created. If multipart upload is initiated by an IAM user, this element provides
     * the parent account ID and display name.</p>
     */
    inline void SetOwner(const Owner& value) { m_owner = value; }

    /**
     * <p> Container element that identifies the object owner, after the object is
     * created. If multipart upload is initiated by an IAM user, this element provides
     * the parent account ID and display name.</p>
     */
    inline void SetOwner(Owner&& value) { m_owner = std::move(value); }

    /**
     * <p> Container element that identifies the object owner, after the object is
     * created. If multipart upload is initiated by an IAM user, this element provides
     * the parent account ID and display name.</p>
     */
    inline ListPartsResult& WithOwner(const Owner& value) { SetOwner(value); return *this;}

    /**
     * <p> Container element that identifies the object owner, after the object is
     * created. If multipart upload is initiated by an IAM user, this element provides
     * the parent account ID and display name.</p>
     */
    inline ListPartsResult& WithOwner(Owner&& value) { SetOwner(std::move(value)); return *this;}


    /**
     * <p>Class of storage (STANDARD or REDUCED_REDUNDANCY) used to store the uploaded
     * object.</p>
     */
    inline const StorageClass& GetStorageClass() const{ return m_storageClass; }

    /**
     * <p>Class of storage (STANDARD or REDUCED_REDUNDANCY) used to store the uploaded
     * object.</p>
     */
    inline void SetStorageClass(const StorageClass& value) { m_storageClass = value; }

    /**
     * <p>Class of storage (STANDARD or REDUCED_REDUNDANCY) used to store the uploaded
     * object.</p>
     */
    inline void SetStorageClass(StorageClass&& value) { m_storageClass = std::move(value); }

    /**
     * <p>Class of storage (STANDARD or REDUCED_REDUNDANCY) used to store the uploaded
     * object.</p>
     */
    inline ListPartsResult& WithStorageClass(const StorageClass& value) { SetStorageClass(value); return *this;}

    /**
     * <p>Class of storage (STANDARD or REDUCED_REDUNDANCY) used to store the uploaded
     * object.</p>
     */
    inline ListPartsResult& WithStorageClass(StorageClass&& value) { SetStorageClass(std::move(value)); return *this;}


    
    inline const RequestCharged& GetRequestCharged() const{ return m_requestCharged; }

    
    inline void SetRequestCharged(const RequestCharged& value) { m_requestCharged = value; }

    
    inline void SetRequestCharged(RequestCharged&& value) { m_requestCharged = std::move(value); }

    
    inline ListPartsResult& WithRequestCharged(const RequestCharged& value) { SetRequestCharged(value); return *this;}

    
    inline ListPartsResult& WithRequestCharged(RequestCharged&& value) { SetRequestCharged(std::move(value)); return *this;}


    /**
     * <p>The algorithm that was used to create a checksum of the object.</p>
     */
    inline const ChecksumAlgorithm& GetChecksumAlgorithm() const{ return m_checksumAlgorithm; }

    /**
     * <p>The algorithm that was used to create a checksum of the object.</p>
     */
    inline void SetChecksumAlgorithm(const ChecksumAlgorithm& value) { m_checksumAlgorithm = value; }

    /**
     * <p>The algorithm that was used to create a checksum of the object.</p>
     */
    inline void SetChecksumAlgorithm(ChecksumAlgorithm&& value) { m_checksumAlgorithm = std::move(value); }

    /**
     * <p>The algorithm that was used to create a checksum of the object.</p>
     */
    inline ListPartsResult& WithChecksumAlgorithm(const ChecksumAlgorithm& value) { SetChecksumAlgorithm(value); return *this;}

    /**
     * <p>The algorithm that was used to create a checksum of the object.</p>
     */
    inline ListPartsResult& WithChecksumAlgorithm(ChecksumAlgorithm&& value) { SetChecksumAlgorithm(std::move(value)); return *this;}


    
    inline const Aws::String& GetRequestId() const{ return m_requestId; }

    
    inline void SetRequestId(const Aws::String& value) { m_requestId = value; }

    
    inline void SetRequestId(Aws::String&& value) { m_requestId = std::move(value); }

    
    inline void SetRequestId(const char* value) { m_requestId.assign(value); }

    
    inline ListPartsResult& WithRequestId(const Aws::String& value) { SetRequestId(value); return *this;}

    
    inline ListPartsResult& WithRequestId(Aws::String&& value) { SetRequestId(std::move(value)); return *this;}

    
    inline ListPartsResult& WithRequestId(const char* value) { SetRequestId(value); return *this;}

  private:

    Aws::Utils::DateTime m_abortDate;

    Aws::String m_abortRuleId;

    Aws::String m_bucket;

    Aws::String m_key;

    Aws::String m_uploadId;

    int m_partNumberMarker;

    int m_nextPartNumberMarker;

    int m_maxParts;

    bool m_isTruncated;

    Aws::Vector<Part> m_parts;

    Initiator m_initiator;

    Owner m_owner;

    StorageClass m_storageClass;

    RequestCharged m_requestCharged;

    ChecksumAlgorithm m_checksumAlgorithm;

    Aws::String m_requestId;
  };

} // namespace Model
} // namespace S3
} // namespace Aws
