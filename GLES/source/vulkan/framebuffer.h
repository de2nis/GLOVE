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
 *  @brief      Framebuffer Functionality in Vulkan
 *
 */

#ifndef __VKFRAMEBUFFER_H__
#define __VKFRAMEBUFFER_H__

#include "utils/globals.h"

namespace vulkanAPI {

class Framebuffer {

private:

    const
    vkContext_t *           mVkContext;

    VkFramebuffer           mVkFramebuffer;

public:
// Constructor
    Framebuffer(const vkContext_t *vkContext = nullptr);

// Destructor
    ~Framebuffer();

// Create Functions
    bool                    Create  (vector<VkImageView> *imageViews, VkRenderPass *renderpass, uint32_t width, uint32_t height);

// Release Functions
    void                    Release (void);

// Get functions
    inline VkFramebuffer*   GetFramebuffer(void)                                { FUN_ENTRY(GL_LOG_TRACE); return &mVkFramebuffer; }
};

}

#endif // __VKFRAMEBUFFER_H__
