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
 *  @file       utils.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Vulkan Utility Functions
 *
 */

#ifndef __VKUTILS_H__
#define __VKUTILS_H__

#include "vulkan/vulkan.h"

size_t                  nBytesOfVkIntFormat(VkFormat format, bool *unsigned_type);
VkFormat                VkIntFormatToVkFloatFormat(VkFormat format);
bool                    VkFormatIsDepthStencil(VkFormat format);
bool                    VkFormatIsDepth(VkFormat format);
bool                    VkFormatIsStencil(VkFormat format);
bool                    VkFormatIsColor(VkFormat format);
const char *            VkResultToString(VkResult res);

#endif // __VKUTILS_H__
