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
 *  @file       stateFragmentOperations.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Fragment Operations
 *
 */

#ifndef __STATEFRAGMENTOPERATIONS_H__
#define __STATEFRAGMENTOPERATIONS_H__

#include "GLES/gl.h"
#include "GLES2/gl2.h"
#include "resources/rect.h"
#include "utils/glLogger.h"
#include "utils/color.hpp"

class StateFragmentOperations {

private:
      // 1. Scissor Test
      GLboolean               mScissorTestEnabled;
      Rect                    mScissorTestRectangle;

      // 2. Depth Bounds (not in core OpenGL ES) - requires DepthBoundsEXT() from 'EXT_depth_bounds_test' extension
      GLboolean               mDepthBoundsTestEnabled;
      GLfloat                 mMinDepthBounds;
      GLfloat                 mMaxDepthBounds;

      // 3. Multisampling
      const
      GLboolean               mMultiSamplingEnabled;
      GLint                   mSampleCoverageBits;
      GLfloat                 mSampleCoverageValue;
      GLboolean               mSampleCoverageInvert;
      GLboolean               mSampleCoverageEnabled;
      GLboolean               mSampleAlphaToCoverageEnabled;
      const
      GLboolean               mSampleAlphaToOneEnabled;
      // -- requires MinSampleShadingOES() from 'OES_sample_shading' extension (OpenGL ES 3.0+)
      const
      GLboolean               mSampleShadingEnabled;
      const
      GLfloat                 mMinSampleShading;

      // 4. Stencil Test
      GLboolean               mStencilTestEnabled;
      GLenum                  mStencilTestFuncCompareFront;
      GLenum                  mStencilTestFuncCompareBack;
      GLint                   mStencilTestFuncRefFront;
      GLint                   mStencilTestFuncRefBack;
      GLuint                  mStencilTestFuncMaskFront;
      GLuint                  mStencilTestFuncMaskBack;

      GLenum                  mStencilTestOpFailFront;
      GLenum                  mStencilTestOpZfailFront;
      GLenum                  mStencilTestOpZpassFront;
      GLenum                  mStencilTestOpFailBack;
      GLenum                  mStencilTestOpZfailBack;
      GLenum                  mStencilTestOpZpassBack;

      // 5. Depth Buffer Test
      GLenum                  mDepthTestFunc;
      GLboolean               mDepthTestEnabled;

      // 6. Blending
      GLboolean               mBlendingEnabled;
      GLfloat                 mBlendingColor[4];
      GLenum                  mBlendingEquationRGB;
      GLenum                  mBlendingEquationAlpha;
      GLenum                  mBlendingFactorSourceRGB;
      GLenum                  mBlendingFactorSourceAlpha;
      GLenum                  mBlendingFactorDestinationRGB;
      GLenum                  mBlendingFactorDestinationAlpha;
      const
      uint32_t                mBlendingColorAttachmentCount;
      const
      GLenum                  mBlendingLogicOp;
      const
      GLboolean               mBlendingLogicOpEnabled;                          // NOTE: glEnable(GL_COLOR_LOGIC_OP) (OpenGL ES 1.1)

