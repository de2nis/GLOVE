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
 *  @file       contextFrameBuffer.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Framebuffer
 *
 */

#include "context.h"

void
Context::BindFramebuffer(GLenum target, GLuint framebuffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_FRAMEBUFFER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    Framebuffer *fbo;
    if(framebuffer) {
        fbo = mResourceManager.GetFramebuffer(framebuffer);
        if(fbo->GetTarget() == GL_INVALID_VALUE) {
            fbo->SetTarget(target);
            fbo->SetVkContext(mVkContext);
        }
    }

    mWriteFBO = framebuffer ? fbo : mSystemFBO;
    mStateManager.GetActiveObjectsState()->SetActiveFramebufferObjectID(framebuffer);
    mPipeline->SetUpdatePipeline(true);
    mPipeline->SetUpdateViewportState(true);
}

GLenum
Context::CheckFramebufferStatus(GLenum target)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_FRAMEBUFFER) {
        RecordError(GL_INVALID_ENUM);
        return 0;
    }

    return (mStateManager.GetActiveObjectsState()->IsDefaultFramebufferObjectActive()) ?
            GL_FRAMEBUFFER_COMPLETE :
            mResourceManager.GetFramebuffer(mStateManager.GetActiveObjectsState()->GetActiveFramebufferObjectID())->CheckStatus();
}

void
Context::DeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(n < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    while(n-- != 0) {
        uint32_t fboindex = *framebuffers++;

        if(fboindex && mResourceManager.FramebufferExists(fboindex)) {
            Framebuffer *fbo = mResourceManager.GetFramebuffer(fboindex);

            if(fbo == mWriteFBO) {
                mWriteFBO = mSystemFBO;

                mStateManager.GetActiveObjectsState()->SetActiveFramebufferObjectID(0);
                mPipeline->SetUpdatePipeline(true);
                mPipeline->SetUpdateViewportState(true);
            }

            mResourceManager.DeallocateFramebuffer(fboindex);
        }
    }
}

void
Context::FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_FRAMEBUFFER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(renderbuffertarget != GL_RENDERBUFFER && renderbuffer) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if( mStateManager.GetActiveObjectsState()->IsDefaultFramebufferObjectActive() ||
      ( renderbuffer && !mResourceManager.RenderbufferExists(renderbuffer))) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    switch(attachment) {
    case GL_COLOR_ATTACHMENT0:
        mWriteFBO->SetColorAttachmentTexture(renderbuffer ? mResourceManager.GetRenderbuffer(renderbuffer)->GetTexture() : nullptr);
        mWriteFBO->SetColorAttachmentType(renderbuffer ? GL_RENDERBUFFER : GL_NONE);
        mWriteFBO->SetColorAttachmentName(renderbuffer);
        break;
    case GL_DEPTH_ATTACHMENT:
        mWriteFBO->SetDepthAttachmentTexture(renderbuffer ? mResourceManager.GetRenderbuffer(renderbuffer)->GetTexture() : nullptr);
        mWriteFBO->SetDepthAttachmentType(renderbuffer ? GL_RENDERBUFFER : GL_NONE);
        mWriteFBO->SetDepthAttachmentName(renderbuffer);
        break;
    case GL_STENCIL_ATTACHMENT:
        mWriteFBO->SetStencilAttachmentTexture(renderbuffer ? mResourceManager.GetRenderbuffer(renderbuffer)->GetTexture() : nullptr);
        mWriteFBO->SetStencilAttachmentType(renderbuffer ? GL_RENDERBUFFER : GL_NONE);
        mWriteFBO->SetStencilAttachmentName(renderbuffer);
        break;
    default:
        RecordError(GL_INVALID_ENUM);
        break;
    }
}

