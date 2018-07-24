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
 *  @file       vulkanWSI.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Base class connecting EGL to WSI callbacks.
 *
 */

#ifndef __VULKAN_WSI_H__
#define __VULKAN_WSI_H__

#include "EGL/egl.h"
#include <vulkan/vulkan.h>
#include "api/eglSurface.h"

#define GET_WSI_FUNCTION_PTR(entrypoint)                                                     \
{                                                                                                        \
    mWsiCallbacks.fp##entrypoint = (PFN_vk##entrypoint) vkGetInstanceProcAddr(mVkInstance, "vk"#entrypoint);    \
    if(mWsiCallbacks.fp##entrypoint == nullptr) {                                                        \
        assert(mWsiCallbacks.fp##entrypoint && "Could not get function pointer to "#entrypoint);         \
        return EGL_FALSE;                                                                                \
    }                                                                                                    \
}

class VulkanWSI
{
public:
typedef struct wsiCallbacks {
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR        fpGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR   fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR        fpGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR   fpGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkCreateSwapchainKHR                        fpCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR                       fpDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR                     fpGetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR                       fpAcquireNextImageKHR;
    PFN_vkQueuePresentKHR                           fpQueuePresentKHR;
    void *                                          fpCreateSurface;
}wsiCallbacks_t;

protected:
    VkInstance                                      mVkInstance;
    VkDevice                                        mVkDevice;
    wsiCallbacks_t                                  mWsiCallbacks;

    virtual EGLBoolean                              SetSurfaceCallback() = 0;

public:
    VulkanWSI() {}
    virtual ~VulkanWSI() {}

    EGLBoolean                                      Initialize(const VkInstance vkInstance,
                                                               const VkDevice vkDevice);

    virtual VkSurfaceKHR                            CreateSurface(EGLDisplay dpy,
                                                                 EGLNativeWindowType win,
                                                                 EGLSurface_t *surface) = 0;

    const wsiCallbacks_t                           *GetWsiCallbacks() { return &mWsiCallbacks; }
};

#endif // __VULKAN_WSI_H__
