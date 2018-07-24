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
 *  @file       VkToGlConverter.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Utility functions to convert Vulkan modes/types to OpengGL ES
 *
 */

#ifndef __VKTOGLCONVERTER_H__
#define __VKTOGLCONVERTER_H__

#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include "vulkan/vulkan.h"

GLenum    VkFormatToGlInternalformat(VkFormat format);

#endif // __VKTOGLCONVERTER_H__
