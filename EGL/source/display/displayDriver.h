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
 *  @file       displayDriver.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      EGL Display driver module. It is responsible of communicating to the platform so as to init/terminate a display connection,
 *              create/destroy a Window Surface etc. It connects to the window platform via the abstract class PlatformWindowInterface
 *
 */

#ifndef __DISPLAY_DRIVER_H__
#define __DISPLAY_DRIVER_H__

#include "api/eglContext.h"
#include "thread/renderingThread.h"
#include "utils/egl_defs.h"
#include "utils/eglLogger.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "platform/platformWindowInterface.h"

#ifdef DEBUG_DEPTH
#   undef DEBUG_DEPTH
#endif // DEBUG_DEPTH
#define DEBUG_DEPTH                                 EGL_LOG_DEBUG

void setCallingThread(RenderingThread *thread);

class DisplayDriver {
private:
    EGLDisplay                   mDisplay;
    EGLContext_t                *mActiveContext;
    PlatformWindowInterface     *mWindowInterface;

    EGLImageKHR                  CreateImageNativeBufferAndroid(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list);
    void                         CreateEGLSurfaceInterface(EGLSurface_t *surface);
    EGLenum                      SetErrorAndReturn(EGLenum error);

public:

    DisplayDriver(void);
    virtual ~DisplayDriver(void)                                                { FUN_ENTRY(EGL_LOG_TRACE); }

    inline EGLDisplay            GetDisplay()                                   { FUN_ENTRY(EGL_LOG_TRACE); return mDisplay; }
    inline void                  SetDisplay(EGLDisplay display)                 { FUN_ENTRY(EGL_LOG_TRACE); mDisplay = display; }
    inline void                  SetActiveContext(EGLContext ctx)               { FUN_ENTRY(EGL_LOG_TRACE); mActiveContext = static_cast<EGLContext_t *>(ctx); }

    /// EGL API core functions
    EGLBoolean                   Initialize(EGLDisplay dpy, EGLint *major, EGLint *minor);
    EGLBoolean                   Terminate(EGLDisplay dpy);
    EGLBoolean                   GetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
    EGLBoolean                   ChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
    EGLBoolean                   GetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value);
    EGLSurface                   CreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list);
    EGLSurface                   CreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
    EGLSurface                   CreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list);
    EGLBoolean                   DestroySurface(EGLDisplay dpy, EGLSurface surface);
    EGLBoolean                   QuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
    EGLSurface                   CreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list);
    EGLBoolean                   SurfaceAttrib(EGLDisplay dpy, EGLSurface surface,EGLint attribute, EGLint value);
    EGLBoolean                   BindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
    EGLBoolean                   ReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
    EGLBoolean                   SwapInterval(EGLDisplay dpy, EGLint interval);
    EGLBoolean                   SwapBuffers(EGLDisplay dpy, EGLSurface surface);
    EGLBoolean                   CopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
    __eglMustCastToProperFunctionPointerType
                                 GetProcAddress(const char *procname);


    /// EGL API extension functions
    EGLImageKHR                  CreateImageKHR(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list);
    EGLBoolean                   DestroyImageKHR(EGLDisplay dpy, EGLImageKHR image);
    EGLSyncKHR                   CreateSyncKHR(EGLDisplay dpy, EGLenum type, const EGLint *attrib_list);
    EGLBoolean                   DestroySyncKHR(EGLDisplay dpy, EGLSyncKHR sync);
    EGLint                       ClientWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout);
};

#endif // __DISPLAY_DRIVER_H__
