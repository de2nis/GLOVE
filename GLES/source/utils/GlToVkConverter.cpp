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
 *  @file       GlToVkConverter.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Utility functions to convert OpengGL ES modes/types to Vulkan
 *
 */

#include "GlToVkConverter.h"
#include "glLogger.h"

#ifdef NDEBUG
#   define NOT_REACHED()                                printf("You shouldn't be here. (function %s at line %d of file %s)\n", __func__, __LINE__, __FILE__)
#   define NOT_FOUND_ENUM(inv_enum)                     printf("Invalid enum: %#04x. (function %s at line %d of file %s)\n", inv_enum, __func__, __LINE__, __FILE__)
#else
#   define NOT_REACHED()                                assert(0 && "You shouldn't be here.")
#   define NOT_FOUND_ENUM(inv_enum)                     { printf("Invalid enum: %#04x\n", inv_enum); assert(0); }
#endif // NDEBUG

VkPolygonMode
GLPrimitiveModeToVkPolygonMode(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_POINTS:                return VK_POLYGON_MODE_POINT;
    case GL_LINE_STRIP:
    case GL_LINE_LOOP:
    case GL_LINES:                 return VK_POLYGON_MODE_LINE;
    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
    case GL_TRIANGLES:             return VK_POLYGON_MODE_FILL;
    default: NOT_FOUND_ENUM(mode); return VK_POLYGON_MODE_FILL;
    }
}

VkSampleCountFlagBits
GlSampleCoverageBitsToVkSampleCountFlagBits(GLint bits)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(bits) {
    case 1 :                return VK_SAMPLE_COUNT_1_BIT;
    case 2 :                return VK_SAMPLE_COUNT_2_BIT;
    case 4 :                return VK_SAMPLE_COUNT_4_BIT;
    case 8 :                return VK_SAMPLE_COUNT_8_BIT;
    case 16:                return VK_SAMPLE_COUNT_16_BIT;
    case 32:                return VK_SAMPLE_COUNT_32_BIT;
    case 64:                return VK_SAMPLE_COUNT_64_BIT;
    default: NOT_REACHED(); return VK_SAMPLE_COUNT_1_BIT;
    }
}

VkStencilOp
GlStencilFuncToVkStencilOp(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_KEEP:                     return VK_STENCIL_OP_KEEP;
    case GL_ZERO:                     return VK_STENCIL_OP_ZERO;
    case GL_REPLACE:                  return VK_STENCIL_OP_REPLACE;
    case GL_INCR:                     return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
    case GL_INCR_WRAP:                return VK_STENCIL_OP_INCREMENT_AND_WRAP;
    case GL_DECR:                     return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
    case GL_DECR_WRAP:                return VK_STENCIL_OP_DECREMENT_AND_WRAP;
    case GL_INVERT:                   return VK_STENCIL_OP_INVERT;
    default: NOT_FOUND_ENUM(mode);    return VK_STENCIL_OP_KEEP;
    }
}

VkLogicOp
GlLogicOpToVkLogicOp(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_CLEAR:                          return VK_LOGIC_OP_CLEAR;
    case GL_AND:                            return VK_LOGIC_OP_AND;
    case GL_AND_REVERSE:                    return VK_LOGIC_OP_AND_REVERSE;
    case GL_COPY:                           return VK_LOGIC_OP_COPY;
    case GL_AND_INVERTED:                   return VK_LOGIC_OP_AND_INVERTED;
    case GL_NOOP:                           return VK_LOGIC_OP_NO_OP;
    case GL_XOR:                            return VK_LOGIC_OP_XOR;
    case GL_OR:                             return VK_LOGIC_OP_OR;
    case GL_NOR:                            return VK_LOGIC_OP_NOR;
    case GL_EQUIV:                          return VK_LOGIC_OP_EQUIVALENT;
    case GL_INVERT:                         return VK_LOGIC_OP_INVERT;
    case GL_OR_REVERSE:                     return VK_LOGIC_OP_OR_REVERSE;
    case GL_COPY_INVERTED:                  return VK_LOGIC_OP_COPY_INVERTED;
    case GL_OR_INVERTED:                    return VK_LOGIC_OP_OR_INVERTED;
    case GL_NAND:                           return VK_LOGIC_OP_NAND;
    case GL_SET:                            return VK_LOGIC_OP_SET;
    default: NOT_FOUND_ENUM(mode);          return VK_LOGIC_OP_CLEAR;
    }
}

