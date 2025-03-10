﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iam/IAM_EXPORTS.h>
#include <aws/iam/IAMRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace IAM
{
namespace Model
{

  /**
   */
  class DeleteRolePermissionsBoundaryRequest : public IAMRequest
  {
  public:
    AWS_IAM_API DeleteRolePermissionsBoundaryRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteRolePermissionsBoundary"; }

    AWS_IAM_API Aws::String SerializePayload() const override;

  protected:
    AWS_IAM_API void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>The name (friendly name, not ARN) of the IAM role from which you want to
     * remove the permissions boundary.</p>
     */
    inline const Aws::String& GetRoleName() const{ return m_roleName; }

    /**
     * <p>The name (friendly name, not ARN) of the IAM role from which you want to
     * remove the permissions boundary.</p>
     */
    inline bool RoleNameHasBeenSet() const { return m_roleNameHasBeenSet; }

    /**
     * <p>The name (friendly name, not ARN) of the IAM role from which you want to
     * remove the permissions boundary.</p>
     */
    inline void SetRoleName(const Aws::String& value) { m_roleNameHasBeenSet = true; m_roleName = value; }

    /**
     * <p>The name (friendly name, not ARN) of the IAM role from which you want to
     * remove the permissions boundary.</p>
     */
    inline void SetRoleName(Aws::String&& value) { m_roleNameHasBeenSet = true; m_roleName = std::move(value); }

    /**
     * <p>The name (friendly name, not ARN) of the IAM role from which you want to
     * remove the permissions boundary.</p>
     */
    inline void SetRoleName(const char* value) { m_roleNameHasBeenSet = true; m_roleName.assign(value); }

    /**
     * <p>The name (friendly name, not ARN) of the IAM role from which you want to
     * remove the permissions boundary.</p>
     */
    inline DeleteRolePermissionsBoundaryRequest& WithRoleName(const Aws::String& value) { SetRoleName(value); return *this;}

    /**
     * <p>The name (friendly name, not ARN) of the IAM role from which you want to
     * remove the permissions boundary.</p>
     */
    inline DeleteRolePermissionsBoundaryRequest& WithRoleName(Aws::String&& value) { SetRoleName(std::move(value)); return *this;}

    /**
     * <p>The name (friendly name, not ARN) of the IAM role from which you want to
     * remove the permissions boundary.</p>
     */
    inline DeleteRolePermissionsBoundaryRequest& WithRoleName(const char* value) { SetRoleName(value); return *this;}

  private:

    Aws::String m_roleName;
    bool m_roleNameHasBeenSet = false;
  };

} // namespace Model
} // namespace IAM
} // namespace Aws
