﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iam/IAM_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/iam/model/StatusType.h>
#include <aws/core/utils/DateTime.h>
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
namespace IAM
{
namespace Model
{

  /**
   * <p>Contains information about an X.509 signing certificate.</p> <p>This data
   * type is used as a response element in the <a>UploadSigningCertificate</a> and
   * <a>ListSigningCertificates</a> operations. </p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/iam-2010-05-08/SigningCertificate">AWS
   * API Reference</a></p>
   */
  class SigningCertificate
  {
  public:
    AWS_IAM_API SigningCertificate();
    AWS_IAM_API SigningCertificate(const Aws::Utils::Xml::XmlNode& xmlNode);
    AWS_IAM_API SigningCertificate& operator=(const Aws::Utils::Xml::XmlNode& xmlNode);

    AWS_IAM_API void OutputToStream(Aws::OStream& ostream, const char* location, unsigned index, const char* locationValue) const;
    AWS_IAM_API void OutputToStream(Aws::OStream& oStream, const char* location) const;


    /**
     * <p>The name of the user the signing certificate is associated with.</p>
     */
    inline const Aws::String& GetUserName() const{ return m_userName; }

    /**
     * <p>The name of the user the signing certificate is associated with.</p>
     */
    inline bool UserNameHasBeenSet() const { return m_userNameHasBeenSet; }

    /**
     * <p>The name of the user the signing certificate is associated with.</p>
     */
    inline void SetUserName(const Aws::String& value) { m_userNameHasBeenSet = true; m_userName = value; }

    /**
     * <p>The name of the user the signing certificate is associated with.</p>
     */
    inline void SetUserName(Aws::String&& value) { m_userNameHasBeenSet = true; m_userName = std::move(value); }

    /**
     * <p>The name of the user the signing certificate is associated with.</p>
     */
    inline void SetUserName(const char* value) { m_userNameHasBeenSet = true; m_userName.assign(value); }

    /**
     * <p>The name of the user the signing certificate is associated with.</p>
     */
    inline SigningCertificate& WithUserName(const Aws::String& value) { SetUserName(value); return *this;}

    /**
     * <p>The name of the user the signing certificate is associated with.</p>
     */
    inline SigningCertificate& WithUserName(Aws::String&& value) { SetUserName(std::move(value)); return *this;}

    /**
     * <p>The name of the user the signing certificate is associated with.</p>
     */
    inline SigningCertificate& WithUserName(const char* value) { SetUserName(value); return *this;}


    /**
     * <p>The ID for the signing certificate.</p>
     */
    inline const Aws::String& GetCertificateId() const{ return m_certificateId; }

    /**
     * <p>The ID for the signing certificate.</p>
     */
    inline bool CertificateIdHasBeenSet() const { return m_certificateIdHasBeenSet; }

    /**
     * <p>The ID for the signing certificate.</p>
     */
    inline void SetCertificateId(const Aws::String& value) { m_certificateIdHasBeenSet = true; m_certificateId = value; }

    /**
     * <p>The ID for the signing certificate.</p>
     */
    inline void SetCertificateId(Aws::String&& value) { m_certificateIdHasBeenSet = true; m_certificateId = std::move(value); }

    /**
     * <p>The ID for the signing certificate.</p>
     */
    inline void SetCertificateId(const char* value) { m_certificateIdHasBeenSet = true; m_certificateId.assign(value); }

    /**
     * <p>The ID for the signing certificate.</p>
     */
    inline SigningCertificate& WithCertificateId(const Aws::String& value) { SetCertificateId(value); return *this;}

    /**
     * <p>The ID for the signing certificate.</p>
     */
    inline SigningCertificate& WithCertificateId(Aws::String&& value) { SetCertificateId(std::move(value)); return *this;}

    /**
     * <p>The ID for the signing certificate.</p>
     */
    inline SigningCertificate& WithCertificateId(const char* value) { SetCertificateId(value); return *this;}


    /**
     * <p>The contents of the signing certificate.</p>
     */
    inline const Aws::String& GetCertificateBody() const{ return m_certificateBody; }

    /**
     * <p>The contents of the signing certificate.</p>
     */
    inline bool CertificateBodyHasBeenSet() const { return m_certificateBodyHasBeenSet; }

    /**
     * <p>The contents of the signing certificate.</p>
     */
    inline void SetCertificateBody(const Aws::String& value) { m_certificateBodyHasBeenSet = true; m_certificateBody = value; }

