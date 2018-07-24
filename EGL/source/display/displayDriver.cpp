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
 *  @file       displayDriver.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      EGL Display driver module. It is responsible of communicating to the platform so as to init/terminate a display connection,
 *              create/destroy a Window Surface etc. It connects to the window platform via the abstract class PlatformWindowInterface
 *
 */

#include <vector>
#include "displayDriver.h"
#include "api/eglConfig.h"
#include "api/eglSurface.h"
#include "utils/egl_defs.h"
#include "platform/platformFactory.h"
#include "EGL/egl.h"

#define EGL_VERSION_MAJOR   1
#define EGL_VERSION_MINOR   4

RenderingThread *callingThread = nullptr;

void setCallingThread(RenderingThread *thread) { callingThread = thread; }

DisplayDriver::DisplayDriver(void)
: mDisplay(nullptr), mActiveContext(nullptr), mWindowInterface(nullptr)
{
    FUN_ENTRY(EGL_LOG_TRACE);
}

EGLenum
DisplayDriver::SetErrorAndReturn(EGLenum error)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    callingThread->RecordError(error);

    return error;
}

EGLBoolean
DisplayDriver::Initialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(major) *major = EGL_VERSION_MAJOR;
    if(minor) *minor = EGL_VERSION_MINOR;

    PlatformFactory::ChoosePlatform();
    mWindowInterface = PlatformFactory::GetWindowInterface();

    if(EGL_FALSE == mWindowInterface->Initialize()) {
        return SetErrorAndReturn(EGL_NOT_INITIALIZED);
    }

    return EGL_TRUE;
}

EGLBoolean
DisplayDriver::Terminate(EGLDisplay dpy)
{
    FUN_ENTRY(DEBUG_DEPTH);

#ifdef VK_USE_PLATFORM_XCB_KHR
    if(dpy) {
        delete (eglDisplay_t *)dpy;
    }
#endif

    if(EGL_FALSE == mWindowInterface->Terminate()) {
        return EGL_FALSE;
    }

    delete mWindowInterface;

    PlatformFactory::DestroyInstance();

    return EGL_TRUE;
}

EGLBoolean
DisplayDriver::GetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    FUN_ENTRY(DEBUG_DEPTH);

    int32_t count = 0;

    if(!num_config) {
        callingThread->RecordError(EGL_BAD_PARAMETER);
        return EGL_FALSE;
    }

    if(!configs) {
        *num_config = ARRAY_SIZE(EglConfigs);
        return EGL_TRUE;
    }

    if(config_size > count) {
        count = config_size < ARRAY_SIZE(EglConfigs) ? config_size : ARRAY_SIZE(EglConfigs);
    }

    *num_config = count;

    for(int32_t i = 0; i < count; ++i) {
        configs[i] = (EGLConfig)&EglConfigs[i];
    }

    return EGL_TRUE;
}

EGLBoolean
DisplayDriver::ChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLConfig_t criteria;
    if(ParseConfigAttribList(&criteria, &dpy, attrib_list) == EGL_FALSE) {
        callingThread->RecordError(EGL_BAD_ATTRIBUTE);
        return EGL_FALSE;
    }

    return FilterConfigArray((EGLConfig_t **)configs, config_size, num_config, &criteria);
}

EGLBoolean
DisplayDriver::GetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(attribute < EGL_BUFFER_SIZE || attribute > EGL_CONFORMANT) {
        callingThread->RecordError(EGL_BAD_ATTRIBUTE);
        return EGL_FALSE;
    }

    if(config) {
        *value = GetConfigKey((EGLConfig_t *)config, attribute);
        return EGL_TRUE;
    } else {
        return EGL_FALSE;
    }
}

EGLSurface
DisplayDriver::CreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLSurface_t *eglSurface = new EGLSurface_t();
    if(!eglSurface) {
        assert(0);
        callingThread->RecordError(EGL_BAD_ALLOC);
        return EGL_NO_SURFACE;
    }

    if(eglSurface->InitSurface(EGL_WINDOW_BIT, (EGLConfig_t *)config, attrib_list) != EGL_TRUE) {
        callingThread->RecordError(EGL_BAD_PARAMETER);
        delete eglSurface;
        return EGL_NO_SURFACE;
    }

    PlatformResources *platformResources = PlatformFactory::GetResources();
    eglSurface->SetPlatformResources(platformResources);

    if(mWindowInterface->CreateSurface(dpy, win, eglSurface) == EGL_FALSE) {
        delete eglSurface;
        return EGL_NO_SURFACE;
    }

    mWindowInterface->AllocateSurfaceImages(eglSurface);
    mWindowInterface->AcquireNextImage(eglSurface);

    CreateEGLSurfaceInterface(eglSurface);

    return (EGLSurface)eglSurface;
}

