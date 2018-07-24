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
 *  @file       stateViewportTransformation.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Viewport Transformation
 *
 */

#ifndef __STATEVIEWPORTTRANSFORMATION_H__
#define __STATEVIEWPORTTRANSFORMATION_H__

#include "GLES2/gl2.h"
#include "resources/rect.h"
#include "utils/glLogger.h"
#include "utils/color.hpp"

class StateViewportTransformation {

private:
      GLfloat                 mMinDepthRange;
      GLfloat                 mMaxDepthRange;
      Rect                    mViewportRectangle;

      const
      uint32_t                mViewportCount;
      const
      uint32_t                mScissorCount;

public:
      StateViewportTransformation();
      ~StateViewportTransformation();

// Get Functions
      inline void             GetViewportRect(GLboolean *params)         const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = mViewportRectangle.x      ? GL_TRUE : GL_FALSE;
                                                                                                        params[1] = mViewportRectangle.y      ? GL_TRUE : GL_FALSE;
                                                                                                        params[2] = mViewportRectangle.width  ? GL_TRUE : GL_FALSE;
                                                                                                        params[3] = mViewportRectangle.height ? GL_TRUE : GL_FALSE; }
      inline void             GetViewportRect(GLint *params)             const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = mViewportRectangle.x;
                                                                                                        params[1] = mViewportRectangle.y;
                                                                                                        params[2] = mViewportRectangle.width;
                                                                                                        params[3] = mViewportRectangle.height; }
      inline void             GetViewportRect(GLfloat *params)           const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = static_cast<GLfloat>(mViewportRectangle.x);
                                                                                                        params[1] = static_cast<GLfloat>(mViewportRectangle.y);
                                                                                                        params[2] = static_cast<GLfloat>(mViewportRectangle.width);
                                                                                                        params[3] = static_cast<GLfloat>(mViewportRectangle.height); }
      inline int              GetViewportRectX(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mViewportRectangle.x; }
      inline int              GetViewportRectY(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mViewportRectangle.y; }
      inline int              GetViewportRectWidth(void)                 const   { FUN_ENTRY(GL_LOG_TRACE); return mViewportRectangle.width; }
      inline int              GetViewportRectHeight(void)                const   { FUN_ENTRY(GL_LOG_TRACE); return mViewportRectangle.height; }
      inline GLfloat          GetMinDepthRange(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mMinDepthRange; }
      inline GLfloat          GetMaxDepthRange(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mMaxDepthRange; }
      inline GLfloat          GetDiffDepthRange(void)                    const   { FUN_ENTRY(GL_LOG_TRACE); return mMaxDepthRange - mMinDepthRange; }
      inline uint32_t         GetViewportCount(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mViewportCount; }
      inline uint32_t         GetScissorCount(void)                      const   { FUN_ENTRY(GL_LOG_TRACE); return mScissorCount; }

// Set Functions
      inline void             SetViewportRect(Rect *fbo)                         { FUN_ENTRY(GL_LOG_TRACE); mViewportRectangle.x = fbo->x;
                                                                                                        mViewportRectangle.y = fbo->y;
                                                                                                        mViewportRectangle.width = fbo->width;
                                                                                                        mViewportRectangle.height = fbo->height; }
      inline void             SetViewportRectX(GLint x)                          { FUN_ENTRY(GL_LOG_TRACE); mViewportRectangle.x = x; }
      inline void             SetViewportRectY(GLint y)                          { FUN_ENTRY(GL_LOG_TRACE); mViewportRectangle.y = y; }
      inline void             SetViewportRectWidth(GLint width)                  { FUN_ENTRY(GL_LOG_TRACE); mViewportRectangle.width = width; }
      inline void             SetViewportRectHeight(GLint height)                { FUN_ENTRY(GL_LOG_TRACE); mViewportRectangle.height = height; }
      inline void             SetMinDepthRange(GLfloat min_depth)                { FUN_ENTRY(GL_LOG_TRACE); mMinDepthRange = CLAMP_F(min_depth); }
      inline void             SetMaxDepthRange(GLfloat max_depth)                { FUN_ENTRY(GL_LOG_TRACE); mMaxDepthRange = CLAMP_F(max_depth); }

// Update Functions
      inline bool             UpdateDepthRange(GLfloat min_depth,
                                               GLfloat max_depth)                { FUN_ENTRY(GL_LOG_TRACE); GLfloat min_depth_f = CLAMP_F(min_depth);
                                                                                                        GLfloat max_depth_f = CLAMP_F(max_depth);
                                                                                                        bool res = (mMinDepthRange != min_depth_f) || (mMaxDepthRange != max_depth_f);
                                                                                                        mMinDepthRange = min_depth_f;
                                                                                                        mMaxDepthRange = max_depth_f;
                                                                                                        return res; }

      inline bool             UpdateViewportRect(GLint x,
                                                 GLint y,
                                                 GLint width,
                                                 GLint height)                   { FUN_ENTRY(GL_LOG_TRACE); bool res = (mViewportRectangle.x != x) || (mViewportRectangle.y != y) || (mViewportRectangle.width != width) || (mViewportRectangle.height != height);
                                                                                                        mViewportRectangle.x      = x;
                                                                                                        mViewportRectangle.y      = y;
                                                                                                        mViewportRectangle.width  = width;
                                                                                                        mViewportRectangle.height = height;
                                                                                                        return res; }
};

#endif // __STATEVIEWPORTTRANSFORMATION_H__
