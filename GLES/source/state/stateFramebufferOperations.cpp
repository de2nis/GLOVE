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
 *  @file       stateFramebufferOperations.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Framebuffer Operations
 *
 *  @section
 *
 *  Fragment Operations include clearing the (a) Color and (b) Depth/Stencil
 *  buffers.
 *
 */

#include "stateFramebufferOperations.h"

StateFramebufferOperations::StateFramebufferOperations()
: mClearDepth(1.0f), mClearStencil(0),
mColorMask(0xf), mDepthMask(GL_TRUE), mStencilMaskFront(0xffffffff), mStencilMaskBack(0xffffffff)
{
    FUN_ENTRY(GL_LOG_TRACE);

    mClearColor[0] = 0.0f;
    mClearColor[1] = 0.0f;
    mClearColor[2] = 0.0f;
    mClearColor[3] = 0.0f;
}

StateFramebufferOperations::~StateFramebufferOperations()
{
    FUN_ENTRY(GL_LOG_TRACE);
}
