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
 *  @file       stateFragmentOperations.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Fragment Operations
 *
 *  @section
 *
 *  Fragment Operations include (a) Scissor Test, (b) Depth Bounds Test,
 *  (c) Multisampling, (d) Stencil Test, (e) Depth Buffer Test, (f) Blending
 *  Test, (g) Dithering
 *
 */

#include "stateFragmentOperations.h"

StateFragmentOperations::StateFragmentOperations()
: mScissorTestEnabled(GL_FALSE),
mDepthBoundsTestEnabled(GL_FALSE), mMinDepthBounds(0.0f), mMaxDepthBounds(1.0f),
mMultiSamplingEnabled(GL_FALSE), mSampleCoverageBits(1), mSampleCoverageValue(1.0f), mSampleCoverageInvert(GL_FALSE), mSampleCoverageEnabled(GL_FALSE), mSampleAlphaToCoverageEnabled(GL_FALSE),
mSampleAlphaToOneEnabled(GL_FALSE), mSampleShadingEnabled(GL_FALSE), mMinSampleShading(1.0f),
mStencilTestEnabled(GL_FALSE), mStencilTestFuncCompareFront(GL_ALWAYS), mStencilTestFuncCompareBack(GL_ALWAYS),
mStencilTestFuncRefFront(0), mStencilTestFuncRefBack(0), mStencilTestFuncMaskFront(0xffffffff), mStencilTestFuncMaskBack(0xffffffff),
mStencilTestOpFailFront(GL_KEEP), mStencilTestOpZfailFront(GL_KEEP), mStencilTestOpZpassFront(GL_KEEP),
mStencilTestOpFailBack(GL_KEEP), mStencilTestOpZfailBack(GL_KEEP), mStencilTestOpZpassBack(GL_KEEP),
mDepthTestFunc(GL_LESS), mDepthTestEnabled(GL_FALSE),
mBlendingEnabled(GL_FALSE), mBlendingEquationRGB(GL_FUNC_ADD), mBlendingEquationAlpha(GL_FUNC_ADD),
mBlendingFactorSourceRGB(GL_ONE), mBlendingFactorSourceAlpha(GL_ONE), mBlendingFactorDestinationRGB(GL_ZERO), mBlendingFactorDestinationAlpha(GL_ZERO), mBlendingColorAttachmentCount(1), 
mBlendingLogicOp(GL_CLEAR), mBlendingLogicOpEnabled(GL_FALSE),
mDitheringEnabled(GL_TRUE)
{
    FUN_ENTRY(GL_LOG_TRACE);

    mBlendingColor[0] = 0.0f;
    mBlendingColor[1] = 0.0f;
    mBlendingColor[2] = 0.0f;
    mBlendingColor[3] = 0.0f;
}

StateFragmentOperations::~StateFragmentOperations()
{
    FUN_ENTRY(GL_LOG_TRACE);
}
