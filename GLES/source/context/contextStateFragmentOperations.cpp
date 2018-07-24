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
 *  @file       contextStateFragmentOperations.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Per-Fragment Operations
 *
 */

#include "context.h"

void
Context::BlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mStateManager.GetFragmentOperationsState()->UpdateBlendingColor(red, green, blue, alpha)) {
        GLfloat    blendcolor[4];
        mStateManager.GetFragmentOperationsState()->GetBlendingColor(blendcolor);
        mPipeline->SetColorBlendConstants(blendcolor);
    }
}

void
Context::BlendEquation(GLenum mode)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mode != GL_FUNC_ADD && mode != GL_FUNC_SUBTRACT && mode != GL_FUNC_REVERSE_SUBTRACT) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateBlendingEquation(mode, mode)) {
        mPipeline->SetColorBlendAttachmentColorOp(GlBlendEquationToVkBlendOp(mStateManager.GetFragmentOperationsState()->GetBlendingEquationRGB()));
        mPipeline->SetColorBlendAttachmentAlphaOp(GlBlendEquationToVkBlendOp(mStateManager.GetFragmentOperationsState()->GetBlendingEquationAlpha()));
    }
}

void
Context::BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if((modeRGB != GL_FUNC_ADD && modeRGB != GL_FUNC_SUBTRACT && modeRGB != GL_FUNC_REVERSE_SUBTRACT) ||
       (modeAlpha != GL_FUNC_ADD && modeAlpha != GL_FUNC_SUBTRACT && modeAlpha != GL_FUNC_REVERSE_SUBTRACT)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateBlendingEquation(modeRGB, modeAlpha)) {
        mPipeline->SetColorBlendAttachmentColorOp(GlBlendEquationToVkBlendOp(mStateManager.GetFragmentOperationsState()->GetBlendingEquationRGB()));
        mPipeline->SetColorBlendAttachmentAlphaOp(GlBlendEquationToVkBlendOp(mStateManager.GetFragmentOperationsState()->GetBlendingEquationAlpha()));
    }
}

void
Context::BlendFunc(GLenum sfactor, GLenum dfactor)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    GLenum factors[2] = {sfactor, dfactor};

    for(int i = 0; i < 2; ++i) {
        if(factors[i] != GL_ZERO && factors[i] != GL_ONE && factors[i] != GL_SRC_COLOR && factors[i] != GL_ONE_MINUS_SRC_COLOR &&
           factors[i] != GL_DST_COLOR && factors[i] != GL_ONE_MINUS_DST_COLOR && factors[i] != GL_SRC_ALPHA &&
           factors[i] != GL_ONE_MINUS_SRC_ALPHA && factors[i] != GL_DST_ALPHA && factors[i] != GL_ONE_MINUS_DST_ALPHA &&
           factors[i] != GL_CONSTANT_COLOR && factors[i] != GL_ONE_MINUS_CONSTANT_COLOR && factors[i] != GL_CONSTANT_ALPHA &&
           factors[i] != GL_ONE_MINUS_CONSTANT_ALPHA && factors[i] != GL_SRC_ALPHA_SATURATE) {
            RecordError(GL_INVALID_ENUM);
            return;
        }
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateBlendingFactors(sfactor, sfactor, dfactor, dfactor)) {
        mPipeline->SetColorBlendAttachmentSrcColorFactor(GlBlendFactorToVkBlendFactor(mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceRGB()));
        mPipeline->SetColorBlendAttachmentDstColorFactor(GlBlendFactorToVkBlendFactor(mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationRGB()));
        mPipeline->SetColorBlendAttachmentSrcAlphaFactor(GlBlendFactorToVkBlendFactor(mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceAlpha()));
        mPipeline->SetColorBlendAttachmentDstAlphaFactor(GlBlendFactorToVkBlendFactor(mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationAlpha()));
    }
}

