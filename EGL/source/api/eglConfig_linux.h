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
 *  @file       eglConfig_linux.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Supported EGL Configurations for Linux Platform
 *
 */

#if EGL_SUPPORT_ONLY_PBUFFER_SURFACE == 0
#   define EGL_AVAILABLE_SURFACES (EGL_PBUFFER_BIT | EGL_WINDOW_BIT)
#else
#   define EGL_AVAILABLE_SURFACES (EGL_PBUFFER_BIT)
#endif

const EGLConfig_t EglConfigs[2] = {
                                   { 0,   // Display
                                    32,   // BufferSize
                                     8,   // AlphaSize
                                     8,   // BlueSize
                                     8,   // GreenSize
                                     8,   // RedSize
                                    24,   // DepthSize
                                     8,   // StencilSize
                              EGL_NONE,   // ConfigCaveat
                                     1,   // ConfigID
                                     0,   // Level
                                  1080,   // MaxPbufferHeight
                           1920 * 1080,   // MaxPbufferPixels
                                  1920,   // MaxPbufferWidth
                             EGL_FALSE,   // NativeRenderable
                                  0x21,   // NativeVisualID
                              EGL_NONE,   // NativeVisualType
                                     0,   // Samples
                                     0,   // SampleBuffers
                EGL_AVAILABLE_SURFACES,   // SurfaceType
                              EGL_NONE,   // TransparentType
                                     0,   // TransparentBlueValue
                                     0,   // TransparentGreenValue
                                     0,   // TransparentRedValue
                             EGL_FALSE,   // BindToTextureRGB
                              EGL_TRUE,   // BindToTextureRGBA
                                     0,   // MinSwapInterval
                                     0,   // MaxSwapInterval
                                     0,   // LuminanceSize
                                     0,   // AlphaMaskSize
                        EGL_RGB_BUFFER,   // ColorBufferType
                    EGL_OPENGL_ES2_BIT,   // RenderableType
                              EGL_NONE,   // MatchNativePixmap
                                   0x4,   // Conformant
                             EGL_FALSE,   // RecordableAndroid
                             EGL_FALSE},  // FramebufferTargetAndroid

                                   { 0,   // Display
                                    24,   // BufferSize
                                     0,   // AlphaSize
                                     8,   // BlueSize
                                     8,   // GreenSize
                                     8,   // RedSize
                                    24,   // DepthSize
                                     8,   // StencilSize
                              EGL_NONE,   // ConfigCaveat
                                     2,   // ConfigID
                                     0,   // Level
                                  1080,   // MaxPbufferHeight
                           1920 * 1080,   // MaxPbufferPixels
                                  1920,   // MaxPbufferWidth
                             EGL_FALSE,   // NativeRenderable
                                  0x21,   // NativeVisualID
                              EGL_NONE,   // NativeVisualType
                                     0,   // Samples
                                     0,   // SampleBuffers
                EGL_AVAILABLE_SURFACES,   // SurfaceType
                              EGL_NONE,   // TransparentType
                                     0,   // TransparentBlueValue
                                     0,   // TransparentGreenValue
                                     0,   // TransparentRedValue
                             EGL_FALSE,   // BindToTextureRGB
                              EGL_TRUE,   // BindToTextureRGBA
                                     0,   // MinSwapInterval
                                     0,   // MaxSwapInterval
                                     0,   // LuminanceSize
                                     0,   // AlphaMaskSize
                        EGL_RGB_BUFFER,   // ColorBufferType
                    EGL_OPENGL_ES2_BIT,   // RenderableType
                              EGL_NONE,   // MatchNativePixmap
                                   0x4,   // Conformant
                             EGL_FALSE,   // RecordableAndroid
                             EGL_FALSE},  // FramebufferTargetAndroid                                   
};