      // 7. Dithering
      GLboolean               mDitheringEnabled;

public:
      StateFragmentOperations();
      ~StateFragmentOperations();

// Get Functions
      inline void             GetBlendingColor(GLboolean *params)       const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = mBlendingColor[0] == 0.0f ? GL_FALSE : GL_TRUE;
                                                                                                       params[1] = mBlendingColor[1] == 0.0f ? GL_FALSE : GL_TRUE;
                                                                                                       params[2] = mBlendingColor[2] == 0.0f ? GL_FALSE : GL_TRUE;
                                                                                                       params[3] = mBlendingColor[3] == 0.0f ? GL_FALSE : GL_TRUE;}
      inline void             GetBlendingColor(GLint *params)           const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = static_cast<GLint>(mBlendingColor[0]);
                                                                                                       params[1] = static_cast<GLint>(mBlendingColor[1]);
                                                                                                       params[2] = static_cast<GLint>(mBlendingColor[2]);
                                                                                                       params[3] = static_cast<GLint>(mBlendingColor[3]);}
      inline void             GetBlendingColor(GLfloat *params)         const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = mBlendingColor[0];
                                                                                                       params[1] = mBlendingColor[1];
                                                                                                       params[2] = mBlendingColor[2];
                                                                                                       params[3] = mBlendingColor[3];}
      inline GLenum           GetBlendingLogicOp(void)                  const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingLogicOp;}
      inline GLboolean        GetBlendingLogicOpEnabled(void)           const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingLogicOpEnabled;}
      inline uint32_t         GetBlendingColorAttachmentCount(void)     const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingColorAttachmentCount;}
      inline GLenum           GetBlendingEquationRGB(void)              const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingEquationRGB; }
      inline GLenum           GetBlendingEquationAlpha(void)            const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingEquationAlpha; }

      inline GLenum           GetBlendingFactorSourceRGB(void)          const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingFactorSourceRGB; }
      inline GLenum           GetBlendingFactorSourceAlpha(void)        const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingFactorSourceAlpha; }
      inline GLenum           GetBlendingFactorDestinationRGB(void)     const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingFactorDestinationRGB; }
      inline GLenum           GetBlendingFactorDestinationAlpha(void)   const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingFactorDestinationAlpha; }

      inline GLboolean        GetScissorTestEnabled(void)               const   { FUN_ENTRY(GL_LOG_TRACE); return mScissorTestEnabled; }
      inline void             GetScissorRect(GLboolean *params)         const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = mScissorTestRectangle.x      ? GL_TRUE : GL_FALSE;
                                                                                                       params[1] = mScissorTestRectangle.y      ? GL_TRUE : GL_FALSE;
                                                                                                       params[2] = mScissorTestRectangle.width  ? GL_TRUE : GL_FALSE;
                                                                                                       params[3] = mScissorTestRectangle.height ? GL_TRUE : GL_FALSE;  }
      inline void             GetScissorRect(GLint *params)             const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = mScissorTestRectangle.x;
                                                                                                       params[1] = mScissorTestRectangle.y;
                                                                                                       params[2] = mScissorTestRectangle.width;
                                                                                                       params[3] = mScissorTestRectangle.height; }
      inline void             GetScissorRect(GLfloat *params)           const   { FUN_ENTRY(GL_LOG_TRACE); params[0] = static_cast<GLfloat>(mScissorTestRectangle.x);
                                                                                                       params[1] = static_cast<GLfloat>(mScissorTestRectangle.y);
                                                                                                       params[2] = static_cast<GLfloat>(mScissorTestRectangle.width);
                                                                                                       params[3] = static_cast<GLfloat>(mScissorTestRectangle.height); }
      inline int              GetScissorRectX(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mScissorTestRectangle.x; }
      inline int              GetScissorRectY(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mScissorTestRectangle.y; }
      inline int              GetScissorRectWidth(void)                 const   { FUN_ENTRY(GL_LOG_TRACE); return mScissorTestRectangle.width; }
      inline int              GetScissorRectHeight(void)                const   { FUN_ENTRY(GL_LOG_TRACE); return mScissorTestRectangle.height; }
      inline GLint            GetSampleCoverageBits(void)               const   { FUN_ENTRY(GL_LOG_TRACE); return mSampleCoverageBits; }
      inline GLfloat          GetSampleCoverageValue(void)              const   { FUN_ENTRY(GL_LOG_TRACE); return mSampleCoverageValue; }
      inline GLfloat          GetMinSampleShading(void)                 const   { FUN_ENTRY(GL_LOG_TRACE); return mMinSampleShading; }
      inline GLboolean        GetSampleCoverageInvert(void)             const   { FUN_ENTRY(GL_LOG_TRACE); return mSampleCoverageInvert; }
      inline GLboolean        GetSampleCoverageEnabled(void)            const   { FUN_ENTRY(GL_LOG_TRACE); return mSampleCoverageEnabled; }
      inline GLboolean        GetSampleAlphaToCoverageEnabled(void)     const   { FUN_ENTRY(GL_LOG_TRACE); return mSampleAlphaToCoverageEnabled; }
      inline GLboolean        GetSampleAlphaToOneEnabled(void)          const   { FUN_ENTRY(GL_LOG_TRACE); return mSampleAlphaToOneEnabled; }
      inline GLboolean        GetSampleShadingEnabled(void)             const   { FUN_ENTRY(GL_LOG_TRACE); return mSampleShadingEnabled; }
      inline GLboolean        GetMultiSamplingEnabled(void)             const   { FUN_ENTRY(GL_LOG_TRACE); return mMultiSamplingEnabled; }
      inline GLboolean        GetStencilTestEnabled(void)               const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestEnabled; }

      inline GLenum           GetStencilTestFuncCompareFront(void)      const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestFuncCompareFront; }
      inline GLint            GetStencilTestFuncRefFront (void)         const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestFuncRefFront ; }
      inline GLuint           GetStencilTestFuncMaskFront(void)         const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestFuncMaskFront; }
      inline GLenum           GetStencilTestFuncCompareBack(void)       const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestFuncCompareBack; }
      inline GLint            GetStencilTestFuncRefBack (void)          const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestFuncRefBack ; }
      inline GLuint           GetStencilTestFuncMaskBack(void)          const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestFuncMaskBack; }

      inline GLenum           GetStencilTestOpFailFront(void)           const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestOpFailFront; }
      inline GLenum           GetStencilTestOpZfailFront(void)          const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestOpZfailFront; }
      inline GLenum           GetStencilTestOpZpassFront(void)          const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestOpZpassFront; }
      inline GLenum           GetStencilTestOpFailBack(void)            const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestOpFailBack; }
      inline GLenum           GetStencilTestOpZfailBack(void)           const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestOpZfailBack; }
      inline GLenum           GetStencilTestOpZpassBack(void)           const   { FUN_ENTRY(GL_LOG_TRACE); return mStencilTestOpZpassBack; }

      inline GLenum           GetDepthTestFunc(void)                    const   { FUN_ENTRY(GL_LOG_TRACE); return mDepthTestFunc; }
      inline GLboolean        GetDepthTestEnabled(void)                 const   { FUN_ENTRY(GL_LOG_TRACE); return mDepthTestEnabled; }
      inline GLboolean        GetDepthBoundsTestEnabled(void)           const   { FUN_ENTRY(GL_LOG_TRACE); return mDepthBoundsTestEnabled; }
      inline GLfloat          GetMinDepthBounds(void)                   const   { FUN_ENTRY(GL_LOG_TRACE); return mMinDepthBounds; }
      inline GLfloat          GetMaxDepthBounds(void)                   const   { FUN_ENTRY(GL_LOG_TRACE); return mMaxDepthBounds; }
      inline GLboolean        GetBlendingEnabled(void)                  const   { FUN_ENTRY(GL_LOG_TRACE); return mBlendingEnabled; }
      inline GLboolean        GetDitheringEnabled(void)                 const   { FUN_ENTRY(GL_LOG_TRACE); return mDitheringEnabled; }

