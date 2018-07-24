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
 *  @file       imageView.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Image Memory Access Functionality via Image Views in Vulkan
 *
 */

#ifndef __VKIMAGEVIEW_H__
#define __VKIMAGEVIEW_H__

#include "utils/globals.h"
#include "image.h"

namespace vulkanAPI {

class ImageView {

private:

    const
    vkContext_t *                     mVkContext;

    VkImageView                       mVkImageView;

public:
// Constructor
    ImageView(const vkContext_t *vkContext = nullptr);

// Destructor
    ~ImageView();

// Create Functions
    bool                              Create(vulkanAPI::Image *image);

// Release Functions
    void                              Release(void);

// Get Functions
    inline VkImageView                GetImageView(void)                  const { FUN_ENTRY(GL_LOG_TRACE); return mVkImageView; }

// Set Functions
    inline void                       SetContext(const vkContext_t *vkContext)  { FUN_ENTRY(GL_LOG_TRACE); mVkContext = vkContext; }
};

}

#endif // __VKIMAGEVIEW_H__
