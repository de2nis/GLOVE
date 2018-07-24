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
 *  @file       context.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Default Context and Resources Initialization
 *
 */

#include "context.h"
#include "utils/VkToGlConverter.h"

static Context *currentContext = nullptr;

Context *GetCurrentContext()
{
    FUN_ENTRY(GL_LOG_TRACE);

    return currentContext;
}

void SetCurrentContext(Context *ctx)
{
    FUN_ENTRY(GL_LOG_TRACE);

    currentContext = ctx;
}

Context::Context()
{
    FUN_ENTRY(GL_LOG_TRACE);

    mVkContext      = vulkanAPI::GetContext();

    mShaderCompiler = new GlslangShaderCompiler();
    mClearPass      = new vulkanAPI::ClearPass();
    mPipeline       = new vulkanAPI::Pipeline(mVkContext);

    mStateManager.InitVkPipelineStates(mPipeline);

    InitializeDefaultTextures();

    mWriteSurface = nullptr;
    mReadSurface  = nullptr;
    mWriteFBO     = nullptr;
    mSystemFBO    = nullptr;
    mTempIbo      = nullptr;
}

Context::~Context()
{
    FUN_ENTRY(GL_LOG_TRACE);

    if(mSystemFBO) {
        for(uint32_t i = 0; i < mSystemTextures.size(); ++i) {
            if(mSystemTextures[i]) {
                delete mSystemTextures[i];
                mSystemTextures[i] = nullptr;
            }
        }
        mSystemTextures.clear();

        delete mSystemFBO;
    }

    delete mShaderCompiler;
    delete mPipeline;
    delete mClearPass;
    delete mTempIbo;
}

void
Context::InitializeDefaultTextures()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(int i = 0; i < GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS; ++i) {
        Texture *tex = mResourceManager.GetDefaultTexture(GL_TEXTURE_2D);
                 tex->SetVkContext(mVkContext);
        mStateManager.GetActiveObjectsState()->SetActiveTexture(GL_TEXTURE_2D      , i, tex);
                 tex = mResourceManager.GetDefaultTexture(GL_TEXTURE_CUBE_MAP);
                 tex->SetVkContext(mVkContext);
        mStateManager.GetActiveObjectsState()->SetActiveTexture(GL_TEXTURE_CUBE_MAP, i, tex);
    }
}

Framebuffer *
Context::CreateFBOFromEGLSurface(EGLSurfaceInterface *eglSurfaceInterface)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    /// Create frame buffer object
    Framebuffer *fbo = nullptr;
    if(EGL_WINDOW_BIT == eglSurfaceInterface->type) {
        fbo = InitializeFrameBuffer(eglSurfaceInterface);
    } else if(EGL_PBUFFER_BIT == eglSurfaceInterface->type) {
        fbo = AllocatePBufferTexture(eglSurfaceInterface);
    } else {
        return nullptr;
    }

    Texture *tex = CreateDepthStencil(eglSurfaceInterface);
    fbo->SetDepthStencilAttachmentTexture(tex);
    fbo->CreateVkRenderPass(true, false);
    fbo->Create();
    mSystemTextures.push_back(tex);

    return fbo;
}

Framebuffer *
Context::InitializeFrameBuffer(EGLSurfaceInterface *eglSurfaceInterface)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkImage *swapChainImages = reinterpret_cast<VkImage *>(eglSurfaceInterface->images);

    Framebuffer *fbo = new Framebuffer(mVkContext);

    /// Create textures
    for(uint32_t i = 0; i < eglSurfaceInterface->imageCount; ++i) {
        Texture *tex = new Texture(mVkContext);

        GLenum glformat = VkFormatToGlInternalformat(static_cast<VkFormat>(eglSurfaceInterface->surfaceColorFormat));

        tex->SetTarget(GL_TEXTURE_2D);
        tex->SetWidth(eglSurfaceInterface->width);
        tex->SetHeight(eglSurfaceInterface->height);
        tex->SetInternalFormat(glformat);
        tex->SetExplicitInternalFormat(glformat);
        tex->SetFormat(GlInternalFormatToGlFormat(glformat));
        tex->SetType(GlInternalFormatToGlType(glformat));
        tex->SetExplicitType(GlInternalFormatToGlType(glformat));

        tex->SetVkImage(swapChainImages[i]);
        tex->SetVkFormat(static_cast<VkFormat>(eglSurfaceInterface->surfaceColorFormat));
        tex->SetVkImageUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        tex->SetVkImageTarget(vulkanAPI::Image::VK_IMAGE_TARGET_2D);
        tex->CreateVkImageSubResourceRange();
        tex->CreateVkImageView();

        fbo->AddColorAttachment(tex);
        mSystemTextures.push_back(tex);
    }

    fbo->SetTarget(GL_FRAMEBUFFER);
    fbo->SetWriteBufferIndex(eglSurfaceInterface->nextImageIndex);

    return fbo;
}

