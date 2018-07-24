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
 *  @file       clearPass.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Clearing Images Inside A Render Pass Instance in Vulkan
 *
 */

#ifndef __VKCLEARPASS_H__
#define __VKCLEARPASS_H__

#include "utils/glLogger.h"
#include "vulkan/vulkan.h"
#include <memory>
#include <limits>

namespace vulkanAPI {

class ClearPass {

private:
    VkClearRect                 mVkClearRect;
    VkClearAttachment           mVkClearAttachments[2];

    const
    uint32_t                    mClearRectCount;
    uint32_t                    mClearAttachmentsCount;

    struct {
    VkBool32                    Color;
    VkBool32                    Depth;
    VkBool32                    Stencil;
    }                           mUpdateState;

public:
// Constructor
    ClearPass();
    
// Destructor
    ~ClearPass();

// Get functions
    inline uint32_t             GetAttachmentsCount(void)                 const { FUN_ENTRY(GL_LOG_TRACE); return mClearAttachmentsCount;}
    inline uint32_t             GetRectCount(void)                        const { FUN_ENTRY(GL_LOG_TRACE); return mClearRectCount;}
    inline VkBool32             GetUpdateColorState(void)                 const { FUN_ENTRY(GL_LOG_TRACE); return mUpdateState.Color;}
    inline VkBool32             GetUpdateDepthState(void)                 const { FUN_ENTRY(GL_LOG_TRACE); return mUpdateState.Depth;}
    inline VkBool32             GetUpdateStencilState(void)               const { FUN_ENTRY(GL_LOG_TRACE); return mUpdateState.Stencil;}
    inline VkClearRect *        GetRect(void)                                   { FUN_ENTRY(GL_LOG_TRACE); return &mVkClearRect;}
    inline VkClearAttachment *  GetAttachments(void)                            { FUN_ENTRY(GL_LOG_TRACE); return mVkClearAttachments;}

// Set Functions
    inline void                 SetUpdateColorState(VkBool32 enable)            { FUN_ENTRY(GL_LOG_TRACE); mUpdateState.Color   = enable;}
    inline void                 SetUpdateDepthState(VkBool32 enable)            { FUN_ENTRY(GL_LOG_TRACE); mUpdateState.Depth   = enable;}
    inline void                 SetUpdateStencilState(VkBool32 enable)          { FUN_ENTRY(GL_LOG_TRACE); mUpdateState.Stencil = enable;}

    inline void                 SetAttachmentsCount(uint32_t count)             { FUN_ENTRY(GL_LOG_TRACE); mClearAttachmentsCount = count;}
    inline void                 SetColorAttachment(uint32_t j, float *color)    { FUN_ENTRY(GL_LOG_TRACE); mVkClearAttachments[j].aspectMask                      = VK_IMAGE_ASPECT_COLOR_BIT;
                                                                                                       mVkClearAttachments[j].colorAttachment                 = 0;
                                                                                                       mVkClearAttachments[j].clearValue.color.float32[0]     = color[0];
                                                                                                       mVkClearAttachments[j].clearValue.color.float32[1]     = color[1];
                                                                                                       mVkClearAttachments[j].clearValue.color.float32[2]     = color[2];
                                                                                                       mVkClearAttachments[j].clearValue.color.float32[3]     = color[3];
                                                                                                       mUpdateState.Color  = false; }

    inline void                 SetDepthAttachment(uint32_t j, float depth)     { FUN_ENTRY(GL_LOG_TRACE); mVkClearAttachments[j].aspectMask                      = VK_IMAGE_ASPECT_DEPTH_BIT;
                                                                                                       mVkClearAttachments[j].colorAttachment                 = VK_ATTACHMENT_UNUSED;
                                                                                                       mVkClearAttachments[j].clearValue.depthStencil.stencil = 0;

#ifdef WORKAROUNDS
                                                                                                       //workaround for mesa Vulkan Intel driver
                                                                                                       mVkClearAttachments[j].clearValue.depthStencil.depth   = depth * std::numeric_limits<float>::max();

#else
                                                                                                       //correct solution
                                                                                                       mVkClearAttachments[j].clearValue.depthStencil.depth   = depth;
#endif
                                                                                                       mUpdateState.Depth   = false; }
    inline void                 SetStencilAttachment(uint32_t j, int stencil)   { FUN_ENTRY(GL_LOG_TRACE); mVkClearAttachments[j].aspectMask                      = VK_IMAGE_ASPECT_STENCIL_BIT;
                                                                                                       mVkClearAttachments[j].colorAttachment                 = VK_ATTACHMENT_UNUSED;
                                                                                                       mVkClearAttachments[j].clearValue.depthStencil.stencil = stencil;
                                                                                                       mVkClearAttachments[j].clearValue.depthStencil.depth   = 0.f;
                                                                                                       mUpdateState.Stencil = false; }
    inline void                 SetDepthStencilAttachment(uint32_t j,
                                                          float depth,
                                                          int stencil)          { FUN_ENTRY(GL_LOG_TRACE); mVkClearAttachments[j].aspectMask                      = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
                                                                                                       mVkClearAttachments[j].colorAttachment                 = VK_ATTACHMENT_UNUSED;
                                                                                                       mVkClearAttachments[j].clearValue.depthStencil.depth   = depth;
                                                                                                       mVkClearAttachments[j].clearValue.depthStencil.stencil = stencil;
                                                                                                       mUpdateState.Depth   = false;
                                                                                                       mUpdateState.Stencil = false; }
    inline void                 SetRect( int32_t x,  int32_t y,
                                        uint32_t w, uint32_t h)                 { FUN_ENTRY(GL_LOG_TRACE); mVkClearRect.rect.offset.x      = x;
                                                                                                       mVkClearRect.rect.offset.y      = y;
                                                                                                       mVkClearRect.rect.extent.width  = w;
                                                                                                       mVkClearRect.rect.extent.height = h; }
};

}

#endif // __VKCLEARPASS_H__
