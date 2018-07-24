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
 *  @file       framebuffer.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Framebuffer Functionality in GLOVE
 *
 *  @scope
 *
 *  Framebuffer Objects allow the creation of user-defined framebuffers for
 *  offscreen rendering.
 *
 */

#include "framebuffer.h"
#include "utils/VkToGlConverter.h"
#include "utils/glUtils.h"

Framebuffer::Framebuffer(const vkContext_t *vkContext)
: mVkContext(vkContext), mTarget(GL_INVALID_VALUE), mWriteBufferIndex(0), mUpdated(true), mDepthStencilTexture(nullptr)
{
    FUN_ENTRY(GL_LOG_TRACE);

    mRenderPass           = new vulkanAPI::RenderPass(vkContext);
    mAttachmentDepth      = new Attachment();
    mAttachmentStencil    = new Attachment();
}

Framebuffer::~Framebuffer()
{
    FUN_ENTRY(GL_LOG_TRACE);

    delete mRenderPass;
    delete mAttachmentDepth;
    delete mAttachmentStencil;

    if(mDepthStencilTexture) {
        delete mDepthStencilTexture;
    }

    for(auto color : mAttachmentColors) {
        if(color) {
            delete color;
        }
    }
    mAttachmentColors.clear();

    Release();
}

void
Framebuffer::Release(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(auto fb : mFramebuffers) {
        delete fb;
    }

    mFramebuffers.clear();
}

void
Framebuffer::AddColorAttachment(Texture *texture)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    Attachment *att = new Attachment(texture);
    mAttachmentColors.push_back(att);

    SetWidth(texture->GetWidth());
    SetHeight(texture->GetHeight());

    mUpdated = true;
}

void
Framebuffer::SetColorAttachmentTexture(Texture *texture)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mAttachmentColors.size() == 0) {
        Attachment *att = new Attachment(texture);
        mAttachmentColors.push_back(att);
    } else {
        mAttachmentColors[0]->SetTexture(texture);
    }

    if(texture) {
        SetWidth(texture->GetWidth());
        SetHeight(texture->GetHeight());
    }

    mUpdated = true;
}

GLenum
Framebuffer::CheckStatus(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    GLenum colorType = mAttachmentColors.size() == 0 ? GL_NONE : mAttachmentColors[0]->GetType();

    if(colorType                      == GL_NONE  &&
       mAttachmentDepth->GetType()    == GL_NONE  &&
       mAttachmentStencil->GetType()  == GL_NONE) {
        return GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
    }

    if((colorType == GL_COLOR_ATTACHMENT0   &&
        (!GlFormatIsColor(mAttachmentColors[0]->GetTexture()->GetInternalFormat()) ||
         mAttachmentColors[0]->GetTexture()->GetWidth()  < 0   ||
         mAttachmentColors[0]->GetTexture()->GetHeight() < 0)) ||
       (mAttachmentDepth->GetType() == GL_DEPTH_ATTACHMENT    &&
        (!GlFormatIsDepth(mAttachmentDepth->GetTexture()->GetInternalFormat())  ||
         mAttachmentDepth->GetTexture()->GetWidth()  < 0 ||
         mAttachmentDepth->GetTexture()->GetHeight() < 0)) ||
       (mAttachmentStencil->GetType() == GL_STENCIL_ATTACHMENT  &&
        (!GlFormatIsStencil(mAttachmentStencil->GetTexture()->GetInternalFormat())  ||
         mAttachmentStencil->GetTexture()->GetWidth()  < 0 ||
         mAttachmentStencil->GetTexture()->GetHeight() < 0))) {
        return GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
    }

    if(colorType != GL_NONE && mAttachmentDepth->GetType() != GL_NONE) {
        if(mAttachmentColors[0]->GetTexture()->GetWidth()  != mAttachmentDepth->GetTexture()->GetWidth() ||
           mAttachmentColors[0]->GetTexture()->GetHeight() != mAttachmentDepth->GetTexture()->GetHeight()) {
            return GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS;
        }
    }

    if(colorType != GL_NONE && mAttachmentStencil->GetType() != GL_NONE) {
        if(mAttachmentColors[0]->GetTexture()->GetWidth()  != mAttachmentStencil->GetTexture()->GetWidth() ||
           mAttachmentColors[0]->GetTexture()->GetHeight() != mAttachmentStencil->GetTexture()->GetHeight()) {
            return GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS;
        }
    }

    if(mAttachmentDepth->GetType() != GL_NONE && mAttachmentStencil->GetType() != GL_NONE) {
        if(mAttachmentDepth->GetTexture()->GetWidth()  != mAttachmentStencil->GetTexture()->GetWidth() ||
           mAttachmentDepth->GetTexture()->GetHeight() != mAttachmentStencil->GetTexture()->GetHeight()) {
            return GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS;
        }
    }

    // The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.
    //  if() {
    //      return GL_FRAMEBUFFER_UNSUPPORTED;
    //  }

    return GL_FRAMEBUFFER_COMPLETE;
}