// Set Functions
      inline void             SetBlendingColor(GLclampf red,
                                               GLclampf green,
                                               GLclampf blue,
                                               GLclampf alpha)                  { FUN_ENTRY(GL_LOG_TRACE); mBlendingColor[0] = CLAMP_F(red);
                                                                                                       mBlendingColor[1] = CLAMP_F(green);
                                                                                                       mBlendingColor[2] = CLAMP_F(blue);
                                                                                                       mBlendingColor[3] = CLAMP_F(alpha);}
      inline void             SetScissorTestEnabled(GLboolean enable)           { FUN_ENTRY(GL_LOG_TRACE); mScissorTestEnabled = enable; }
      inline void             SetScissorRect(const Rect *fbo)                   { FUN_ENTRY(GL_LOG_TRACE); mScissorTestRectangle.x = fbo->x;
                                                                                                       mScissorTestRectangle.y = fbo->y;
                                                                                                       mScissorTestRectangle.width = fbo->width;
                                                                                                       mScissorTestRectangle.height = fbo->height;}
      inline void             SetScissorRectX(GLint x)                          { FUN_ENTRY(GL_LOG_TRACE); mScissorTestRectangle.x = x; }
      inline void             SetScissorRectY(GLint y)                          { FUN_ENTRY(GL_LOG_TRACE); mScissorTestRectangle.y = y; }
      inline void             SetScissorRectWidth(GLint width)                  { FUN_ENTRY(GL_LOG_TRACE); mScissorTestRectangle.width = width; }
      inline void             SetScissorRectHeight(GLint height)                { FUN_ENTRY(GL_LOG_TRACE); mScissorTestRectangle.height = height; }
      inline void             SetSampleCoverageValue(GLfloat value)             { FUN_ENTRY(GL_LOG_TRACE); mSampleCoverageValue = CLAMP_F(value); }
      inline void             SetSampleCoverageInvert(GLboolean invert)         { FUN_ENTRY(GL_LOG_TRACE); mSampleCoverageInvert = invert; }
      inline void             SetSampleCoverageEnabled(GLboolean enable)        { FUN_ENTRY(GL_LOG_TRACE); mSampleCoverageEnabled = enable; }
      inline void             SetSampleAlphaToCoverageEnabled(GLboolean enable) { FUN_ENTRY(GL_LOG_TRACE); mSampleAlphaToCoverageEnabled = enable; }
      inline void             SetStencilTestEnabled(GLboolean enable)           { FUN_ENTRY(GL_LOG_TRACE); mStencilTestEnabled = enable; }
      inline void             SetBlendingEnabled(GLboolean enable)              { FUN_ENTRY(GL_LOG_TRACE); mBlendingEnabled = enable; }
      inline void             SetDitheringEnabled(GLboolean enable)             { FUN_ENTRY(GL_LOG_TRACE); mDitheringEnabled = enable; }