Framebuffer *
Context::AllocatePBufferTexture(EGLSurfaceInterface *eglSurfaceInterface)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    Framebuffer *fbo = new Framebuffer(mVkContext);
    fbo->SetTarget(GL_FRAMEBUFFER);

    Texture *tex = new Texture(mVkContext);
    tex->SetTarget(GL_TEXTURE_2D);
    tex->SetVkFormat(static_cast<VkFormat>(eglSurfaceInterface->surfaceColorFormat));
    tex->SetVkImageUsage(static_cast<VkImageUsageFlagBits>(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT));
    tex->SetVkImageTarget(vulkanAPI::Image::VK_IMAGE_TARGET_2D);
    GLenum glformat = VkFormatToGlInternalformat(static_cast<VkFormat>(eglSurfaceInterface->surfaceColorFormat));
    tex->InitState();

    tex->SetState(eglSurfaceInterface->width, eglSurfaceInterface->height,
                  0, 0,
                  GlInternalFormatToGlFormat(glformat),
                  GlInternalFormatToGlType(glformat),
                  Texture::GetDefaultInternalAlignment(),
                  NULL);
    tex->Allocate();

    fbo->AddColorAttachment(tex);
    mSystemTextures.push_back(tex);

    return fbo;
}

Texture *
Context::CreateDepthStencil(EGLSurfaceInterface *eglSurfaceInterface)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkFormat depthFormat = VK_FORMAT_UNDEFINED;

    switch(eglSurfaceInterface->depthSize) {
    case 0:  depthFormat = eglSurfaceInterface->stencilSize ? VK_FORMAT_S8_UINT            : VK_FORMAT_UNDEFINED;           break;
    case 16: depthFormat = eglSurfaceInterface->stencilSize ? VK_FORMAT_D16_UNORM_S8_UINT  : VK_FORMAT_D16_UNORM;           break;
    case 24: depthFormat = eglSurfaceInterface->stencilSize ? VK_FORMAT_D24_UNORM_S8_UINT  : VK_FORMAT_X8_D24_UNORM_PACK32; break;
    case 32: depthFormat = eglSurfaceInterface->stencilSize ? VK_FORMAT_D32_SFLOAT_S8_UINT : VK_FORMAT_D32_SFLOAT;          break;
    default: NOT_REACHED(); break;
    }

    if(depthFormat == VK_FORMAT_UNDEFINED) {
        return nullptr;
    }

    Texture *tex = new Texture(mVkContext);
    tex->SetTarget(GL_TEXTURE_2D);
    tex->SetVkFormat(depthFormat);
    tex->SetVkImageUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    tex->SetVkImageTiling(VK_IMAGE_TILING_OPTIMAL);
    tex->SetVkImageTarget(vulkanAPI::Image::VK_IMAGE_TARGET_2D);

    GLenum glformat = VkFormatToGlInternalformat(depthFormat);
    tex->InitState();
    tex->SetState(eglSurfaceInterface->width, eglSurfaceInterface->height,
                  0, 0,
                  GlInternalFormatToGlFormat(glformat),
                  GlInternalFormatToGlType(glformat),
                  Texture::GetDefaultInternalAlignment(),
                  NULL);
    return tex->Allocate() ? tex : nullptr;
}

void
Context::SetWriteSurface(EGLSurfaceInterface *eglSurfaceInterface)
{
    FUN_ENTRY(GL_LOG_TRACE);

    mWriteSurface = eglSurfaceInterface->surface;
    mWriteFBO     = CreateFBOFromEGLSurface(eglSurfaceInterface);

    SetSystemFramebuffer(mWriteFBO);
}

void
Context::SetReadSurface(EGLSurfaceInterface *eglSurfaceInterface)
{
    FUN_ENTRY(GL_LOG_TRACE);

    mReadSurface = eglSurfaceInterface->surface;
}

void
Context::SetSystemFramebuffer(Framebuffer *FBO)
{
    FUN_ENTRY(GL_LOG_TRACE);

    mSystemFBO = FBO;

    mStateManager.GetViewportTransformationState()->SetViewportRect(mSystemFBO->GetRect());
    mStateManager.GetFragmentOperationsState()->SetScissorRect(mSystemFBO->GetRect());
    mPipeline->SetUpdatePipeline(true);
    mPipeline->SetUpdateViewportState(true);
}

void
Context::EGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    NOT_IMPLEMENTED();
}

void
Context::EGLImageTargetRenderBufferStorageOES(GLenum target, GLeglImageOES image)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    NOT_IMPLEMENTED();

    return;
}

void
Context::InsertEventMarkerEXT(GLsizei length, const GLchar *marker)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    NOT_IMPLEMENTED();

    return;
}

void
Context::PushGroupMarkerEXT(GLsizei length, const GLchar *marker)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    NOT_IMPLEMENTED();

    return;
}

void
Context::PushGroupMarkerEXT(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    NOT_IMPLEMENTED();

    return;
}
