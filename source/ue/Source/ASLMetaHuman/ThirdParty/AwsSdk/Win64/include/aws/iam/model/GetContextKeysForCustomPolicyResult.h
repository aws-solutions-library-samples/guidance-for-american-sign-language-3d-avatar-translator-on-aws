﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iam/IAM_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/iam/model/ResponseMetadata.h>
#include <aws/core/utils/memory/stl/AWSString.h>
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
   * <p>Contains the response to a successful <a>GetContextKeysForPrincipalPolicy</a>
   * or <a>GetContextKeysForCustomPolicy</a> request. </p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/iam-2010-05-08/GetContextKeysForPolicyResponse">AWS
   * API Reference</a></p>
   */
  class GetContextKeysForCustomPolicyResult
  {
  public:
    AWS_IAM_API GetContextKeysForCustomPolicyResult();
    AWS_IAM_API GetContextKeysForCustomPolicyResult(const Aws::AmazonWebServiceResult<Aws::Utils::Xml::XmlDocument>& result);
    AWS_IAM_API GetContextKeysForCustomPolicyResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Xml::XmlDocument>& result);


    /**
     * <p>The list of context keys that are referenced in the input policies.</p>
     */
    inline const Aws::Vector<Aws::String>& GetContextKeyNames() const{ return m_contextKeyNames; }

    /**
     * <p>The list of context keys that are referenced in the input policies.</p>
     */
    inline void SetContextKeyNames(const Aws::Vector<Aws::String>& value) { m_contextKeyNames = value; }

    /**
     * <p>The list of context keys that are referenced in the input policies.</p>
     */
    inline void SetContextKeyNames(Aws::Vector<Aws::String>&& value) { m_contextKeyNames = std::move(value); }

    /**
     * <p>The list of context keys that are referenced in the input policies.</p>
     */
    inline GetContextKeysForCustomPolicyResult& WithContextKeyNames(const Aws::Vector<Aws::String>& value) { SetContextKeyNames(value); return *this;}

    /**
     * <p>The list of context keys that are referenced in the input policies.</p>
     */
    inline GetContextKeysForCustomPolicyResult& WithContextKeyNames(Aws::Vector<Aws::String>&& value) { SetContextKeyNames(std::move(value)); return *this;}

    /**
     * <p>The list of context keys that are referenced in the input policies.</p>
     */
    inline GetContextKeysForCustomPolicyResult& AddContextKeyNames(const Aws::String& value) { m_contextKeyNames.push_back(value); return *this; }

    /**
     * <p>The list of context keys that are referenced in the input policies.</p>
     */
    inline GetContextKeysForCustomPolicyResult& AddContextKeyNames(Aws::String&& value) { m_contextKeyNames.push_back(std::move(value)); return *this; }

    /**
     * <p>The list of context keys that are referenced in the input policies.</p>
     */
    inline GetContextKeysForCustomPolicyResult& AddContextKeyNames(const char* value) { m_contextKeyNames.push_back(value); return *this; }


    
    inline const ResponseMetadata& GetResponseMetadata() const{ return m_responseMetadata; }

    
    inline void SetResponseMetadata(const ResponseMetadata& value) { m_responseMetadata = value; }

    
    inline void SetResponseMetadata(ResponseMetadata&& value) { m_responseMetadata = std::move(value); }

    
    inline GetContextKeysForCustomPolicyResult& WithResponseMetadata(const ResponseMetadata& value) { SetResponseMetadata(value); return *this;}

    
    inline GetContextKeysForCustomPolicyResult& WithResponseMetadata(ResponseMetadata&& value) { SetResponseMetadata(std::move(value)); return *this;}

  private:

    Aws::Vector<Aws::String> m_contextKeyNames;

    ResponseMetadata m_responseMetadata;
  };

} // namespace Model
} // namespace IAM
} // namespace Aws
