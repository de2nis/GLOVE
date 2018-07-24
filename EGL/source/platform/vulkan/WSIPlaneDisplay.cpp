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
 *  @file       WSIPlaneDisplay.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      WSI Plane Display module. It gets Plane Display VkSurface.
 *
 */

#include "WSIPlaneDisplay.h"

EGLBoolean
WSIPlaneDisplay::SetSurfaceCallback(void)
{
    FUN_ENTRY(DEBUG_DEPTH);

    PFN_vkCreateDisplayPlaneSurfaceKHR fpCreateDisplayPlaneSurfaceKHR = nullptr;

    fpCreateDisplayPlaneSurfaceKHR = (PFN_vkCreateDisplayPlaneSurfaceKHR) vkGetInstanceProcAddr(mVkInstance, "vkCreateDisplayPlaneSurfaceKHR");
    if(fpCreateDisplayPlaneSurfaceKHR == nullptr) {
        assert(fpCreateDisplayPlaneSurfaceKHR && "Could not get function pointer to CreateDisplayPlaneSurfaceKHR");
        return EGL_FALSE;
    }

    mWsiCallbacks.fpCreateSurface = reinterpret_cast<void *>(fpCreateDisplayPlaneSurfaceKHR);

    return EGL_TRUE;
}

VkSurfaceKHR
WSIPlaneDisplay::CreateSurface(EGLDisplay dpy, EGLNativeWindowType win, EGLSurface_t *surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(!surface) {
        return VK_NULL_HANDLE;
    }

    PFN_vkCreateDisplayPlaneSurfaceKHR fpCreateDisplayPlaneSurfaceKHR = reinterpret_cast<PFN_vkCreateDisplayPlaneSurfaceKHR>(mWsiCallbacks.fpCreateSurface);

    /// Create a vk surface
    VkSurfaceKHR vkSurface;
    VkDisplaySurfaceCreateInfoKHR surfaceCreateInfo;
    memset((void *)&surfaceCreateInfo, 0 ,sizeof(surfaceCreateInfo));
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = NULL;
    surfaceCreateInfo.imageExtent.width = surface->GetWidth();
    surfaceCreateInfo.imageExtent.height = surface->GetHeight();

    if(VK_SUCCESS != fpCreateDisplayPlaneSurfaceKHR(mVkInstance, &surfaceCreateInfo, NULL, &vkSurface)) {
        return VK_NULL_HANDLE;
    }

    return vkSurface;
}
