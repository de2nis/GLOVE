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
 *  @file       framebuffer.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Framebuffer Functionality in GLOVE
 *
 */

#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "attachment.h"
#include "renderbuffer.h"
#include "vulkan/renderPass.h"
#include "vulkan/framebuffer.h"

class Framebuffer {
private:

    const vkContext_t *             mVkContext;

    Rect                            mDims;
    GLenum                          mTarget;
    uint32_t                        mWriteBufferIndex;
    bool                            mUpdated;

    vulkanAPI::RenderPass*          mRenderPass;
    vector<vulkanAPI::Framebuffer*> mFramebuffers;

    vector<Attachment*>             mAttachmentColors;
    Attachment*                     mAttachmentDepth;
    Attachment*                     mAttachmentStencil;
    Texture*                        mDepthStencilTexture;

    void                            Release(void);

public:
    Framebuffer(const vkContext_t *vkContext = nullptr);
    ~Framebuffer();

// Create Functions
    bool                    Create(void);
    void                    CreateDepthStencilTexture(void);

// RenderPass Functions
    bool                    CreateVkRenderPass(bool enableDepthWrite, bool enableStencilWrite);
    void                    BeginVkRenderPass(bool enableDepthWrite, bool enableStencilWrite);
    void                    EndVkRenderPass(void);
    void                    PrepareVkImage(VkImageLayout newImageLayout);

// Add Functions
    void                    AddColorAttachment(Texture *texture);

// Check Functions
    GLenum                  CheckStatus(void);

// Get Functions
    inline Rect *           GetRect(void)                                       { FUN_ENTRY(GL_LOG_TRACE); return &mDims; }
    inline int              GetX(void)                                  const   { FUN_ENTRY(GL_LOG_TRACE); return mDims.x; }
    inline int              GetY(void)                                  const   { FUN_ENTRY(GL_LOG_TRACE); return mDims.y; }
    inline int              GetWidth(void)                              const   { FUN_ENTRY(GL_LOG_TRACE); return mDims.width; }
    inline int              GetHeight(void)                             const   { FUN_ENTRY(GL_LOG_TRACE); return mDims.height; }
    inline GLenum           GetTarget(void)                             const   { FUN_ENTRY(GL_LOG_TRACE); return mTarget; }
    inline VkRenderPass *   GetVkRenderPass(void)                       const   { FUN_ENTRY(GL_LOG_TRACE); return mRenderPass->GetRenderPass(); }

    inline GLenum           GetColorAttachmentType(void)                const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentColors.size() ? mAttachmentColors[0]->GetType()  : GL_NONE; }
    inline uint32_t         GetColorAttachmentName(void)                const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentColors.size() ? mAttachmentColors[0]->GetName()  : 0; }
    inline GLint            GetColorAttachmentLevel(void)               const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentColors.size() ? mAttachmentColors[0]->GetLevel() : 0; }
    inline GLenum           GetColorAttachmentLayer(void)               const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentColors.size() ? mAttachmentColors[0]->GetLayer() : GL_TEXTURE_CUBE_MAP_POSITIVE_X; }
    inline Texture *        GetColorAttachmentTexture(void)             const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentColors.size() ? mAttachmentColors[mWriteBufferIndex]->GetTexture() : nullptr; }

    inline GLenum           GetDepthAttachmentType(void)                const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentDepth->GetType();   }
    inline uint32_t         GetDepthAttachmentName(void)                const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentDepth->GetName();   }
    inline GLint            GetDepthAttachmentLevel(void)               const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentDepth->GetLevel();  }
    inline GLenum           GetDepthAttachmentLayer(void)               const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentDepth->GetLayer();  }
    inline Texture *        GetDepthAttachmentTexture(void)             const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentDepth->GetTexture();}

    inline GLenum           GetStencilAttachmentType(void)              const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentStencil->GetType();   }
    inline uint32_t         GetStencilAttachmentName(void)              const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentStencil->GetName();   }
    inline GLint            GetStencilAttachmentLevel(void)             const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentStencil->GetLevel();  }
    inline GLenum           GetStencilAttachmentLayer(void)             const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentStencil->GetLayer();  }
    inline Texture *        GetStencilAttachmentTexture(void)           const   { FUN_ENTRY(GL_LOG_TRACE); return mAttachmentStencil->GetTexture();}

