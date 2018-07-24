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
 *  @file       utils.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Vulkan Utility Functions
 *
 *  @section
 *
 *  A variety of Vulkan utility functions.
 *
 */

#include "utils.h"
#include "utils/glLogger.h"
#include "utils/parser_helpers.h"

#ifdef NDEBUG
#   define NOT_REACHED()                                printf("You shouldn't be here. (function %s at line %d of file %s)\n", __func__, __LINE__, __FILE__)
#   define NOT_FOUND_ENUM(inv_enum)                     printf("Invalid enum: %#04x. (function %s at line %d of file %s)\n", inv_enum, __func__, __LINE__, __FILE__)
#else
#   define NOT_REACHED()                                assert(0 && "You shouldn't be here.")
#   define NOT_FOUND_ENUM(inv_enum)                     { printf("Invalid enum: %#04x\n", inv_enum); assert(0); }
#endif // NDEBUG

VkFormat
VkIntFormatToVkFloatFormat(VkFormat format)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(format) {
    case VK_FORMAT_R32_SFLOAT:
    case VK_FORMAT_R32G32_SFLOAT:
    case VK_FORMAT_R32G32B32_SFLOAT:
    case VK_FORMAT_R32G32B32A32_SFLOAT:
    // TODO: The formats below should NOT be here (VkIntFormatToVkFloatFormat).
    // They should be handled properly at this level and changed as needed
    // in the lower layers
    case VK_FORMAT_R8_SNORM:
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R8G8_SNORM:
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R8G8B8_SNORM:
    case VK_FORMAT_R8G8B8_UNORM:
    case VK_FORMAT_R8G8B8A8_SNORM:
    case VK_FORMAT_R8G8B8A8_UNORM:
                                            return format;

    case VK_FORMAT_R8_SINT:
    case VK_FORMAT_R8_UINT:
    case VK_FORMAT_R16_SINT:
    case VK_FORMAT_R16_UINT:
    case VK_FORMAT_R32_SINT:
    case VK_FORMAT_R32_UINT:
    case VK_FORMAT_R16_SSCALED:             return VK_FORMAT_R32_SFLOAT;

    case VK_FORMAT_R8G8_SINT:
    case VK_FORMAT_R8G8_UINT:
    case VK_FORMAT_R16G16_SINT:
    case VK_FORMAT_R16G16_UINT:
    case VK_FORMAT_R32G32_SINT:
    case VK_FORMAT_R32G32_UINT:
    case VK_FORMAT_R16G16_SSCALED:          return VK_FORMAT_R32G32_SFLOAT;

    case VK_FORMAT_R8G8B8_SINT:
    case VK_FORMAT_R8G8B8_UINT:
    case VK_FORMAT_R16G16B16_SINT:
    case VK_FORMAT_R16G16B16_UINT:
    case VK_FORMAT_R32G32B32_SINT:
    case VK_FORMAT_R32G32B32_UINT:
    case VK_FORMAT_R16G16B16_SSCALED:       return VK_FORMAT_R32G32B32_SFLOAT;

    case VK_FORMAT_R8G8B8A8_SINT:
    case VK_FORMAT_R8G8B8A8_UINT:
    case VK_FORMAT_R16G16B16A16_SINT:
    case VK_FORMAT_R16G16B16A16_UINT:
    case VK_FORMAT_R32G32B32A32_SINT:
    case VK_FORMAT_R32G32B32A32_UINT:
    case VK_FORMAT_R16G16B16A16_SSCALED:    return VK_FORMAT_R32G32B32A32_SFLOAT;

    default: { NOT_FOUND_ENUM(format);      return VK_FORMAT_UNDEFINED; }
    }
}

bool
VkFormatIsDepthStencil(VkFormat format)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch (format) {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_S8_UINT:
    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return true;
    default:
        return false;
    }
}

bool
VkFormatIsDepth(VkFormat format)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch (format) {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return true;
    default:
        return false;
    }
}

bool
VkFormatIsStencil(VkFormat format)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch (format) {
    case VK_FORMAT_S8_UINT:
    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return true;
    default:
        return false;
    }
}

