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
 *  @file       vulkanAPI.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Vulkan API module. It provides an interface to all Vulkan calls.
 *
 */

#ifndef __VULKAN_API_H__
#define __VULKAN_API_H__

#include "vulkanResources.h"
#include "vulkanWSI.h"
#include "rendering_api_interface.h"
#include <vector>

#ifdef DEBUG_DEPTH
#   undef DEBUG_DEPTH
#endif // DEBUG_DEPTH
#define DEBUG_DEPTH EGL_LOG_DEBUG

class VulkanAPI
{
private:
    vkInterface_t                    *mVkInterface;
    const VulkanWSI::wsiCallbacks_t  *mWsiCallbacks;

public:
    VulkanAPI(vkInterface_t *vkInterface);
    ~VulkanAPI();

    EGLBoolean                   DoesSupportPresent(const VulkanResources *vkResources);
    VkSwapchainKHR               CreateSwapchain(const VulkanResources *vkResources,
                                                 uint32_t desiredNumberOfSwapChainImages,
                                                 VkSurfaceCapabilitiesKHR surfCapabilities,
                                                 VkExtent2D swapChainExtent,
                                                 VkPresentModeKHR swapchainPresentMode,
                                                 VkFormat surfaceColorFormat);

    EGLBoolean                   GetSwapChainImages(const VulkanResources *vkResources, uint32_t imageCount, VkImage *images);
    uint32_t                     GetSwapChainImagesCount(const VulkanResources *vkResources);
    EGLBoolean                   GetPhysicalDevFormats(const VulkanResources *vkResources, uint32_t formatCount, VkSurfaceFormatKHR *formats);
    uint32_t                     GetPhysicalDevFormatsCount(const VulkanResources *vkResources);
    void                         GetPhysicalDevFormatProperties(VkFormat format, VkFormatProperties *formatProperties);
    EGLBoolean                   GetPhysicalDevPresentModes(const VulkanResources *vkResources, uint32_t presentModeCount, VkPresentModeKHR *presentModes);
    uint32_t                     GetPhysicalDevPresentModesCount(const VulkanResources *vkResources);
    EGLBoolean                   GetPhysicalDevSurfaceCapabilities(const VulkanResources *vkResources, VkSurfaceCapabilitiesKHR *surfCapabilities);
    EGLBoolean                   AcquireNextImage(const VulkanResources *vkResources, uint32_t *imageIndex);
    EGLBoolean                   PresentImage(const VulkanResources *vkResources, uint32_t imageIndex, std::vector<VkSemaphore> &vkSemaphores);

    void                         DestroySwapchain(const VulkanResources *vkResources);
    void                         DestroyPlatformSurface(const VulkanResources *vkResources);

    void                         SetWSICallbacks(const VulkanWSI::wsiCallbacks_t *wsiCallbacks) { mWsiCallbacks = wsiCallbacks; }
};


#endif // __VULKAN_API_H__
