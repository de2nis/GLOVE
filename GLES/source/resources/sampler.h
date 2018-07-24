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
 *  @file       sampler.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      (Texture) Sampler Functionality in GLOVE
 *
 */

#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "GLES2/gl2.h"
#include "utils/glLogger.h"

class Sampler {
private:
    GLenum                  mMinFilter;
    GLenum                  mMagFilter;
    GLenum                  mWrapS;
    GLenum                  mWrapT;

public:
    Sampler();
    ~Sampler();

// Get Functions
    inline GLenum           GetMinFilter(void)                          const   { FUN_ENTRY(GL_LOG_TRACE); return mMinFilter; }
    inline GLenum           GetMagFilter(void)                          const   { FUN_ENTRY(GL_LOG_TRACE); return mMagFilter; }
    inline GLenum           GetWrapS(void)                              const   { FUN_ENTRY(GL_LOG_TRACE); return mWrapS; }
    inline GLenum           GetWrapT(void)                              const   { FUN_ENTRY(GL_LOG_TRACE); return mWrapT; }

// Update Functions
    inline bool             UpdateMinFilter(GLenum mode)                        { FUN_ENTRY(GL_LOG_TRACE); bool res = mMinFilter != mode;
                                                                                                       mMinFilter = mode;
                                                                                                       return res; }
    inline bool             UpdateMagFilter(GLenum mode)                        { FUN_ENTRY(GL_LOG_TRACE); bool res = mMagFilter != mode;
                                                                                                       mMagFilter = mode;
                                                                                                       return res; }
    inline bool             UpdateWrapS(GLenum mode)                            { FUN_ENTRY(GL_LOG_TRACE); bool res = mWrapS != mode;
                                                                                                       mWrapS = mode;
                                                                                                       return res; }
    inline bool             UpdateWrapT(GLenum mode)                            { FUN_ENTRY(GL_LOG_TRACE); bool res = mWrapT != mode;
                                                                                                       mWrapT = mode;
                                                                                                       return res; }
};

#endif // __SAMPLER_H__