void
Context::BlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    const GLenum factors[4] = {srcRGB, dstRGB, srcAlpha, dstAlpha};

    for(int i = 0; i < 4; ++i) {
        if(factors[i] != GL_ZERO && factors[i] != GL_ONE && factors[i] != GL_SRC_COLOR && factors[i] != GL_ONE_MINUS_SRC_COLOR &&
           factors[i] != GL_DST_COLOR && factors[i] != GL_ONE_MINUS_DST_COLOR && factors[i] != GL_SRC_ALPHA &&
           factors[i] != GL_ONE_MINUS_SRC_ALPHA && factors[i] != GL_DST_ALPHA && factors[i] != GL_ONE_MINUS_DST_ALPHA &&
           factors[i] != GL_CONSTANT_COLOR && factors[i] != GL_ONE_MINUS_CONSTANT_COLOR && factors[i] != GL_CONSTANT_ALPHA &&
           factors[i] != GL_ONE_MINUS_CONSTANT_ALPHA && factors[i] != GL_SRC_ALPHA_SATURATE) {
            RecordError(GL_INVALID_ENUM);
            return;
        }
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateBlendingFactors(srcRGB, srcAlpha, dstRGB, dstAlpha)) {
        mPipeline->SetColorBlendAttachmentSrcColorFactor(GlBlendFactorToVkBlendFactor(mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceRGB()));
        mPipeline->SetColorBlendAttachmentDstColorFactor(GlBlendFactorToVkBlendFactor(mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationRGB()));
        mPipeline->SetColorBlendAttachmentSrcAlphaFactor(GlBlendFactorToVkBlendFactor(mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceAlpha()));
        mPipeline->SetColorBlendAttachmentDstAlphaFactor(GlBlendFactorToVkBlendFactor(mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationAlpha()));
    }
}

void
Context::DepthFunc(GLenum func)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if( func != GL_NEVER    && func != GL_EQUAL  && func != GL_LEQUAL && func != GL_GREATER  &&
        func != GL_NOTEQUAL && func != GL_GEQUAL && func != GL_ALWAYS && func != GL_LESS
      ) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateDepthTestFunc(func)) {
        mPipeline->SetDepthCompareOp(GlCompareFuncToVkCompareOp(mStateManager.GetFragmentOperationsState()->GetDepthTestFunc()));
    }
}

void
Context::SampleCoverage(GLclampf value, GLboolean invert)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mStateManager.GetFragmentOperationsState()->UpdateSampleCoverageValue(value) ||
       mStateManager.GetFragmentOperationsState()->UpdateSampleCoverageInvert(invert) ) {
        //mPipeline->SetMultisampleMask(value, invert); // TODO
    }
}

void
Context::Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(width < 0 || height < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateScissorRect(x, y, width, height)) {
        mPipeline->SetUpdateViewportState(true);
    }
}

void
Context::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(func != GL_NEVER    && func != GL_LESS   && func != GL_LEQUAL &&
       func != GL_GREATER  && func != GL_GEQUAL && func != GL_EQUAL  &&
       func != GL_NOTEQUAL && func != GL_ALWAYS) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateStencilTestFunc(GL_FRONT, func, ref, mask)) {
        mPipeline->SetStencilFrontCompareOp(GlCompareFuncToVkCompareOp(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareFront()));
        mPipeline->SetStencilFrontCompareMask(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskFront());
        mPipeline->SetStencilFrontReference(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefFront());
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateStencilTestFunc(GL_BACK , func, ref, mask)) {
        mPipeline->SetStencilBackCompareOp(GlCompareFuncToVkCompareOp(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareBack()));
        mPipeline->SetStencilBackCompareMask(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskBack());
        mPipeline->SetStencilBackReference(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefBack());
    }
}

