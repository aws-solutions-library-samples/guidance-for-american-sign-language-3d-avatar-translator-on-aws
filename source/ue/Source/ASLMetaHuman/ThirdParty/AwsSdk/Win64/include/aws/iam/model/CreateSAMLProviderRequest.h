﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iam/IAM_EXPORTS.h>
#include <aws/iam/IAMRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/iam/model/Tag.h>
#include <utility>

namespace Aws
{
namespace IAM
{
namespace Model
{

  /**
   */
  class CreateSAMLProviderRequest : public IAMRequest
  {
  public:
    AWS_IAM_API CreateSAMLProviderRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CreateSAMLProvider"; }

    AWS_IAM_API Aws::String SerializePayload() const override;

  protected:
    AWS_IAM_API void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>An XML document generated by an identity provider (IdP) that supports SAML
     * 2.0. The document includes the issuer's name, expiration information, and keys
     * that can be used to validate the SAML authentication response (assertions) that
     * are received from the IdP. You must generate the metadata document using the
     * identity management software that is used as your organization's IdP.</p> <p>For
     * more information, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_roles_providers_saml.html">About
     * SAML 2.0-based federation</a> in the <i>IAM User Guide</i> </p>
     */
    inline const Aws::String& GetSAMLMetadataDocument() const{ return m_sAMLMetadataDocument; }

    /**
     * <p>An XML document generated by an identity provider (IdP) that supports SAML
     * 2.0. The document includes the issuer's name, expiration information, and keys
     * that can be used to validate the SAML authentication response (assertions) that
     * are received from the IdP. You must generate the metadata document using the
     * identity management software that is used as your organization's IdP.</p> <p>For
     * more information, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_roles_providers_saml.html">About
     * SAML 2.0-based federation</a> in the <i>IAM User Guide</i> </p>
     */
    inline bool SAMLMetadataDocumentHasBeenSet() const { return m_sAMLMetadataDocumentHasBeenSet; }

    /**
     * <p>An XML document generated by an identity provider (IdP) that supports SAML
     * 2.0. The document includes the issuer's name, expiration information, and keys
     * that can be used to validate the SAML authentication response (assertions) that
     * are received from the IdP. You must generate the metadata document using the
     * identity management software that is used as your organization's IdP.</p> <p>For
     * more information, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_roles_providers_saml.html">About
     * SAML 2.0-based federation</a> in the <i>IAM User Guide</i> </p>
     */
    inline void SetSAMLMetadataDocument(const Aws::String& value) { m_sAMLMetadataDocumentHasBeenSet = true; m_sAMLMetadataDocument = value; }

    /**
     * <p>An XML document generated by an identity provider (IdP) that supports SAML
     * 2.0. The document includes the issuer's name, expiration information, and keys
     * that can be used to validate the SAML authentication response (assertions) that
     * are received from the IdP. You must generate the metadata document using the
     * identity management software that is used as your organization's IdP.</p> <p>For
     * more information, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_roles_providers_saml.html">About
     * SAML 2.0-based federation</a> in the <i>IAM User Guide</i> </p>
     */
    inline void SetSAMLMetadataDocument(Aws::String&& value) { m_sAMLMetadataDocumentHasBeenSet = true; m_sAMLMetadataDocument = std::move(value); }

    /**
     * <p>An XML document generated by an identity provider (IdP) that supports SAML
     * 2.0. The document includes the issuer's name, expiration information, and keys
     * that can be used to validate the SAML authentication response (assertions) that
     * are received from the IdP. You must generate the metadata document using the
     * identity management software that is used as your organization's IdP.</p> <p>For
     * more information, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_roles_providers_saml.html">About
     * SAML 2.0-based federation</a> in the <i>IAM User Guide</i> </p>
     */
    inline void SetSAMLMetadataDocument(const char* value) { m_sAMLMetadataDocumentHasBeenSet = true; m_sAMLMetadataDocument.assign(value); }

    /**
     * <p>An XML document generated by an identity provider (IdP) that supports SAML
     * 2.0. The document includes the issuer's name, expiration information, and keys
     * that can be used to validate the SAML authentication response (assertions) that
     * are received from the IdP. You must generate the metadata document using the
     * identity management software that is used as your organization's IdP.</p> <p>For
     * more information, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_roles_providers_saml.html">About
     * SAML 2.0-based federation</a> in the <i>IAM User Guide</i> </p>
     */
    inline CreateSAMLProviderRequest& WithSAMLMetadataDocument(const Aws::String& value) { SetSAMLMetadataDocument(value); return *this;}

