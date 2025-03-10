﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/s3/S3_EXPORTS.h>
#include <aws/s3/S3Request.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSMap.h>
#include <utility>

namespace Aws
{
namespace Http
{
    class URI;
} //namespace Http
namespace S3
{
namespace Model
{

  /**
   */
  class GetBucketPolicyRequest : public S3Request
  {
  public:
    AWS_S3_API GetBucketPolicyRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetBucketPolicy"; }

    AWS_S3_API Aws::String SerializePayload() const override;

    AWS_S3_API void AddQueryStringParameters(Aws::Http::URI& uri) const override;

    AWS_S3_API Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;

    /**
     * Helper function to collect parameters (configurable and static hardcoded) required for endpoint computation.
     */
    AWS_S3_API EndpointParameters GetEndpointContextParams() const override;

    /**
     * <p>The bucket name for which to get the bucket policy.</p> <p>To use this API
     * operation against an access point, provide the alias of the access point in
     * place of the bucket name.</p> <p>To use this API operation against an Object
     * Lambda access point, provide the alias of the Object Lambda access point in
     * place of the bucket name. If the Object Lambda access point alias in a request
     * is not valid, the error code <code>InvalidAccessPointAliasError</code> is
     * returned. For more information about <code>InvalidAccessPointAliasError</code>,
     * see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/API/ErrorResponses.html#ErrorCodeList">List
     * of Error Codes</a>.</p>
     */
    inline const Aws::String& GetBucket() const{ return m_bucket; }

    /**
     * <p>The bucket name for which to get the bucket policy.</p> <p>To use this API
     * operation against an access point, provide the alias of the access point in
     * place of the bucket name.</p> <p>To use this API operation against an Object
     * Lambda access point, provide the alias of the Object Lambda access point in
     * place of the bucket name. If the Object Lambda access point alias in a request
     * is not valid, the error code <code>InvalidAccessPointAliasError</code> is
     * returned. For more information about <code>InvalidAccessPointAliasError</code>,
     * see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/API/ErrorResponses.html#ErrorCodeList">List
     * of Error Codes</a>.</p>
     */
    inline bool BucketHasBeenSet() const { return m_bucketHasBeenSet; }

    /**
     * <p>The bucket name for which to get the bucket policy.</p> <p>To use this API
     * operation against an access point, provide the alias of the access point in
     * place of the bucket name.</p> <p>To use this API operation against an Object
     * Lambda access point, provide the alias of the Object Lambda access point in
     * place of the bucket name. If the Object Lambda access point alias in a request
     * is not valid, the error code <code>InvalidAccessPointAliasError</code> is
     * returned. For more information about <code>InvalidAccessPointAliasError</code>,
     * see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/API/ErrorResponses.html#ErrorCodeList">List
     * of Error Codes</a>.</p>
     */
    inline void SetBucket(const Aws::String& value) { m_bucketHasBeenSet = true; m_bucket = value; }

    /**
     * <p>The bucket name for which to get the bucket policy.</p> <p>To use this API
     * operation against an access point, provide the alias of the access point in
     * place of the bucket name.</p> <p>To use this API operation against an Object
     * Lambda access point, provide the alias of the Object Lambda access point in
     * place of the bucket name. If the Object Lambda access point alias in a request
     * is not valid, the error code <code>InvalidAccessPointAliasError</code> is
     * returned. For more information about <code>InvalidAccessPointAliasError</code>,
     * see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/API/ErrorResponses.html#ErrorCodeList">List
     * of Error Codes</a>.</p>
     */
    inline void SetBucket(Aws::String&& value) { m_bucketHasBeenSet = true; m_bucket = std::move(value); }

    /**
     * <p>The bucket name for which to get the bucket policy.</p> <p>To use this API
     * operation against an access point, provide the alias of the access point in
     * place of the bucket name.</p> <p>To use this API operation against an Object
     * Lambda access point, provide the alias of the Object Lambda access point in
     * place of the bucket name. If the Object Lambda access point alias in a request
     * is not valid, the error code <code>InvalidAccessPointAliasError</code> is
     * returned. For more information about <code>InvalidAccessPointAliasError</code>,
     * see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/API/ErrorResponses.html#ErrorCodeList">List
     * of Error Codes</a>.</p>
     */
    inline void SetBucket(const char* value) { m_bucketHasBeenSet = true; m_bucket.assign(value); }

