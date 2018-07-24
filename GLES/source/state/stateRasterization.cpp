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
 *  @file       stateRasterization.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Rasterization (e.g. primitive clipping)
 *
 */

#include "stateRasterization.h"

StateRasterization::StateRasterization()
: mCullFace(GL_BACK), mCullEnabled(GL_FALSE), mFrontFace(GL_CCW),
mLineWidth(1.0f), mDepthClampEnabled(GL_FALSE), mRasterizerDiscardEnabled(GL_FALSE),
mPolygonOffsetFactor(0.0f), mPolygonOffsetUnits(0.0f), mPolygonOffsetFillEnabled(GL_FALSE), mPolygonOffsetClamp(0.0f)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

StateRasterization::~StateRasterization()
{
    FUN_ENTRY(GL_LOG_TRACE);
}
