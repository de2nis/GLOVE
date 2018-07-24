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
 *  @file       stateRasterization.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Rasterization
 *
 */

#ifndef __STATERASTERIZATION_H__
#define __STATERASTERIZATION_H__

#include "GLES2/gl2.h"
#include "utils/glLogger.h"

class StateRasterization {

private:

      GLenum                 mCullFace;
      GLboolean              mCullEnabled;

      GLenum                 mFrontFace;

      GLfloat                mLineWidth;

      GLboolean              mDepthClampEnabled;        // glEnable(GL_DEPTH_CLAMP)        - OpenGL ES 3.0
      GLboolean              mRasterizerDiscardEnabled; // glEnable(GL_RASTERIZER_DISCARD) - OpenGL ES 3.0

      GLfloat                mPolygonOffsetFactor;
      GLfloat                mPolygonOffsetUnits;
      GLboolean              mPolygonOffsetFillEnabled;
      const
      GLfloat                mPolygonOffsetClamp;       // requires glPolygonOffsetClampEXT() from 'EXT_polygon_offset_clamp' extension

public:
      StateRasterization();
      ~StateRasterization();

// Get Functions
      inline GLenum           GetCullFace(void)                           const   { FUN_ENTRY(GL_LOG_TRACE); return mCullFace;                  }
      inline GLboolean        GetCullEnabled(void)                        const   { FUN_ENTRY(GL_LOG_TRACE); return mCullEnabled;               }
      inline GLenum           GetFrontFace(void)                          const   { FUN_ENTRY(GL_LOG_TRACE); return mFrontFace;                 }
      inline GLfloat          GetLineWidth(void)                          const   { FUN_ENTRY(GL_LOG_TRACE); return mLineWidth;                 }
      inline GLfloat          GetPolygonOffsetFactor(void)                const   { FUN_ENTRY(GL_LOG_TRACE); return mPolygonOffsetFactor;       }
      inline GLfloat          GetPolygonOffsetUnits(void)                 const   { FUN_ENTRY(GL_LOG_TRACE); return mPolygonOffsetUnits;        }
      inline GLfloat          GetPolygonOffsetClamp(void)                 const   { FUN_ENTRY(GL_LOG_TRACE); return mPolygonOffsetClamp;        }
      inline GLboolean        GetPolygonOffsetFillEnabled(void)           const   { FUN_ENTRY(GL_LOG_TRACE); return mPolygonOffsetFillEnabled;  }
      inline GLboolean        GetDepthClampEnabled(void)                  const   { FUN_ENTRY(GL_LOG_TRACE); return mDepthClampEnabled;         }
      inline GLboolean        GetRasterizerDiscardEnabled(void)           const   { FUN_ENTRY(GL_LOG_TRACE); return mRasterizerDiscardEnabled;  }

// Set Functions
      inline void             SetCullFace(GLenum cullFace)                        { FUN_ENTRY(GL_LOG_TRACE); mCullFace                 = cullFace;    }
      inline void             SetCullEnabled(GLboolean cullEnabled)               { FUN_ENTRY(GL_LOG_TRACE); mCullEnabled              = cullEnabled; }
      inline void             SetFrontFace(GLenum frontFace)                      { FUN_ENTRY(GL_LOG_TRACE); mFrontFace                = frontFace;   }
      inline void             SetLineWidth(GLfloat lineWidth)                     { FUN_ENTRY(GL_LOG_TRACE); mLineWidth                = lineWidth;   }
      inline void             SetPolygonOffsetFactor(GLfloat poFactor)            { FUN_ENTRY(GL_LOG_TRACE); mPolygonOffsetFactor      = poFactor;    }
      inline void             SetPolygonOffsetUnits(GLfloat poUnits)              { FUN_ENTRY(GL_LOG_TRACE); mPolygonOffsetUnits       = poUnits;     }
      inline void             SetPolygonOffsetFillEnabled(GLboolean poEnabled)    { FUN_ENTRY(GL_LOG_TRACE); mPolygonOffsetFillEnabled = poEnabled;   }

// Update Function
      inline bool             UpdateCullEnabled(GLboolean enable)                 { FUN_ENTRY(GL_LOG_TRACE); bool res = (mCullEnabled != enable);
                                                                                                              mCullEnabled = enable;
                                                                                                              return res; }
      inline bool             UpdateCullFace(GLenum cullFace)                     { FUN_ENTRY(GL_LOG_TRACE); bool res = (mCullFace != cullFace);
                                                                                                              mCullFace = cullFace;
                                                                                                              return res; }
      inline bool             UpdateFrontFace(GLenum frontFace)                   { FUN_ENTRY(GL_LOG_TRACE); bool res = (mFrontFace != frontFace);
                                                                                                              mFrontFace = frontFace;
                                                                                                              return res; }
      inline bool             UpdatePolygonOffsetFactor(GLfloat poFactor)         { FUN_ENTRY(GL_LOG_TRACE); bool res = (mPolygonOffsetFactor != poFactor);
                                                                                                              mPolygonOffsetFactor = poFactor;
                                                                                                              return res; }
      inline bool             UpdatePolygonOffsetUnits(GLfloat poUnits)           { FUN_ENTRY(GL_LOG_TRACE); bool res = (mPolygonOffsetUnits != poUnits);
                                                                                                              mPolygonOffsetUnits = poUnits;
                                                                                                              return res; }
      inline bool             UpdatePolygonOffsetFillEnabled(GLboolean poEnabled) { FUN_ENTRY(GL_LOG_TRACE); bool res = (mPolygonOffsetFillEnabled != poEnabled);
                                                                                                              mPolygonOffsetFillEnabled = poEnabled;
                                                                                                              return res; }
};

#endif //__STATERASTERIZATION_H__
