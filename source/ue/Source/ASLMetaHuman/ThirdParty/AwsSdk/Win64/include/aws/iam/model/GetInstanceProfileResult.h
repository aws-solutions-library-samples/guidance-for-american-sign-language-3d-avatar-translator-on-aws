﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iam/IAM_EXPORTS.h>
#include <aws/iam/model/InstanceProfile.h>
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
   * <p>Contains the response to a successful <a>GetInstanceProfile</a> request.
   * </p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/iam-2010-05-08/GetInstanceProfileResponse">AWS
   * API Reference</a></p>
   */
  class GetInstanceProfileResult
  {
  public:
    AWS_IAM_API GetInstanceProfileResult();
    AWS_IAM_API GetInstanceProfileResult(const Aws::AmazonWebServiceResult<Aws::Utils::Xml::XmlDocument>& result);
    AWS_IAM_API GetInstanceProfileResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Xml::XmlDocument>& result);


    /**
     * <p>A structure containing details about the instance profile.</p>
     */
    inline const InstanceProfile& GetInstanceProfile() const{ return m_instanceProfile; }

    /**
     * <p>A structure containing details about the instance profile.</p>
     */
    inline void SetInstanceProfile(const InstanceProfile& value) { m_instanceProfile = value; }

    /**
     * <p>A structure containing details about the instance profile.</p>
     */
    inline void SetInstanceProfile(InstanceProfile&& value) { m_instanceProfile = std::move(value); }

    /**
     * <p>A structure containing details about the instance profile.</p>
     */
    inline GetInstanceProfileResult& WithInstanceProfile(const InstanceProfile& value) { SetInstanceProfile(value); return *this;}

    /**
     * <p>A structure containing details about the instance profile.</p>
     */
    inline GetInstanceProfileResult& WithInstanceProfile(InstanceProfile&& value) { SetInstanceProfile(std::move(value)); return *this;}


    
    inline const ResponseMetadata& GetResponseMetadata() const{ return m_responseMetadata; }

    
    inline void SetResponseMetadata(const ResponseMetadata& value) { m_responseMetadata = value; }

    
    inline void SetResponseMetadata(ResponseMetadata&& value) { m_responseMetadata = std::move(value); }

    
    inline GetInstanceProfileResult& WithResponseMetadata(const ResponseMetadata& value) { SetResponseMetadata(value); return *this;}

    
    inline GetInstanceProfileResult& WithResponseMetadata(ResponseMetadata&& value) { SetResponseMetadata(std::move(value)); return *this;}

  private:

    InstanceProfile m_instanceProfile;

    ResponseMetadata m_responseMetadata;
  };

} // namespace Model
} // namespace IAM
} // namespace Aws
