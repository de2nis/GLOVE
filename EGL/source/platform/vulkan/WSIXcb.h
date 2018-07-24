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
 *  @file       WSIXcb.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      WSI XCB module. It gets VkSurface for XCB Window platform.
 *
 */

#ifndef __WSI_XCB_H__
#define __WSI_XCB_H__

#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include "vulkanWSI.h"

class WSIXcb : public VulkanWSI
{
private:
    typedef struct {
        eglDisplay_t     *  dpy;
        xcb_connection_t *  connection;
        xcb_screen_t     *  screen;
    } xcbContext;

    void               GetXCBConnection(xcbContext *xcb);

protected:
    EGLBoolean         SetSurfaceCallback() override;

public:
    WSIXcb()  {}
    ~WSIXcb() {}

    VkSurfaceKHR       CreateSurface(EGLDisplay dpy,
                                    EGLNativeWindowType win,
                                    EGLSurface_t *surface) override;
};

#endif // __WSI_XCB_H__