VkCompareOp
GlCompareFuncToVkCompareOp(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_NEVER:                          return VK_COMPARE_OP_NEVER;
    case GL_EQUAL:                          return VK_COMPARE_OP_EQUAL;
    case GL_LEQUAL:                         return VK_COMPARE_OP_LESS_OR_EQUAL;
    case GL_GREATER:                        return VK_COMPARE_OP_GREATER;
    case GL_NOTEQUAL:                       return VK_COMPARE_OP_NOT_EQUAL;
    case GL_GEQUAL:                         return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case GL_ALWAYS:                         return VK_COMPARE_OP_ALWAYS;
    case GL_LESS:                           return VK_COMPARE_OP_LESS;
    default: NOT_FOUND_ENUM(mode);          return VK_COMPARE_OP_LESS;
    }
}

VkCullModeFlagBits
GlCullModeToVkCullMode(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_BACK:                           return VK_CULL_MODE_BACK_BIT;
    case GL_FRONT:                          return VK_CULL_MODE_FRONT_BIT;
    case GL_FRONT_AND_BACK:                 return VK_CULL_MODE_FRONT_AND_BACK;
    default: NOT_FOUND_ENUM(mode);          return VK_CULL_MODE_NONE;
    }
}

VkFrontFace
GlFrontFaceToVkFrontFace(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_CW:                             return VK_FRONT_FACE_CLOCKWISE;
    case GL_CCW:                            return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    default: NOT_FOUND_ENUM(mode);          return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }
}

VkSamplerAddressMode
GlTexAddressToVkTexAddress(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_CLAMP_TO_EDGE:                  return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case GL_REPEAT:                         return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case GL_MIRRORED_REPEAT:                return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    default: NOT_REACHED();                 return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    }
}

VkFilter
GlTexFilterToVkTexFilter(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_NEAREST:
    case GL_NEAREST_MIPMAP_NEAREST:
    case GL_NEAREST_MIPMAP_LINEAR:          return VK_FILTER_NEAREST;

    case GL_LINEAR:
    case GL_LINEAR_MIPMAP_NEAREST:
    case GL_LINEAR_MIPMAP_LINEAR:           return VK_FILTER_LINEAR;
    default: NOT_FOUND_ENUM(mode);          return VK_FILTER_LINEAR;
    }
}

VkSamplerMipmapMode
GlTexMipMapModeToVkMipMapMode(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_LINEAR:
    case GL_NEAREST:
    case GL_NEAREST_MIPMAP_NEAREST:
    case GL_LINEAR_MIPMAP_NEAREST:          return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case GL_NEAREST_MIPMAP_LINEAR:
    case GL_LINEAR_MIPMAP_LINEAR:           return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    default: NOT_FOUND_ENUM(mode);          return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }
}

VkPrimitiveTopology
GlPrimitiveTopologyToVkPrimitiveTopology(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_POINTS:                         return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case GL_LINES:                          return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case GL_LINE_LOOP:
    case GL_LINE_STRIP:                     return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case GL_TRIANGLES:                      return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case GL_TRIANGLE_STRIP:                 return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    case GL_TRIANGLE_FAN:                   return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
    default: NOT_FOUND_ENUM(mode);          return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
}

VkBlendFactor
GlBlendFactorToVkBlendFactor(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_ONE:                            return VK_BLEND_FACTOR_ONE;
    case GL_ZERO:                           return VK_BLEND_FACTOR_ZERO;
    case GL_SRC_COLOR:                      return VK_BLEND_FACTOR_SRC_COLOR;
    case GL_ONE_MINUS_SRC_COLOR:            return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case GL_DST_COLOR:                      return VK_BLEND_FACTOR_DST_COLOR;
    case GL_ONE_MINUS_DST_COLOR:            return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case GL_SRC_ALPHA:                      return VK_BLEND_FACTOR_SRC_ALPHA;
    case GL_ONE_MINUS_SRC_ALPHA:            return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case GL_DST_ALPHA:                      return VK_BLEND_FACTOR_DST_ALPHA;
    case GL_ONE_MINUS_DST_ALPHA:            return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case GL_CONSTANT_COLOR:                 return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case GL_ONE_MINUS_CONSTANT_COLOR:       return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case GL_CONSTANT_ALPHA:                 return VK_BLEND_FACTOR_CONSTANT_ALPHA;
    case GL_ONE_MINUS_CONSTANT_ALPHA:       return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    case GL_SRC_ALPHA_SATURATE:             return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    default: NOT_FOUND_ENUM(mode);          return VK_BLEND_FACTOR_ZERO;
    }
}