    /**
     * <p>The bucket name for which to get the bucket policy.</p> <p>To use this API
     * operation against an access point, provide the alias of the access point in
     * place of the bucket name.</p> <p>To use this API operation against an Object
     * Lambda access point, provide the alias of the Object Lambda access point in
     * place of the bucket name. If the Object Lambda access point alias in a request
     * is not valid, the error code <code>InvalidAccessPointAliasError</code> is
     * returned. For more information about <code>InvalidAccessPointAliasError</code>,
     * see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/API/ErrorResponses.html#ErrorCodeList">List
     * of Error Codes</a>.</p>
     */
    inline GetBucketPolicyRequest& WithBucket(const Aws::String& value) { SetBucket(value); return *this;}

    /**
     * <p>The bucket name for which to get the bucket policy.</p> <p>To use this API
     * operation against an access point, provide the alias of the access point in
     * place of the bucket name.</p> <p>To use this API operation against an Object
     * Lambda access point, provide the alias of the Object Lambda access point in
     * place of the bucket name. If the Object Lambda access point alias in a request
     * is not valid, the error code <code>InvalidAccessPointAliasError</code> is
     * returned. For more information about <code>InvalidAccessPointAliasError</code>,
     * see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/API/ErrorResponses.html#ErrorCodeList">List
     * of Error Codes</a>.</p>
     */
    inline GetBucketPolicyRequest& WithBucket(Aws::String&& value) { SetBucket(std::move(value)); return *this;}

    /**
     * <p>The bucket name for which to get the bucket policy.</p> <p>To use this API
     * operation against an access point, provide the alias of the access point in
     * place of the bucket name.</p> <p>To use this API operation against an Object
     * Lambda access point, provide the alias of the Object Lambda access point in
     * place of the bucket name. If the Object Lambda access point alias in a request
     * is not valid, the error code <code>InvalidAccessPointAliasError</code> is
     * returned. For more information about <code>InvalidAccessPointAliasError</code>,
     * see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/API/ErrorResponses.html#ErrorCodeList">List
     * of Error Codes</a>.</p>
     */
    inline GetBucketPolicyRequest& WithBucket(const char* value) { SetBucket(value); return *this;}


    /**
     * <p>The account ID of the expected bucket owner. If the bucket is owned by a
     * different account, the request fails with the HTTP status code <code>403
     * Forbidden</code> (access denied).</p>
     */
    inline const Aws::String& GetExpectedBucketOwner() const{ return m_expectedBucketOwner; }

    /**
     * <p>The account ID of the expected bucket owner. If the bucket is owned by a
     * different account, the request fails with the HTTP status code <code>403
     * Forbidden</code> (access denied).</p>
     */
    inline bool ExpectedBucketOwnerHasBeenSet() const { return m_expectedBucketOwnerHasBeenSet; }

    /**
     * <p>The account ID of the expected bucket owner. If the bucket is owned by a
     * different account, the request fails with the HTTP status code <code>403
     * Forbidden</code> (access denied).</p>
     */
    inline void SetExpectedBucketOwner(const Aws::String& value) { m_expectedBucketOwnerHasBeenSet = true; m_expectedBucketOwner = value; }

    /**
     * <p>The account ID of the expected bucket owner. If the bucket is owned by a
     * different account, the request fails with the HTTP status code <code>403
     * Forbidden</code> (access denied).</p>
     */
    inline void SetExpectedBucketOwner(Aws::String&& value) { m_expectedBucketOwnerHasBeenSet = true; m_expectedBucketOwner = std::move(value); }

    /**
     * <p>The account ID of the expected bucket owner. If the bucket is owned by a
     * different account, the request fails with the HTTP status code <code>403
     * Forbidden</code> (access denied).</p>
     */
    inline void SetExpectedBucketOwner(const char* value) { m_expectedBucketOwnerHasBeenSet = true; m_expectedBucketOwner.assign(value); }

    /**
     * <p>The account ID of the expected bucket owner. If the bucket is owned by a
     * different account, the request fails with the HTTP status code <code>403
     * Forbidden</code> (access denied).</p>
     */
    inline GetBucketPolicyRequest& WithExpectedBucketOwner(const Aws::String& value) { SetExpectedBucketOwner(value); return *this;}

