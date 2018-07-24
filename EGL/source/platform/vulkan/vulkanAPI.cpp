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
 *  @file       vulkanAPI.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Vulkan API module. It provides an interface to all Vulkan calls.
 *
 */

#include "vulkanAPI.h"

VulkanAPI::VulkanAPI(vkInterface_t *vkInterface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    mVkInterface = vkInterface;
}

VulkanAPI::~VulkanAPI()
{
    FUN_ENTRY(DEBUG_DEPTH);
}

EGLBoolean
VulkanAPI::DoesSupportPresent(const VulkanResources *vkResources)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkBool32 supportsPresent;
    vkGetPhysicalDeviceSurfaceSupportKHR(mVkInterface->vkGpus[0], mVkInterface->vkGraphicsQueueNodeIndex, vkResources->GetSurface(), &supportsPresent);

    return (supportsPresent == VK_TRUE) ? EGL_TRUE : EGL_FALSE;
}

VkSwapchainKHR
VulkanAPI::CreateSwapchain(const VulkanResources *vkResources,
                                    uint32_t desiredNumberOfSwapChainImages,
                                    VkSurfaceCapabilitiesKHR surfCapabilities,
                                    VkExtent2D swapChainExtent,
                                    VkPresentModeKHR swapchainPresentMode,
                                    VkFormat surfaceColorFormat)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else {
        preTransform = surfCapabilities.currentTransform;
    }


    VkSwapchainCreateInfoKHR swapChainCreateInfo;
    memset((void *)&swapChainCreateInfo, 0 ,sizeof(swapChainCreateInfo));
    swapChainCreateInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.pNext                 = NULL;
    swapChainCreateInfo.surface               = vkResources->GetSurface();
    swapChainCreateInfo.minImageCount         = desiredNumberOfSwapChainImages;
    swapChainCreateInfo.imageFormat           = surfaceColorFormat;
    swapChainCreateInfo.imageExtent.width     = swapChainExtent.width;
    swapChainCreateInfo.imageExtent.height    = swapChainExtent.height;
    swapChainCreateInfo.preTransform          = preTransform;
    swapChainCreateInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.imageArrayLayers      = 1;
    swapChainCreateInfo.presentMode           = swapchainPresentMode;
    swapChainCreateInfo.oldSwapchain          = VK_NULL_HANDLE;
    swapChainCreateInfo.clipped               = true;
    swapChainCreateInfo.imageColorSpace       = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapChainCreateInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    swapChainCreateInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    swapChainCreateInfo.queueFamilyIndexCount = 0;
    swapChainCreateInfo.pQueueFamilyIndices   = NULL;

    VkSwapchainKHR vkSwapchain;
    VkResult res = mWsiCallbacks->fpCreateSwapchainKHR(mVkInterface->vkDevice, &swapChainCreateInfo, NULL, &vkSwapchain);

    return (VK_SUCCESS == res) ? vkSwapchain : VK_NULL_HANDLE;
}

EGLBoolean
VulkanAPI::GetSwapChainImages(const VulkanResources *vkResources, uint32_t imageCount, VkImage *images)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkResult res = mWsiCallbacks->fpGetSwapchainImagesKHR(mVkInterface->vkDevice, vkResources->GetSwapchain(), &imageCount, images);

    return (VK_SUCCESS == res) ? EGL_TRUE : EGL_FALSE;
}

uint32_t
VulkanAPI::GetSwapChainImagesCount(const VulkanResources *vkResources)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkResult res;
    uint32_t imageCount = 0;
    res = mWsiCallbacks->fpGetSwapchainImagesKHR(mVkInterface->vkDevice, vkResources->GetSwapchain(), &imageCount, NULL);

    return (VK_SUCCESS == res) ? imageCount : 0;
}

EGLBoolean
VulkanAPI::GetPhysicalDevFormats(const VulkanResources *vkResources, uint32_t formatCount, VkSurfaceFormatKHR *formats)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkResult  res = vkGetPhysicalDeviceSurfaceFormatsKHR(mVkInterface->vkGpus[0], vkResources->GetSurface(), &formatCount, formats);

    return (VK_SUCCESS == res) ? EGL_TRUE : EGL_FALSE;
}

