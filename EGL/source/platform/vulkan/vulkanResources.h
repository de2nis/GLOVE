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
 *  @file       vulkanResources.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      It hosts vulkan surface & swapchain. It implements PlatformResources interface class. 
 *
 */

#ifndef __VULKAN_RESOURCES_H__
#define __VULKAN_RESOURCES_H__

#include "platform/platformResources.h"
#include <vulkan/vulkan.h>

class VulkanResources : public PlatformResources
{
private:
    VkSwapchainKHR                   mSwapchain;
    VkSurfaceKHR                     mSurface;
    uint32_t                         mSwapChainImageCount;
    VkImage                         *mSwapChainImages;

public:
    VulkanResources() : mSwapchain(VK_NULL_HANDLE), mSurface(VK_NULL_HANDLE), mSwapChainImageCount(0), mSwapChainImages(nullptr) { }
    ~VulkanResources() { if(mSwapChainImages) { delete[] mSwapChainImages; mSwapChainImageCount = 0; } }

    inline VkSurfaceKHR              GetSurface()                                   const { return mSurface; }
    inline VkSwapchainKHR            GetSwapchain()                                 const { return mSwapchain; }
    inline uint32_t                  GetSwapchainImageCount()                    override { return mSwapChainImageCount; }
    inline void *                    GetSwapchainImages()                        override { return reinterpret_cast<void *>(mSwapChainImages); }

    inline void                      SetSurface(VkSurfaceKHR surface)                     { mSurface = surface; }
    inline void                      SetSwapchain(VkSwapchainKHR swapchain)               { mSwapchain = swapchain; }
    inline void                      SetSwapChainImageCount(uint32_t swapChainImageCount) { mSwapChainImageCount = swapChainImageCount; }
    inline void                      SetSwapChainImages(VkImage *swapChainImages)         { mSwapChainImages = swapChainImages; }
};

#endif // #define __VULKAN_RESOURCES_H__
