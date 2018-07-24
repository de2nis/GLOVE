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
 *  @file       contextStateQueries.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to State Queries
 *
 */

#include "context.h"

static glove_program_binary_formats_e glove_program_binary_formats[GLOVE_MAX_BINARY_FORMATS] = {
    GLOVE_HOST_X86_BINARY,
    GLOVE_HOST_ARM_BINARY,
    GLOVE_DEV_BINARY
};

void
Context::GetBooleanv(GLenum pname, GLboolean* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    switch(pname) {
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT:   *params = GL_TRUE; break;
    case GL_IMPLEMENTATION_COLOR_READ_TYPE:     *params = GL_TRUE; break;
    case GL_TEXTURE_BINDING_2D:                 *params = mResourceManager.GetTextureID(mStateManager.GetActiveObjectsState()->GetActiveTexture(GL_TEXTURE_2D)) == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_TEXTURE_BINDING_CUBE_MAP:           *params = mResourceManager.GetTextureID(mStateManager.GetActiveObjectsState()->GetActiveTexture(GL_TEXTURE_CUBE_MAP)) == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_FRAMEBUFFER_BINDING:                *params = mStateManager.GetActiveObjectsState()->GetActiveFramebufferObjectID() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_RENDERBUFFER_BINDING:               *params = mStateManager.GetActiveObjectsState()->GetActiveRenderbufferObjectID() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_ACTIVE_TEXTURE:                     *params = mStateManager.GetActiveObjectsState()->GetActiveTextureUnit() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_CURRENT_PROGRAM:                    *params = GetProgramId(mStateManager.GetActiveShaderProgram()) == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_ARRAY_BUFFER_BINDING:               *params = mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ARRAY_BUFFER)         ? mResourceManager.GetBufferID(mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ARRAY_BUFFER)        ) == 0 ? GL_FALSE : GL_TRUE : GL_FALSE; break;
    case GL_ELEMENT_ARRAY_BUFFER_BINDING:       *params = mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ELEMENT_ARRAY_BUFFER) ? mResourceManager.GetBufferID(mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ELEMENT_ARRAY_BUFFER)) == 0 ? GL_FALSE : GL_TRUE : GL_FALSE; break;
    case GL_NUM_SHADER_BINARY_FORMATS:          *params = GLOVE_NUM_SHADER_BINARY_FORMATS == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_COMPRESSED_TEXTURE_FORMATS:         *params = GL_FALSE; break;
    case GL_NUM_COMPRESSED_TEXTURE_FORMATS:     *params = GL_FALSE; break;
    case GL_BLEND_COLOR:                        mStateManager.GetFragmentOperationsState()->GetBlendingColor(params); break;
    case GL_BLEND_DST_ALPHA:                    *params = mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationAlpha() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_BLEND_DST_RGB:                      *params = mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationRGB() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_BLEND_EQUATION_ALPHA:               *params = mStateManager.GetFragmentOperationsState()->GetBlendingEquationAlpha() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_BLEND_EQUATION_RGB:                 *params = mStateManager.GetFragmentOperationsState()->GetBlendingEquationRGB() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_BLEND_SRC_ALPHA:                    *params = mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceAlpha() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_BLEND_SRC_RGB:                      *params = mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceRGB() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_DEPTH_CLEAR_VALUE:                  *params = mStateManager.GetFramebufferOperationsState()->GetClearDepth() == 0.0f ? GL_FALSE : GL_TRUE;
    case GL_PACK_ALIGNMENT:                     *params = mStateManager.GetPixelStorageState()->GetPixelStorePack() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_UNPACK_ALIGNMENT:                   *params = mStateManager.GetPixelStorageState()->GetPixelStoreUnpack() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_WRITEMASK:                  *params = mStateManager.GetFramebufferOperationsState()->GetStencilMaskFront() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_BACK_WRITEMASK:             *params = mStateManager.GetFramebufferOperationsState()->GetStencilMaskBack()  == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_CLEAR_VALUE:                *params = mStateManager.GetFramebufferOperationsState()->GetClearStencil() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_FUNC:                       *params = mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareFront() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_VALUE_MASK:                 *params = mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskFront() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_REF:                        *params = mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefFront() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_FAIL:                       *params = mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailFront() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_PASS_DEPTH_FAIL:            *params = mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailFront() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_PASS_DEPTH_PASS:            *params = mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassFront() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_BACK_FUNC:                  *params = mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareBack() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_BACK_FAIL:                  *params = mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailBack() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_BACK_PASS_DEPTH_FAIL:       *params = mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailBack() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_BACK_PASS_DEPTH_PASS:       *params = mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassBack() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_BACK_REF:                   *params = mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefBack() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_STENCIL_BACK_VALUE_MASK:            *params = mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskBack() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_BLEND:                              *params = mStateManager.GetFragmentOperationsState()->GetBlendingEnabled(); break;
    case GL_COLOR_WRITEMASK:                    mStateManager.GetFramebufferOperationsState()->GetColorMask(params); break;
    case GL_COLOR_CLEAR_VALUE:                  mStateManager.GetFramebufferOperationsState()->GetClearColor(params); break;
    case GL_CULL_FACE:                          *params = mStateManager.GetRasterizationState()->GetCullEnabled(); break;
    case GL_CULL_FACE_MODE:                     *params = mStateManager.GetRasterizationState()->GetCullFace() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_DEPTH_TEST:                         *params = mStateManager.GetFragmentOperationsState()->GetDepthTestEnabled(); break;
    case GL_DEPTH_FUNC:                         *params = mStateManager.GetFragmentOperationsState()->GetDepthTestFunc() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_DEPTH_WRITEMASK:                    *params = mStateManager.GetFramebufferOperationsState()->GetDepthMask(); break;
    case GL_DEPTH_RANGE:                        params[0] = mStateManager.GetViewportTransformationState()->GetMinDepthRange() == 0.0f ? GL_FALSE : GL_TRUE;
                                                params[1] = mStateManager.GetViewportTransformationState()->GetMaxDepthRange() == 0.0f ? GL_FALSE : GL_TRUE; break;
    case GL_DITHER:                             *params = mStateManager.GetFragmentOperationsState()->GetDitheringEnabled(); break;
    case GL_FRONT_FACE:                         *params = mStateManager.GetRasterizationState()->GetFrontFace() == 0.0f ? GL_FALSE : GL_TRUE; break;
    case GL_LINE_WIDTH:                         *params = mStateManager.GetRasterizationState()->GetLineWidth() == 0.0f ? GL_FALSE : GL_TRUE; break;
    case GL_POLYGON_OFFSET_FILL:                *params = mStateManager.GetRasterizationState()->GetPolygonOffsetFillEnabled(); break;
    case GL_POLYGON_OFFSET_FACTOR:              *params = mStateManager.GetRasterizationState()->GetPolygonOffsetFactor() == 0.0f ? GL_FALSE : GL_TRUE; break;
    case GL_POLYGON_OFFSET_UNITS:               *params = mStateManager.GetRasterizationState()->GetPolygonOffsetUnits() == 0.0f ? GL_FALSE : GL_TRUE; break;
    case GL_GENERATE_MIPMAP_HINT:               *params = mStateManager.GetHintAspectsState()->GetMode(GL_GENERATE_MIPMAP_HINT) == 0 ? GL_FALSE : GL_TRUE; break;

    case GL_SAMPLE_ALPHA_TO_COVERAGE:           *params = mStateManager.GetFragmentOperationsState()->GetSampleAlphaToCoverageEnabled(); break;
    case GL_SAMPLE_COVERAGE_INVERT:             *params = mStateManager.GetFragmentOperationsState()->GetSampleCoverageInvert(); break;
    case GL_SAMPLE_COVERAGE_VALUE:              *params = mStateManager.GetFragmentOperationsState()->GetSampleCoverageValue() == 0.0f ? GL_FALSE : GL_TRUE; break;
    case GL_SAMPLE_COVERAGE:                    *params = mStateManager.GetFragmentOperationsState()->GetSampleCoverageEnabled(); break;
    case GL_SCISSOR_TEST:                       *params = mStateManager.GetFragmentOperationsState()->GetScissorTestEnabled(); break;
    case GL_STENCIL_TEST:                       *params = mStateManager.GetFragmentOperationsState()->GetStencilTestEnabled(); break;
    case GL_SAMPLES:                            *params = mStateManager.GetFragmentOperationsState()->GetSampleCoverageBits() == 0 ? GL_FALSE : GL_TRUE; break;
    case GL_SAMPLE_BUFFERS:                     *params = mStateManager.GetFragmentOperationsState()->GetMultiSamplingEnabled(); break;
    case GL_SCISSOR_BOX:                        mStateManager.GetFragmentOperationsState()->GetScissorRect(params); break;
    case GL_VIEWPORT:                           mStateManager.GetViewportTransformationState()->GetViewportRect(params); break;
    case GL_MAX_VIEWPORT_DIMS:                  params[0] = GL_TRUE;
                                                params[1] = GL_TRUE; break;
    case GL_ALIASED_LINE_WIDTH_RANGE:           params[0] = GL_TRUE;
                                                params[1] = GL_TRUE; break;
    case GL_ALIASED_POINT_SIZE_RANGE:           params[0] = GL_TRUE;
                                                params[1] = GL_TRUE; break;
    case GL_RED_BITS:                           GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), params, NULL, NULL, NULL, NULL, NULL); break;
    case GL_BLUE_BITS:                          GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), NULL, params, NULL, NULL, NULL, NULL); break;
    case GL_GREEN_BITS:                         GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), NULL, NULL, params, NULL, NULL, NULL); break;
    case GL_ALPHA_BITS:                         GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), NULL, NULL, NULL, params, NULL, NULL); break;
    case GL_DEPTH_BITS:                         GlFormatToStorageBits(mWriteFBO->GetDepthAttachmentTexture()   ? mWriteFBO->GetDepthAttachmentTexture()->GetInternalFormat()   : GL_INVALID_VALUE, NULL, NULL, NULL, NULL, params, NULL); break;
    case GL_STENCIL_BITS:                       GlFormatToStorageBits(mWriteFBO->GetStencilAttachmentTexture() ? mWriteFBO->GetStencilAttachmentTexture()->GetInternalFormat() : GL_INVALID_VALUE, NULL, NULL, NULL, NULL, NULL, params); break;
    case GL_SUBPIXEL_BITS:
    case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
    case GL_MAX_VERTEX_UNIFORM_VECTORS:
    case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
    case GL_MAX_VERTEX_ATTRIBS:
    case GL_MAX_VARYING_VECTORS:
    case GL_MAX_TEXTURE_IMAGE_UNITS:
    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
    case GL_MAX_RENDERBUFFER_SIZE:
    case GL_MAX_TEXTURE_SIZE:
    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
    case GL_SHADER_COMPILER:
    case GL_TEXTURE_BINDING_EXTERNAL_OES:       *params = GL_TRUE; break;
    default:                                    RecordError(GL_INVALID_ENUM); break;
    }
}