// Update Functions
      inline bool             UpdateSampleCoverageEnabled(GLboolean enable)     { FUN_ENTRY(GL_LOG_TRACE); bool res = mSampleCoverageEnabled != enable;
                                                                                                       mSampleCoverageEnabled = enable;
                                                                                                       return res; }
      inline bool             UpdateSampleAlphaToCoverageEnabled(GLboolean enable)
                                                                                { FUN_ENTRY(GL_LOG_TRACE); bool res = mSampleAlphaToCoverageEnabled != enable;
                                                                                                       mSampleAlphaToCoverageEnabled = enable;
                                                                                                       return res; }
      inline bool             UpdateSampleCoverageValue(GLfloat value)          { FUN_ENTRY(GL_LOG_TRACE); GLfloat v = CLAMP_F(value);
                                                                                                       bool res = mSampleCoverageValue != v;
                                                                                                       mSampleCoverageValue = v;
                                                                                                       return res; }

      inline bool             UpdateSampleCoverageInvert(GLboolean invert)      { FUN_ENTRY(GL_LOG_TRACE); bool res = mSampleCoverageInvert != invert;
                                                                                                       mSampleCoverageInvert = invert;
                                                                                                       return res; }
      inline bool             UpdateScissorRect(GLint x,
                                                GLint y,
                                                GLint width,
                                                GLint height)                   { FUN_ENTRY(GL_LOG_TRACE); bool res = (mScissorTestRectangle.x != x) || (mScissorTestRectangle.y != y) || (mScissorTestRectangle.width != width) || (mScissorTestRectangle.height != height);
                                                                                                       mScissorTestRectangle.x = x;
                                                                                                       mScissorTestRectangle.y = y;
                                                                                                       mScissorTestRectangle.width = width;
                                                                                                       mScissorTestRectangle.height = height;
                                                                                                       return res; }
      inline bool             UpdateDepthTestFunc(GLenum func)                  { FUN_ENTRY(GL_LOG_TRACE); bool res = (mDepthTestFunc != func);
                                                                                                       mDepthTestFunc = func;
                                                                                                       return res; }
      inline bool             UpdateDepthTestEnabled(GLenum enable)             { FUN_ENTRY(GL_LOG_TRACE); bool res = (mDepthTestEnabled != enable);
                                                                                                       mDepthTestEnabled = enable;
                                                                                                       return res; }
      inline bool             UpdateScissorTestEnabled(GLboolean enable)        { FUN_ENTRY(GL_LOG_TRACE); bool res = (mScissorTestEnabled != enable);
                                                                                                       mScissorTestEnabled = enable;
                                                                                                       return res; }
      inline bool             UpdateBlendingEnabled(GLenum enable)              { FUN_ENTRY(GL_LOG_TRACE); bool res = (mBlendingEnabled != enable);
                                                                                                       mBlendingEnabled = enable;
                                                                                                       return res; }
      inline bool             UpdateStencilTestEnabled(GLenum enable)           { FUN_ENTRY(GL_LOG_TRACE); bool res = (mStencilTestEnabled != enable);
                                                                                                       mStencilTestEnabled = enable;
                                                                                                       return res; }
      inline bool             UpdateBlendingEquation(GLenum modeRGB,
                                                     GLenum modeAlpha)          { FUN_ENTRY(GL_LOG_TRACE); bool res = (mBlendingEquationRGB != modeRGB) || (mBlendingEquationAlpha != modeAlpha);
                                                                                                       mBlendingEquationRGB = modeRGB;
                                                                                                       mBlendingEquationAlpha = modeAlpha;
                                                                                                       return res;}
      inline bool             UpdateBlendingColor(GLclampf red,
                                                  GLclampf green,
                                                  GLclampf blue,
                                                  GLclampf alpha)               { FUN_ENTRY(GL_LOG_TRACE);
                                                                                                       GLfloat r = CLAMP_F(red);
                                                                                                       GLfloat g = CLAMP_F(green);
                                                                                                       GLfloat b = CLAMP_F(blue);
                                                                                                       GLfloat a = CLAMP_F(alpha);
                                                                                                       bool res = (mBlendingColor[0] != r) || (mBlendingColor[1] != g) || (mBlendingColor[2] != b) || (mBlendingColor[3] != a);
                                                                                                       mBlendingColor[0] = r;
                                                                                                       mBlendingColor[1] = g;
                                                                                                       mBlendingColor[2] = b;
                                                                                                       mBlendingColor[3] = a;
                                                                                                       return res; }
      inline bool             UpdateBlendingFactors(GLenum srcRGB,
                                                    GLenum srcAlpha,
                                                    GLenum dstRGB,
                                                    GLenum dstAlpha)            { FUN_ENTRY(GL_LOG_TRACE); bool res = (mBlendingFactorSourceRGB != srcRGB) || (mBlendingFactorSourceAlpha != srcAlpha) || (mBlendingFactorDestinationRGB != dstRGB) || (mBlendingFactorDestinationAlpha != dstAlpha);
                                                                                                       mBlendingFactorSourceRGB = srcRGB; mBlendingFactorSourceAlpha = srcAlpha; mBlendingFactorDestinationRGB = dstRGB; mBlendingFactorDestinationAlpha = dstAlpha;
                                                                                                       return res; }
      inline bool             UpdateStencilTestFunc(GLenum face,
                                                    GLenum func,
                                                    GLint  ref,
                                                    GLuint mask)                { FUN_ENTRY(GL_LOG_TRACE); bool res = false;
                                                                                                       if(face == GL_FRONT) {
                                                                                                         res = (mStencilTestFuncCompareFront != func) || (mStencilTestFuncRefFront != ref) || (mStencilTestFuncMaskFront != mask);
                                                                                                         mStencilTestFuncCompareFront = func;
                                                                                                         mStencilTestFuncRefFront  = ref;
                                                                                                         mStencilTestFuncMaskFront = mask;
                                                                                                       } else {
                                                                                                         res = (mStencilTestFuncCompareBack != func) || (mStencilTestFuncRefBack != ref) || (mStencilTestFuncMaskBack != mask);
                                                                                                         mStencilTestFuncCompareBack = func;
                                                                                                         mStencilTestFuncRefBack  = ref;
                                                                                                         mStencilTestFuncMaskBack = mask;
                                                                                                       }
                                                                                                       return res; }

      inline bool             UpdateStencilTestOp(GLenum face,
                                                  GLenum fail,
                                                  GLenum zfail,
                                                  GLenum zpass)                 { FUN_ENTRY(GL_LOG_TRACE); bool res = false;
                                                                                                       if(face == GL_FRONT) {
                                                                                                          res = (mStencilTestOpFailFront != fail) || (mStencilTestOpZfailFront != zfail) || (mStencilTestOpZpassFront != zpass);
                                                                                                          mStencilTestOpFailFront  = fail;
                                                                                                          mStencilTestOpZfailFront = zfail;
                                                                                                          mStencilTestOpZpassFront = zpass;
                                                                                                       } else {
                                                                                                          res = (mStencilTestOpFailBack != fail) || (mStencilTestOpZfailBack != zfail) || (mStencilTestOpZpassBack != zpass);
                                                                                                          mStencilTestOpFailBack  = fail;
                                                                                                          mStencilTestOpZfailBack = zfail;
                                                                                                          mStencilTestOpZpassBack = zpass;
                                                                                                       }
                                                                                                       return res; }
};

#endif //__STATEFRAGMENTOPERATIONS_H__
