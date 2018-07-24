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
 *  @file       VkToGlConverter.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Utility functions to convert Vulkan modes/types to OpengGL ES
 *
 */

#include "VkToGlConverter.h"
#include "glLogger.h"

#ifdef NDEBUG
#   define NOT_REACHED()                                printf("You shouldn't be here. (function %s at line %d of file %s)\n", __func__, __LINE__, __FILE__)
#   define NOT_FOUND_ENUM(inv_enum)                     printf("Invalid enum: %#04x. (function %s at line %d of file %s)\n", inv_enum, __func__, __LINE__, __FILE__)
#else
#   define NOT_REACHED()                                assert(0 && "You shouldn't be here.")
#   define NOT_FOUND_ENUM(inv_enum)                     { printf("Invalid enum: %#04x\n", inv_enum); assert(0); }
#endif // NDEBUG

GLenum
VkFormatToGlInternalformat(VkFormat format)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(format) {
    case VK_FORMAT_R5G6B5_UNORM_PACK16:     return GL_RGB565;
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:   return GL_RGBA4;
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:   return GL_RGB5_A1;
    case VK_FORMAT_R8G8B8_UNORM:            return GL_RGB8_OES;

    case VK_FORMAT_R8G8B8A8_UINT:
    case VK_FORMAT_R8G8B8A8_USCALED:
    case VK_FORMAT_R8G8B8A8_SSCALED:
    case VK_FORMAT_R8G8B8A8_SRGB:
    case VK_FORMAT_R8G8B8A8_UNORM:          return GL_RGBA8_OES;

    case VK_FORMAT_B8G8R8A8_SRGB:
    case VK_FORMAT_B8G8R8A8_UNORM:          return GL_BGRA8_EXT;

    case VK_FORMAT_D16_UNORM:               return GL_DEPTH_COMPONENT16;
    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:       return GL_DEPTH24_STENCIL8_OES;
    case VK_FORMAT_X8_D24_UNORM_PACK32:     return GL_DEPTH_COMPONENT24_OES;
    case VK_FORMAT_D32_SFLOAT:              return GL_DEPTH_COMPONENT32_OES;
    case VK_FORMAT_S8_UINT:                 return GL_STENCIL_INDEX8;
    case VK_FORMAT_UNDEFINED:
    default: { NOT_FOUND_ENUM(format);      return GL_INVALID_VALUE; }
    }
}
