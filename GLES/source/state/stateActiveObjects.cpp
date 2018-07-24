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
 *  @file       stateActiveObjects.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Active Resources
 *
 *  @section
 *
 *  State Manager must include the active (a) buffer object, (b) shader program,
 *  (c) framebuffer, (d) renderbuffer and (e) textures for both 2D and CUBEMAP
 *  types.
 *
 */

#include "stateActiveObjects.h"

StateActiveObjects::StateActiveObjects()
: mActiveShaderProgram(nullptr),
mActiveFramebufferObjectID(0),
mActiveRenderbufferObjectID(0),
mActiveTextureUnit(GL_TEXTURE0)
{
    FUN_ENTRY(GL_LOG_TRACE);

    for(uint32_t i=0; i<BUFFER_OBJECT_TARGET_ALL; ++i) {
        mActiveBufferObjects[i] = nullptr;
    }

    memset((void *)mActiveTextures, 0, sizeof(mActiveTextures));
}

StateActiveObjects::~StateActiveObjects()
{
    FUN_ENTRY(GL_LOG_TRACE);
}
