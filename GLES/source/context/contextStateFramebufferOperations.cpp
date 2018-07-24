/**
 * Copyright (C) 2015-2018 Think Silicon S.A. (https://think-silicon.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public v3
 * License as published by the Free Software Foundation;
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

/**
 *  @file       contextStateFramebufferOperations.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Framebuffer Operations
 *
 */

#include "context.h"

void
Context::ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mStateManager.GetFramebufferOperationsState()->UpdateClearColor(red, green, blue, alpha)) {
        mClearPass->SetUpdateColorState(true);
    }
}

void
Context::ClearDepthf(GLclampf depth)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mStateManager.GetFramebufferOperationsState()->UpdateClearDepth(depth)) {
        mClearPass->SetUpdateDepthState(true);
    }
}

void
Context::ClearStencil(GLint s)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mStateManager.GetFramebufferOperationsState()->UpdateClearStencil(s)) {
        mClearPass->SetUpdateStencilState(true);
    }
}

void
Context::ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mStateManager.GetFramebufferOperationsState()->UpdateColorMask(red, green, blue, alpha)) {
        mPipeline->SetColorBlendAttachmentWriteMask(static_cast<VkColorComponentFlagBits>(mStateManager.GetFramebufferOperationsState()->GetColorMask()));
    }
}

void
Context::DepthMask(GLboolean flag)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mStateManager.GetFramebufferOperationsState()->UpdateDepthMask(flag)) {
        mPipeline->SetDepthWriteEnable(GlBooleanToVkBool(mStateManager.GetFramebufferOperationsState()->GetDepthMask()));
    }
}

void
Context::StencilMask(GLuint mask)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mStateManager.GetFramebufferOperationsState()->UpdateStencilMask(mask)) {
        mPipeline->SetStencilBackWriteMask(mStateManager.GetFramebufferOperationsState()->GetStencilMaskBack());
        mPipeline->SetStencilFrontWriteMask(mStateManager.GetFramebufferOperationsState()->GetStencilMaskFront());
    }
}

void
Context::StencilMaskSeparate(GLenum face, GLuint mask)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if (face != GL_FRONT && face != GL_BACK && face != GL_FRONT_AND_BACK) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if((face == GL_FRONT || face == GL_FRONT_AND_BACK) && mStateManager.GetFramebufferOperationsState()->UpdateStencilMaskFront(mask)) {
        mPipeline->SetStencilFrontWriteMask(mStateManager.GetFramebufferOperationsState()->GetStencilMaskFront());
    }

    if((face == GL_BACK || face == GL_FRONT_AND_BACK) && mStateManager.GetFramebufferOperationsState()->UpdateStencilMaskBack(mask)) {
        mPipeline->SetStencilBackWriteMask(mStateManager.GetFramebufferOperationsState()->GetStencilMaskBack());
    }
}
