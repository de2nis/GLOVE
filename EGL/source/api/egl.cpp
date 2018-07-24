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
 *  @file       egl.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Entry points for the EGL API calls
 *
 */

#include "display/displayDriversContainer.h"
#include "display/displayDriver.h"
#include "utils/eglLogger.h"
#include "thread/renderingThread.h"

#ifdef DEBUG_DEPTH
#   undef DEBUG_DEPTH
#endif // DEBUG_DEPTH
#define DEBUG_DEPTH                          EGL_LOG_INFO

static RenderingThread currentThread;

#define THREAD_EXEC_RETURN(func)             FUN_ENTRY(DEBUG_DEPTH);                                                        \
                                             return currentThread.func;

#define DRIVER_EXEC(display, func)           FUN_ENTRY(DEBUG_DEPTH);                                                        \
                                             DisplayDriver *eglDriver = DisplayDriversContainer::GetDisplayDriver(display); \
                                             eglDriver->func;

#define DRIVER_EXEC_RETURN(display, func)    FUN_ENTRY(DEBUG_DEPTH);                                                        \
                                             DisplayDriver *eglDriver = DisplayDriversContainer::GetDisplayDriver(display); \
                                             return eglDriver ? eglDriver->func : 0;

static void cleanUpResources()
{
    FUN_ENTRY(EGL_LOG_DEBUG);

    if(DisplayDriversContainer::IsEmpty()) {
        DisplayDriversContainer::Destroy();
        EGLLogger::Shutdown();
    }
}

EGLAPI EGLDisplay EGLAPIENTRY
eglGetDisplay(EGLNativeDisplayType display_id)
{
    FUN_ENTRY(DEBUG_DEPTH);

#ifdef VK_USE_PLATFORM_ANDROID_KHR
    if (display == EGL_DEFAULT_DISPLAY) {
        EGLDisplay dpy = (EGLDisplay)1;
        return dpy;
    }
    return EGL_NO_DISPLAY;
#else
    eglDisplay_t *eglDisplay = new eglDisplay_t();
    if(EGL_DEFAULT_DISPLAY) {
        eglDisplay->nativeDisplay = XOpenDisplay(NULL);
    } else {
        eglDisplay->nativeDisplay = display_id;
    }

    return static_cast<EGLDisplay>(eglDisplay);
#endif

    return nullptr;
}

EGLAPI EGLint EGLAPIENTRY
eglGetError(void)
{
    THREAD_EXEC_RETURN(GetError());
}

EGLAPI EGLBoolean EGLAPIENTRY
eglBindAPI(EGLenum api)
{
    THREAD_EXEC_RETURN(BindAPI(api));
}

EGLAPI EGLenum EGLAPIENTRY
eglQueryAPI(void)
{
    THREAD_EXEC_RETURN(QueryAPI());
}

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitClient(void)
{
    THREAD_EXEC_RETURN(WaitClient());
}

EGLAPI EGLBoolean EGLAPIENTRY
eglReleaseThread(void)
{
    THREAD_EXEC_RETURN(ReleaseThread());
}

EGLAPI EGLContext EGLAPIENTRY
eglGetCurrentContext(void)
{
    THREAD_EXEC_RETURN(GetCurrentContext());
}

EGLAPI EGLSurface EGLAPIENTRY
eglGetCurrentSurface(EGLint readdraw)
{
    THREAD_EXEC_RETURN(GetCurrentSurface(readdraw));
}

EGLAPI EGLDisplay EGLAPIENTRY
eglGetCurrentDisplay(void)
{
    THREAD_EXEC_RETURN(GetCurrentDisplay());
}

