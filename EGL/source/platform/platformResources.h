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
 *  @file       platformResources.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Interface class to host platformResources. It must contain at least the swapchain resources. It used in EGLSurface_t 
 *
 */

#ifndef __PLATFORM_RESOURCES_H__
#define __PLATFORM_RESOURCES_H__

#include <stdint.h>
#include "utils/eglLogger.h"

class PlatformResources
{
public:
    PlatformResources()          { FUN_ENTRY(EGL_LOG_TRACE); }
    virtual ~PlatformResources() { FUN_ENTRY(EGL_LOG_TRACE); }

    virtual uint32_t    GetSwapchainImageCount() = 0;
    virtual void       *GetSwapchainImages()     = 0;
};

#endif // __PLATFORM_RESOURCES_H__
