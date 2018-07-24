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
 *  @file       image.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Resource Creation Functionality via Images in Vulkan
 *
 *  @section
 *
 *  Images represent multidimensional - up to 3 - arrays of data which can be
 *  used for various purposes (e.g. attachments, textures), by binding them to
 *  a graphics or compute pipeline via descriptor sets, or by directly specifying
 *  them as parameters to certain commands.
 *  Images are represented by VkImage handles.
 *
 */

#include "image.h"
#include "context/context.h"

namespace vulkanAPI {

Image::Image(const vkContext_t *vkContext)
: mVkContext(vkContext), mVkImage(VK_NULL_HANDLE), mVkFormat(VK_FORMAT_UNDEFINED), mVkImageType(VK_IMAGE_TYPE_2D),
mVkImageUsage(VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM), mVkImageLayout(VK_IMAGE_LAYOUT_UNDEFINED),
mVkImageTiling(VK_IMAGE_TILING_LINEAR), mVkImageTarget(VK_IMAGE_TARGET_2D),
mVkSampleCount(VK_SAMPLE_COUNT_1_BIT), mVkSharingMode(VK_SHARING_MODE_EXCLUSIVE),
mWidth(0), mHeight(0), mMipLevels(1), mLayers(1), mDelete(true)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

Image::~Image()
{
    FUN_ENTRY(GL_LOG_TRACE);

    if(mDelete) {
        Release();
    }
}

void
Image::Release(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkImage != VK_NULL_HANDLE) {
        vkDestroyImage(mVkContext->vkDevice, mVkImage, NULL);
        mVkImage = VK_NULL_HANDLE;
    }

    mWidth      = 0;
    mHeight     = 0;
    mMipLevels  = 1;
    mLayers     = 1;
    mDelete     = true;
}

bool
Image::Create()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkImageCreateInfo info;
    info.sType          = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext          = NULL;
    info.flags          = mVkImageTarget == VK_IMAGE_TARGET_2D ? 0 : VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    info.imageType      = mVkImageType;
    info.format         = mVkFormat;
    info.extent.width   = mWidth;
    info.extent.height  = mHeight;
    info.extent.depth   = 1;
    info.arrayLayers    = mVkImageTarget == VK_IMAGE_TARGET_2D ? TEXTURE_2D_LAYERS : TEXTURE_CUBE_MAP_LAYERS;
    info.mipLevels      = mMipLevels;
    info.samples        = mVkSampleCount;
    info.tiling         = mVkImageTiling;
    info.usage          = mVkImageUsage;
    info.sharingMode    = mVkSharingMode;
    info.initialLayout  = mVkImageLayout;

    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices   = NULL;

    VkResult err = vkCreateImage(mVkContext->vkDevice, &info, NULL, &mVkImage);
    assert(!err);

    mDelete = VK_TRUE;
    mLayers = info.arrayLayers;

    CreateImageSubresourceRange();

    return (err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY);
}

void
Image::CreateBufferImageCopy(int32_t offsetX, int32_t offsetY, uint32_t extentWidth, uint32_t extentHeight, uint32_t miplevel, uint32_t layer, uint32_t layerCount)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mVkBufferImageCopy.imageSubresource.aspectMask     = mVkImageSubresourceRange.aspectMask;
    mVkBufferImageCopy.imageSubresource.mipLevel       = miplevel;
    mVkBufferImageCopy.imageSubresource.baseArrayLayer = layer;
    mVkBufferImageCopy.imageSubresource.layerCount     = layerCount;
    mVkBufferImageCopy.imageOffset.x                   = offsetX;
    mVkBufferImageCopy.imageOffset.y                   = offsetY;
    mVkBufferImageCopy.imageOffset.z                   = 0;
    mVkBufferImageCopy.imageExtent.width               = extentWidth;
    mVkBufferImageCopy.imageExtent.height              = extentHeight;
    mVkBufferImageCopy.imageExtent.depth               = 1;
    mVkBufferImageCopy.bufferOffset                    = 0;
    mVkBufferImageCopy.bufferRowLength                 = 0;
    mVkBufferImageCopy.bufferImageHeight               = 0;
}

void
Image::CopyBufferToImage(VkCommandBuffer *activeCmdBuffer, VkBuffer srcBuffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    vkCmdCopyBufferToImage(*activeCmdBuffer, srcBuffer, mVkImage, mVkImageLayout, 1, &mVkBufferImageCopy);
}