EGLSurface
DisplayDriver::CreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLSurface_t *eglSurface = new EGLSurface_t();
    if(!eglSurface) {
        assert(0);
        callingThread->RecordError(EGL_BAD_ALLOC);
        return EGL_NO_SURFACE;
    }

    if(eglSurface->InitSurface(EGL_PBUFFER_BIT, (EGLConfig_t *)config, attrib_list) != EGL_TRUE) {
        callingThread->RecordError(EGL_BAD_PARAMETER);
        delete eglSurface;
        return EGL_NO_SURFACE;
    }

    if(!eglSurface->GetWidth() || !eglSurface->GetHeight()) {
        eglSurface->SetWidth(EglConfigs[0].MaxPbufferWidth);
        eglSurface->SetHeight(EglConfigs[0].MaxPbufferHeight);
    }

    CreateEGLSurfaceInterface(eglSurface);

    return (EGLSurface)eglSurface;
}

void
DisplayDriver::CreateEGLSurfaceInterface(EGLSurface_t *surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLSurfaceInterface_t *surfaceInterface = surface->GetEGLSurfaceInterface();

    memset(surfaceInterface, 0, sizeof(*surfaceInterface));

    surfaceInterface->surface = reinterpret_cast<void *>(surface);
    if(surface->GetType() == EGL_WINDOW_BIT) {
        surfaceInterface->images = surface->GetPlatformSurfaceImages();
        surfaceInterface->imageCount = surface->GetPlatformSurfaceImageCount();
    }
    surfaceInterface->type                = surface->GetType();
    surfaceInterface->width               = surface->GetWidth();
    surfaceInterface->height              = surface->GetHeight();
    surfaceInterface->depthSize           = surface->GetDepthSize();
    surfaceInterface->stencilSize         = surface->GetStencilSize();
    surfaceInterface->surfaceColorFormat  = surface->GetColorFormat();
    surfaceInterface->nextImageIndex      = surface->GetCurrentImageIndex();
}

EGLSurface
DisplayDriver::CreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return NULL;
}

EGLBoolean
DisplayDriver::DestroySurface(EGLDisplay dpy, EGLSurface surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLSurface_t *eglSurface = static_cast<EGLSurface_t *>(surface);

    if(eglSurface) {
        mWindowInterface->DestroySurfaceImages(eglSurface);

        delete eglSurface->GetPlatformResources();
        delete eglSurface;
    }

    return EGL_TRUE;
}

EGLBoolean
DisplayDriver::QuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLSurface_t *eglSurface = (EGLSurface_t *)surface;
    if(eglSurface->QuerySurface(attribute, value) == EGL_FALSE) {
        callingThread->RecordError(EGL_BAD_ATTRIBUTE);
        return EGL_FALSE;
    }

    return EGL_TRUE;
}

EGLSurface
DisplayDriver::CreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return NULL;
}

EGLBoolean
DisplayDriver::SurfaceAttrib(EGLDisplay dpy, EGLSurface surface,EGLint attribute, EGLint value)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return EGL_FALSE;
}

EGLBoolean
DisplayDriver::BindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return EGL_FALSE;
}

EGLBoolean
DisplayDriver::ReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return EGL_FALSE;
}

EGLBoolean
DisplayDriver::SwapInterval(EGLDisplay dpy, EGLint interval)
{
    FUN_ENTRY(DEBUG_DEPTH);

    // 0 interval can be considered as supported with the current implementation
    if(!interval) {
        return EGL_TRUE;
    }

    NOT_IMPLEMENTED();

    return EGL_FALSE;
}

EGLBoolean
DisplayDriver::SwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    FUN_ENTRY(DEBUG_DEPTH);

    assert(surface);

    EGLSurface_t *eglSurface = reinterpret_cast<EGLSurface_t *>(surface);
    uint32_t imageIndex = 0;

    if(eglSurface->GetType() != EGL_WINDOW_BIT) {
        return EGL_TRUE;
    }

    mActiveContext->Finish();

    if(EGL_FALSE == mWindowInterface->PresentImage(eglSurface)) {
        return EGL_FALSE;
    }

    imageIndex = mWindowInterface->AcquireNextImage(eglSurface);
    mActiveContext->SetNextImageIndex(imageIndex);

    return EGL_TRUE;
}