void
Context::StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(func != GL_NEVER    && func != GL_LESS   && func != GL_LEQUAL &&
       func != GL_GREATER  && func != GL_GEQUAL && func != GL_EQUAL  &&
       func != GL_NOTEQUAL && func != GL_ALWAYS) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(face != GL_FRONT && face != GL_BACK && face != GL_FRONT_AND_BACK) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if((face == GL_FRONT || face ==  GL_FRONT_AND_BACK) && mStateManager.GetFragmentOperationsState()->UpdateStencilTestFunc(GL_FRONT, func, ref, mask)) {
        mPipeline->SetStencilFrontCompareOp(GlCompareFuncToVkCompareOp(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareFront()));
        mPipeline->SetStencilFrontCompareMask(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskFront());
        mPipeline->SetStencilFrontReference(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefFront());
    }

    if((face == GL_BACK || face ==  GL_FRONT_AND_BACK) && mStateManager.GetFragmentOperationsState()->UpdateStencilTestFunc(GL_BACK , func, ref, mask)) {
        mPipeline->SetStencilBackCompareOp(GlCompareFuncToVkCompareOp(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareBack()));
        mPipeline->SetStencilBackCompareMask(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskBack());
        mPipeline->SetStencilBackReference(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefBack());
    }
}

void
Context::StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if((fail != GL_KEEP && fail != GL_REPLACE && fail != GL_INCR && fail != GL_DECR && fail != GL_INVERT && fail != GL_INCR_WRAP && fail != GL_DECR_WRAP && fail != GL_ZERO) ||
       (zfail != GL_KEEP && zfail != GL_REPLACE && zfail != GL_INCR && zfail != GL_DECR && zfail != GL_INVERT && zfail != GL_INCR_WRAP && zfail != GL_DECR_WRAP && zfail != GL_ZERO) ||
       (zpass != GL_KEEP && zpass != GL_REPLACE && zpass != GL_INCR && zpass != GL_DECR && zpass != GL_INVERT && zpass != GL_INCR_WRAP && zpass != GL_DECR_WRAP && zpass != GL_ZERO)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateStencilTestOp(GL_FRONT, fail, zfail, zpass)) {
        mPipeline->SetStencilFrontFailOp (GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailFront()));
        mPipeline->SetStencilFrontZFailOp(GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailFront()));
        mPipeline->SetStencilFrontPassOp (GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassFront()));
    }

    if(mStateManager.GetFragmentOperationsState()->UpdateStencilTestOp(GL_BACK , fail, zfail, zpass)) {
        mPipeline->SetStencilBackFailOp (GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailBack()));
        mPipeline->SetStencilBackZFailOp(GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailBack()));
        mPipeline->SetStencilBackPassOp (GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassBack()));
    }
}

void
Context::StencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if((fail  != GL_KEEP && fail  != GL_REPLACE && fail  != GL_INCR && fail  != GL_DECR && fail  != GL_INVERT && fail  != GL_INCR_WRAP && fail  != GL_DECR_WRAP && fail  != GL_ZERO) ||
       (zfail != GL_KEEP && zfail != GL_REPLACE && zfail != GL_INCR && zfail != GL_DECR && zfail != GL_INVERT && zfail != GL_INCR_WRAP && zfail != GL_DECR_WRAP && zfail != GL_ZERO) ||
       (zpass != GL_KEEP && zpass != GL_REPLACE && zpass != GL_INCR && zpass != GL_DECR && zpass != GL_INVERT && zpass != GL_INCR_WRAP && zpass != GL_DECR_WRAP && zpass != GL_ZERO)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(face != GL_FRONT && face != GL_BACK && face != GL_FRONT_AND_BACK) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if((face == GL_FRONT || face ==  GL_FRONT_AND_BACK) && mStateManager.GetFragmentOperationsState()->UpdateStencilTestOp(GL_FRONT, fail, zfail, zpass)) {
        mPipeline->SetStencilFrontFailOp (GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailFront()));
        mPipeline->SetStencilFrontZFailOp(GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailFront()));
        mPipeline->SetStencilFrontPassOp (GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassFront()));
    }

    if((face == GL_BACK || face ==  GL_FRONT_AND_BACK) && mStateManager.GetFragmentOperationsState()->UpdateStencilTestOp(GL_BACK , fail, zfail, zpass)) {
        mPipeline->SetStencilBackFailOp (GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailBack()));
        mPipeline->SetStencilBackZFailOp(GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailBack()));
        mPipeline->SetStencilBackPassOp (GlStencilFuncToVkStencilOp(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassBack()));
    }
}