VkBlendOp
GlBlendEquationToVkBlendOp(GLenum mode)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(mode) {
    case GL_FUNC_ADD:                       return VK_BLEND_OP_ADD;
    case GL_FUNC_SUBTRACT:                  return VK_BLEND_OP_SUBTRACT;
    case GL_FUNC_REVERSE_SUBTRACT:          return VK_BLEND_OP_REVERSE_SUBTRACT;
    default: NOT_FOUND_ENUM(mode);          return VK_BLEND_OP_ADD;
    }
}

VkFormat
GlInternalFormatToVkFormat(GLenum internalformatDepth, GLenum internalformatStencil)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(internalformatDepth) {
    case GL_DEPTH24_STENCIL8_OES:
    case GL_UNSIGNED_INT_24_8_OES:          return VK_FORMAT_D24_UNORM_S8_UINT;
    case GL_DEPTH_COMPONENT16:              return internalformatStencil == GL_STENCIL_INDEX8 ? VK_FORMAT_D16_UNORM_S8_UINT  : VK_FORMAT_D16_UNORM;
    case GL_DEPTH_COMPONENT24_OES:          return internalformatStencil == GL_STENCIL_INDEX8 ? VK_FORMAT_D24_UNORM_S8_UINT  : VK_FORMAT_X8_D24_UNORM_PACK32;
    case GL_DEPTH_COMPONENT32_OES:          return internalformatStencil == GL_STENCIL_INDEX8 ? VK_FORMAT_D32_SFLOAT_S8_UINT : VK_FORMAT_D32_SFLOAT;
    default:
        switch(internalformatStencil) {
        case GL_STENCIL_INDEX1_OES:
        case GL_STENCIL_INDEX4_OES:
        case GL_STENCIL_INDEX8:             return VK_FORMAT_S8_UINT;
        }
    }

    return VK_FORMAT_UNDEFINED;
}

VkFormat
GlInternalFormatToVkFormat(GLenum internalformat)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(internalformat) {
    case GL_RGB565:                           return VK_FORMAT_R5G6B5_UNORM_PACK16;
    case GL_RGBA4:                            return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
    case GL_RGB5_A1:                          return VK_FORMAT_R5G5B5A1_UNORM_PACK16;

    case GL_RGB8_OES:                         return VK_FORMAT_R8G8B8_UNORM;

    case GL_ALPHA:
    case GL_LUMINANCE:
    case GL_LUMINANCE_ALPHA:
    case GL_RGBA:
    case GL_RGBA8_OES:                        return VK_FORMAT_R8G8B8A8_UNORM;

    case GL_BGRA8_EXT:
    case GL_BGRA_EXT:                         return VK_FORMAT_B8G8R8A8_UNORM;

    case GL_DEPTH_COMPONENT16:                return VK_FORMAT_D16_UNORM;
    case GL_DEPTH24_STENCIL8_OES:
    case GL_UNSIGNED_INT_24_8_OES:            return VK_FORMAT_D24_UNORM_S8_UINT;
    case GL_DEPTH_COMPONENT24_OES:            return VK_FORMAT_X8_D24_UNORM_PACK32;
    case GL_DEPTH_COMPONENT32_OES:            return VK_FORMAT_D32_SFLOAT;

    case GL_STENCIL_INDEX1_OES:
    case GL_STENCIL_INDEX4_OES:
    case GL_STENCIL_INDEX8:                   return VK_FORMAT_S8_UINT;

    default: { NOT_FOUND_ENUM(internalformat);return VK_FORMAT_UNDEFINED; }
    }
}

VkFormat
GlAttribTypeToVkFormat(GLenum type)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(type) {
    case GL_BOOL:                           return VK_FORMAT_R8_UINT;
    case GL_BOOL_VEC2:                      return VK_FORMAT_R8G8_UINT;
    case GL_BOOL_VEC3:                      return VK_FORMAT_R8G8B8_UINT;
    case GL_BOOL_VEC4:                      return VK_FORMAT_R8G8B8A8_UINT;

    case GL_INT:                            return VK_FORMAT_R32_SINT;
    case GL_INT_VEC2:                       return VK_FORMAT_R32G32_SINT;
    case GL_INT_VEC3:                       return VK_FORMAT_R32G32B32_SINT;
    case GL_INT_VEC4:                       return VK_FORMAT_R32G32B32A32_SINT;

    case GL_FLOAT:                          return VK_FORMAT_R32_SFLOAT;
    case GL_FLOAT_VEC2:                     return VK_FORMAT_R32G32_SFLOAT;
    case GL_FLOAT_VEC3:                     return VK_FORMAT_R32G32B32_SFLOAT;
    case GL_FLOAT_VEC4:                     return VK_FORMAT_R32G32B32A32_SFLOAT;

    // TODO: missing cases
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT4:
    default: { NOT_FOUND_ENUM(type);        return VK_FORMAT_UNDEFINED; }
    }
}