    /**
     * <p>The account ID of the expected bucket owner. If the bucket is owned by a
     * different account, the request fails with the HTTP status code <code>403
     * Forbidden</code> (access denied).</p>
     */
    inline GetBucketPolicyRequest& WithExpectedBucketOwner(Aws::String&& value) { SetExpectedBucketOwner(std::move(value)); return *this;}

    /**
     * <p>The account ID of the expected bucket owner. If the bucket is owned by a
     * different account, the request fails with the HTTP status code <code>403
     * Forbidden</code> (access denied).</p>
     */
    inline GetBucketPolicyRequest& WithExpectedBucketOwner(const char* value) { SetExpectedBucketOwner(value); return *this;}


    
    inline const Aws::Map<Aws::String, Aws::String>& GetCustomizedAccessLogTag() const{ return m_customizedAccessLogTag; }

    
    inline bool CustomizedAccessLogTagHasBeenSet() const { return m_customizedAccessLogTagHasBeenSet; }

    
    inline void SetCustomizedAccessLogTag(const Aws::Map<Aws::String, Aws::String>& value) { m_customizedAccessLogTagHasBeenSet = true; m_customizedAccessLogTag = value; }

    
    inline void SetCustomizedAccessLogTag(Aws::Map<Aws::String, Aws::String>&& value) { m_customizedAccessLogTagHasBeenSet = true; m_customizedAccessLogTag = std::move(value); }

    
    inline GetBucketPolicyRequest& WithCustomizedAccessLogTag(const Aws::Map<Aws::String, Aws::String>& value) { SetCustomizedAccessLogTag(value); return *this;}

    
    inline GetBucketPolicyRequest& WithCustomizedAccessLogTag(Aws::Map<Aws::String, Aws::String>&& value) { SetCustomizedAccessLogTag(std::move(value)); return *this;}

    
    inline GetBucketPolicyRequest& AddCustomizedAccessLogTag(const Aws::String& key, const Aws::String& value) { m_customizedAccessLogTagHasBeenSet = true; m_customizedAccessLogTag.emplace(key, value); return *this; }

    
    inline GetBucketPolicyRequest& AddCustomizedAccessLogTag(Aws::String&& key, const Aws::String& value) { m_customizedAccessLogTagHasBeenSet = true; m_customizedAccessLogTag.emplace(std::move(key), value); return *this; }

    
    inline GetBucketPolicyRequest& AddCustomizedAccessLogTag(const Aws::String& key, Aws::String&& value) { m_customizedAccessLogTagHasBeenSet = true; m_customizedAccessLogTag.emplace(key, std::move(value)); return *this; }

    
    inline GetBucketPolicyRequest& AddCustomizedAccessLogTag(Aws::String&& key, Aws::String&& value) { m_customizedAccessLogTagHasBeenSet = true; m_customizedAccessLogTag.emplace(std::move(key), std::move(value)); return *this; }

    
    inline GetBucketPolicyRequest& AddCustomizedAccessLogTag(const char* key, Aws::String&& value) { m_customizedAccessLogTagHasBeenSet = true; m_customizedAccessLogTag.emplace(key, std::move(value)); return *this; }

    
    inline GetBucketPolicyRequest& AddCustomizedAccessLogTag(Aws::String&& key, const char* value) { m_customizedAccessLogTagHasBeenSet = true; m_customizedAccessLogTag.emplace(std::move(key), value); return *this; }

    
    inline GetBucketPolicyRequest& AddCustomizedAccessLogTag(const char* key, const char* value) { m_customizedAccessLogTagHasBeenSet = true; m_customizedAccessLogTag.emplace(key, value); return *this; }

  private:

    Aws::String m_bucket;
    bool m_bucketHasBeenSet = false;

    Aws::String m_expectedBucketOwner;
    bool m_expectedBucketOwnerHasBeenSet = false;

    Aws::Map<Aws::String, Aws::String> m_customizedAccessLogTag;
    bool m_customizedAccessLogTagHasBeenSet = false;
  };

} // namespace Model
} // namespace S3
} // namespace Aws