void
Image::CopyImageToBuffer(VkCommandBuffer *activeCmdBuffer, VkBuffer srcBuffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    vkCmdCopyImageToBuffer(*activeCmdBuffer, mVkImage, mVkImageLayout, srcBuffer, 1, &mVkBufferImageCopy);
}

void
Image::BlitImage(VkCommandBuffer *activeCmdBuffer, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageBlit* imageBlit, VkFilter imageFilter)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    vkCmdBlitImage(*activeCmdBuffer, GetImage(), srcImageLayout, dstImage, dstImageLayout, 1, imageBlit, imageFilter);
}

void
Image::CreateImageSubresourceRange()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkImageAspectFlagBits aspectMask = static_cast<VkImageAspectFlagBits>(0);
    if(VkFormatIsColor(mVkFormat)) {
            aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    } else {
        if(VkFormatIsDepth(mVkFormat)) {
            aspectMask = static_cast<VkImageAspectFlagBits>(aspectMask | VK_IMAGE_ASPECT_DEPTH_BIT);
        }
        if(VkFormatIsStencil(mVkFormat)) {
            aspectMask = static_cast<VkImageAspectFlagBits>(aspectMask | VK_IMAGE_ASPECT_STENCIL_BIT);
        }
    }

    mVkImageSubresourceRange.aspectMask      = aspectMask;
    mVkImageSubresourceRange.baseMipLevel    = 0;
    mVkImageSubresourceRange.levelCount      = mMipLevels;
    mVkImageSubresourceRange.baseArrayLayer  = 0;
    mVkImageSubresourceRange.layerCount      = mLayers;
}

void
Image::ModifyImageSubresourceRange(uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mVkImageSubresourceRange.baseMipLevel    = baseMipLevel;
    mVkImageSubresourceRange.levelCount      = levelCount;
    mVkImageSubresourceRange.baseArrayLayer  = baseArrayLayer;
    mVkImageSubresourceRange.layerCount      = layerCount;
}

void
Image::ModifyImageLayout(VkCommandBuffer *activeCmdBuffer, VkImageLayout newImageLayout)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkImageLayout oldImageLayout = mVkImageLayout;

    VkImageMemoryBarrier imageMemoryBarrier;
    imageMemoryBarrier.sType                = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext                = NULL;
    imageMemoryBarrier.oldLayout            = oldImageLayout;
    imageMemoryBarrier.newLayout            = newImageLayout;
    imageMemoryBarrier.srcQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex  = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image                = mVkImage;
    imageMemoryBarrier.subresourceRange     = mVkImageSubresourceRange;

    // Source layouts (old)
    // Source access mask controls actions that have to be finished on the old layout
    // before it will be transitioned to the new layout
    switch(oldImageLayout) {
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
            // Image is preinitialized
            // Only valid as initial layout for linear images, preserves memory contents
            // Make sure host writes have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            // Image is a color attachment
            // Make sure any writes to the color buffer have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            // Image is a depth/stencil attachment
            // Make sure any writes to the depth/stencil buffer have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            // Image is a transfer source
            // Make sure any reads from the image have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            // Image is a transfer destination
            // Make sure any writes to the image have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            // Image is read by a shader
            // Make sure any shader reads from the image have been finished
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;

    case VK_IMAGE_LAYOUT_UNDEFINED:
    default:
            // Image layout is undefined (or does not matter)
            // Only valid as initial layout
            // No flags required, listed only for completeness
            imageMemoryBarrier.srcAccessMask = 0;
            break;
    }

    // Target layouts (new)
    // Destination access mask controls the dependency for the new image layout
    switch(newImageLayout) {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        // Image will be used as a transfer destination
        // Make sure any writes to the image have been finished
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        // Image will be used as a transfer source
        // Make sure any reads from and writes to the image have been finished
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        // Image will be used as a color attachment
        // Make sure any writes to the color buffer have been finished
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        // Image layout will be used as a depth/stencil attachment
        // Make sure any writes to depth/stencil buffer have been finished
        imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        // Image will be read in a shader (sampler, input attachment)
        // Make sure any writes to the image have been finished
        if(imageMemoryBarrier.srcAccessMask == 0) {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        }
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_GENERAL:
        // Image layout supports all operations
        imageMemoryBarrier.dstAccessMask = 0;
        break;

    default:
        NOT_REACHED();
        break;
    }

    // Put barrier on top
    VkPipelineStageFlags srcStages  = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(*activeCmdBuffer, srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier);

    mVkImageLayout = newImageLayout;
}

}
