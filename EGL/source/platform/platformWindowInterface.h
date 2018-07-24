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
 *  @file       platformWindowInterface.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Interface class to connect to a window platform. It should implement functions for creating/destroying 
 *              a window surface and relative resources.
 *
 */

#ifndef __PLATFORM_WINDOW_INTERFACE_H__
#define __PLATFORM_WINDOW_INTERFACE_H__

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "api/eglSurface.h"

class PlatformWindowInterface
{
public:
    PlatformWindowInterface() { }
    virtual ~PlatformWindowInterface() { }

    virtual EGLBoolean           Initialize() = 0;
    virtual EGLBoolean           Terminate() = 0;;
    virtual EGLBoolean           CreateSurface(EGLDisplay dpy, EGLNativeWindowType win, EGLSurface_t *surface) = 0;
    virtual void                 AllocateSurfaceImages(EGLSurface_t *surface) = 0;
    virtual void                 DestroySurfaceImages(EGLSurface_t *eglSurface) = 0;
    virtual uint                 AcquireNextImage(EGLSurface_t *surface) = 0;
    virtual EGLBoolean           PresentImage(EGLSurface_t *eglSurface) = 0;
};

#endif // __PLATFORM_WINDOW_INTERFACE_H__