bool
VkFormatIsColor(VkFormat format)
{
    FUN_ENTRY(GL_LOG_TRACE);

    return (format != VK_FORMAT_UNDEFINED) && !VkFormatIsDepthStencil(format);
}

size_t
nBytesOfVkIntFormat(VkFormat format, bool *unsigned_type)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(format){
    case VK_FORMAT_R8_SINT:
    case VK_FORMAT_R8G8_SINT:
    case VK_FORMAT_R8G8B8_SINT:
    case VK_FORMAT_R8G8B8A8_SINT:          { *unsigned_type = false; return 1; }

    case VK_FORMAT_R8_UINT:
    case VK_FORMAT_R8G8_UINT:
    case VK_FORMAT_R8G8B8_UINT:
    case VK_FORMAT_R8G8B8A8_UINT:          { *unsigned_type = true; return 1; }

    case VK_FORMAT_R16_SINT:
    case VK_FORMAT_R16G16_SINT:
    case VK_FORMAT_R16G16B16_SINT:
    case VK_FORMAT_R16G16B16A16_SINT:      { *unsigned_type = false; return 2; }

    case VK_FORMAT_R16_UINT:
    case VK_FORMAT_R16G16_UINT:
    case VK_FORMAT_R16G16B16_UINT:
    case VK_FORMAT_R16G16B16A16_UINT:      { *unsigned_type = true; return 2; }

    case VK_FORMAT_R16_SSCALED:
    case VK_FORMAT_R16G16_SSCALED:
    case VK_FORMAT_R16G16B16_SSCALED:
    case VK_FORMAT_R16G16B16A16_SSCALED:

    case VK_FORMAT_R32_SINT:
    case VK_FORMAT_R32G32_SINT:
    case VK_FORMAT_R32G32B32_SINT:
    case VK_FORMAT_R32G32B32A32_SINT:       { *unsigned_type = false; return 4; }

    case VK_FORMAT_R32_UINT:
    case VK_FORMAT_R32G32_UINT:
    case VK_FORMAT_R32G32B32_UINT:
    case VK_FORMAT_R32G32B32A32_UINT:       { *unsigned_type = true; return 4; }

    default: { NOT_FOUND_ENUM(format);                               return 0; }
    }

    return 0;
}

#define CASE_STR(c)                     case VK_ ##c: return "VK_" STRINGIFY(c);

const char *
VkResultToString(VkResult res)
{
    switch(res) {
    CASE_STR(SUCCESS);
    CASE_STR(NOT_READY);
    CASE_STR(TIMEOUT);
    CASE_STR(EVENT_SET);
    CASE_STR(EVENT_RESET);
    CASE_STR(INCOMPLETE);
    CASE_STR(ERROR_OUT_OF_HOST_MEMORY);
    CASE_STR(ERROR_OUT_OF_DEVICE_MEMORY);
    CASE_STR(ERROR_INITIALIZATION_FAILED);
    CASE_STR(ERROR_DEVICE_LOST);
    CASE_STR(ERROR_MEMORY_MAP_FAILED);
    CASE_STR(ERROR_LAYER_NOT_PRESENT);
    CASE_STR(ERROR_EXTENSION_NOT_PRESENT);
    CASE_STR(ERROR_FEATURE_NOT_PRESENT);
    CASE_STR(ERROR_INCOMPATIBLE_DRIVER);
    CASE_STR(ERROR_TOO_MANY_OBJECTS);
    CASE_STR(ERROR_FORMAT_NOT_SUPPORTED);
    CASE_STR(ERROR_FRAGMENTED_POOL);
    CASE_STR(ERROR_SURFACE_LOST_KHR);
    CASE_STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
    CASE_STR(SUBOPTIMAL_KHR);
    CASE_STR(ERROR_OUT_OF_DATE_KHR);
    CASE_STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
    CASE_STR(ERROR_VALIDATION_FAILED_EXT);
    CASE_STR(ERROR_INVALID_SHADER_NV);

    default:  { NOT_REACHED(); return STRINGIFY(GL_INVALID_ENUM); }
    }
}

#undef CASE_STR
