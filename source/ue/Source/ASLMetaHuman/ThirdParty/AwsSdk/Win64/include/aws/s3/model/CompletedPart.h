﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/s3/S3_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Xml
{
  class XmlNode;
} // namespace Xml
} // namespace Utils
namespace S3
{
namespace Model
{

  /**
   * <p>Details of the parts that were uploaded.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/s3-2006-03-01/CompletedPart">AWS
   * API Reference</a></p>
   */
  class CompletedPart
  {
  public:
    AWS_S3_API CompletedPart();
    AWS_S3_API CompletedPart(const Aws::Utils::Xml::XmlNode& xmlNode);
    AWS_S3_API CompletedPart& operator=(const Aws::Utils::Xml::XmlNode& xmlNode);

    AWS_S3_API void AddToNode(Aws::Utils::Xml::XmlNode& parentNode) const;


    /**
     * <p>Entity tag returned when the part was uploaded.</p>
     */
    inline const Aws::String& GetETag() const{ return m_eTag; }

    /**
     * <p>Entity tag returned when the part was uploaded.</p>
     */
    inline bool ETagHasBeenSet() const { return m_eTagHasBeenSet; }

    /**
     * <p>Entity tag returned when the part was uploaded.</p>
     */
    inline void SetETag(const Aws::String& value) { m_eTagHasBeenSet = true; m_eTag = value; }

    /**
     * <p>Entity tag returned when the part was uploaded.</p>
     */
    inline void SetETag(Aws::String&& value) { m_eTagHasBeenSet = true; m_eTag = std::move(value); }

    /**
     * <p>Entity tag returned when the part was uploaded.</p>
     */
    inline void SetETag(const char* value) { m_eTagHasBeenSet = true; m_eTag.assign(value); }

    /**
     * <p>Entity tag returned when the part was uploaded.</p>
     */
    inline CompletedPart& WithETag(const Aws::String& value) { SetETag(value); return *this;}

    /**
     * <p>Entity tag returned when the part was uploaded.</p>
     */
    inline CompletedPart& WithETag(Aws::String&& value) { SetETag(std::move(value)); return *this;}

    /**
     * <p>Entity tag returned when the part was uploaded.</p>
     */
    inline CompletedPart& WithETag(const char* value) { SetETag(value); return *this;}


