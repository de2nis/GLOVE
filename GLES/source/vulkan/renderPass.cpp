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
 *  @file       renderPass.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Render pass Functionality in Vulkan
 *
 *  @section
 *
 *  A render pass represents a collection of attachments, subpasses, and
 *  dependencies between the subpasses, and describes how the attachments
 *  are used over the course of the subpasses.
 *  Render passes are represented by VkRenderPass handles.
 *
 */

#include "renderPass.h"

namespace vulkanAPI {

RenderPass::RenderPass(const vkContext_t *vkContext)
: mVkContext(vkContext),
  mVkSubpassContents(VK_SUBPASS_CONTENTS_INLINE), mVkPipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS),
  mVkRenderPass(VK_NULL_HANDLE),
  mDepthWriteEnabled(true), mStencilWriteEnabled(false), mStarted(false)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

RenderPass::~RenderPass()
{
    FUN_ENTRY(GL_LOG_TRACE);

    Release();
}

void
RenderPass::Release(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkRenderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(mVkContext->vkDevice, mVkRenderPass, NULL);
        mVkRenderPass = VK_NULL_HANDLE;
    }
}

bool
RenderPass::Create(VkFormat colorFormat, VkFormat depthstencilFormat)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    Release();

    VkAttachmentReference           color;
    VkAttachmentReference           depthstencil;
    vector<VkAttachmentDescription> attachments;

    /// Color attachment
    VkAttachmentDescription attachmentColor;
    attachmentColor.flags           = 0;
    attachmentColor.format          = colorFormat;
    attachmentColor.samples         = VK_SAMPLE_COUNT_1_BIT;
    attachmentColor.loadOp          = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentColor.storeOp         = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentColor.stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentColor.stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentColor.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentColor.finalLayout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    attachments.push_back(attachmentColor);

    color.attachment           = attachments.size() - 1;
    color.layout               = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    /// Depth/Stencil attachment
    if(depthstencilFormat != VK_FORMAT_UNDEFINED) {
        VkAttachmentDescription attachmentDepthStencil;

        attachmentDepthStencil.flags          = 0;
        attachmentDepthStencil.format         = depthstencilFormat;
        attachmentDepthStencil.samples        = VK_SAMPLE_COUNT_1_BIT;
        attachmentDepthStencil.loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDepthStencil.storeOp        = mDepthWriteEnabled   ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentDepthStencil.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDepthStencil.stencilStoreOp = mStencilWriteEnabled ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentDepthStencil.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentDepthStencil.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        attachments.push_back(attachmentDepthStencil);

        depthstencil.attachment   = attachments.size() - 1;
        depthstencil.layout       = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }

    VkSubpassDescription subpass;
    subpass.pipelineBindPoint       = mVkPipelineBindPoint;
    subpass.flags                   = 0;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &color;
    subpass.pDepthStencilAttachment = depthstencilFormat != VK_FORMAT_UNDEFINED ? &depthstencil : NULL;
    subpass.pResolveAttachments     = NULL;
    subpass.inputAttachmentCount    = 0;
    subpass.pInputAttachments       = NULL;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments    = NULL;

    VkRenderPassCreateInfo info;
    info.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext            = NULL;
    info.flags            = 0;
    info.attachmentCount  = attachments.size();
    info.pAttachments     = attachments.data();
    info.subpassCount     = 1;
    info.pSubpasses       = &subpass;
    info.dependencyCount  = 0;
    info.pDependencies    = NULL;

    VkResult err = vkCreateRenderPass(mVkContext->vkDevice, &info, NULL, &mVkRenderPass);
    assert(!err);

    return (err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY);
}


void
RenderPass::Begin(VkCommandBuffer *activeCmdBuffer, VkFramebuffer *framebuffer, uint32_t width, uint32_t height)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkRenderPassBeginInfo info;
    info.sType                     = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.pNext                     = NULL;
    info.framebuffer               = *framebuffer;
    info.renderPass                = mVkRenderPass;
    info.renderArea.offset.x       = 0;
    info.renderArea.offset.y       = 0;
    info.renderArea.extent.width   = width;
    info.renderArea.extent.height  = height;
    info.clearValueCount           = 0;
    info.pClearValues              = NULL;

    vkCmdBeginRenderPass(*activeCmdBuffer, &info, mVkSubpassContents);

    mStarted = true;
}

void
RenderPass::End(VkCommandBuffer *activeCmdBuffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if (mStarted) {
        mStarted = false;
        vkCmdEndRenderPass(*activeCmdBuffer);
    }
}

}