void
Context::GetIntegerv(GLenum pname, GLint* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    switch(pname) {
    case GL_MAX_VERTEX_ATTRIBS:                 *params = GLOVE_MAX_VERTEX_ATTRIBS; break;
    case GL_MAX_VERTEX_UNIFORM_VECTORS:         *params = GLOVE_MAX_VERTEX_UNIFORM_VECTORS; break;
    case GL_MAX_VARYING_VECTORS:                *params = GLOVE_MAX_VARYING_VECTORS; break;
    case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:   *params = GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS; break;
    case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:     *params = GLOVE_MAX_VERTEX_TEXTURE_IMAGE_UNITS; break;
    case GL_MAX_TEXTURE_IMAGE_UNITS:            *params = GLOVE_MAX_TEXTURE_IMAGE_UNITS; break;
    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:       *params = GLOVE_MAX_FRAGMENT_UNIFORM_VECTORS; break;
    case GL_MAX_RENDERBUFFER_SIZE:              *params = GLOVE_MAX_RENDERBUFFER_SIZE; break;
    case GL_MAX_TEXTURE_SIZE:                   *params = GLOVE_MAX_TEXTURE_SIZE; break;
    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:          *params = GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE; break;
    case GL_MAX_VIEWPORT_DIMS:                  params[0] = GLOVE_MAX_TEXTURE_SIZE;
                                                params[1] = GLOVE_MAX_TEXTURE_SIZE; break;
    case GL_FRAMEBUFFER_BINDING:                *params = mStateManager.GetActiveObjectsState()->GetActiveFramebufferObjectID(); break;
    case GL_RENDERBUFFER_BINDING:               *params = mStateManager.GetActiveObjectsState()->GetActiveRenderbufferObjectID(); break;
    case GL_CURRENT_PROGRAM:                    *params = GetProgramId(mStateManager.GetActiveShaderProgram()); break;
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT:   *params = GL_RGBA; break;
    case GL_IMPLEMENTATION_COLOR_READ_TYPE:     *params = GL_UNSIGNED_BYTE; break;
    case GL_ARRAY_BUFFER_BINDING:               *params = mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ARRAY_BUFFER)         ? mResourceManager.GetBufferID(mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ARRAY_BUFFER))   : 0; break;
    case GL_ELEMENT_ARRAY_BUFFER_BINDING:       *params = mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ELEMENT_ARRAY_BUFFER) ? mResourceManager.GetBufferID(mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ELEMENT_ARRAY_BUFFER)) : 0; break;
    case GL_RED_BITS:                           GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), params, NULL, NULL, NULL, NULL, NULL); break;
    case GL_BLUE_BITS:                          GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), NULL, params, NULL, NULL, NULL, NULL); break;
    case GL_GREEN_BITS:                         GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), NULL, NULL, params, NULL, NULL, NULL); break;
    case GL_ALPHA_BITS:                         GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), NULL, NULL, NULL, params, NULL, NULL); break;
    case GL_DEPTH_BITS:                         GlFormatToStorageBits(mWriteFBO->GetDepthAttachmentTexture()   ? mWriteFBO->GetDepthAttachmentTexture()->GetInternalFormat()   : GL_INVALID_VALUE, NULL, NULL, NULL, NULL, params, NULL); break;
    case GL_STENCIL_BITS:                       GlFormatToStorageBits(mWriteFBO->GetStencilAttachmentTexture() ? mWriteFBO->GetStencilAttachmentTexture()->GetInternalFormat() : GL_INVALID_VALUE, NULL, NULL, NULL, NULL, NULL, params); break;
    case GL_GENERATE_MIPMAP_HINT:               *params = static_cast<GLint>(mStateManager.GetHintAspectsState()->GetMode(GL_GENERATE_MIPMAP_HINT)); break;
    case GL_BLEND:                              *params = mStateManager.GetFragmentOperationsState()->GetBlendingEnabled(); break;
    case GL_COLOR_CLEAR_VALUE:                  mStateManager.GetFramebufferOperationsState()->GetClearColor(params); break;
    case GL_COLOR_WRITEMASK:                    mStateManager.GetFramebufferOperationsState()->GetColorMask(params); break;
    case GL_CULL_FACE:                          *params = mStateManager.GetRasterizationState()->GetCullEnabled(); break;
    case GL_CULL_FACE_MODE:                     *params = mStateManager.GetRasterizationState()->GetCullFace(); break;
    case GL_DEPTH_TEST:                         *params = mStateManager.GetFragmentOperationsState()->GetDepthTestEnabled(); break;
    case GL_DEPTH_WRITEMASK:                    *params = mStateManager.GetFramebufferOperationsState()->GetDepthMask(); break;
    case GL_DEPTH_CLEAR_VALUE:                  *params = mStateManager.GetFramebufferOperationsState()->GetClearDepth() == 1.0f ?
                                                          0x7fffffff :  static_cast<GLint>(mStateManager.GetFramebufferOperationsState()->GetClearDepth() * 0x7fffffff); break;
    case GL_DEPTH_FUNC:                         *params = mStateManager.GetFragmentOperationsState()->GetDepthTestFunc(); break;
    case GL_DEPTH_RANGE:                        params[0] = mStateManager.GetViewportTransformationState()->GetMinDepthRange() == 1.0f ?
                                                          0x7fffffff :  static_cast<GLint>(mStateManager.GetViewportTransformationState()->GetMinDepthRange() * 0x7fffffff);
                                                params[1] = mStateManager.GetViewportTransformationState()->GetMaxDepthRange() == 1.0f ?
                                                          0x7fffffff :  static_cast<GLint>(mStateManager.GetViewportTransformationState()->GetMaxDepthRange() * 0x7fffffff); break;
    case GL_DITHER:                             *params = mStateManager.GetFragmentOperationsState()->GetDitheringEnabled(); break;
    case GL_FRONT_FACE:                         *params = mStateManager.GetRasterizationState()->GetFrontFace(); break;
    case GL_LINE_WIDTH:                         *params = mStateManager.GetRasterizationState()->GetLineWidth(); break;
    case GL_PACK_ALIGNMENT:                     *params = mStateManager.GetPixelStorageState()->GetPixelStorePack(); break;
    case GL_POLYGON_OFFSET_FACTOR:              *params = static_cast<GLint>(std::roundf(mStateManager.GetRasterizationState()->GetPolygonOffsetFactor())); break;
    case GL_POLYGON_OFFSET_UNITS:               *params = static_cast<GLint>(std::roundf(mStateManager.GetRasterizationState()->GetPolygonOffsetUnits())); break;
    case GL_POLYGON_OFFSET_FILL:                *params = mStateManager.GetRasterizationState()->GetPolygonOffsetFillEnabled(); break;
    case GL_SCISSOR_BOX:                        mStateManager.GetFragmentOperationsState()->GetScissorRect(params); break;
    case GL_UNPACK_ALIGNMENT:                   *params = mStateManager.GetPixelStorageState()->GetPixelStoreUnpack(); break;
    case GL_VIEWPORT:                           mStateManager.GetViewportTransformationState()->GetViewportRect(params); break;
    case GL_SCISSOR_TEST:                       *params = mStateManager.GetFragmentOperationsState()->GetScissorTestEnabled(); break;
    case GL_STENCIL_TEST:                       *params = mStateManager.GetFragmentOperationsState()->GetStencilTestEnabled(); break;
    case GL_ALIASED_LINE_WIDTH_RANGE:           params[0] = 1;
                                                params[1] = 1; break;
    case GL_ALIASED_POINT_SIZE_RANGE:           params[0] = 1;
                                                params[1] = 1; break;
    case GL_COMPRESSED_TEXTURE_FORMATS:         *params = 0; break;
    case GL_NUM_COMPRESSED_TEXTURE_FORMATS:     *params = 0; break;
    case GL_SAMPLES:                            *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetSampleCoverageBits()); break;
    case GL_SAMPLE_BUFFERS:                     *params = mStateManager.GetFragmentOperationsState()->GetMultiSamplingEnabled(); break;
    case GL_SAMPLE_COVERAGE:                    *params = mStateManager.GetFragmentOperationsState()->GetSampleCoverageEnabled(); break;
    case GL_SAMPLE_COVERAGE_INVERT:             *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetSampleCoverageInvert()); break;
    case GL_SAMPLE_COVERAGE_VALUE:              *params = static_cast<GLint>(roundf(mStateManager.GetFragmentOperationsState()->GetSampleCoverageValue())); break;
    case GL_SAMPLE_ALPHA_TO_COVERAGE:           *params = mStateManager.GetFragmentOperationsState()->GetSampleAlphaToCoverageEnabled(); break;
    case GL_SHADER_COMPILER:                    *params = 1; break;
    case GL_SUBPIXEL_BITS:                      *params = 4; break;
    case GL_TEXTURE_BINDING_2D:                 *params = static_cast<GLint>(mResourceManager.GetTextureID(mStateManager.GetActiveObjectsState()->GetActiveTexture(GL_TEXTURE_2D))); break;
    case GL_TEXTURE_BINDING_CUBE_MAP:           *params = static_cast<GLint>(mResourceManager.GetTextureID(mStateManager.GetActiveObjectsState()->GetActiveTexture(GL_TEXTURE_CUBE_MAP))); break;
    case GL_ACTIVE_TEXTURE:                     *params = static_cast<GLint>(mStateManager.GetActiveObjectsState()->GetActiveTextureUnit()); break;
    case GL_STENCIL_WRITEMASK:                  *params = static_cast<GLint>(mStateManager.GetFramebufferOperationsState()->GetStencilMaskFront()); break;
    case GL_STENCIL_BACK_WRITEMASK:             *params = static_cast<GLint>(mStateManager.GetFramebufferOperationsState()->GetStencilMaskBack());  break;
    case GL_STENCIL_CLEAR_VALUE:                *params = static_cast<GLint>(mStateManager.GetFramebufferOperationsState()->GetClearStencil()); break;
    case GL_STENCIL_FUNC:                       *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareFront()); break;
    case GL_STENCIL_VALUE_MASK:                 *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskFront()); break;
    case GL_STENCIL_REF:                        *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefFront()); break;
    case GL_STENCIL_FAIL:                       *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailFront()); break;
    case GL_STENCIL_PASS_DEPTH_FAIL:            *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailFront()); break;
    case GL_STENCIL_PASS_DEPTH_PASS:            *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassFront()); break;
    case GL_STENCIL_BACK_FUNC:                  *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareBack()); break;
    case GL_STENCIL_BACK_FAIL:                  *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailBack()); break;
    case GL_STENCIL_BACK_PASS_DEPTH_FAIL:       *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailBack()); break;
    case GL_STENCIL_BACK_PASS_DEPTH_PASS:       *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassBack()); break;
    case GL_STENCIL_BACK_REF:                   *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefBack()); break;
    case GL_STENCIL_BACK_VALUE_MASK:            *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskBack()); break;
    case GL_BLEND_COLOR:                        mStateManager.GetFragmentOperationsState()->GetBlendingColor(params); break;
    case GL_BLEND_DST_ALPHA:                    *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationAlpha()); break;
    case GL_BLEND_DST_RGB:                      *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationRGB()); break;
    case GL_BLEND_EQUATION_ALPHA:               *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetBlendingEquationAlpha()); break;
    case GL_BLEND_EQUATION_RGB:                 *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetBlendingEquationRGB()); break;
    case GL_BLEND_SRC_ALPHA:                    *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceAlpha()); break;
    case GL_BLEND_SRC_RGB:                      *params = static_cast<GLint>(mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceRGB()); break;
    case GL_NUM_SHADER_BINARY_FORMATS:          *params = GLOVE_NUM_SHADER_BINARY_FORMATS; break;
    case GL_NUM_PROGRAM_BINARY_FORMATS_OES:     *params = GLOVE_NUM_PROGRAM_BINARY_FORMATS; break;
    case GL_PROGRAM_BINARY_FORMATS_OES:         params = reinterpret_cast<GLint *>(&glove_program_binary_formats); break;
    default:                                    RecordError(GL_INVALID_ENUM); break;
    }
}

