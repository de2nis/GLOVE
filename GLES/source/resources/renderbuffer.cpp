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
 *  @file       renderbuffer.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Renderbuffer Functionality in GLOVE
 *
 *  @scope
 *
 *  Renderbuffer objects contain images (texture). They are created and used
 *  specifically with Framebuffer Objects. 
 *
 */

#include "renderbuffer.h"
#include "utils/glUtils.h"

Renderbuffer::Renderbuffer(const vkContext_t *vkContext)
: mVkContext(vkContext), mInternalFormat(GL_RGBA4), mTarget(GL_INVALID_VALUE), mTexture(nullptr)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

Renderbuffer::~Renderbuffer()
{
    FUN_ENTRY(GL_LOG_TRACE);

    Release();
}

void
Renderbuffer::Release()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mTexture) {
        delete mTexture;
        mTexture = nullptr;
    }
}

bool
Renderbuffer::Allocate(GLint width, GLint height, GLenum internalformat)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    Release();

    mDims.width     = width;
    mDims.height    = height;
    mInternalFormat = internalformat;

    mTexture = new Texture(mVkContext);
    mTexture->SetTarget(GL_TEXTURE_2D);
    mTexture->SetVkFormat(GlInternalFormatToVkFormat(internalformat));

    if(GlFormatIsColor(mInternalFormat)) {
        mTexture->SetVkImageUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        mTexture->SetVkImageLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    } else {
        mTexture->SetVkImageUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        mTexture->SetVkImageLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }
    mTexture->SetVkImageTarget(vulkanAPI::Image::VK_IMAGE_TARGET_2D);
    mTexture->InitState();
    mTexture->SetState(width, height, 0, 0, GlInternalFormatToGlFormat(mInternalFormat), GlInternalFormatToGlType(mInternalFormat), Texture::GetDefaultInternalAlignment(), NULL);

    return mTexture->Allocate();
}
