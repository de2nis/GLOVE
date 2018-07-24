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
 *  @file       clearPass.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Clearing Images Inside A Render Pass Instance in Vulkan
 *
 *  @section
 *
 *  Clear one region of color and depth/stencil attachments
 *  inside a render pass instance.
 *
 */

#include "clearPass.h"

#include <string.h>

namespace vulkanAPI {

ClearPass::ClearPass()
: mClearRectCount(1), mClearAttachmentsCount(1)
{
    FUN_ENTRY(GL_LOG_TRACE);

    memset((void *)&mVkClearRect, 0, sizeof(mVkClearRect));
    mVkClearRect.baseArrayLayer      = 0;
    mVkClearRect.layerCount          = 1;

    mUpdateState.Color   = true;
    mUpdateState.Depth   = true;
    mUpdateState.Stencil = true;
}

ClearPass::~ClearPass()
{
    FUN_ENTRY(GL_LOG_TRACE);
}

}
