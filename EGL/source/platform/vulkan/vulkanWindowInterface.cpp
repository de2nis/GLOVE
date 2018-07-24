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
 *  @file       vulkanwindowInterface.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Vulkan Window Interface functionality (implements PlatformWindowInterface). It connects EGL to the WSI module of Vulkan driver
 *
 */

#include "vulkanWindowInterface.h"
#include "WSIXcb.h"
#include "api/eglSurface.h"
#include "EGL/egl.h"
#include "rendering_api/rendering_api.h"

#include <vector>

VulkanWindowInterface::VulkanWindowInterface(void)
: mVkInitialized(false), mGLES2Interface(nullptr), mVkAPI(nullptr), mVkWSI(nullptr)
{
    FUN_ENTRY(DEBUG_DEPTH);
}

VulkanWindowInterface::~VulkanWindowInterface(void)
{
    FUN_ENTRY(DEBUG_DEPTH);

    delete mVkWSI;
}

EGLBoolean
VulkanWindowInterface::InitializeVulkanAPI()
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(!mVkInitialized) {
        rendering_api_return_e ret = RENDERING_API_init_api(EGL_OPENGL_ES_API, EGL_GL_VERSION_2, &mGLES2Interface);

        if(RENDERING_API_INIT_SUCCESS != ret && RENDERING_API_ALREADY_INIT != ret) {
            return EGL_FALSE;
        }

        mVkInterface = reinterpret_cast<vkInterface_t *>(mGLES2Interface->state);

        mVkAPI = new VulkanAPI(mVkInterface);

        if(EGL_FALSE == mVkWSI->Initialize(mVkInterface->vkInstance, mVkInterface->vkDevice)) {
            return EGL_FALSE;
        }

        mVkAPI->SetWSICallbacks(mVkWSI->GetWsiCallbacks());

        mVkInitialized = true;
    }

    return EGL_TRUE;
}

void
VulkanWindowInterface::TerminateVulkanAPI()
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(mVkAPI) {
        RENDERING_API_terminate_gles2_api();
        delete mVkAPI;
    }
}

EGLBoolean
VulkanWindowInterface::InitSwapchainExtension(const EGLSurface_t *surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(!mVkAPI->DoesSupportPresent(dynamic_cast<const VulkanResources *>(surface->GetPlatformResources()))) {
        return EGL_FALSE;
    }

    return EGL_TRUE;
}

VkExtent2D
VulkanWindowInterface::SetSwapchainExtent(EGLSurface_t* surface, VkSurfaceCapabilitiesKHR *surfCapabilities)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLBoolean ASSERT_ONLY res;

    res = mVkAPI->GetPhysicalDevSurfaceCapabilities(dynamic_cast<const VulkanResources *>(surface->GetPlatformResources()), surfCapabilities);
    assert(res == EGL_TRUE);

    VkExtent2D swapChainExtent;
    if(surfCapabilities->currentExtent.width == (uint32_t)-1) {
        swapChainExtent.width = surface->GetWidth();
        swapChainExtent.height = surface->GetHeight();
    } else {
        swapChainExtent = surfCapabilities->currentExtent;
    }

    return swapChainExtent;
}

