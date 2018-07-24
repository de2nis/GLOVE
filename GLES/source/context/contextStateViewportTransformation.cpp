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
 *  @file       contextStateViewportTransformation.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Viewport Transformation
 *
 */

#include "context.h"

void
Context::DepthRangef(GLclampf zNear, GLclampf zFar)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mStateManager.GetViewportTransformationState()->SetMinDepthRange(zNear);
    mStateManager.GetViewportTransformationState()->SetMaxDepthRange(zFar);
    mPipeline->SetUpdateViewportState(true);
}

void
Context::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(width < 0 || height < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(mStateManager.GetViewportTransformationState()->UpdateViewportRect(x, y, width, height)) {
        mPipeline->SetUpdateViewportState(true);
    }
}