EGLAPI EGLContext EGLAPIENTRY
eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
{
    THREAD_EXEC_RETURN(CreateContext(dpy, config, share_context, attrib_list));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
    THREAD_EXEC_RETURN(DestroyContext(dpy, ctx));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
    DRIVER_EXEC(dpy, SetActiveContext(ctx));
    THREAD_EXEC_RETURN(MakeCurrent(dpy, draw, read, ctx));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value)
{
    THREAD_EXEC_RETURN(QueryContext(dpy, ctx, attribute, value));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitGL(void)
{
    THREAD_EXEC_RETURN(WaitGL());
}

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitNative(EGLint engine)
{
    THREAD_EXEC_RETURN(WaitNative(engine));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
    DRIVER_EXEC_RETURN(dpy, Initialize(dpy, major, minor));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglTerminate(EGLDisplay dpy)
{
    DRIVER_EXEC(dpy, Terminate(dpy));
    DisplayDriversContainer::RemoveDisplayDriver(dpy);
    cleanUpResources();
    
    return EGL_TRUE;
}

EGLAPI const char * EGLAPIENTRY
eglQueryString(EGLDisplay dpy, EGLint name)
{
    FUN_ENTRY(DEBUG_DEPTH);

    switch(name) {
    case EGL_CLIENT_APIS:   return "EGL_OPENGL_ES_API\0"; break;
    case EGL_VENDOR:        return "GLOVE (GL Over Vulkan)\0"; break;
    case EGL_VERSION:       return "1.4\0"; break;
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    case EGL_EXTENSIONS:    return "EGL_KHR_image_base EGL_ANDROID_image_native_buffer\0"; break;
#else
    case EGL_EXTENSIONS:    return "\0"; break;
#endif
    default:                return "\0"; break;
    }

    return "\0";
}

EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    DRIVER_EXEC_RETURN(dpy, GetConfigs(dpy, configs, config_size, num_config));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    DRIVER_EXEC_RETURN(dpy, ChooseConfig(dpy, attrib_list, configs, config_size, num_config));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
    DRIVER_EXEC_RETURN(dpy, GetConfigAttrib(dpy, config, attribute, value));
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
    DRIVER_EXEC_RETURN(dpy, CreateWindowSurface(dpy, config, win, attrib_list));
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
    DRIVER_EXEC_RETURN(dpy, CreatePbufferSurface(dpy, config, attrib_list));
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
    DRIVER_EXEC_RETURN(dpy, CreatePixmapSurface(dpy, config, pixmap, attrib_list));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
    DRIVER_EXEC_RETURN(dpy, DestroySurface(dpy, surface));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
    DRIVER_EXEC_RETURN(dpy, QuerySurface(dpy, surface, attribute, value));
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
{
    DRIVER_EXEC_RETURN(dpy, CreatePbufferFromClientBuffer(dpy, buftype, buffer, config, attrib_list));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
{
    DRIVER_EXEC_RETURN(dpy, SurfaceAttrib(dpy, surface, attribute, value));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    DRIVER_EXEC_RETURN(dpy, BindTexImage(dpy, surface, buffer));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    DRIVER_EXEC_RETURN(dpy, ReleaseTexImage(dpy, surface, buffer));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
    DRIVER_EXEC_RETURN(dpy, SwapInterval(dpy, interval));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    DRIVER_EXEC_RETURN(dpy, SwapBuffers(dpy, surface));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
    DRIVER_EXEC_RETURN(dpy, CopyBuffers(dpy, surface, target));
}

EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY
eglGetProcAddress(const char *procname)
{
    NOT_IMPLEMENTED();

    return NULL;
}

EGLAPI EGLImageKHR EGLAPIENTRY
eglCreateImageKHR(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list)
{
    DRIVER_EXEC_RETURN(dpy, CreateImageKHR(dpy, ctx, target, buffer, attrib_list));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroyImageKHR(EGLDisplay dpy, EGLImageKHR image)
{
    DRIVER_EXEC_RETURN(dpy, DestroyImageKHR(dpy, image));
}

//TODO: Implement the KHR_fence_sync extension
EGLAPI EGLSyncKHR EGLAPIENTRY
eglCreateSyncKHR(EGLDisplay dpy, EGLenum type, const EGLint *attrib_list)
{
    DRIVER_EXEC_RETURN(dpy, CreateSyncKHR(dpy, type, attrib_list));
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroySyncKHR(EGLDisplay dpy, EGLSyncKHR sync)
{
    DRIVER_EXEC_RETURN(dpy, DestroySyncKHR(dpy, sync));
}

EGLAPI EGLint EGLAPIENTRY
eglClientWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout)
{
    DRIVER_EXEC_RETURN(dpy, ClientWaitSyncKHR(dpy, sync, flags, timeout));
}
