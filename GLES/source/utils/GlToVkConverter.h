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
 *  @file       GlToVkConverter.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Utility functions to convert OpengGL ES modes/types to Vulkan
 *
 */

#ifndef __GLTOVKCONVERTER_H__
#define __GLTOVKCONVERTER_H__

#include "GLES/gl.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include "vulkan/vulkan.h"

inline VkBool32         GlBooleanToVkBool(GLboolean value)                      { return value ? VK_TRUE : VK_FALSE; }
VkBlendFactor           GlBlendFactorToVkBlendFactor(GLenum mode);
VkLogicOp           	GlLogicOpToVkLogicOp(GLenum mode);
VkBlendOp               GlBlendEquationToVkBlendOp(GLenum mode);
VkCompareOp             GlCompareFuncToVkCompareOp(GLenum mode);
VkCullModeFlagBits      GlCullModeToVkCullMode(GLenum mode);
VkFrontFace             GlFrontFaceToVkFrontFace(GLenum mode);
VkPolygonMode           GLPrimitiveModeToVkPolygonMode(GLenum mode);
VkPrimitiveTopology     GlPrimitiveTopologyToVkPrimitiveTopology(GLenum mode);
VkSampleCountFlagBits   GlSampleCoverageBitsToVkSampleCountFlagBits(GLint bits);
VkStencilOp             GlStencilFuncToVkStencilOp(GLenum mode);
VkSamplerAddressMode    GlTexAddressToVkTexAddress(GLenum mode);
VkFilter                GlTexFilterToVkTexFilter(GLenum mode);
VkSamplerMipmapMode     GlTexMipMapModeToVkMipMapMode(GLenum mode);
VkFormat                GlTexInternalFormatToVkFormat(GLenum internalformat);
VkFormat                GlInternalFormatToVkFormat(GLenum internalformat);
VkFormat                GlInternalFormatToVkFormat(GLenum internalformatDepth, GLenum internalformatStencil);
VkFormat                GlAttribPointerToVkFormat(size_t nElements, GLenum type);
VkFormat                GlAttribTypeToVkFormat(GLenum type);

#endif // __GLTOVKCONVERTER_H__
