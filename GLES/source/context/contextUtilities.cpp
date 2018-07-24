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
 *  @file       contextUtilities.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to GL Utilities
 *
 */

#include "context.h"

GLenum
Context::GetError(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    GLenum result = mStateManager.GetError();

    mStateManager.SetError(GL_NO_ERROR);

    return result;
}

const GLubyte*
Context::GetString(GLenum name)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    static const char *strings[] {"GLOVE (GL Over Vulkan)\0",
                                  "OpenGL ES 2.0 Over Vulkan\0",
                                  "OpenGL ES 2.0\0",
                                  "OpenGL ES GLSL ES 1.00\0",
                                  "GL_OES_get_program_binary\0"};
    switch(name) {
    case GL_VENDOR:                     return (const GLubyte *)strings[0];
    case GL_RENDERER:                   return (const GLubyte *)strings[1];
    case GL_VERSION:                    return (const GLubyte *)strings[2];
    case GL_SHADING_LANGUAGE_VERSION:   return (const GLubyte *)strings[3];
    case GL_EXTENSIONS:                 return (const GLubyte *)strings[4];
    default:                            RecordError(GL_INVALID_ENUM); return nullptr; 
    }
}
