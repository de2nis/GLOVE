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
 *  @file       contextRenderBuffer.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Renderbuffer
 *
 */

#include "context.h"

void
Context::BindRenderbuffer(GLenum target, GLuint renderbuffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_RENDERBUFFER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(!renderbuffer) {
        mStateManager.GetActiveObjectsState()->SetActiveRenderbufferObjectID(0);
        return;
    }

    Renderbuffer *rendbuff = mResourceManager.GetRenderbuffer(renderbuffer);
    if(rendbuff->GetTarget() == GL_INVALID_VALUE) {
        rendbuff->SetVkContext(mVkContext);
        rendbuff->SetTarget(target);
    }
    mStateManager.GetActiveObjectsState()->SetActiveRenderbufferObjectID(renderbuffer);
}

void
Context::DeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(n < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    while(n-- != 0) {
        uint32_t index = *renderbuffers++;

        if(index && mResourceManager.RenderbufferExists(index)) {
            Renderbuffer *rbo = mResourceManager.GetRenderbuffer(index);

            if(rbo->GetTexture() == mWriteFBO->GetColorAttachmentTexture()) {
                mWriteFBO->SetColorAttachmentTexture(nullptr);
                mWriteFBO->SetColorAttachmentType(GL_NONE);
                mWriteFBO->SetColorAttachmentName(0);
            }

            if(rbo->GetTexture() == mWriteFBO->GetDepthAttachmentTexture()) {
                mWriteFBO->SetDepthAttachmentTexture(nullptr);
                mWriteFBO->SetDepthAttachmentType(GL_NONE);
                mWriteFBO->SetDepthAttachmentName(0);
            }

            if(rbo->GetTexture() == mWriteFBO->GetStencilAttachmentTexture()) {
                mWriteFBO->SetStencilAttachmentTexture(nullptr);
                mWriteFBO->SetStencilAttachmentType(GL_NONE);
                mWriteFBO->SetStencilAttachmentName(0);
            }

            if(mStateManager.GetActiveObjectsState()->EqualsActiveRenderbufferObject(index)) {
                mStateManager.GetActiveObjectsState()->SetActiveRenderbufferObjectID(0);
            }
            mResourceManager.DeallocateRenderbuffer(index);
        }
    }
}

void
Context::GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(n < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    while (n != 0) {
        *renderbuffers++ = mResourceManager.AllocateRenderbuffer();
        --n;
    }
}

void
Context::GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_RENDERBUFFER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    uint32_t activeRenderbufferId = mStateManager.GetActiveObjectsState()->GetActiveRenderbufferObjectID();

    if(!activeRenderbufferId) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    Renderbuffer* activeRenderbuffer = mResourceManager.GetRenderbuffer(activeRenderbufferId);

    if(activeRenderbuffer->GetTarget() == GL_INVALID_VALUE) {
        *params = (pname == GL_RENDERBUFFER_INTERNAL_FORMAT) ? GL_RGBA4 : 0;
        return;
    }

    switch(pname) {
    case GL_RENDERBUFFER_WIDTH:             *params = activeRenderbuffer->GetWidth(); break;
    case GL_RENDERBUFFER_HEIGHT:            *params = activeRenderbuffer->GetHeight(); break;
    case GL_RENDERBUFFER_INTERNAL_FORMAT:   *params = activeRenderbuffer->GetInternalFormat(); break;
    case GL_RENDERBUFFER_RED_SIZE:          GlFormatToStorageBits(activeRenderbuffer->GetInternalFormat(), params, NULL, NULL, NULL, NULL, NULL); break;
    case GL_RENDERBUFFER_GREEN_SIZE:        GlFormatToStorageBits(activeRenderbuffer->GetInternalFormat(), NULL, params, NULL, NULL, NULL, NULL); break;
    case GL_RENDERBUFFER_BLUE_SIZE:         GlFormatToStorageBits(activeRenderbuffer->GetInternalFormat(), NULL, NULL, params, NULL, NULL, NULL); break;
    case GL_RENDERBUFFER_ALPHA_SIZE:        GlFormatToStorageBits(activeRenderbuffer->GetInternalFormat(), NULL, NULL, NULL, params, NULL, NULL); break;
    case GL_RENDERBUFFER_DEPTH_SIZE:        GlFormatToStorageBits(activeRenderbuffer->GetInternalFormat(), NULL, NULL, NULL, NULL, params, NULL); break;
    case GL_RENDERBUFFER_STENCIL_SIZE:      GlFormatToStorageBits(activeRenderbuffer->GetInternalFormat(), NULL, NULL, NULL, NULL, NULL, params); break;
    default:                                RecordError(GL_INVALID_ENUM); break;
    }
}

GLboolean
Context::IsRenderbuffer(GLuint renderbuffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(renderbuffer && mResourceManager.RenderbufferExists(renderbuffer)) {
        Renderbuffer *pRenderBuffer = mResourceManager.GetRenderbuffer(renderbuffer);
        return (pRenderBuffer && pRenderBuffer->GetTarget() != GL_INVALID_VALUE) ? GL_TRUE : GL_FALSE;
    }

    return GL_FALSE;
}

void
Context::RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_RENDERBUFFER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(width < 0 || width > GLOVE_MAX_RENDERBUFFER_SIZE || height < 0 || height > GLOVE_MAX_RENDERBUFFER_SIZE) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(internalformat != GL_RGBA4             && internalformat != GL_RGB565 && internalformat != GL_RGB5_A1 &&
       internalformat != GL_DEPTH_COMPONENT16 && internalformat != GL_STENCIL_INDEX8) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    uint32_t activeRenderbufferId = mStateManager.GetActiveObjectsState()->GetActiveRenderbufferObjectID();
    if(!activeRenderbufferId) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    Renderbuffer* activeRenderbuffer = mResourceManager.GetRenderbuffer(activeRenderbufferId);
    if(!activeRenderbuffer->Allocate(width, height, internalformat)) {
        RecordError(GL_OUT_OF_MEMORY);
        return;
    }
}
