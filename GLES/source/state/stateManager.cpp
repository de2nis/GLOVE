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
 *  @file       stateManager.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State Manager Functionality
 *
 *  @section
 *
 *  OpenGL ES maintains a considerable amount of information that can be
 *  divided into state parameteres for (a) Active Objects, (b) Framebuffer
 *  Operations, (c) Fragment Operations, (d) Input Assembly, (e) Pixel Storage,
 *  (f) Rasterization, (g) Viewport Transformation and (h) implementation-specific
 *  hints.
 *
 */

#include "stateManager.h"

StateManager::StateManager()
: mError(GL_NO_ERROR)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

StateManager::~StateManager()
{
    FUN_ENTRY(GL_LOG_TRACE);
}

GLboolean
StateManager::IsCapabilityEnabled(GLenum cap)
{
    FUN_ENTRY(GL_LOG_TRACE);

    GLboolean res = GL_FALSE;

    switch(cap) {
    case GL_BLEND:                    res = mFragmentOperationsState.GetBlendingEnabled();              break;
    case GL_CULL_FACE:                res = mRasterizationState.GetCullEnabled();                       break;
    case GL_DEPTH_TEST:               res = mFragmentOperationsState.GetDepthTestEnabled();             break;
    case GL_DITHER:                   res = mFragmentOperationsState.GetDitheringEnabled();             break;
    case GL_POLYGON_OFFSET_FILL:      res = mRasterizationState.GetPolygonOffsetFillEnabled();          break;
    case GL_SAMPLE_ALPHA_TO_COVERAGE: res = mFragmentOperationsState.GetSampleAlphaToCoverageEnabled(); break;
    case GL_SAMPLE_COVERAGE:          res = mFragmentOperationsState.GetSampleCoverageEnabled();        break;
    case GL_SCISSOR_TEST:             res = mFragmentOperationsState.GetScissorTestEnabled();           break;
    case GL_STENCIL_TEST:             res = mFragmentOperationsState.GetStencilTestEnabled();           break;
    }

    return res;
}

