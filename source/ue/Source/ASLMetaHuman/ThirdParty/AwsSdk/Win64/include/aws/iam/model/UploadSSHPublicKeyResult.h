﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iam/IAM_EXPORTS.h>
#include <aws/iam/model/SSHPublicKey.h>
#include <aws/iam/model/ResponseMetadata.h>
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
namespace IAM
{
namespace Model
{
  /**
   * <p>Contains the response to a successful <a>UploadSSHPublicKey</a>
   * request.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/iam-2010-05-08/UploadSSHPublicKeyResponse">AWS
   * API Reference</a></p>
   */
  class UploadSSHPublicKeyResult
  {
  public:
    AWS_IAM_API UploadSSHPublicKeyResult();
    AWS_IAM_API UploadSSHPublicKeyResult(const Aws::AmazonWebServiceResult<Aws::Utils::Xml::XmlDocument>& result);
    AWS_IAM_API UploadSSHPublicKeyResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Xml::XmlDocument>& result);


    /**
     * <p>Contains information about the SSH public key.</p>
     */
    inline const SSHPublicKey& GetSSHPublicKey() const{ return m_sSHPublicKey; }

    /**
     * <p>Contains information about the SSH public key.</p>
     */
    inline void SetSSHPublicKey(const SSHPublicKey& value) { m_sSHPublicKey = value; }

    /**
     * <p>Contains information about the SSH public key.</p>
     */
    inline void SetSSHPublicKey(SSHPublicKey&& value) { m_sSHPublicKey = std::move(value); }

    /**
     * <p>Contains information about the SSH public key.</p>
     */
    inline UploadSSHPublicKeyResult& WithSSHPublicKey(const SSHPublicKey& value) { SetSSHPublicKey(value); return *this;}

    /**
     * <p>Contains information about the SSH public key.</p>
     */
    inline UploadSSHPublicKeyResult& WithSSHPublicKey(SSHPublicKey&& value) { SetSSHPublicKey(std::move(value)); return *this;}


    
    inline const ResponseMetadata& GetResponseMetadata() const{ return m_responseMetadata; }

    
    inline void SetResponseMetadata(const ResponseMetadata& value) { m_responseMetadata = value; }

    
    inline void SetResponseMetadata(ResponseMetadata&& value) { m_responseMetadata = std::move(value); }

    
    inline UploadSSHPublicKeyResult& WithResponseMetadata(const ResponseMetadata& value) { SetResponseMetadata(value); return *this;}

    
    inline UploadSSHPublicKeyResult& WithResponseMetadata(ResponseMetadata&& value) { SetResponseMetadata(std::move(value)); return *this;}

  private:

    SSHPublicKey m_sSHPublicKey;

    ResponseMetadata m_responseMetadata;
  };

} // namespace Model
} // namespace IAM
} // namespace Aws
