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
 *  @file       vulkanwindowInterface.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Vulkan Window Interface functionality (implements PlatformWindowInterface). It connects EGL to the WSI module of Vulkan driver
 *
 */

#ifndef __VULKAN_WINDOW_INTERFACE_H__
#define __VULKAN_WINDOW_INTERFACE_H__

#include "platform/platformWindowInterface.h"
#include "utils/egl_defs.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "vulkanAPI.h"

#ifdef DEBUG_DEPTH
#   undef DEBUG_DEPTH
#endif // DEBUG_DEPTH
#define DEBUG_DEPTH                                 EGL_LOG_DEBUG

class VulkanWindowInterface : public PlatformWindowInterface
{
private:
    EGLBoolean                   mVkInitialized;
    rendering_api_interface_t   *mGLES2Interface;

    VulkanAPI                   *mVkAPI;
    VulkanWSI                   *mVkWSI;
    vkInterface_t *              mVkInterface;

    const VkFormat               mVkDefaultFormat = VK_FORMAT_B8G8R8A8_UNORM;

    EGLBoolean                   InitializeVulkanAPI();
    void                         TerminateVulkanAPI();
    EGLBoolean                   InitSwapchainExtension(const EGLSurface_t *surface);
    VkExtent2D                   SetSwapchainExtent(EGLSurface_t* surface, VkSurfaceCapabilitiesKHR *surfCapabilities);

    VkPresentModeKHR             SetSwapchainPresentMode(EGLSurface_t* surface);
    void                         SetSurfaceColorFormat(EGLSurface_t *surface);
    void                         CreateVkSwapchain(EGLSurface_t* surface,
                                                   VkPresentModeKHR swapchainPresentMode,
                                                   VkExtent2D swapChainExtent,
                                                   VkSurfaceCapabilitiesKHR surfCapabilities);

    void                         SetEGLSurfaceVkSwapchainImages(EGLSurface_t* surface);

public:

    VulkanWindowInterface(void);
    ~VulkanWindowInterface(void);

    /// EGL API core functions
    EGLBoolean                   Initialize() override;
    EGLBoolean                   Terminate() override;
    EGLBoolean                   CreateSurface(EGLDisplay dpy, EGLNativeWindowType win, EGLSurface_t *surface) override;
    void                         AllocateSurfaceImages(EGLSurface_t *surface) override;
    void                         DestroySurfaceImages(EGLSurface_t *eglSurface) override;
    uint32_t                     AcquireNextImage(EGLSurface_t *surface) override;
    EGLBoolean                   PresentImage(EGLSurface_t *eglSurface) override;

    inline void                  SetWSI(VulkanWSI *vkWSI)                       { mVkWSI = vkWSI; }
};

#endif //__VULKAN_WINDOW_INTERFACE_H__
