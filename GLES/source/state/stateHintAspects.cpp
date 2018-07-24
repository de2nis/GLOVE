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
 *  @file       stateHintAspects.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to implementation-specific hints
 *
 */

#include "stateHintAspects.h"

StateHintAspects::StateHintAspects()
{
    FUN_ENTRY(GL_LOG_TRACE);

    SetMode(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);
}

StateHintAspects::~StateHintAspects()
{
    FUN_ENTRY(GL_LOG_TRACE);
}
