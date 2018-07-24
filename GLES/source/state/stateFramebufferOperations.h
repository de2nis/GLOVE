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
 *  @file       stateFramebufferOperations.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Framebuffer Operations
 *
 */

#ifndef __STATEFRAMEBUFFEROPERATIONS_H__
#define __STATEFRAMEBUFFEROPERATIONS_H__

#include "GLES2/gl2.h"
#include "utils/glLogger.h"
#include "utils/color.hpp"

#include <cmath>

class StateFramebufferOperations {

private:
      GLfloat             mClearColor[4];
      GLfloat             mClearDepth;
      GLint               mClearStencil;

      GLchar              mColorMask;
      GLboolean           mDepthMask;
      GLuint              mStencilMaskFront;
      GLuint              mStencilMaskBack;

public:
      StateFramebufferOperations();
      ~StateFramebufferOperations();

// Get Functions
      inline void             GetClearColor(GLboolean *params)          const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = mClearColor[0] == 0.0f ? GL_FALSE : GL_TRUE;
                                                                                                       params[1] = mClearColor[1] == 0.0f ? GL_FALSE : GL_TRUE;
                                                                                                       params[2] = mClearColor[2] == 0.0f ? GL_FALSE : GL_TRUE;
                                                                                                       params[3] = mClearColor[3] == 0.0f ? GL_FALSE : GL_TRUE;}
      inline void             GetClearColor(GLint *params)              const   { FUN_ENTRY(GL_LOG_TRACE); 
                                                                                                       params[0] = mClearColor[0] == 1.0f ? 0x7fffffff : static_cast<GLint>(mClearColor[0] * 0x7fffffff);
                                                                                                       params[1] = mClearColor[1] == 1.0f ? 0x7fffffff : static_cast<GLint>(mClearColor[1] * 0x7fffffff);
                                                                                                       params[2] = mClearColor[2] == 1.0f ? 0x7fffffff : static_cast<GLint>(mClearColor[2] * 0x7fffffff);
                                                                                                       params[3] = mClearColor[3] == 1.0f ? 0x7fffffff : static_cast<GLint>(mClearColor[3] * 0x7fffffff);}
      inline void             GetClearColor(GLfloat *params)            const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = mClearColor[0];
                                                                                                       params[1] = mClearColor[1];
                                                                                                       params[2] = mClearColor[2];
                                                                                                       params[3] = mClearColor[3];}
      inline GLfloat          GetClearDepth(void)                       const   { FUN_ENTRY(GL_LOG_TRACE); return mClearDepth; }
      inline GLint            GetClearStencil(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mClearStencil; }

      inline GLboolean        GetDepthMask(void)                        const   { FUN_ENTRY(GL_LOG_TRACE); return mDepthMask; }
      inline GLchar           GetColorMask(void)                        const   { FUN_ENTRY(GL_LOG_TRACE); return mColorMask; }
      inline void             GetColorMask(GLboolean *params)           const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = (mColorMask & 0x8) ? GL_TRUE : GL_FALSE;
                                                                                                       params[1] = (mColorMask & 0x4) ? GL_TRUE : GL_FALSE;
                                                                                                       params[2] = (mColorMask & 0x2) ? GL_TRUE : GL_FALSE;
                                                                                                       params[3] = (mColorMask & 0x1) ? GL_TRUE : GL_FALSE; }
      inline void             GetColorMask(GLint     *params)           const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = (mColorMask & 0x8) ? GL_TRUE : GL_FALSE;
                                                                                                       params[1] = (mColorMask & 0x4) ? GL_TRUE : GL_FALSE;
                                                                                                       params[2] = (mColorMask & 0x2) ? GL_TRUE : GL_FALSE;
                                                                                                       params[3] = (mColorMask & 0x1) ? GL_TRUE : GL_FALSE; }
      inline void             GetColorMask(GLfloat   *params)           const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = (mColorMask & 0x8) ? 1.0f : 0.0f;
                                                                                                       params[1] = (mColorMask & 0x4) ? 1.0f : 0.0f;
                                                                                                       params[2] = (mColorMask & 0x2) ? 1.0f : 0.0f;
                                                                                                       params[3] = (mColorMask & 0x1) ? 1.0f : 0.0f; }

      inline GLuint           GetStencilMaskFront(void)                 const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilMaskFront; }
      inline GLuint           GetStencilMaskBack(void)                  const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilMaskBack;  }

