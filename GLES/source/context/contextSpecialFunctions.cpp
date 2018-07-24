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
 *  @file       contextSpecialFunctions.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Special Functions (e.g. glHint)
 *
 */

#include "context.h"

void
Context::Hint(GLenum target, GLenum mode)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_GENERATE_MIPMAP_HINT || (mode != GL_FASTEST && mode != GL_NICEST && mode != GL_DONT_CARE)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    mStateManager.GetHintAspectsState()->SetMode(target, mode);
}