void
Context::FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_FRAMEBUFFER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(texture &&
       (textarget != GL_TEXTURE_2D && (textarget < GL_TEXTURE_CUBE_MAP_POSITIVE_X || textarget > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z))) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(texture && level) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(mStateManager.GetActiveObjectsState()->IsDefaultFramebufferObjectActive() ||
       (texture &&  mResourceManager.TextureExists(texture) &&
       ((mResourceManager.GetTexture(texture)->GetTarget() == GL_TEXTURE_2D       && textarget != GL_TEXTURE_2D) ||
        (mResourceManager.GetTexture(texture)->GetTarget() == GL_TEXTURE_CUBE_MAP && textarget == GL_TEXTURE_2D))
      )
      ) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    switch(attachment) {
    case GL_COLOR_ATTACHMENT0:
        mWriteFBO->SetColorAttachmentTexture(texture ? mResourceManager.GetTexture(texture) : nullptr);
        mWriteFBO->SetColorAttachmentType(texture ? GL_TEXTURE : GL_NONE);
        mWriteFBO->SetColorAttachmentName(texture);
        mWriteFBO->SetColorAttachmentLayer(texture && mResourceManager.GetTexture(texture)->IsCubeMap() ? textarget : GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        break;
    case GL_DEPTH_ATTACHMENT:
        mWriteFBO->SetDepthAttachmentTexture(texture ? mResourceManager.GetTexture(texture) : nullptr);
        mWriteFBO->SetDepthAttachmentType(texture ? GL_TEXTURE : GL_NONE);
        mWriteFBO->SetDepthAttachmentName(texture);
        mWriteFBO->SetDepthAttachmentLayer(texture && mResourceManager.GetTexture(texture)->IsCubeMap() ? textarget : GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        break;
    case GL_STENCIL_ATTACHMENT:
        mWriteFBO->SetStencilAttachmentTexture(texture ? mResourceManager.GetTexture(texture) : nullptr);
        mWriteFBO->SetStencilAttachmentType(texture ? GL_TEXTURE : GL_NONE);
        mWriteFBO->SetStencilAttachmentName(texture);
        mWriteFBO->SetStencilAttachmentLayer(texture && mResourceManager.GetTexture(texture)->IsCubeMap() ? textarget : GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        break;
    default:
        RecordError(GL_INVALID_ENUM);
        break;
    }
}

void
Context::GenFramebuffers(GLsizei n, GLuint* framebuffers)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(n < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    while (n != 0) {
        *framebuffers++ = mResourceManager.AllocateFramebuffer();
        --n;
    }
}

void
Context::GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_FRAMEBUFFER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(mStateManager.GetActiveObjectsState()->IsDefaultFramebufferObjectActive()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    uint32_t activeFBOid = mStateManager.GetActiveObjectsState()->GetActiveFramebufferObjectID();
    Framebuffer *fbo = mResourceManager.GetFramebuffer(activeFBOid);

    GLenum type  = GL_NONE;
    GLenum name  = 0;
    GLint  level = 0;
    GLenum layer = GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    switch(attachment) {
    case GL_COLOR_ATTACHMENT0:
        type  = fbo->GetColorAttachmentType();
        name  = fbo->GetColorAttachmentName();
        if(type == GL_TEXTURE) {
            level = fbo->GetColorAttachmentLevel();
            layer = mResourceManager.GetTexture(name)->IsCubeMap() ? fbo->GetColorAttachmentLayer() : 0;
        }
        break;
    case GL_DEPTH_ATTACHMENT:
        type  = fbo->GetDepthAttachmentType();
        name  = fbo->GetDepthAttachmentName();
        if(type == GL_TEXTURE) {
            level = fbo->GetDepthAttachmentLevel();
            layer = mResourceManager.GetTexture(name)->IsCubeMap() ? fbo->GetDepthAttachmentLayer() : 0;
        }
        break;
    case GL_STENCIL_ATTACHMENT:
        type = fbo->GetStencilAttachmentType();
        name = fbo->GetStencilAttachmentName();
        if(type == GL_TEXTURE) {
            level = fbo->GetStencilAttachmentLevel();
            layer = mResourceManager.GetTexture(name)->IsCubeMap() ? fbo->GetStencilAttachmentLayer() : 0;
        }
        break;
    default:
        RecordError(GL_INVALID_ENUM);
        break;
    }

    if(type == GL_NONE && pname != GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(type == GL_RENDERBUFFER &&
      (pname != GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE   && pname != GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(type == GL_TEXTURE &&
      (pname != GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE   && pname != GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME        &&
       pname != GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL && pname != GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE)
      ) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    switch(pname) {
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:             *params = static_cast<GLint>(type);   break;
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:             *params = static_cast<GLint>(name);   break;
    case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:           *params = level;                      break;
    case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE:   *params = static_cast<GLint>(layer);  break;
    }
}

GLboolean
Context::IsFramebuffer(GLuint framebuffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(framebuffer && mResourceManager.FramebufferExists(framebuffer)) {
        Framebuffer *pFrameBuffer = mResourceManager.GetFramebuffer(framebuffer);
        return (pFrameBuffer && pFrameBuffer->GetTarget() != GL_INVALID_VALUE) ? GL_TRUE : GL_FALSE;
    }

    return GL_FALSE;
}
