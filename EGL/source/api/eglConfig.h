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
 *  @file       eglConfig.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      EGL Configuration handler module
 *
 */

#ifndef __EGL_CONFIG_H__
#define __EGL_CONFIG_H__

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "utils/egl_defs.h"
#include "utils/eglLogger.h"

#ifdef DEBUG_DEPTH
#   undef DEBUG_DEPTH
#endif // DEBUG_DEPTH
#define DEBUG_DEPTH         EGL_LOG_DEBUG

typedef struct
{
    EGLDisplay *Display;

    EGLint BufferSize;
    EGLint AlphaSize;
    EGLint BlueSize;
    EGLint GreenSize;
    EGLint RedSize;
    EGLint DepthSize;
    EGLint StencilSize;
    EGLint ConfigCaveat;
    EGLint ConfigID;
    EGLint Level;
    EGLint MaxPbufferHeight;
    EGLint MaxPbufferPixels;
    EGLint MaxPbufferWidth;
    EGLint NativeRenderable;
    EGLint NativeVisualID;
    EGLint NativeVisualType;
    EGLint Samples;
    EGLint SampleBuffers;
    EGLint SurfaceType;
    EGLint TransparentType;
    EGLint TransparentBlueValue;
    EGLint TransparentGreenValue;
    EGLint TransparentRedValue;
    EGLint BindToTextureRGB;
    EGLint BindToTextureRGBA;
    EGLint MinSwapInterval;
    EGLint MaxSwapInterval;
    EGLint LuminanceSize;
    EGLint AlphaMaskSize;
    EGLint ColorBufferType;
    EGLint RenderableType;
    EGLint MatchNativePixmap;
    EGLint Conformant;
    EGLint RecordableAndroid;
    EGLint FramebufferTargetAndroid;
} EGLConfig_t;

/**
 * Map an EGL attribute enum to the offset of the member in EGLConfig_t.
 */
static EGLint
OffsetOfConfig(EGLint attr)
{
    FUN_ENTRY(DEBUG_DEPTH);

    switch(attr) {
#define ATTRIB_MAP(attr, memb) case attr: return offsetof(EGLConfig_t, memb)
        /* core */
        ATTRIB_MAP(EGL_BUFFER_SIZE,                   BufferSize);
        ATTRIB_MAP(EGL_ALPHA_SIZE,                    AlphaSize);
        ATTRIB_MAP(EGL_BLUE_SIZE,                     BlueSize);
        ATTRIB_MAP(EGL_GREEN_SIZE,                    GreenSize);
        ATTRIB_MAP(EGL_RED_SIZE,                      RedSize);
        ATTRIB_MAP(EGL_DEPTH_SIZE,                    DepthSize);
        ATTRIB_MAP(EGL_STENCIL_SIZE,                  StencilSize);
        ATTRIB_MAP(EGL_CONFIG_CAVEAT,                 ConfigCaveat);
        ATTRIB_MAP(EGL_CONFIG_ID,                     ConfigID);
        ATTRIB_MAP(EGL_LEVEL,                         Level);
        ATTRIB_MAP(EGL_MAX_PBUFFER_HEIGHT,            MaxPbufferHeight);
        ATTRIB_MAP(EGL_MAX_PBUFFER_PIXELS,            MaxPbufferPixels);
        ATTRIB_MAP(EGL_MAX_PBUFFER_WIDTH,             MaxPbufferWidth);
        ATTRIB_MAP(EGL_NATIVE_RENDERABLE,             NativeRenderable);
        ATTRIB_MAP(EGL_NATIVE_VISUAL_ID,              NativeVisualID);
        ATTRIB_MAP(EGL_NATIVE_VISUAL_TYPE,            NativeVisualType);
        ATTRIB_MAP(EGL_SAMPLES,                       Samples);
        ATTRIB_MAP(EGL_SAMPLE_BUFFERS,                SampleBuffers);
        ATTRIB_MAP(EGL_SURFACE_TYPE,                  SurfaceType);
        ATTRIB_MAP(EGL_TRANSPARENT_TYPE,              TransparentType);
        ATTRIB_MAP(EGL_TRANSPARENT_BLUE_VALUE,        TransparentBlueValue);
        ATTRIB_MAP(EGL_TRANSPARENT_GREEN_VALUE,       TransparentGreenValue);
        ATTRIB_MAP(EGL_TRANSPARENT_RED_VALUE,         TransparentRedValue);
        ATTRIB_MAP(EGL_BIND_TO_TEXTURE_RGB,           BindToTextureRGB);
        ATTRIB_MAP(EGL_BIND_TO_TEXTURE_RGBA,          BindToTextureRGBA);
        ATTRIB_MAP(EGL_MIN_SWAP_INTERVAL,             MinSwapInterval);
        ATTRIB_MAP(EGL_MAX_SWAP_INTERVAL,             MaxSwapInterval);
        ATTRIB_MAP(EGL_LUMINANCE_SIZE,                LuminanceSize);
        ATTRIB_MAP(EGL_ALPHA_MASK_SIZE,               AlphaMaskSize);
        ATTRIB_MAP(EGL_COLOR_BUFFER_TYPE,             ColorBufferType);
        ATTRIB_MAP(EGL_RENDERABLE_TYPE,               RenderableType);
        ATTRIB_MAP(EGL_MATCH_NATIVE_PIXMAP,           MatchNativePixmap);
        ATTRIB_MAP(EGL_CONFORMANT,                    Conformant);
        ATTRIB_MAP(EGL_RECORDABLE_ANDROID,            RecordableAndroid);
        ATTRIB_MAP(EGL_FRAMEBUFFER_TARGET_ANDROID,    FramebufferTargetAndroid);
#undef ATTRIB_MAP
    default:
        return -1;
    }
}

