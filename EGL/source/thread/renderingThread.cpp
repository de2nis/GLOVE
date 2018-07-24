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
 *  @file       renderingThread.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Rendering thread module. It implements EGL API calls bound to the rendering thread.
 *
 */

#include "renderingThread.h"
#include "api/eglSurface.h"
#include "utils/eglLogger.h"

const char * const RenderingThread::EGLErrors[] = {   "EGL_SUCCESS",
                                                      "EGL_NOT_INITIALIZED",
                                                      "EGL_BAD_ACCESS",
                                                      "EGL_BAD_ALLOC",
                                                      "EGL_BAD_ATTRIBUTE",
                                                      "EGL_BAD_CONFIG",
                                                      "EGL_BAD_CONTEXT",
                                                      "EGL_BAD_CURRENT_SURFACE",
                                                      "EGL_BAD_DISPLAY",
                                                      "EGL_BAD_MATCH",
                                                      "EGL_BAD_NATIVE_PIXMAP",
                                                      "EGL_BAD_NATIVE_WINDOW",
                                                      "EGL_BAD_PARAMETER",
                                                      "EGL_BAD_SURFACE",
                                                      "EGL_CONTEXT_LOST"};


RenderingThread::RenderingThread()
: mLastError(EGL_SUCCESS)
{
    FUN_ENTRY(EGL_LOG_TRACE);
}

void
RenderingThread::RecordError(EGLenum error)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    if (error != EGL_SUCCESS && mLastError == EGL_SUCCESS) {
        mLastError = error;
    }
}

EGLint
RenderingThread::GetError(void)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    if(mLastError != 0x3000) {
        printf("Error: %s\n", EGLErrors[mLastError - 0x3000]);
    }

    EGLint result = mLastError;
    mLastError = EGL_SUCCESS;

    return result;
}

EGLBoolean
RenderingThread::BindAPI(EGLenum api)
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(api != EGL_OPENGL_API && api != EGL_OPENGL_ES_API && api != EGL_OPENVG_API) {
        RecordError(EGL_BAD_PARAMETER);
        return EGL_FALSE;
    }

    mCurrentAPI = api;

    return EGL_TRUE;
}

EGLenum
RenderingThread::QueryAPI(void)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    return mCurrentAPI;
}

EGLBoolean
RenderingThread::WaitClient(void)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLContext_t *activeContext = static_cast<EGLContext_t *>(GetCurrentContext());
    if (activeContext == nullptr) {
        return EGL_FALSE;
    }

    activeContext->Finish();

    return EGL_TRUE;
}

EGLBoolean
RenderingThread::ReleaseThread(void)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return EGL_FALSE;
}

EGLContext
RenderingThread::GetCurrentContext(void)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    switch(mCurrentAPI) {
        case EGL_OPENGL_ES_API:    return mGLESCurrentContext; break;
        case EGL_OPENVG_API:       return mVGCurrentContext; break;
        default:                   return nullptr;
    }
}

EGLSurface
RenderingThread::GetCurrentSurface(EGLint readdraw)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    if(EGL_READ != readdraw && EGL_DRAW != readdraw) {
        RecordError(EGL_BAD_PARAMETER);
        return EGL_NO_SURFACE;
    }

    EGLContext_t *activeContext = static_cast<EGLContext_t *>(GetCurrentContext());
    if (activeContext == nullptr) {
        return EGL_NO_SURFACE;
    }

    return (EGL_READ == readdraw) ? activeContext->getReadSurface() : activeContext->getDrawSurface();

}

EGLDisplay
RenderingThread::GetCurrentDisplay(void)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    EGLContext_t *activeContext = static_cast<EGLContext_t *>(GetCurrentContext());
    if (activeContext == nullptr) {
        return EGL_NO_DISPLAY;
    }

    return activeContext->getDisplay();
}

EGLContext
RenderingThread::CreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    EGLContext_t *eglContext = new EGLContext_t(mCurrentAPI, attrib_list);

    if(EGL_FALSE == eglContext->CreateRenderingContext()) {
        delete eglContext;
        return nullptr;
    }

    return eglContext;
}

EGLBoolean
RenderingThread::DestroyContext(EGLDisplay dpy, EGLContext ctx)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    EGLContext_t *eglContext = static_cast<EGLContext_t *>(ctx);
    assert(ctx);
    if (eglContext == nullptr) {
        RecordError(EGL_BAD_CONTEXT);
        return EGL_FALSE;
    }

    if(EGL_FALSE == eglContext->DestroyRenderingContext()) {
        return EGL_FALSE;
    }

    delete eglContext;
    return EGL_TRUE;
}

EGLBoolean
RenderingThread::QueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return EGL_FALSE;
}

EGLBoolean
RenderingThread::MakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
    FUN_ENTRY(DEBUG_DEPTH);

    if(!ctx) {
        return EGL_FALSE;
    }

    EGLContext_t *eglContext = static_cast<EGLContext_t *>(ctx);
    assert(eglContext);
    if(eglContext == nullptr) {
        RecordError(EGL_BAD_CONTEXT);
        return EGL_FALSE;
    }
    EGLSurface_t *drawSurface = static_cast<EGLSurface_t *>(draw);
    EGLSurface_t *readSurface = static_cast<EGLSurface_t *>(read);

    if (drawSurface == nullptr || readSurface == nullptr) {
        RecordError(EGL_BAD_SURFACE);
        return EGL_FALSE;
    }

    if(eglContext->MakeCurrent(dpy, draw, read) != EGL_TRUE) {
        return EGL_FALSE;
    }

    switch(mCurrentAPI) {
        case EGL_OPENGL_ES_API:
            mGLESCurrentContext = eglContext;
            break;
        case EGL_OPENVG_API:
            mVGCurrentContext = eglContext;
            break;
    }

    return EGL_TRUE;
}

EGLBoolean
RenderingThread::WaitGL(void)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLBoolean ret = EGL_FALSE;
    EGLenum api = mCurrentAPI;
    mCurrentAPI = EGL_OPENGL_ES_API;
    ret = WaitClient();
    mCurrentAPI = api;

    return ret;
}

EGLBoolean
RenderingThread::WaitNative(EGLint engine)
{
    FUN_ENTRY(DEBUG_DEPTH);

    NOT_IMPLEMENTED();

    return EGL_FALSE;
}
