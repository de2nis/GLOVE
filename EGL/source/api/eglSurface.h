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
 *  @file       eglSurface.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      EGL Surface container. It connects EGL to platform resources via the abstract class PlatformResources 
 *
 */

#ifndef __EGL_SURFACE_H__
#define __EGL_SURFACE_H__

#include "EGL/egl.h"
#include "eglContext.h"
#include "eglConfig.h"
#include "rendering_api/rendering_api.h"
#include "utils/egl_defs.h"
#include "platform/platformResources.h"

#ifdef DEBUG_DEPTH
#   undef DEBUG_DEPTH
#endif // DEBUG_DEPTH
#define DEBUG_DEPTH EGL_LOG_DEBUG

class EGLSurface_t
{
private:
    EGLConfig_t                     *Config;

    /* one of EGL_WINDOW_BIT, EGL_PIXMAP_BIT or EGL_PBUFFER_BIT */
    EGLint                           Type;

    /* attributes set by attribute list */
    EGLint                           Width, Height;
    EGLint                           DepthSize, StencilSize;
    EGLint                           RedSize, GreenSize, BlueSize, AlphaSize;
    EGLenum                          TextureFormat;
    EGLenum                          TextureTarget;
    EGLBoolean                       MipmapTexture;
    EGLBoolean                       LargestPbuffer;
    EGLenum                          RenderBuffer;
    EGLenum                          VGAlphaFormat;
    EGLenum                          VGColorspace;

    /* attributes set by eglSurfaceAttrib */
    EGLint                           MipmapLevel;
    EGLenum                          MultisampleResolve;
    EGLenum                          SwapBehavior;

    EGLint                           HorizontalResolution, VerticalResolution;
    EGLint                           AspectRatio;

    EGLint                           SwapInterval;

    /* True if the surface is bound to an OpenGL ES texture */
    EGLBoolean                       BoundToTexture;

    EGLBoolean                       PostSubBufferSupportedNV;
    EGLint                           CurrentImageIndex;
    EGLint                           ColorFormat;
    EGLSurfaceInterface_t            SurfaceInterface;

    PlatformResources               *mPlatformResources;

public:
    EGLSurface_t();
    ~EGLSurface_t() {}

    EGLBoolean                       InitSurface(EGLint type, EGLConfig_t *config, const EGLint *attrib_list);

    /**
     * Parse the list of surface attributes and return the proper error code.
     */
    EGLint                           ParseSurfaceAttribList(const EGLint *attrib_list);

    /**
     * Do error check on parameters and initialize the given EGLSurface_t object.
     * \return EGL_TRUE if no errors, EGL_FALSE otherwise.
     */
    EGLBoolean                       QuerySurface(EGLint attribute, EGLint *value);

    inline void                      SetCurrentImageIndex(EGLint imageIndex)                    { FUN_ENTRY(EGL_LOG_TRACE); CurrentImageIndex = imageIndex; }
    inline void                      SetWidth(EGLint width)                                     { FUN_ENTRY(EGL_LOG_TRACE); Width = width; }
    inline void                      SetHeight(EGLint height)                                   { FUN_ENTRY(EGL_LOG_TRACE); Height = height; }
    inline void                      SetColorFormat(EGLint colorFormat)                         { FUN_ENTRY(EGL_LOG_TRACE); ColorFormat = colorFormat; }
    inline void                      SetPlatformResources(PlatformResources *platformResources) { FUN_ENTRY(EGL_LOG_TRACE); mPlatformResources = platformResources; }

    inline EGLint                    GetType()                                                  { FUN_ENTRY(EGL_LOG_TRACE); return Type; }
    inline EGLint                    GetWidth()                                                 { FUN_ENTRY(EGL_LOG_TRACE); return Width; }
    inline EGLint                    GetHeight()                                                { FUN_ENTRY(EGL_LOG_TRACE); return Height; }
    inline EGLint                    GetDepthSize()                                             { FUN_ENTRY(EGL_LOG_TRACE); return DepthSize; }
    inline EGLint                    GetStencilSize()                                           { FUN_ENTRY(EGL_LOG_TRACE); return StencilSize; }
    inline EGLint                    GetCurrentImageIndex()                                     { FUN_ENTRY(EGL_LOG_TRACE); return CurrentImageIndex; }
    inline EGLint                    GetColorFormat()                                           { FUN_ENTRY(EGL_LOG_TRACE); return ColorFormat; }
    inline EGLSurfaceInterface_t    *GetEGLSurfaceInterface()                                   { FUN_ENTRY(EGL_LOG_TRACE); return &SurfaceInterface; }
    inline const PlatformResources  *GetPlatformResources()                               const { FUN_ENTRY(EGL_LOG_TRACE); return mPlatformResources; }
    inline PlatformResources        *GetPlatformResources()                                     { FUN_ENTRY(EGL_LOG_TRACE); return mPlatformResources; }
    inline uint32_t                  GetPlatformSurfaceImageCount()                             { FUN_ENTRY(EGL_LOG_TRACE); return mPlatformResources->GetSwapchainImageCount(); }
    inline void                     *GetPlatformSurfaceImages()                                 { FUN_ENTRY(EGL_LOG_TRACE); return mPlatformResources->GetSwapchainImages(); }
};

#endif // __EGL_SURFACE_H__