uint32_t
VulkanAPI::GetPhysicalDevFormatsCount(const VulkanResources *vkResources)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkResult res;
    uint32_t formatCount = 0;
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(mVkInterface->vkGpus[0], vkResources->GetSurface(), &formatCount, NULL);

    return (VK_SUCCESS == res) ? formatCount : 0;
}

void
VulkanAPI::GetPhysicalDevFormatProperties(VkFormat format, VkFormatProperties *formatProperties)
{
    FUN_ENTRY(DEBUG_DEPTH);

    vkGetPhysicalDeviceFormatProperties(mVkInterface->vkGpus[0], format, formatProperties);
}

EGLBoolean
VulkanAPI::GetPhysicalDevPresentModes(const VulkanResources *vkResources, uint32_t presentModeCount, VkPresentModeKHR *presentModes)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkResult res = vkGetPhysicalDeviceSurfacePresentModesKHR(mVkInterface->vkGpus[0], vkResources->GetSurface(), &presentModeCount, presentModes);

    return (VK_SUCCESS == res) ? EGL_TRUE : EGL_FALSE;
}

uint32_t
VulkanAPI::GetPhysicalDevPresentModesCount(const VulkanResources *vkResources)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkResult res;
    uint32_t presentModeCount = 0;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(mVkInterface->vkGpus[0], vkResources->GetSurface(), &presentModeCount, NULL);

    return (VK_SUCCESS == res) ? presentModeCount : 0;
}

EGLBoolean
VulkanAPI::GetPhysicalDevSurfaceCapabilities(const VulkanResources *vkResources, VkSurfaceCapabilitiesKHR *surfCapabilities)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mVkInterface->vkGpus[0], vkResources->GetSurface(), surfCapabilities);

    return (VK_SUCCESS == res) ? EGL_TRUE : EGL_FALSE;
}

EGLBoolean
VulkanAPI::AcquireNextImage(const VulkanResources *vkResources, uint32_t *imageIndex)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkResult res = mWsiCallbacks->fpAcquireNextImageKHR(mVkInterface->vkDevice,
                                                        vkResources->GetSwapchain(),
                                                        UINT64_MAX,
                                                        mVkInterface->vkSyncItems->vkAcquireSemaphore,
                                                        VK_NULL_HANDLE,
                                                        imageIndex);

    assert(*imageIndex != UINT32_MAX);

    return (VK_SUCCESS == res) ? EGL_TRUE : EGL_FALSE;
}

EGLBoolean
VulkanAPI::PresentImage(const VulkanResources *vkResources, uint32_t imageIndex, std::vector<VkSemaphore> &vkSemaphores)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkPresentInfoKHR presentInfo;
    VkSwapchainKHR swapchain        = vkResources->GetSwapchain();
    presentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext               = NULL;
    presentInfo.waitSemaphoreCount  = vkSemaphores.size();
    presentInfo.pWaitSemaphores     = vkSemaphores.data();
    presentInfo.swapchainCount      = 1;
    presentInfo.pSwapchains         = &swapchain;
    presentInfo.pImageIndices       = &imageIndex;
    presentInfo.pResults            = NULL;

    VkResult res = mWsiCallbacks->fpQueuePresentKHR(mVkInterface->vkQueue, &presentInfo);

    if(res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
        assert(0);
    } else {
        assert(!res);
    }

    return (VK_SUCCESS == res) ? EGL_TRUE : EGL_FALSE;
}

void
VulkanAPI::DestroySwapchain(const VulkanResources *vkResources)
{
    FUN_ENTRY(DEBUG_DEPTH);

    vkDestroySwapchainKHR(mVkInterface->vkDevice, vkResources->GetSwapchain(), NULL);
}

void
VulkanAPI::DestroyPlatformSurface(const VulkanResources *vkResources)
{
    FUN_ENTRY(DEBUG_DEPTH);

    vkDestroySurfaceKHR(mVkInterface->vkInstance, vkResources->GetSurface(), NULL);
}
