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
 *  @file       imageView.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Image Memory Access Functionality via Image Views in Vulkan
 *
 *  @section
 *
 *  Image objects are not directly accessed by pipeline shaders for reading
 *  or writing image data. Instead, image views representing contiguous
 *  ranges of the image subresources and containing additional metadata are
 *  used for that purpose. Views must be created on images of compatible types,
 *  and must represent a valid subset of image subresources.
 *  Image views are represented by VkImageView handles.
 *
 */

#include "imageView.h"

namespace vulkanAPI {

ImageView::ImageView(const vkContext_t *vkContext)
: mVkContext(vkContext), mVkImageView(VK_NULL_HANDLE)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

ImageView::~ImageView()
{
    FUN_ENTRY(GL_LOG_TRACE);

    Release();
}

void
ImageView::Release(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(mVkContext->vkDevice, mVkImageView, NULL);
        mVkImageView = VK_NULL_HANDLE;
    }
}

bool
ImageView::Create(vulkanAPI::Image *image)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkImageViewCreateInfo info;
    info.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.pNext            = NULL;
    info.flags            = 0;
    info.viewType         = (image->GetImageTarget() == Image::VK_IMAGE_TARGET_2D) ? VK_IMAGE_VIEW_TYPE_2D : VK_IMAGE_VIEW_TYPE_CUBE;
    info.image            = image->GetImage();
    info.format           = image->GetFormat();
    info.components.r     = VK_COMPONENT_SWIZZLE_R;
    info.components.g     = VK_COMPONENT_SWIZZLE_G;
    info.components.b     = VK_COMPONENT_SWIZZLE_B;
    info.components.a     = VK_COMPONENT_SWIZZLE_A;
    info.subresourceRange = image->GetImageSubresourceRange();

    VkResult err = vkCreateImageView(mVkContext->vkDevice, &info, 0, &mVkImageView);
    assert(!err);

    return (err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY);
}

}
