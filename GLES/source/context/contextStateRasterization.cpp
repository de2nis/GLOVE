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
 *  @file       contextStateRasterization.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Rasterization
 *
 */

#include "context.h"

void
Context::CullFace(GLenum mode)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mode != GL_BACK && mode != GL_FRONT && mode != GL_FRONT_AND_BACK) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(mStateManager.GetRasterizationState()->UpdateCullFace(mode)) {
        mPipeline->SetRasterizationCullMode(GlBooleanToVkBool(mStateManager.GetRasterizationState()->GetCullEnabled()),
                                            GlCullModeToVkCullMode(mStateManager.GetRasterizationState()->GetCullFace()));
    }
}

void
Context::FrontFace(GLenum mode)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mode != GL_CCW && mode != GL_CW) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(mStateManager.GetRasterizationState()->UpdateFrontFace(mode)) {
        mPipeline->SetRasterizationFrontFace(GlFrontFaceToVkFrontFace(mStateManager.GetRasterizationState()->GetFrontFace()));
    }
}

void
Context::LineWidth(GLfloat width)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(width <= 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    mStateManager.GetRasterizationState()->SetLineWidth(width);
}

void
Context::PolygonOffset(GLfloat factor, GLfloat units)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mStateManager.GetRasterizationState()->UpdatePolygonOffsetFactor(factor)) {
         mPipeline->SetRasterizationDepthBiasConstantFactor(mStateManager.GetRasterizationState()->GetPolygonOffsetFactor());
    }

    if(mStateManager.GetRasterizationState()->UpdatePolygonOffsetUnits(units)) {
         mPipeline->SetRasterizationDepthBiasSlopeFactor(mStateManager.GetRasterizationState()->GetPolygonOffsetUnits());
    }
}
