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
 *  @file       eglSurface.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      EGL Surface container. It connects EGL to platform resources via the abstract class PlatformResources 
 *
 */

#include "eglSurface.h"

EGLSurface_t::EGLSurface_t()
: Config(nullptr), Type(0), Width(0), Height(0),
DepthSize(0), StencilSize(0), RedSize(0), GreenSize(0), BlueSize(0), AlphaSize(0),
TextureFormat(0), TextureTarget(0), MipmapTexture(EGL_FALSE),
LargestPbuffer(EGL_FALSE), RenderBuffer(0), VGAlphaFormat(0), VGColorspace(0),
MipmapLevel(0), MultisampleResolve(0), SwapBehavior(0), HorizontalResolution(0),
VerticalResolution(0), AspectRatio(0), SwapInterval(0), BoundToTexture(EGL_FALSE), PostSubBufferSupportedNV(0),
CurrentImageIndex(0), mPlatformResources(nullptr)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    memset(&SurfaceInterface, 0, sizeof(SurfaceInterface));
}

/**
 * Parse the list of surface attributes and return the proper error code.
 */
EGLint
EGLSurface_t::ParseSurfaceAttribList(const EGLint *attrib_list)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLint type = Type;
    EGLint texture_type = EGL_PBUFFER_BIT;
    EGLint i, err = EGL_SUCCESS;

    if(!attrib_list) {
        return EGL_SUCCESS;
    }

    for(i = 0; attrib_list[i] != EGL_NONE; i++) {
        EGLint attr = attrib_list[i++];
        EGLint val = attrib_list[i];

        switch(attr) {
        /* common attributes */
        case EGL_VG_COLORSPACE:
            switch(val) {
            case EGL_VG_COLORSPACE_sRGB:
            case EGL_VG_COLORSPACE_LINEAR:
                break;
            default:
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            if(err != EGL_SUCCESS) {
                break;
            }
            VGColorspace = val;
            break;
        case EGL_VG_ALPHA_FORMAT:
            switch(val) {
            case EGL_VG_ALPHA_FORMAT_NONPRE:
            case EGL_VG_ALPHA_FORMAT_PRE:
                break;
            default:
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            if(err != EGL_SUCCESS) {
                break;
            }
            VGAlphaFormat = val;
            break;
        /* window surface attributes */
        case EGL_RENDER_BUFFER:
            if(type != EGL_WINDOW_BIT) {
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            if(val != EGL_BACK_BUFFER && val != EGL_SINGLE_BUFFER) {
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            RenderBuffer = val;
            break;
        /* pbuffer surface attributes */
        case EGL_WIDTH:
            if(type != EGL_PBUFFER_BIT) {
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            if(val < 0) {
                err = EGL_BAD_PARAMETER;
                break;
            }
            Width = val;
            break;
        case EGL_HEIGHT:
            if(type != EGL_PBUFFER_BIT) {
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            if(val < 0) {
                err = EGL_BAD_PARAMETER;
                break;
            }
            Height = val;
            break;
        case EGL_LARGEST_PBUFFER:
            if(type != EGL_PBUFFER_BIT) {
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            LargestPbuffer = !!val;
            break;
        /* for eglBindTexImage */
        case EGL_TEXTURE_FORMAT:
            if(!(type & texture_type)) {
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            switch(val) {
            case EGL_TEXTURE_RGB:
            case EGL_TEXTURE_RGBA:
            case EGL_NO_TEXTURE:
                break;
            default:
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            if(err != EGL_SUCCESS) {
                break;
            }
            TextureFormat = val;
            break;
        case EGL_TEXTURE_TARGET:
            if(!(type & texture_type)) {
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            switch(val) {
            case EGL_TEXTURE_2D:
            case EGL_NO_TEXTURE:
                break;
            default:
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            if(err != EGL_SUCCESS) {
                break;
            }
            TextureTarget = val;
            break;
        case EGL_MIPMAP_TEXTURE:
            if(!(type & texture_type)) {
                err = EGL_BAD_ATTRIBUTE;
                break;
            }
            MipmapTexture = !!val;
            break;
        /* no pixmap surface specific attributes */
        default:
            err = EGL_BAD_ATTRIBUTE;
            break;
        }

        if(err != EGL_SUCCESS) {
            // bad surface attribute
            break;
        }
    }

    return err;
}

/**
 * Do error check on parameters and initialize the given EGLSurface_t object.
 * \return EGL_TRUE if no errors, EGL_FALSE otherwise.
 */
EGLBoolean
EGLSurface_t::InitSurface(EGLint type, EGLConfig_t *conf, const EGLint *attrib_list)
{
    FUN_ENTRY(DEBUG_DEPTH);

    EGLint renderBuffer = EGL_BACK_BUFFER;
    EGLint swapBehavior = EGL_BUFFER_PRESERVED;
    EGLint err;

    switch(type) {
    case EGL_WINDOW_BIT:
        swapBehavior = EGL_BUFFER_DESTROYED;
        break;
    case EGL_PIXMAP_BIT:
        renderBuffer = EGL_SINGLE_BUFFER;
        break;
    case EGL_PBUFFER_BIT:
        renderBuffer = EGL_SINGLE_BUFFER;
        break;
    default:
        return EGL_FALSE;
    }

    if(conf && conf->SurfaceType && (conf->SurfaceType & type) == 0) {
        /* The config can't be used to create a surface of this type */
        return EGL_FALSE;
    }

    Type        = type;
    Config      = conf;

    Width       = 0;
    Height      = 0;
    RedSize     = GetConfigKey(conf, EGL_RED_SIZE);
    BlueSize    = GetConfigKey(conf, EGL_BLUE_SIZE);
    GreenSize   = GetConfigKey(conf, EGL_GREEN_SIZE);
    AlphaSize   = GetConfigKey(conf, EGL_ALPHA_SIZE);
    DepthSize   = GetConfigKey(conf, EGL_DEPTH_SIZE);
    StencilSize = GetConfigKey(conf, EGL_STENCIL_SIZE);

    TextureFormat = EGL_NO_TEXTURE;
    TextureTarget = EGL_NO_TEXTURE;
    MipmapTexture = EGL_FALSE;
    LargestPbuffer= EGL_FALSE;
    RenderBuffer  = renderBuffer;
    VGAlphaFormat = EGL_VG_ALPHA_FORMAT_NONPRE;
    VGColorspace  = EGL_VG_COLORSPACE_sRGB;

    MipmapLevel        = 0;
    MultisampleResolve = EGL_MULTISAMPLE_RESOLVE_DEFAULT;
    SwapBehavior       = swapBehavior;

    HorizontalResolution = EGL_UNKNOWN;
    VerticalResolution   = EGL_UNKNOWN;
    AspectRatio          = EGL_UNKNOWN;

    PostSubBufferSupportedNV = EGL_FALSE;

    err = ParseSurfaceAttribList(attrib_list);
    if(err != EGL_SUCCESS) {
        return EGL_FALSE;
    }

    return EGL_TRUE;
}

EGLBoolean
EGLSurface_t::QuerySurface(EGLint attribute, EGLint *value)
{
    FUN_ENTRY(DEBUG_DEPTH);

    switch(attribute) {
    case EGL_WIDTH:
        *value = Width;
        break;
    case EGL_HEIGHT:
        *value = Height;
        break;
    case EGL_CONFIG_ID:
        *value = Config->ConfigID;
        break;
    case EGL_LARGEST_PBUFFER:
        *value = LargestPbuffer;
        break;
    case EGL_TEXTURE_FORMAT:
        /* texture attributes: only for pbuffers, no error otherwise */
        if(Type == EGL_PBUFFER_BIT) {
            *value = TextureFormat;
        }
        break;
    case EGL_TEXTURE_TARGET:
        if(Type == EGL_PBUFFER_BIT) {
            *value = TextureTarget;
        }
        break;
    case EGL_MIPMAP_TEXTURE:
        if(Type == EGL_PBUFFER_BIT) {
            *value = MipmapTexture;
        }
        break;
    case EGL_MIPMAP_LEVEL:
        if(Type == EGL_PBUFFER_BIT) {
            *value = MipmapLevel;
        }
        break;
    case EGL_SWAP_BEHAVIOR:
        *value = SwapBehavior;
        break;
    case EGL_RENDER_BUFFER:
        *value = RenderBuffer;
        break;
    case EGL_PIXEL_ASPECT_RATIO:
        *value = AspectRatio;
        break;
    case EGL_HORIZONTAL_RESOLUTION:
        *value = HorizontalResolution;
        break;
    case EGL_VERTICAL_RESOLUTION:
        *value = VerticalResolution;
        break;
    case EGL_MULTISAMPLE_RESOLVE:
        *value = MultisampleResolve;
        break;
    case EGL_VG_ALPHA_FORMAT:
        *value = VGAlphaFormat;
        break;
    case EGL_VG_COLORSPACE:
        *value = VGColorspace;
        break;
    default:
        return EGL_FALSE;
    }

    return EGL_TRUE;
}
