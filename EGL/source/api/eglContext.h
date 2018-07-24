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
 *  @file       eglContext.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      EGL Context functionality. It connects EGL to the client API
 *
 */

#ifndef __EGL_CONTEXT_H__
#define __EGL_CONTEXT_H__

#include "EGL/egl.h"
#include "rendering_api/rendering_api.h"
#include "utils/eglLogger.h"

#ifdef DEBUG_DEPTH
#   undef DEBUG_DEPTH
#endif // DEBUG_DEPTH
#define DEBUG_DEPTH              EGL_LOG_DEBUG


class EGLContext_t {
private:
    api_context_t                mAPIContext;
    EGLenum                      mRenderingAPI;
    EGLenum                      mClientVersion;
    rendering_api_interface_t   *mAPIInterface;
    EGLDisplay                   mDisplay;
    EGLSurface                   mReadSurface;
    EGLSurface                   mDrawSurface;

    EGLenum                      GetClientVersionFromConfig(const EGLint *attribList);

public:
    EGLContext_t(EGLenum rendering_api, const EGLint *attribList);
    ~EGLContext_t();

    EGLDisplay                   getDisplay()                             const { FUN_ENTRY(EGL_LOG_TRACE); return mDisplay; }
    EGLSurface                   getReadSurface()                         const { FUN_ENTRY(EGL_LOG_TRACE); return mReadSurface; }
    EGLSurface                   getDrawSurface()                         const { FUN_ENTRY(EGL_LOG_TRACE); return mDrawSurface; }

    EGLBoolean                   CreateRenderingContext();
    EGLBoolean                   DestroyRenderingContext();
    EGLBoolean                   MakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read);
    void                         SetNextImageIndex(uint32_t index);
    void                         Finish();

};

#endif // __EGL_CONTEXT_H__