VkFormat
GlAttribPointerToVkFormat(size_t nElements, GLenum type)
{
    FUN_ENTRY(GL_LOG_TRACE);

    switch(type) {
    case GL_FLOAT:
        switch(nElements) {
        case 1:                             return VK_FORMAT_R32_SFLOAT;
        case 2:                             return VK_FORMAT_R32G32_SFLOAT;
        case 3:                             return VK_FORMAT_R32G32B32_SFLOAT;
        case 4:                             return VK_FORMAT_R32G32B32A32_SFLOAT;
        default: { NOT_REACHED();           return VK_FORMAT_UNDEFINED; }
        }
    break;

    case GL_INT:
        switch(nElements) {
        case 1:                             return VK_FORMAT_R32_SINT;
        case 2:                             return VK_FORMAT_R32G32_SINT;
        case 3:                             return VK_FORMAT_R32G32B32_SINT;
        case 4:                             return VK_FORMAT_R32G32B32A32_SINT;
        default: { NOT_REACHED();           return VK_FORMAT_UNDEFINED; }
        }
    break;

    case GL_UNSIGNED_INT:
        switch(nElements) {
        case 1:                             return VK_FORMAT_R32_UINT;
        case 2:                             return VK_FORMAT_R32G32_UINT;
        case 3:                             return VK_FORMAT_R32G32B32_UINT;
        case 4:                             return VK_FORMAT_R32G32B32A32_UINT;
        default: { NOT_REACHED();           return VK_FORMAT_UNDEFINED; }
        }
    break;

    case GL_BYTE:
        switch(nElements) {
        case 1:                             return VK_FORMAT_R8_SNORM;
        case 2:                             return VK_FORMAT_R8G8_SNORM;
        case 3:                             return VK_FORMAT_R8G8B8_SNORM;
        case 4:                             return VK_FORMAT_R8G8B8A8_SNORM;
        default: { NOT_REACHED();           return VK_FORMAT_UNDEFINED; }
        }
    break;

    case GL_UNSIGNED_BYTE:
        switch(nElements) {
        case 1:                             return VK_FORMAT_R8_UNORM;
        case 2:                             return VK_FORMAT_R8G8_UNORM;
        case 3:                             return VK_FORMAT_R8G8B8_UNORM;
        case 4:                             return VK_FORMAT_R8G8B8A8_UNORM;
        default: { NOT_REACHED();           return VK_FORMAT_UNDEFINED; }
        }
    break;

    case GL_SHORT:
        switch(nElements) {
        case 1:                             return VK_FORMAT_R16_SINT;
        case 2:                             return VK_FORMAT_R16G16_SINT;
        case 3:                             return VK_FORMAT_R16G16B16_SINT;
        case 4:                             return VK_FORMAT_R16G16B16A16_SINT;
        default: { NOT_REACHED();           return VK_FORMAT_UNDEFINED; }
        }
    break;

    case GL_UNSIGNED_SHORT:
        switch(nElements) {
        case 1:                             return VK_FORMAT_R16_UINT;
        case 2:                             return VK_FORMAT_R16G16_UINT;
        case 3:                             return VK_FORMAT_R16G16B16_UINT;
        case 4:                             return VK_FORMAT_R16G16B16A16_UINT;
        default: { NOT_REACHED();           return VK_FORMAT_UNDEFINED; }
        }
    break;

    case GL_FIXED:
        switch(nElements) {
        case 1:                             return VK_FORMAT_R16_SSCALED;
        case 2:                             return VK_FORMAT_R16G16_SSCALED;
        case 3:                             return VK_FORMAT_R16G16B16_SSCALED;
        case 4:                             return VK_FORMAT_R16G16B16A16_SSCALED;
    default: { NOT_REACHED();               return VK_FORMAT_UNDEFINED; }
	}

    default: { NOT_REACHED();               return VK_FORMAT_UNDEFINED; }
    }

    return VK_FORMAT_UNDEFINED;
}