void
StateManager::InitVkPipelineStates(vulkanAPI::Pipeline *pipeline)
{
    FUN_ENTRY(GL_LOG_TRACE);

    VkBool32            primitiveRestartEnable = GlBooleanToVkBool(GetInputAssemblyState()->GetPrimitiveRestartEnabled());
    VkPrimitiveTopology topology               = GlPrimitiveTopologyToVkPrimitiveTopology(GetInputAssemblyState()->GetPrimitiveMode());

    VkPolygonMode      polygonMode             = GLPrimitiveModeToVkPolygonMode(GetInputAssemblyState()->GetPrimitiveMode());
    VkCullModeFlagBits cullMode                = GlCullModeToVkCullMode(GetRasterizationState()->GetCullFace());
    VkFrontFace        frontFace               = GlFrontFaceToVkFrontFace(GetRasterizationState()->GetFrontFace());
    VkBool32           depthClampEnable        = GlBooleanToVkBool(GetRasterizationState()->GetDepthClampEnabled());
    VkBool32           rasterizerDiscardEnable = GlBooleanToVkBool(GetRasterizationState()->GetRasterizerDiscardEnabled());
    VkBool32           depthBiasEnable         = GlBooleanToVkBool(GetRasterizationState()->GetPolygonOffsetFillEnabled());
    float              depthBiasConstantFactor = GetRasterizationState()->GetPolygonOffsetFactor();
    float              depthBiasSlopeFactor    = GetRasterizationState()->GetPolygonOffsetUnits();
    float              depthBiasClamp          = GetRasterizationState()->GetPolygonOffsetClamp();
    VkBool32           blendEnable             = GlBooleanToVkBool(GetFragmentOperationsState()->GetBlendingEnabled());
    VkColorComponentFlagBits colorWriteMask    = static_cast<VkColorComponentFlagBits>(GetFramebufferOperationsState()->GetColorMask());

    VkBlendFactor srcColorBlendFactor = GlBlendFactorToVkBlendFactor(GetFragmentOperationsState()->GetBlendingFactorSourceRGB());
    VkBlendFactor dstColorBlendFactor = GlBlendFactorToVkBlendFactor(GetFragmentOperationsState()->GetBlendingFactorDestinationRGB());
    VkBlendFactor srcAlphaBlendFactor = GlBlendFactorToVkBlendFactor(GetFragmentOperationsState()->GetBlendingFactorSourceAlpha());
    VkBlendFactor dstAlphaBlendFactor = GlBlendFactorToVkBlendFactor(GetFragmentOperationsState()->GetBlendingFactorDestinationAlpha());

    VkBlendOp   colorBlendOp         = GlBlendEquationToVkBlendOp(GetFragmentOperationsState()->GetBlendingEquationRGB());
    VkBlendOp   alphaBlendOp         = GlBlendEquationToVkBlendOp(GetFragmentOperationsState()->GetBlendingEquationAlpha());
    VkLogicOp   logicOp              = GlLogicOpToVkLogicOp(GetFragmentOperationsState()->GetBlendingLogicOp());
    VkBool32    logicOpEnable        = GlBooleanToVkBool(GetFragmentOperationsState()->GetBlendingLogicOpEnabled());
    uint32_t    colorAttachmentCount = GetFragmentOperationsState()->GetBlendingColorAttachmentCount();

    GLfloat     blendcolor[4];
    GetFragmentOperationsState()->GetBlendingColor(blendcolor);

    uint32_t    viewportCount = GetViewportTransformationState()->GetViewportCount();
    uint32_t    scissorCount  = GetViewportTransformationState()->GetScissorCount();

    VkBool32              alphaToOneEnable      = GlBooleanToVkBool(GetFragmentOperationsState()->GetSampleAlphaToOneEnabled());
    VkBool32              alphaToCoverageEnable = GlBooleanToVkBool(GetFragmentOperationsState()->GetSampleAlphaToCoverageEnabled());
    VkSampleCountFlagBits rasterizationSamples  = GlSampleCoverageBitsToVkSampleCountFlagBits(GetFragmentOperationsState()->GetSampleCoverageBits());
    VkBool32              sampleShadingEnable   = GlBooleanToVkBool(GetFragmentOperationsState()->GetSampleShadingEnabled());
    float                 minSampleShading      = GetFragmentOperationsState()->GetMinSampleShading();

    VkBool32    depthTestEnable       = GlBooleanToVkBool(GetFragmentOperationsState()->GetDepthTestEnabled());
    VkBool32    depthWriteEnable      = GlBooleanToVkBool(GetFramebufferOperationsState()->GetDepthMask());
    VkCompareOp depthCompareOp        = GlCompareFuncToVkCompareOp(GetFragmentOperationsState()->GetDepthTestFunc());
    VkBool32    depthBoundsTestEnable = GlBooleanToVkBool(GetFragmentOperationsState()->GetDepthBoundsTestEnabled());
    float       minDepthBounds        = GetFragmentOperationsState()->GetMinDepthBounds();
    float       maxDepthBounds        = GetFragmentOperationsState()->GetMaxDepthBounds();

    VkBool32    stencilTestEnable     = GlBooleanToVkBool(GetFragmentOperationsState()->GetStencilTestEnabled());
    VkStencilOp backfailOp            = GlStencilFuncToVkStencilOp(GetFragmentOperationsState()->GetStencilTestOpFailBack());
    VkStencilOp backpassOp            = GlStencilFuncToVkStencilOp(GetFragmentOperationsState()->GetStencilTestOpZpassBack());
    VkStencilOp backdepthFailOp       = GlStencilFuncToVkStencilOp(GetFragmentOperationsState()->GetStencilTestOpZfailBack());
    uint32_t    backwriteMask         = GetFramebufferOperationsState()->GetStencilMaskBack();
    VkCompareOp backcompareOp         = GlCompareFuncToVkCompareOp(GetFragmentOperationsState()->GetStencilTestFuncCompareBack());
    uint32_t    backcompareMask       = GetFragmentOperationsState()->GetStencilTestFuncMaskBack();
    uint32_t    backreference         = GetFragmentOperationsState()->GetStencilTestFuncRefBack();
    VkStencilOp frontfailOp           = GlStencilFuncToVkStencilOp(GetFragmentOperationsState()->GetStencilTestOpFailFront());
    VkStencilOp frontpassOp           = GlStencilFuncToVkStencilOp(GetFragmentOperationsState()->GetStencilTestOpZpassFront());
    VkStencilOp frontdepthFailOp      = GlStencilFuncToVkStencilOp(GetFragmentOperationsState()->GetStencilTestOpZfailFront());
    uint32_t    frontwriteMask        = GetFramebufferOperationsState()->GetStencilMaskFront();
    VkCompareOp frontcompareOp        = GlCompareFuncToVkCompareOp(GetFragmentOperationsState()->GetStencilTestFuncCompareFront());
    uint32_t    frontcompareMask      = GetFragmentOperationsState()->GetStencilTestFuncMaskFront();
    uint32_t    frontreference        = GetFragmentOperationsState()->GetStencilTestFuncRefFront();

    pipeline->CreateInputAssemblyState(primitiveRestartEnable, topology);
    pipeline->CreateRasterizationState(polygonMode, cullMode, frontFace, depthBiasEnable, depthBiasConstantFactor, depthBiasSlopeFactor, depthBiasClamp, depthClampEnable, rasterizerDiscardEnable );
    pipeline->CreateColorBlendState(blendEnable, colorWriteMask, srcColorBlendFactor, dstColorBlendFactor, srcAlphaBlendFactor, dstAlphaBlendFactor, colorBlendOp, alphaBlendOp, logicOp, logicOpEnable, colorAttachmentCount, blendcolor);
    pipeline->CreateDepthStencilState(depthTestEnable, depthWriteEnable, depthCompareOp, depthBoundsTestEnable, minDepthBounds, maxDepthBounds, stencilTestEnable, backfailOp, backpassOp,
                            backdepthFailOp, backwriteMask, backcompareOp, backcompareMask, backreference, frontfailOp, frontpassOp, frontdepthFailOp, frontwriteMask, frontcompareOp, frontcompareMask, frontreference );
    pipeline->CreateViewportState(viewportCount, scissorCount);
    pipeline->CreateMultisampleState(alphaToOneEnable, alphaToCoverageEnable, rasterizationSamples, sampleShadingEnable, minSampleShading);
    pipeline->CreateDynamicState();
    pipeline->CreateInfo();
}