// Set Functions
      inline void             SetClearColor(GLclampf red,
                                            GLclampf green,
                                            GLclampf blue,
                                            GLclampf alpha)                     { FUN_ENTRY(GL_LOG_TRACE); mClearColor[0] = CLAMP_F(red);
                                                                                                       mClearColor[1] = CLAMP_F(green);
                                                                                                       mClearColor[2] = CLAMP_F(blue);
                                                                                                       mClearColor[3] = CLAMP_F(alpha);
                                                                                                     }
      inline void             SetClearDepth(GLfloat depth)                      { FUN_ENTRY(GL_LOG_TRACE); mClearDepth   = CLAMP_F(depth); }
      inline void             SetClearStencil(GLint stencil)                    { FUN_ENTRY(GL_LOG_TRACE); mClearStencil = stencil; }
      inline void             SetColorMask(GLchar mask)                         { FUN_ENTRY(GL_LOG_TRACE); mColorMask    = mask; }

// Update Functions
      inline bool             UpdateClearColor(GLclampf red,
                                               GLclampf green,
                                               GLclampf blue,
                                               GLclampf alpha)                  { FUN_ENTRY(GL_LOG_TRACE); GLfloat r = CLAMP_F(red);
                                                                                                       GLfloat g = CLAMP_F(green);
                                                                                                       GLfloat b = CLAMP_F(blue);
                                                                                                       GLfloat a = CLAMP_F(alpha);
                                                                                                       bool res = (mClearColor[0] != r) || (mClearColor[1] != g) || (mClearColor[2] != b) || (mClearColor[3] != a);
                                                                                                       mClearColor[0] = r; mClearColor[1] = g; mClearColor[2] = b; mClearColor[3] = a;
                                                                                                       return res; }

      inline bool             UpdateClearDepth(GLfloat depth)                   { FUN_ENTRY(GL_LOG_TRACE); float d = CLAMP_F(depth);
                                                                                                       bool res = (mClearDepth != d);
                                                                                                       mClearDepth = d;
                                                                                                       return res; }
      inline bool             UpdateClearStencil(GLint stencil)                 { FUN_ENTRY(GL_LOG_TRACE); bool res = (mClearStencil != stencil);
                                                                                                       mClearStencil = stencil;
                                                                                                       return res; }
      inline bool             UpdateColorMask(GLboolean red,
                                              GLboolean green,
                                              GLboolean blue,
                                              GLboolean alpha)                  { FUN_ENTRY(GL_LOG_TRACE); GLchar mask = (!!(red) << 3) | (!!(green) << 2) | (!!(blue) << 1) | (!!(alpha));
                                                                                                       bool res = (mColorMask != mask);
                                                                                                       mColorMask = mask;
                                                                                                       return res; }
      inline bool             UpdateDepthMask(GLenum enable)                    { FUN_ENTRY(GL_LOG_TRACE); bool res = (mDepthMask != enable);
                                                                                                       mDepthMask = enable;
                                                                                                       return res; }
      inline bool             UpdateStencilMask(GLuint mask)                    { FUN_ENTRY(GL_LOG_TRACE); bool res = (mStencilMaskFront != mask) || (mStencilMaskBack != mask);
                                                                                                       mStencilMaskFront = mask;
                                                                                                       mStencilMaskBack = mask;
                                                                                                       return res; }
      inline bool             UpdateStencilMaskFront(GLuint mask)               { FUN_ENTRY(GL_LOG_TRACE); bool res = (mStencilMaskFront != mask);
                                                                                                       mStencilMaskFront = mask;
                                                                                                       return res; }
      inline bool             UpdateStencilMaskBack(GLuint mask)                { FUN_ENTRY(GL_LOG_TRACE); bool res = (mStencilMaskBack  != mask);
                                                                                                       mStencilMaskBack = mask;
                                                                                                       return res; }
};

#endif //__STATEFRAMEBUFFEROPERATIONS_H__