    /**
     * <p>The base64-encoded, 32-bit CRC32 checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline const Aws::String& GetChecksumCRC32() const{ return m_checksumCRC32; }

    /**
     * <p>The base64-encoded, 32-bit CRC32 checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline bool ChecksumCRC32HasBeenSet() const { return m_checksumCRC32HasBeenSet; }

    /**
     * <p>The base64-encoded, 32-bit CRC32 checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumCRC32(const Aws::String& value) { m_checksumCRC32HasBeenSet = true; m_checksumCRC32 = value; }

    /**
     * <p>The base64-encoded, 32-bit CRC32 checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumCRC32(Aws::String&& value) { m_checksumCRC32HasBeenSet = true; m_checksumCRC32 = std::move(value); }

    /**
     * <p>The base64-encoded, 32-bit CRC32 checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumCRC32(const char* value) { m_checksumCRC32HasBeenSet = true; m_checksumCRC32.assign(value); }

    /**
     * <p>The base64-encoded, 32-bit CRC32 checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumCRC32(const Aws::String& value) { SetChecksumCRC32(value); return *this;}

    /**
     * <p>The base64-encoded, 32-bit CRC32 checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumCRC32(Aws::String&& value) { SetChecksumCRC32(std::move(value)); return *this;}

    /**
     * <p>The base64-encoded, 32-bit CRC32 checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumCRC32(const char* value) { SetChecksumCRC32(value); return *this;}


    /**
     * <p>The base64-encoded, 32-bit CRC32C checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline const Aws::String& GetChecksumCRC32C() const{ return m_checksumCRC32C; }

    /**
     * <p>The base64-encoded, 32-bit CRC32C checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline bool ChecksumCRC32CHasBeenSet() const { return m_checksumCRC32CHasBeenSet; }

    /**
     * <p>The base64-encoded, 32-bit CRC32C checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumCRC32C(const Aws::String& value) { m_checksumCRC32CHasBeenSet = true; m_checksumCRC32C = value; }

    /**
     * <p>The base64-encoded, 32-bit CRC32C checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumCRC32C(Aws::String&& value) { m_checksumCRC32CHasBeenSet = true; m_checksumCRC32C = std::move(value); }

    /**
     * <p>The base64-encoded, 32-bit CRC32C checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumCRC32C(const char* value) { m_checksumCRC32CHasBeenSet = true; m_checksumCRC32C.assign(value); }

    /**
     * <p>The base64-encoded, 32-bit CRC32C checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumCRC32C(const Aws::String& value) { SetChecksumCRC32C(value); return *this;}

    /**
     * <p>The base64-encoded, 32-bit CRC32C checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumCRC32C(Aws::String&& value) { SetChecksumCRC32C(std::move(value)); return *this;}

    /**
     * <p>The base64-encoded, 32-bit CRC32C checksum of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumCRC32C(const char* value) { SetChecksumCRC32C(value); return *this;}


    /**
     * <p>The base64-encoded, 160-bit SHA-1 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline const Aws::String& GetChecksumSHA1() const{ return m_checksumSHA1; }

    /**
     * <p>The base64-encoded, 160-bit SHA-1 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline bool ChecksumSHA1HasBeenSet() const { return m_checksumSHA1HasBeenSet; }

    /**
     * <p>The base64-encoded, 160-bit SHA-1 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumSHA1(const Aws::String& value) { m_checksumSHA1HasBeenSet = true; m_checksumSHA1 = value; }

    /**
     * <p>The base64-encoded, 160-bit SHA-1 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumSHA1(Aws::String&& value) { m_checksumSHA1HasBeenSet = true; m_checksumSHA1 = std::move(value); }

    /**
     * <p>The base64-encoded, 160-bit SHA-1 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumSHA1(const char* value) { m_checksumSHA1HasBeenSet = true; m_checksumSHA1.assign(value); }

    /**
     * <p>The base64-encoded, 160-bit SHA-1 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumSHA1(const Aws::String& value) { SetChecksumSHA1(value); return *this;}

    /**
     * <p>The base64-encoded, 160-bit SHA-1 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumSHA1(Aws::String&& value) { SetChecksumSHA1(std::move(value)); return *this;}

    /**
     * <p>The base64-encoded, 160-bit SHA-1 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumSHA1(const char* value) { SetChecksumSHA1(value); return *this;}


    /**
     * <p>The base64-encoded, 256-bit SHA-256 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline const Aws::String& GetChecksumSHA256() const{ return m_checksumSHA256; }

    /**
     * <p>The base64-encoded, 256-bit SHA-256 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline bool ChecksumSHA256HasBeenSet() const { return m_checksumSHA256HasBeenSet; }

    /**
     * <p>The base64-encoded, 256-bit SHA-256 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumSHA256(const Aws::String& value) { m_checksumSHA256HasBeenSet = true; m_checksumSHA256 = value; }

    /**
     * <p>The base64-encoded, 256-bit SHA-256 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumSHA256(Aws::String&& value) { m_checksumSHA256HasBeenSet = true; m_checksumSHA256 = std::move(value); }

    /**
     * <p>The base64-encoded, 256-bit SHA-256 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline void SetChecksumSHA256(const char* value) { m_checksumSHA256HasBeenSet = true; m_checksumSHA256.assign(value); }

    /**
     * <p>The base64-encoded, 256-bit SHA-256 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumSHA256(const Aws::String& value) { SetChecksumSHA256(value); return *this;}

    /**
     * <p>The base64-encoded, 256-bit SHA-256 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumSHA256(Aws::String&& value) { SetChecksumSHA256(std::move(value)); return *this;}

    /**
     * <p>The base64-encoded, 256-bit SHA-256 digest of the object. This will only be
     * present if it was uploaded with the object. With multipart uploads, this may not
     * be a checksum value of the object. For more information about how checksums are
     * calculated with multipart uploads, see <a
     * href="https://docs.aws.amazon.com/AmazonS3/latest/userguide/checking-object-integrity.html#large-object-checksums">
     * Checking object integrity</a> in the <i>Amazon S3 User Guide</i>.</p>
     */
    inline CompletedPart& WithChecksumSHA256(const char* value) { SetChecksumSHA256(value); return *this;}


    /**
     * <p>Part number that identifies the part. This is a positive integer between 1
     * and 10,000.</p>
     */
    inline int GetPartNumber() const{ return m_partNumber; }

    /**
     * <p>Part number that identifies the part. This is a positive integer between 1
     * and 10,000.</p>
     */
    inline bool PartNumberHasBeenSet() const { return m_partNumberHasBeenSet; }

    /**
     * <p>Part number that identifies the part. This is a positive integer between 1
     * and 10,000.</p>
     */
    inline void SetPartNumber(int value) { m_partNumberHasBeenSet = true; m_partNumber = value; }

    /**
     * <p>Part number that identifies the part. This is a positive integer between 1
     * and 10,000.</p>
     */
    inline CompletedPart& WithPartNumber(int value) { SetPartNumber(value); return *this;}

  private:

    Aws::String m_eTag;
    bool m_eTagHasBeenSet = false;

    Aws::String m_checksumCRC32;
    bool m_checksumCRC32HasBeenSet = false;

    Aws::String m_checksumCRC32C;
    bool m_checksumCRC32CHasBeenSet = false;

    Aws::String m_checksumSHA1;
    bool m_checksumSHA1HasBeenSet = false;

    Aws::String m_checksumSHA256;
    bool m_checksumSHA256HasBeenSet = false;

    int m_partNumber;
    bool m_partNumberHasBeenSet = false;
  };

} // namespace Model
} // namespace S3
} // namespace Aws
