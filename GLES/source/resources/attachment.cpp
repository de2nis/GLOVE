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
 *  @file       attachment.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Framebuffer Attachment Functionality in GLOVE
 *
 *  @scope
 *
 *  The creation of user-defined Framebuffers is done by attaching images
 *  of texture to the framebuffer object.
 *
 */

#include "attachment.h"

Attachment::Attachment(Texture *tex)
: mType(GL_NONE), mName(0), mLevel(0), mLayer(GL_TEXTURE_CUBE_MAP_POSITIVE_X), mTexture(tex)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

Attachment::~Attachment()
{
    FUN_ENTRY(GL_LOG_TRACE);
}