VkPresentModeKHR
VulkanWindowInterface::SetSwapchainPresentMode(EGLSurface_t* surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLBoolean ASSERT_ONLY res;
    uint32_t presentModeCount;
    const VulkanResources *vkResources = dynamic_cast<const VulkanResources *>(surface->GetPlatformResources());

    presentModeCount = mVkAPI->GetPhysicalDevPresentModesCount(vkResources);
    assert(presentModeCount);

    VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
    assert(presentModes);

    res = mVkAPI->GetPhysicalDevPresentModes(vkResources, presentModeCount, presentModes);
    assert(res == EGL_TRUE);

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for(size_t i = 0; i < presentModeCount; i++) {
        if(presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
           (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
            swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    free(presentModes);

    return swapchainPresentMode;
}

void
VulkanWindowInterface::SetSurfaceColorFormat(EGLSurface_t *surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    /// Get the list of VkFormats that are supported:
    uint32_t formatCount;
    EGLBoolean ASSERT_ONLY res = EGL_FALSE;
    const VulkanResources *vkResources = dynamic_cast<const VulkanResources *>(surface->GetPlatformResources());

    formatCount = mVkAPI->GetPhysicalDevFormatsCount(vkResources);
    assert(formatCount);

    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    res = mVkAPI->GetPhysicalDevFormats(vkResources, formatCount, surfFormats);
    assert(res == EGL_TRUE);

    VkFormat format = VK_FORMAT_UNDEFINED;
    if(formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
        format = mVkDefaultFormat;
    } else {
        assert(formatCount >= 1);
        format = surfFormats[0].format;
    }

    /// Query if the selected format is actually supported by the physical device
    VkFormatProperties formatDeviceProps;
    mVkAPI->GetPhysicalDevFormatProperties(format, &formatDeviceProps);

    if(!(formatDeviceProps.linearTilingFeatures  & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT) &&
       !(formatDeviceProps.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)) {
        format = VK_FORMAT_UNDEFINED;
    }

    free(surfFormats);

    surface->SetColorFormat(static_cast<EGLint>(format));
}

void
VulkanWindowInterface::CreateVkSwapchain(EGLSurface_t* surface,
                                   VkPresentModeKHR swapchainPresentMode,
                                   VkExtent2D swapChainExtent,
                                   VkSurfaceCapabilitiesKHR surfCapabilities)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLBoolean ASSERT_ONLY wsiSuccess;
    /// Determine number of buffers
    assert(surfCapabilities.minImageCount >= 1);
    uint32_t desiredNumberOfSwapChainImages = 2;

    VulkanResources *vkResources = dynamic_cast<VulkanResources *>(surface->GetPlatformResources());
    VkSwapchainKHR vkSwapchain = mVkAPI->CreateSwapchain(vkResources,
                                                         desiredNumberOfSwapChainImages,
                                                         surfCapabilities,
                                                         swapChainExtent,
                                                         swapchainPresentMode,
                                                         static_cast<VkFormat>(surface->GetColorFormat()));
    assert(vkSwapchain != VK_NULL_HANDLE);

    vkResources->SetSwapchain(vkSwapchain);
}

EGLBoolean
VulkanWindowInterface::CreateSurface(EGLDisplay dpy, EGLNativeWindowType win, EGLSurface_t *surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkSurfaceKHR newSurface = mVkWSI->CreateSurface(dpy, win, surface);
    VulkanResources *vkResources = dynamic_cast<VulkanResources *>(surface->GetPlatformResources());

    if(VK_NULL_HANDLE == newSurface) {
        return EGL_FALSE;
    }

    vkResources->SetSurface(newSurface);

    return EGL_TRUE;
}

void
VulkanWindowInterface::AllocateSurfaceImages(EGLSurface_t *surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VkSurfaceCapabilitiesKHR surfCapabilities;
    VkExtent2D swapChainExtent;
    VkPresentModeKHR swapchainPresentMode;

    /// Is present supported?
    if(InitSwapchainExtension(surface) == EGL_FALSE) {
        assert(0);
        return;
    }

    SetSurfaceColorFormat(surface);
    swapChainExtent = SetSwapchainExtent(surface, &surfCapabilities);
    swapchainPresentMode = SetSwapchainPresentMode(surface);
    CreateVkSwapchain(surface, swapchainPresentMode, swapChainExtent, surfCapabilities);
    SetEGLSurfaceVkSwapchainImages(surface);
}

void
VulkanWindowInterface::SetEGLSurfaceVkSwapchainImages(EGLSurface_t* surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLBoolean ASSERT_ONLY wsiSuccess;
    uint32_t swapChainImageCount = 0;
    VkImage *swapChainImages = nullptr;
    VulkanResources *vkResources = dynamic_cast<VulkanResources *>(surface->GetPlatformResources());

    swapChainImageCount = mVkAPI->GetSwapChainImagesCount(vkResources);
    assert(swapChainImageCount);

    swapChainImages = new VkImage[swapChainImageCount]();
    assert(swapChainImages);

    wsiSuccess = mVkAPI->GetSwapChainImages(vkResources, swapChainImageCount, swapChainImages);
    assert(EGL_TRUE == wsiSuccess);

    vkResources->SetSwapChainImageCount(swapChainImageCount);
    vkResources->SetSwapChainImages(swapChainImages);
}

uint32_t
VulkanWindowInterface::AcquireNextImage(EGLSurface_t *surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    uint32_t imageIndex = UINT32_MAX;
    mVkAPI->AcquireNextImage(dynamic_cast<const VulkanResources *>(surface->GetPlatformResources()), &imageIndex);
    surface->SetCurrentImageIndex(imageIndex);

    return imageIndex;
}

void
VulkanWindowInterface::DestroySurfaceImages(EGLSurface_t *eglSurface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    VulkanResources *vkResources = dynamic_cast<VulkanResources *>(eglSurface->GetPlatformResources());

    if(!vkResources) {
        return;
    }

    if(vkResources->GetSwapchain() != VK_NULL_HANDLE) {
        mVkAPI->DestroySwapchain(vkResources);
        vkResources->SetSwapchain(VK_NULL_HANDLE);
    }

    if(vkResources->GetSurface() != VK_NULL_HANDLE) {
        mVkAPI->DestroyPlatformSurface(vkResources);
        vkResources->SetSurface(VK_NULL_HANDLE);
    }
}

EGLBoolean
VulkanWindowInterface::PresentImage(EGLSurface_t *eglSurface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    std::vector<VkSemaphore> pSems;
    if(mVkInterface->vkSyncItems->drawSemaphoreFlag) {
        pSems.push_back(mVkInterface->vkSyncItems->vkDrawSemaphore);
    }
    if(mVkInterface->vkSyncItems->auxSemaphoreFlag) {
        pSems.push_back(mVkInterface->vkSyncItems->vkAuxSemaphore);
    }

    /// present ready buffer
    uint32_t imageIndex = eglSurface->GetCurrentImageIndex();

    mVkInterface->vkSyncItems->acquireSemaphoreFlag = true;
    mVkInterface->vkSyncItems->drawSemaphoreFlag = false;
    mVkInterface->vkSyncItems->auxSemaphoreFlag = false;

    EGLBoolean ASSERT_ONLY mWsiSuccess;
    mWsiSuccess = mVkAPI->PresentImage(dynamic_cast<const VulkanResources *>(eglSurface->GetPlatformResources()), imageIndex, pSems);
    assert(EGL_TRUE == mWsiSuccess);

    return EGL_TRUE;
}

EGLBoolean
VulkanWindowInterface::Initialize()
{
    FUN_ENTRY(DEBUG_DEPTH);

    return InitializeVulkanAPI();
}

EGLBoolean
VulkanWindowInterface::Terminate()
{
    FUN_ENTRY(DEBUG_DEPTH);

    TerminateVulkanAPI();

    return EGL_TRUE;
}