    /**
     * <p>An XML document generated by an identity provider (IdP) that supports SAML
     * 2.0. The document includes the issuer's name, expiration information, and keys
     * that can be used to validate the SAML authentication response (assertions) that
     * are received from the IdP. You must generate the metadata document using the
     * identity management software that is used as your organization's IdP.</p> <p>For
     * more information, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_roles_providers_saml.html">About
     * SAML 2.0-based federation</a> in the <i>IAM User Guide</i> </p>
     */
    inline CreateSAMLProviderRequest& WithSAMLMetadataDocument(Aws::String&& value) { SetSAMLMetadataDocument(std::move(value)); return *this;}

    /**
     * <p>An XML document generated by an identity provider (IdP) that supports SAML
     * 2.0. The document includes the issuer's name, expiration information, and keys
     * that can be used to validate the SAML authentication response (assertions) that
     * are received from the IdP. You must generate the metadata document using the
     * identity management software that is used as your organization's IdP.</p> <p>For
     * more information, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_roles_providers_saml.html">About
     * SAML 2.0-based federation</a> in the <i>IAM User Guide</i> </p>
     */
    inline CreateSAMLProviderRequest& WithSAMLMetadataDocument(const char* value) { SetSAMLMetadataDocument(value); return *this;}


    /**
     * <p>The name of the provider to create.</p> <p>This parameter allows (through its
     * <a href="http://wikipedia.org/wiki/regex">regex pattern</a>) a string of
     * characters consisting of upper and lowercase alphanumeric characters with no
     * spaces. You can also include any of the following characters: _+=,.@-</p>
     */
    inline const Aws::String& GetName() const{ return m_name; }

    /**
     * <p>The name of the provider to create.</p> <p>This parameter allows (through its
     * <a href="http://wikipedia.org/wiki/regex">regex pattern</a>) a string of
     * characters consisting of upper and lowercase alphanumeric characters with no
     * spaces. You can also include any of the following characters: _+=,.@-</p>
     */
    inline bool NameHasBeenSet() const { return m_nameHasBeenSet; }

    /**
     * <p>The name of the provider to create.</p> <p>This parameter allows (through its
     * <a href="http://wikipedia.org/wiki/regex">regex pattern</a>) a string of
     * characters consisting of upper and lowercase alphanumeric characters with no
     * spaces. You can also include any of the following characters: _+=,.@-</p>
     */
    inline void SetName(const Aws::String& value) { m_nameHasBeenSet = true; m_name = value; }

    /**
     * <p>The name of the provider to create.</p> <p>This parameter allows (through its
     * <a href="http://wikipedia.org/wiki/regex">regex pattern</a>) a string of
     * characters consisting of upper and lowercase alphanumeric characters with no
     * spaces. You can also include any of the following characters: _+=,.@-</p>
     */
    inline void SetName(Aws::String&& value) { m_nameHasBeenSet = true; m_name = std::move(value); }

    /**
     * <p>The name of the provider to create.</p> <p>This parameter allows (through its
     * <a href="http://wikipedia.org/wiki/regex">regex pattern</a>) a string of
     * characters consisting of upper and lowercase alphanumeric characters with no
     * spaces. You can also include any of the following characters: _+=,.@-</p>
     */
    inline void SetName(const char* value) { m_nameHasBeenSet = true; m_name.assign(value); }

    /**
     * <p>The name of the provider to create.</p> <p>This parameter allows (through its
     * <a href="http://wikipedia.org/wiki/regex">regex pattern</a>) a string of
     * characters consisting of upper and lowercase alphanumeric characters with no
     * spaces. You can also include any of the following characters: _+=,.@-</p>
     */
    inline CreateSAMLProviderRequest& WithName(const Aws::String& value) { SetName(value); return *this;}

    /**
     * <p>The name of the provider to create.</p> <p>This parameter allows (through its
     * <a href="http://wikipedia.org/wiki/regex">regex pattern</a>) a string of
     * characters consisting of upper and lowercase alphanumeric characters with no
     * spaces. You can also include any of the following characters: _+=,.@-</p>
     */
    inline CreateSAMLProviderRequest& WithName(Aws::String&& value) { SetName(std::move(value)); return *this;}

    /**
     * <p>The name of the provider to create.</p> <p>This parameter allows (through its
     * <a href="http://wikipedia.org/wiki/regex">regex pattern</a>) a string of
     * characters consisting of upper and lowercase alphanumeric characters with no
     * spaces. You can also include any of the following characters: _+=,.@-</p>
     */
    inline CreateSAMLProviderRequest& WithName(const char* value) { SetName(value); return *this;}