// Set Functions
           void             SetColorAttachmentTexture(Texture *texture);
    inline void             SetVkContext(const vkContext_t *vkContext)          { FUN_ENTRY(GL_LOG_TRACE); mVkContext   = vkContext; mRenderPass->SetVkContext(vkContext); }

    inline void             SetTarget(GLenum target)                            { FUN_ENTRY(GL_LOG_TRACE); mTarget      = target; }
    inline void             SetWidth(int32_t width)                             { FUN_ENTRY(GL_LOG_TRACE); mDims.width  = width;  }
    inline void             SetHeight(int32_t height)                           { FUN_ENTRY(GL_LOG_TRACE); mDims.height = height; }

    inline void             SetColorAttachmentType(GLenum type)                 { FUN_ENTRY(GL_LOG_TRACE); mAttachmentColors[0]->SetType(type);   }
    inline void             SetColorAttachmentName(uint32_t name)               { FUN_ENTRY(GL_LOG_TRACE); mAttachmentColors[0]->SetName(name);   }
    inline void             SetColorAttachmentLevel(GLint level)                { FUN_ENTRY(GL_LOG_TRACE); mAttachmentColors[0]->SetLevel(level); }
    inline void             SetColorAttachmentLayer(GLenum layer)               { FUN_ENTRY(GL_LOG_TRACE); mAttachmentColors[0]->SetLayer(layer); }

    inline void             SetDepthAttachmentType(GLenum type)                 { FUN_ENTRY(GL_LOG_TRACE); mAttachmentDepth->SetType(type);   }
    inline void             SetDepthAttachmentName(uint32_t name)               { FUN_ENTRY(GL_LOG_TRACE); mAttachmentDepth->SetName(name);   }
    inline void             SetDepthAttachmentLevel(GLint level)                { FUN_ENTRY(GL_LOG_TRACE); mAttachmentDepth->SetLevel(level); }
    inline void             SetDepthAttachmentLayer(GLenum layer)               { FUN_ENTRY(GL_LOG_TRACE); mAttachmentDepth->SetLayer(layer); }
    inline void             SetDepthAttachmentTexture(Texture *texture)         { FUN_ENTRY(GL_LOG_TRACE); mAttachmentDepth->SetTexture(texture);
                                                                                                       mUpdated = true; }

    inline void             SetStencilAttachmentType(GLenum type)               { FUN_ENTRY(GL_LOG_TRACE); mAttachmentStencil->SetType(type);   }
    inline void             SetStencilAttachmentName(uint32_t name)             { FUN_ENTRY(GL_LOG_TRACE); mAttachmentStencil->SetName(name);   }
    inline void             SetStencilAttachmentLevel(GLint level)              { FUN_ENTRY(GL_LOG_TRACE); mAttachmentStencil->SetLevel(level); }
    inline void             SetStencilAttachmentLayer(GLenum layer)             { FUN_ENTRY(GL_LOG_TRACE); mAttachmentStencil->SetLayer(layer); }
    inline void             SetStencilAttachmentTexture(Texture *texture)       { FUN_ENTRY(GL_LOG_TRACE); mAttachmentStencil->SetTexture(texture);
                                                                                                       mUpdated = true; }
    inline void             SetDepthStencilAttachmentTexture(Texture *texture)  { FUN_ENTRY(GL_LOG_TRACE); mAttachmentDepth->SetTexture(texture);
                                                                                                       mAttachmentStencil->SetTexture(texture);
                                                                                                       mUpdated = true; }

    inline void             SetWriteBufferIndex(uint32_t buffer)                { FUN_ENTRY(GL_LOG_TRACE); if(mAttachmentColors.size() > 1) mWriteBufferIndex = buffer; }
};

#endif // __FRAMEBUFFER_H__