EGLBoolean
DisplayDriver::CopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return EGL_FALSE;
}

EGLImageKHR
DisplayDriver::CreateImageNativeBufferAndroid(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list)
{
    FUN_ENTRY(DEBUG_DEPTH);

#ifdef VK_USE_PLATFORM_ANDROID_KHR
    (void)target;

    if(dpy == EGL_NO_DISPLAY) {
        callingThread->RecordError(EGL_BAD_DISPLAY);
        return EGL_NO_IMAGE_KHR;
    }

    if(ctx != EGL_NO_CONTEXT) {
        callingThread->RecordError(EGL_BAD_CONTEXT);
        return EGL_NO_IMAGE_KHR;
    }

    ANativeWindowBuffer *native_buffer = (ANativeWindowBuffer*)buffer;

    if(native_buffer->common.magic != ANDROID_NATIVE_BUFFER_MAGIC ||
       native_buffer->common.version != sizeof(ANativeWindowBuffer)) {
        callingThread->RecordError(EGL_BAD_PARAMETER);
        return EGL_NO_IMAGE_KHR;
    }

    switch(native_buffer->format) {
        case HAL_PIXEL_FORMAT_RGBA_8888:
        case HAL_PIXEL_FORMAT_RGBX_8888:
        case HAL_PIXEL_FORMAT_RGB_565:
            break;
        default:
            callingThread->RecordError(EGL_BAD_PARAMETER);
            return EGL_NO_IMAGE_KHR;
    }

    native_buffer->common.incRef(&native_buffer->common);

    return (EGLImageKHR)native_buffer;
#else
    return (EGLImageKHR)EGL_NO_IMAGE_KHR;
#endif
}


EGLImageKHR
DisplayDriver::CreateImageKHR(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list)
{
    FUN_ENTRY(DEBUG_DEPTH);

    switch(target) {
        case EGL_NATIVE_BUFFER_ANDROID:
            return CreateImageNativeBufferAndroid(dpy, ctx, target, buffer, attrib_list);
        case EGL_GL_TEXTURE_2D_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR:
        case EGL_GL_TEXTURE_3D_KHR:
        case EGL_GL_RENDERBUFFER_KHR:
            NOT_IMPLEMENTED();
        default:
            callingThread->RecordError(EGL_BAD_PARAMETER);
            return EGL_NO_IMAGE_KHR;
    }
}

EGLBoolean
DisplayDriver::DestroyImageKHR(EGLDisplay dpy, EGLImageKHR image)
{
    FUN_ENTRY(DEBUG_DEPTH);

#ifdef VK_USE_PLATFORM_ANDROID_KHR
    if(dpy == EGL_NO_DISPLAY) {
        callingThread->RecordError(EGL_BAD_DISPLAY);
        return EGL_FALSE;
    }

    ANativeWindowBuffer* native_buffer = (ANativeWindowBuffer*)image;

    if(native_buffer->common.magic != ANDROID_NATIVE_BUFFER_MAGIC ||
       native_buffer->common.version != sizeof(ANativeWindowBuffer)) {
        callingThread->RecordError(EGL_BAD_PARAMETER);
        return EGL_FALSE;
    }

    native_buffer->common.decRef(&native_buffer->common);

    return EGL_TRUE;
#endif
    return EGL_FALSE;
}

EGLSyncKHR
DisplayDriver::CreateSyncKHR(EGLDisplay dpy, EGLenum type, const EGLint *attrib_list)
{
    FUN_ENTRY(EGL_LOG_TRACE);
    return ((EGLSyncKHR)0xFE4CE000);
}

EGLBoolean
DisplayDriver::DestroySyncKHR(EGLDisplay dpy, EGLSyncKHR sync)
{
    FUN_ENTRY(EGL_LOG_TRACE);
    return EGL_TRUE;
}

EGLint
DisplayDriver::ClientWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout)
{
    FUN_ENTRY(EGL_LOG_TRACE);
    return EGL_CONDITION_SATISFIED_KHR;
}

__eglMustCastToProperFunctionPointerType
DisplayDriver::GetProcAddress(const char *procname)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return NULL;
}
