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
 *  @file       stateViewportTransformation.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Controlling the Viewport
 *
 */

#include "stateViewportTransformation.h"

StateViewportTransformation::StateViewportTransformation()
: mMinDepthRange(0.0f), mMaxDepthRange(1.0f), mViewportCount(1), mScissorCount(1)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

StateViewportTransformation::~StateViewportTransformation()
{
    FUN_ENTRY(GL_LOG_TRACE);
}
