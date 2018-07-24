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
 *  @file       contextStateManager.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to State Manager Functionality
 *
 */

#include "context.h"

void
Context::Disable(GLenum cap)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    SetCapability(cap, GL_FALSE);
}

void
Context::Enable(GLenum cap)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    SetCapability(cap, GL_TRUE);
}

void
Context::SetCapability(GLenum cap, GLboolean enable)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(cap) {
    case GL_SCISSOR_TEST:
        if(mStateManager.GetFragmentOperationsState()->UpdateScissorTestEnabled(enable)) {
            mPipeline->SetUpdateViewportState(true);
        }
        break;
    case GL_DEPTH_TEST:
        if(mStateManager.GetFragmentOperationsState()->UpdateDepthTestEnabled(enable)) {
            mPipeline->SetDepthTestEnable(GlBooleanToVkBool(mStateManager.GetFragmentOperationsState()->GetDepthTestEnabled()));
        }
        break;
    case GL_STENCIL_TEST:
        if(mStateManager.GetFragmentOperationsState()->UpdateStencilTestEnabled(enable)) {
            mPipeline->SetStencilTestEnable(GlBooleanToVkBool(mStateManager.GetFragmentOperationsState()->GetStencilTestEnabled()));
        }
        break;
    case GL_CULL_FACE:
        if(mStateManager.GetRasterizationState()->UpdateCullEnabled(enable)) {
            mPipeline->SetRasterizationCullMode(GlBooleanToVkBool(mStateManager.GetRasterizationState()->GetCullEnabled()), GlCullModeToVkCullMode(mStateManager.GetRasterizationState()->GetCullFace()));
        }
        break;
    case GL_POLYGON_OFFSET_FILL:
        if(mStateManager.GetRasterizationState()->UpdatePolygonOffsetFillEnabled(enable)) {
            mPipeline->SetRasterizationDepthBiasEnable(GlBooleanToVkBool(mStateManager.GetRasterizationState()->GetPolygonOffsetFillEnabled()));
        }
        break;
    case GL_SAMPLE_ALPHA_TO_COVERAGE:
        if(mStateManager.GetFragmentOperationsState()->UpdateSampleAlphaToCoverageEnabled(enable)) {
            mPipeline->SetMultisampleAlphaToCoverage(GlBooleanToVkBool(mStateManager.GetFragmentOperationsState()->GetSampleAlphaToCoverageEnabled()));
        }
        break;
    case GL_SAMPLE_COVERAGE:
        if(mStateManager.GetFragmentOperationsState()->UpdateSampleCoverageEnabled(enable)) {
        //    mPipeline->SetUpdateMultisampleState(true); // TODO
        }
        break;
    case GL_BLEND:
        if(mStateManager.GetFragmentOperationsState()->UpdateBlendingEnabled(enable)) {
            mPipeline->SetColorBlendAttachmentEnable(GlBooleanToVkBool(mStateManager.GetFragmentOperationsState()->GetBlendingEnabled()));
        }
        break;
    case GL_DITHER:
        mStateManager.GetFragmentOperationsState()->SetDitheringEnabled(enable);
        break;
    default:
        RecordError(GL_INVALID_ENUM);
        break;
    }
}