    /**
     * <p>The contents of the signing certificate.</p>
     */
    inline void SetCertificateBody(Aws::String&& value) { m_certificateBodyHasBeenSet = true; m_certificateBody = std::move(value); }

    /**
     * <p>The contents of the signing certificate.</p>
     */
    inline void SetCertificateBody(const char* value) { m_certificateBodyHasBeenSet = true; m_certificateBody.assign(value); }

    /**
     * <p>The contents of the signing certificate.</p>
     */
    inline SigningCertificate& WithCertificateBody(const Aws::String& value) { SetCertificateBody(value); return *this;}

    /**
     * <p>The contents of the signing certificate.</p>
     */
    inline SigningCertificate& WithCertificateBody(Aws::String&& value) { SetCertificateBody(std::move(value)); return *this;}

    /**
     * <p>The contents of the signing certificate.</p>
     */
    inline SigningCertificate& WithCertificateBody(const char* value) { SetCertificateBody(value); return *this;}


    /**
     * <p>The status of the signing certificate. <code>Active</code> means that the key
     * is valid for API calls, while <code>Inactive</code> means it is not.</p>
     */
    inline const StatusType& GetStatus() const{ return m_status; }

    /**
     * <p>The status of the signing certificate. <code>Active</code> means that the key
     * is valid for API calls, while <code>Inactive</code> means it is not.</p>
     */
    inline bool StatusHasBeenSet() const { return m_statusHasBeenSet; }

    /**
     * <p>The status of the signing certificate. <code>Active</code> means that the key
     * is valid for API calls, while <code>Inactive</code> means it is not.</p>
     */
    inline void SetStatus(const StatusType& value) { m_statusHasBeenSet = true; m_status = value; }

    /**
     * <p>The status of the signing certificate. <code>Active</code> means that the key
     * is valid for API calls, while <code>Inactive</code> means it is not.</p>
     */
    inline void SetStatus(StatusType&& value) { m_statusHasBeenSet = true; m_status = std::move(value); }

    /**
     * <p>The status of the signing certificate. <code>Active</code> means that the key
     * is valid for API calls, while <code>Inactive</code> means it is not.</p>
     */
    inline SigningCertificate& WithStatus(const StatusType& value) { SetStatus(value); return *this;}

    /**
     * <p>The status of the signing certificate. <code>Active</code> means that the key
     * is valid for API calls, while <code>Inactive</code> means it is not.</p>
     */
    inline SigningCertificate& WithStatus(StatusType&& value) { SetStatus(std::move(value)); return *this;}


    /**
     * <p>The date when the signing certificate was uploaded.</p>
     */
    inline const Aws::Utils::DateTime& GetUploadDate() const{ return m_uploadDate; }

    /**
     * <p>The date when the signing certificate was uploaded.</p>
     */
    inline bool UploadDateHasBeenSet() const { return m_uploadDateHasBeenSet; }

    /**
     * <p>The date when the signing certificate was uploaded.</p>
     */
    inline void SetUploadDate(const Aws::Utils::DateTime& value) { m_uploadDateHasBeenSet = true; m_uploadDate = value; }

    /**
     * <p>The date when the signing certificate was uploaded.</p>
     */
    inline void SetUploadDate(Aws::Utils::DateTime&& value) { m_uploadDateHasBeenSet = true; m_uploadDate = std::move(value); }

    /**
     * <p>The date when the signing certificate was uploaded.</p>
     */
    inline SigningCertificate& WithUploadDate(const Aws::Utils::DateTime& value) { SetUploadDate(value); return *this;}

    /**
     * <p>The date when the signing certificate was uploaded.</p>
     */
    inline SigningCertificate& WithUploadDate(Aws::Utils::DateTime&& value) { SetUploadDate(std::move(value)); return *this;}

  private:

    Aws::String m_userName;
    bool m_userNameHasBeenSet = false;

    Aws::String m_certificateId;
    bool m_certificateIdHasBeenSet = false;

    Aws::String m_certificateBody;
    bool m_certificateBodyHasBeenSet = false;

    StatusType m_status;
    bool m_statusHasBeenSet = false;

    Aws::Utils::DateTime m_uploadDate;
    bool m_uploadDateHasBeenSet = false;
  };

} // namespace Model
} // namespace IAM
} // namespace Aws