/**
 * Return the value for a given key.
 */
static EGLint
GetConfigKey(const EGLConfig_t *conf, EGLint key)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    EGLint offset = OffsetOfConfig(key);
    assert(offset >= 0);
    return *((EGLint *) ((char *) conf + offset));
}

/**
 * Update a config for a given key.
 *
 * Note that a valid key is not necessarily a valid attribute.  There are gaps
 * in the attribute enums.  The separation is to catch application errors.
 * Drivers should never set a key that is an invalid attribute.
 */
static void
SetConfigKey(EGLConfig_t *conf, EGLint key, EGLint val)
{
    FUN_ENTRY(EGL_LOG_TRACE);

    EGLint offset = OffsetOfConfig(key);
    assert(offset >= 0);
    *((EGLint *) ((char *) conf + offset)) = val;
}

void InitConfig(EGLConfig_t *conf, EGLDisplay *dpy, EGLint id);

/**
 * Return true if a config is valid.  When for_matching is true,
 * EGL_DONT_CARE is accepted as a valid attribute value, and checks
 * for conflicting attribute values are skipped.
 *
 * Note that some attributes are platform-dependent and are not
 * checked.
 */
EGLBoolean ValidateConfig(const EGLConfig_t *conf, EGLBoolean for_matching);

/**
 * Initialize a criteria config from the given attribute list.
 * Return EGL_FALSE if any of the attribute is invalid.
 */
EGLBoolean ParseConfigAttribList(EGLConfig_t *conf, EGLDisplay *dpy, const EGLint *attrib_list);

/**
 * Return true if a config matches the criteria.  This and
 * ParseConfigAttribList together implement the algorithm
 * described in "Selection of EGLConfigs".
 *
 * Note that attributes that are special (currently, only
 * EGL_MATCH_NATIVE_PIXMAP) are ignored.
 */
EGLBoolean MatchConfig(const EGLConfig_t *conf, const EGLConfig_t *criteria);

/**
 * Decide the ordering of conf1 and conf2, under the given criteria.
 * When compare_id is true, this implements the algorithm described
 * in "Sorting of EGLConfigs".  When compare_id is false,
 * EGL_CONFIG_ID is not compared.
 *
 * It returns a negative integer if conf1 is considered to come
 * before conf2; a positive integer if conf2 is considered to come
 * before conf1; zero if the ordering cannot be decided.
 *
 * Note that EGL_NATIVE_VISUAL_TYPE is platform-dependent and is
 * ignored here.
 */
EGLint CompareConfigs(const EGLConfig_t *conf1, const EGLConfig_t *conf2, const EGLConfig_t *criteria, EGLBoolean compare_id);

/**
 * A helper function for implementing eglChooseConfig.  See SortConfigs
 * for the meanings of match and compare.
 */
//FilterConfigArray(EGLConfig_t *configs, EGLint config_size, EGLint *num_configs,
//                  EGLBoolean (*match)(const EGLConfig_t *, void *),
//                  EGLint (*compare)(const EGLConfig_t *, const EGLConfig_t *, void *),
//                  void *filter_data);
EGLBoolean FilterConfigArray(EGLConfig_t **configs, EGLint config_size, EGLint *num_config, const EGLConfig_t *criteria);

#ifdef VK_USE_PLATFORM_ANDROID_KHR
extern const EGLConfig_t EglConfigs[4];
#else
extern const EGLConfig_t EglConfigs[2];
#endif

#endif // __EGL_CONFIG_H__