void
Context::GetFloatv(GLenum pname, GLfloat* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    switch(pname) {
    case GL_ARRAY_BUFFER_BINDING:               *params = mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ARRAY_BUFFER) ? mResourceManager.GetBufferID(mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ARRAY_BUFFER)) : 0; break;
    case GL_BLEND:                              *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetBlendingEnabled()); break;
    case GL_BLEND_COLOR:                        mStateManager.GetFragmentOperationsState()->GetBlendingColor(params); break;
    case GL_BLEND_DST_ALPHA:                    *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationAlpha()); break;
    case GL_BLEND_DST_RGB:                      *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetBlendingFactorDestinationRGB()); break;
    case GL_BLEND_EQUATION_ALPHA:               *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetBlendingEquationAlpha()); break;
    case GL_BLEND_EQUATION_RGB:                 *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetBlendingEquationRGB()); break;
    case GL_BLEND_SRC_ALPHA:                    *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceAlpha()); break;
    case GL_BLEND_SRC_RGB:                      *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetBlendingFactorSourceRGB()); break;
    case GL_COLOR_CLEAR_VALUE:                  mStateManager.GetFramebufferOperationsState()->GetClearColor(params); break;
    case GL_COLOR_WRITEMASK:                    mStateManager.GetFramebufferOperationsState()->GetColorMask(params); break;
    case GL_CULL_FACE:                          *params = static_cast<GLfloat>(mStateManager.GetRasterizationState()->GetCullEnabled()); break;
    case GL_CULL_FACE_MODE:                     *params = mStateManager.GetRasterizationState()->GetCullFace(); break;
    case GL_CURRENT_PROGRAM:                    *params = GetProgramId(mStateManager.GetActiveShaderProgram()); break;
    case GL_DEPTH_CLEAR_VALUE:                  *params = mStateManager.GetFramebufferOperationsState()->GetClearDepth(); break;
    case GL_DEPTH_FUNC:                         *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetDepthTestFunc()); break;
    case GL_DEPTH_TEST:                         *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetDepthTestEnabled()); break;
    case GL_DEPTH_WRITEMASK:                    *params = static_cast<GLfloat>(mStateManager.GetFramebufferOperationsState()->GetDepthMask()); break;
    case GL_DITHER:                             *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetDitheringEnabled()); break;
    case GL_ELEMENT_ARRAY_BUFFER_BINDING:       *params = mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ELEMENT_ARRAY_BUFFER) ? mResourceManager.GetBufferID(mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ELEMENT_ARRAY_BUFFER)) : 0; break;
    case GL_FRAMEBUFFER_BINDING:                *params = static_cast<GLfloat>(mStateManager.GetActiveObjectsState()->GetActiveFramebufferObjectID()); break;
    case GL_FRONT_FACE:                         *params = static_cast<GLfloat>(mStateManager.GetRasterizationState()->GetFrontFace()); break;
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT:   *params = GL_RGBA; break;
    case GL_IMPLEMENTATION_COLOR_READ_TYPE:     *params = GL_UNSIGNED_BYTE; break;
    case GL_LINE_WIDTH:                         *params = mStateManager.GetRasterizationState()->GetLineWidth(); break;
    case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:   *params = GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS; break;
    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:          *params = GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE; break;
    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:       *params = GLOVE_MAX_FRAGMENT_UNIFORM_VECTORS; break;
    case GL_MAX_RENDERBUFFER_SIZE:              *params = GLOVE_MAX_RENDERBUFFER_SIZE; break;
    case GL_MAX_TEXTURE_IMAGE_UNITS:            *params = GLOVE_MAX_TEXTURE_IMAGE_UNITS; break;
    case GL_MAX_TEXTURE_SIZE:                   *params = GLOVE_MAX_TEXTURE_SIZE; break;
    case GL_MAX_VARYING_VECTORS:                *params = GLOVE_MAX_VARYING_VECTORS; break;
    case GL_MAX_VERTEX_ATTRIBS:                 *params = GLOVE_MAX_VERTEX_ATTRIBS; break;
    case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:     *params = GLOVE_MAX_VERTEX_TEXTURE_IMAGE_UNITS; break;
    case GL_MAX_VERTEX_UNIFORM_VECTORS:         *params = GLOVE_MAX_VERTEX_UNIFORM_VECTORS; break;
    case GL_MAX_VIEWPORT_DIMS:                  params[0] = GLOVE_MAX_TEXTURE_SIZE;
                                                params[1] = GLOVE_MAX_TEXTURE_SIZE; break;
    case GL_NUM_SHADER_BINARY_FORMATS:          *params = GLOVE_NUM_SHADER_BINARY_FORMATS; break;
    case GL_NUM_PROGRAM_BINARY_FORMATS_OES:     *params = GLOVE_NUM_PROGRAM_BINARY_FORMATS; break;
    case GL_PACK_ALIGNMENT:                     *params = mStateManager.GetPixelStorageState()->GetPixelStorePack(); break;
    case GL_POLYGON_OFFSET_FACTOR:              *params = mStateManager.GetRasterizationState()->GetPolygonOffsetFactor(); break;
    case GL_POLYGON_OFFSET_FILL:                *params = static_cast<GLfloat>(mStateManager.GetRasterizationState()->GetPolygonOffsetFillEnabled()); break;
    case GL_POLYGON_OFFSET_UNITS:               *params = mStateManager.GetRasterizationState()->GetPolygonOffsetUnits(); break;
    case GL_RENDERBUFFER_BINDING:               *params = static_cast<GLfloat>(mStateManager.GetActiveObjectsState()->GetActiveRenderbufferObjectID()); break;
    case GL_SAMPLE_ALPHA_TO_COVERAGE:           *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetSampleAlphaToCoverageEnabled()); break;
    case GL_SAMPLE_COVERAGE:                    *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetSampleCoverageEnabled()); break;
    case GL_SCISSOR_BOX:                        mStateManager.GetFragmentOperationsState()->GetScissorRect(params); break;
    case GL_SCISSOR_TEST:                       *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetScissorTestEnabled()); break;
    case GL_STENCIL_TEST:                       *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestEnabled()); break;
    case GL_UNPACK_ALIGNMENT:                   *params = mStateManager.GetPixelStorageState()->GetPixelStoreUnpack(); break;
    case GL_VIEWPORT:                           mStateManager.GetViewportTransformationState()->GetViewportRect(params); break;
    case GL_RED_BITS:                           GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), params, NULL, NULL, NULL, NULL, NULL); break;
    case GL_BLUE_BITS:                          GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), NULL, params, NULL, NULL, NULL, NULL); break;
    case GL_GREEN_BITS:                         GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), NULL, NULL, params, NULL, NULL, NULL); break;
    case GL_ALPHA_BITS:                         GlFormatToStorageBits(mWriteFBO->GetColorAttachmentTexture()->GetInternalFormat(), NULL, NULL, NULL, params, NULL, NULL); break;
    case GL_DEPTH_BITS:                         GlFormatToStorageBits(mWriteFBO->GetDepthAttachmentTexture()   ? mWriteFBO->GetDepthAttachmentTexture()->GetInternalFormat()   : GL_INVALID_VALUE, NULL, NULL, NULL, NULL, params, NULL); break;
    case GL_STENCIL_BITS:                       GlFormatToStorageBits(mWriteFBO->GetStencilAttachmentTexture() ? mWriteFBO->GetStencilAttachmentTexture()->GetInternalFormat() : GL_INVALID_VALUE, NULL, NULL, NULL, NULL, NULL, params); break;
    case GL_ALIASED_LINE_WIDTH_RANGE:           params[0] = 1.0f;
                                                params[1] = 1.0f; break;
    case GL_ALIASED_POINT_SIZE_RANGE:           params[0] = 1.0f;
                                                params[1] = 1.0f; break;
    case GL_COMPRESSED_TEXTURE_FORMATS:         *params = 0.0f; break;
    case GL_DEPTH_RANGE:                        params[0] = mStateManager.GetViewportTransformationState()->GetMinDepthRange();
                                                params[1] = mStateManager.GetViewportTransformationState()->GetMaxDepthRange(); break;
    case GL_GENERATE_MIPMAP_HINT:               *params = static_cast<GLfloat>(mStateManager.GetHintAspectsState()->GetMode(GL_GENERATE_MIPMAP_HINT)); break;
    case GL_NUM_COMPRESSED_TEXTURE_FORMATS:     *params = 0.0f; break;
    case GL_SAMPLES:                            *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetSampleCoverageBits()); break;
    case GL_SAMPLE_BUFFERS:                     *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetMultiSamplingEnabled()); break;
    case GL_SAMPLE_COVERAGE_INVERT:             *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetSampleCoverageInvert()); break;
    case GL_SAMPLE_COVERAGE_VALUE:              *params = mStateManager.GetFragmentOperationsState()->GetSampleCoverageValue(); break;
    case GL_SHADER_COMPILER:                    *params = 1.0f; break;
    case GL_SUBPIXEL_BITS:                      *params = 4.0f; break;
    case GL_TEXTURE_BINDING_2D:                 *params = static_cast<GLfloat>(mResourceManager.GetTextureID(mStateManager.GetActiveObjectsState()->GetActiveTexture(GL_TEXTURE_2D))); break;
    case GL_TEXTURE_BINDING_CUBE_MAP:           *params = static_cast<GLfloat>(mResourceManager.GetTextureID(mStateManager.GetActiveObjectsState()->GetActiveTexture(GL_TEXTURE_CUBE_MAP))); break;
    case GL_ACTIVE_TEXTURE:                     *params = static_cast<GLfloat>(mStateManager.GetActiveObjectsState()->GetActiveTextureUnit()); break;
    case GL_STENCIL_WRITEMASK:                  *params = static_cast<GLfloat>(mStateManager.GetFramebufferOperationsState()->GetStencilMaskFront()); break;
    case GL_STENCIL_BACK_WRITEMASK:             *params = static_cast<GLfloat>(mStateManager.GetFramebufferOperationsState()->GetStencilMaskBack());  break;
    case GL_STENCIL_CLEAR_VALUE:                *params = static_cast<GLfloat>(mStateManager.GetFramebufferOperationsState()->GetClearStencil()); break;
    case GL_STENCIL_FUNC:                       *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareFront()); break;
    case GL_STENCIL_VALUE_MASK:                 *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskFront()); break;
    case GL_STENCIL_REF:                        *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefFront()); break;
    case GL_STENCIL_FAIL:                       *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailFront()); break;
    case GL_STENCIL_PASS_DEPTH_FAIL:            *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailFront()); break;
    case GL_STENCIL_PASS_DEPTH_PASS:            *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassFront()); break;
    case GL_STENCIL_BACK_FUNC:                  *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncCompareBack()); break;
    case GL_STENCIL_BACK_FAIL:                  *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpFailBack()); break;
    case GL_STENCIL_BACK_PASS_DEPTH_FAIL:       *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZfailBack()); break;
    case GL_STENCIL_BACK_PASS_DEPTH_PASS:       *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestOpZpassBack()); break;
    case GL_STENCIL_BACK_REF:                   *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncRefBack()); break;
    case GL_STENCIL_BACK_VALUE_MASK:            *params = static_cast<GLfloat>(mStateManager.GetFragmentOperationsState()->GetStencilTestFuncMaskBack()); break;
    default:                                    RecordError(GL_INVALID_ENUM); break;
    }
}

GLboolean
Context::IsEnabled(GLenum cap)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if((cap != GL_BLEND)                    &&
       (cap != GL_CULL_FACE)                &&
       (cap != GL_DEPTH_TEST)               &&
       (cap != GL_DITHER)                   &&
       (cap != GL_POLYGON_OFFSET_FILL)      &&
       (cap != GL_SAMPLE_ALPHA_TO_COVERAGE) &&
       (cap != GL_SAMPLE_COVERAGE)          &&
       (cap != GL_SCISSOR_TEST)             &&
       (cap != GL_STENCIL_TEST) ){
         RecordError(GL_INVALID_ENUM);
         return GL_FALSE;
    }

    return mStateManager.IsCapabilityEnabled(cap);
}