bool
Framebuffer::CreateVkRenderPass(bool enableDepthWrite, bool enableStencilWrite)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mRenderPass->SetDepthWriteEnabled(enableDepthWrite);
    mRenderPass->SetStencilWriteEnabled(enableStencilWrite);

    return mRenderPass->Create(mAttachmentColors[mWriteBufferIndex]->GetTexture()->GetVkFormat(),
                               mDepthStencilTexture ? mDepthStencilTexture->GetVkFormat() : VK_FORMAT_UNDEFINED);
}

void
Framebuffer::CreateDepthStencilTexture(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mDepthStencilTexture) {
        delete mDepthStencilTexture;
        mDepthStencilTexture = nullptr;
    }

    if(mAttachmentDepth->GetTexture() || mAttachmentStencil->GetTexture()) {
        mDepthStencilTexture = new Texture(mVkContext);
        mDepthStencilTexture->SetTarget(GL_TEXTURE_2D);

        VkFormat vkformat = GlInternalFormatToVkFormat(
            mAttachmentDepth->GetTexture()   ? mAttachmentDepth->GetTexture()->GetInternalFormat()   : GL_INVALID_VALUE,
            mAttachmentStencil->GetTexture() ? mAttachmentStencil->GetTexture()->GetInternalFormat() : GL_INVALID_VALUE);
        mDepthStencilTexture->SetVkFormat(vkformat);
        mDepthStencilTexture->SetVkImageUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        mDepthStencilTexture->SetVkImageLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        mDepthStencilTexture->SetVkImageTiling(VK_IMAGE_TILING_OPTIMAL);
        GLenum glformat = VkFormatToGlInternalformat(mDepthStencilTexture->GetVkFormat());
        mDepthStencilTexture->InitState();
        mDepthStencilTexture->SetState(GetWidth(), GetHeight(), 0, 0, GlInternalFormatToGlFormat(glformat), GlInternalFormatToGlType(glformat), Texture::GetDefaultInternalAlignment(), NULL);
        mDepthStencilTexture->Allocate();
    }
}

void
Framebuffer::BeginVkRenderPass(bool enableDepthWrite, bool enableStencilWrite)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if( mAttachmentColors[0]->GetTexture() && (
        mAttachmentColors[0]->GetTexture()->GetWidth()  != GetWidth()  ||
        mAttachmentColors[0]->GetTexture()->GetHeight() != GetHeight() )) {
        SetWidth(mAttachmentColors[0]->GetTexture()->GetWidth());
        SetHeight(mAttachmentColors[0]->GetTexture()->GetHeight());

        mUpdated = true;
    }

    if(mUpdated ||
       mRenderPass->GetDepthWriteEnabled()   != enableDepthWrite   ||
       mRenderPass->GetStencilWriteEnabled() != enableStencilWrite
    ) {
        if(mUpdated) {
            CreateDepthStencilTexture();
        }
        CreateVkRenderPass(enableDepthWrite, enableStencilWrite);
        Create();
        mUpdated = false;
    }

    VkCommandBuffer activeCmdBuffer = mVkContext->mCommandBufferManager->GetActiveCommandBuffer();
    mRenderPass->Begin(&activeCmdBuffer,  mFramebuffers[mWriteBufferIndex]->GetFramebuffer(),
                                          GetWidth(),
                                          GetHeight());
}

void
Framebuffer::EndVkRenderPass(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkCommandBuffer activeCmdBuffer = mVkContext->mCommandBufferManager->GetActiveCommandBuffer();
    mRenderPass->End(&activeCmdBuffer);
}

void
Framebuffer::PrepareVkImage(VkImageLayout newImageLayout)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mAttachmentColors[mWriteBufferIndex]->GetTexture()->PrepareVkImageLayout(newImageLayout);
}

bool
Framebuffer::Create(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    Release();

    for(uint32_t i = 0; i < mAttachmentColors.size(); ++i) {
        vulkanAPI::Framebuffer *fb = new vulkanAPI::Framebuffer(mVkContext);

        vector<VkImageView> imageViews;
        if(mAttachmentColors[i]->GetTexture()) {
            imageViews.push_back(mAttachmentColors[i]->GetTexture()->GetVkImageView());
        }
        if(mDepthStencilTexture) {
            imageViews.push_back(mDepthStencilTexture->GetVkImageView());
        }

        if(!fb->Create(&imageViews, GetVkRenderPass(), GetWidth(), GetHeight())) {
            delete fb;
            return false;
        }

        mFramebuffers.push_back(fb);
    }

    return true;
}
