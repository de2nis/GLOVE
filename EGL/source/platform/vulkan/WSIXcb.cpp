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
 *  @file       WSIXcb.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      WSI XCB module. It gets VkSurface for XCB Window platform.
 *
 */

#include "WSIXcb.h"

void
WSIXcb::GetXCBConnection(xcbContext *xcb)
{
    FUN_ENTRY(DEBUG_DEPTH);

    xcb->connection = NULL;

    if(xcb->dpy->nativeDisplay == EGL_DEFAULT_DISPLAY) {
        int scr;

        if(!(xcb->connection = xcb_connect(NULL, &scr))) {
            printf("xcb_connect failed.\n");
            fflush(stdout);
        }

        const xcb_setup_t *setup = xcb_get_setup(xcb->connection);
        xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
        while(scr-- > 0) {
            xcb_screen_next(&iter);
        }

        xcb->screen = iter.data;
    } else {
        // Get xcb connection from X display
        xcb->connection = XGetXCBConnection(xcb->dpy->nativeDisplay);
    }
}

EGLBoolean
WSIXcb::SetSurfaceCallback(void)
{
    FUN_ENTRY(DEBUG_DEPTH);

    PFN_vkCreateXcbSurfaceKHR fpCreateXcbSurfaceKHR = nullptr;

    fpCreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR) vkGetInstanceProcAddr(mVkInstance, "vkCreateXcbSurfaceKHR");
    if(fpCreateXcbSurfaceKHR == nullptr) {
        assert(fpCreateXcbSurfaceKHR && "Could not get function pointer to CreateXcbSurfaceKHR");
        return EGL_FALSE;
    }

    mWsiCallbacks.fpCreateSurface = reinterpret_cast<void *>(fpCreateXcbSurfaceKHR);

    return EGL_TRUE;
}

VkSurfaceKHR
WSIXcb::CreateSurface(EGLDisplay dpy, EGLNativeWindowType win, EGLSurface_t *surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(!surface) {
        return VK_NULL_HANDLE;
    }

    PFN_vkCreateXcbSurfaceKHR fpCreateXcbSurfaceKHR = reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>(mWsiCallbacks.fpCreateSurface);

    xcbContext xcb = {(eglDisplay_t *)dpy, NULL, NULL};
    GetXCBConnection(&xcb);

    if(!surface->GetWidth() || !surface->GetHeight()) {
        xcb_get_geometry_reply_t *winProps;
        winProps = xcb_get_geometry_reply(xcb.connection,
                                          xcb_get_geometry(xcb.connection, win),
                                          NULL);
        assert(winProps);

        surface->SetWidth(winProps->width);
        surface->SetHeight(winProps->height);

        free(winProps);
    }

    VkSurfaceKHR vkSurface;
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo;
    memset((void *)&surfaceCreateInfo, 0 ,sizeof(surfaceCreateInfo));
    surfaceCreateInfo.sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext      = NULL;
    surfaceCreateInfo.connection = xcb.connection;
    surfaceCreateInfo.window     = (xcb_window_t)win;

    if(VK_SUCCESS != fpCreateXcbSurfaceKHR(mVkInstance, &surfaceCreateInfo, NULL, &vkSurface)) {
        return VK_NULL_HANDLE;
    }

    return vkSurface;
}