    /**
     * <p>A list of tags that you want to attach to the new IAM SAML provider. Each tag
     * consists of a key name and an associated value. For more information about
     * tagging, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_tags.html">Tagging IAM
     * resources</a> in the <i>IAM User Guide</i>.</p>  <p>If any one of the tags
     * is invalid or if you exceed the allowed maximum number of tags, then the entire
     * request fails and the resource is not created.</p> 
     */
    inline const Aws::Vector<Tag>& GetTags() const{ return m_tags; }

    /**
     * <p>A list of tags that you want to attach to the new IAM SAML provider. Each tag
     * consists of a key name and an associated value. For more information about
     * tagging, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_tags.html">Tagging IAM
     * resources</a> in the <i>IAM User Guide</i>.</p>  <p>If any one of the tags
     * is invalid or if you exceed the allowed maximum number of tags, then the entire
     * request fails and the resource is not created.</p> 
     */
    inline bool TagsHasBeenSet() const { return m_tagsHasBeenSet; }

    /**
     * <p>A list of tags that you want to attach to the new IAM SAML provider. Each tag
     * consists of a key name and an associated value. For more information about
     * tagging, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_tags.html">Tagging IAM
     * resources</a> in the <i>IAM User Guide</i>.</p>  <p>If any one of the tags
     * is invalid or if you exceed the allowed maximum number of tags, then the entire
     * request fails and the resource is not created.</p> 
     */
    inline void SetTags(const Aws::Vector<Tag>& value) { m_tagsHasBeenSet = true; m_tags = value; }

    /**
     * <p>A list of tags that you want to attach to the new IAM SAML provider. Each tag
     * consists of a key name and an associated value. For more information about
     * tagging, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_tags.html">Tagging IAM
     * resources</a> in the <i>IAM User Guide</i>.</p>  <p>If any one of the tags
     * is invalid or if you exceed the allowed maximum number of tags, then the entire
     * request fails and the resource is not created.</p> 
     */
    inline void SetTags(Aws::Vector<Tag>&& value) { m_tagsHasBeenSet = true; m_tags = std::move(value); }

    /**
     * <p>A list of tags that you want to attach to the new IAM SAML provider. Each tag
     * consists of a key name and an associated value. For more information about
     * tagging, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_tags.html">Tagging IAM
     * resources</a> in the <i>IAM User Guide</i>.</p>  <p>If any one of the tags
     * is invalid or if you exceed the allowed maximum number of tags, then the entire
     * request fails and the resource is not created.</p> 
     */
    inline CreateSAMLProviderRequest& WithTags(const Aws::Vector<Tag>& value) { SetTags(value); return *this;}

    /**
     * <p>A list of tags that you want to attach to the new IAM SAML provider. Each tag
     * consists of a key name and an associated value. For more information about
     * tagging, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_tags.html">Tagging IAM
     * resources</a> in the <i>IAM User Guide</i>.</p>  <p>If any one of the tags
     * is invalid or if you exceed the allowed maximum number of tags, then the entire
     * request fails and the resource is not created.</p> 
     */
    inline CreateSAMLProviderRequest& WithTags(Aws::Vector<Tag>&& value) { SetTags(std::move(value)); return *this;}

    /**
     * <p>A list of tags that you want to attach to the new IAM SAML provider. Each tag
     * consists of a key name and an associated value. For more information about
     * tagging, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_tags.html">Tagging IAM
     * resources</a> in the <i>IAM User Guide</i>.</p>  <p>If any one of the tags
     * is invalid or if you exceed the allowed maximum number of tags, then the entire
     * request fails and the resource is not created.</p> 
     */
    inline CreateSAMLProviderRequest& AddTags(const Tag& value) { m_tagsHasBeenSet = true; m_tags.push_back(value); return *this; }

    /**
     * <p>A list of tags that you want to attach to the new IAM SAML provider. Each tag
     * consists of a key name and an associated value. For more information about
     * tagging, see <a
     * href="https://docs.aws.amazon.com/IAM/latest/UserGuide/id_tags.html">Tagging IAM
     * resources</a> in the <i>IAM User Guide</i>.</p>  <p>If any one of the tags
     * is invalid or if you exceed the allowed maximum number of tags, then the entire
     * request fails and the resource is not created.</p> 
     */
    inline CreateSAMLProviderRequest& AddTags(Tag&& value) { m_tagsHasBeenSet = true; m_tags.push_back(std::move(value)); return *this; }

  private:

    Aws::String m_sAMLMetadataDocument;
    bool m_sAMLMetadataDocumentHasBeenSet = false;

    Aws::String m_name;
    bool m_nameHasBeenSet = false;

    Aws::Vector<Tag> m_tags;
    bool m_tagsHasBeenSet = false;
  };

} // namespace Model
} // namespace IAM
} // namespace Aws
