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
 *  @file       image.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Resource Creation Functionality via Images in Vulkan
 *
 */

#ifndef __VKIMAGE_H__
#define __VKIMAGE_H__

#include "utils/globals.h"
#include "utils.h"

#define TEXTURE_2D_LAYERS         1
#define TEXTURE_CUBE_MAP_LAYERS   6

namespace vulkanAPI {

class Image {
public:

    typedef enum VkImageTarget {
        VK_IMAGE_TARGET_2D   = 0,
        VK_IMAGE_TARGET_CUBE = 1,
        VK_IMAGE_TARGET_MAX  = 2
    } VkImageTarget;

private:

    const
    vkContext_t *                     mVkContext;

    VkImage                           mVkImage;
    VkFormat                          mVkFormat;
    VkImageType                       mVkImageType;
    VkImageUsageFlagBits              mVkImageUsage;
    VkImageLayout                     mVkImageLayout;
    VkImageTiling                     mVkImageTiling;
    VkImageSubresourceRange           mVkImageSubresourceRange;
    VkImageTarget                     mVkImageTarget;
    VkSampleCountFlagBits             mVkSampleCount;
    VkSharingMode                     mVkSharingMode;
    VkBufferImageCopy                 mVkBufferImageCopy;

    uint32_t                          mWidth;
    uint32_t                          mHeight;
    uint32_t                          mMipLevels;
    uint32_t                          mLayers;
    VkBool32                          mDelete;

public:
// Constructor
    Image(const vkContext_t *vkContext = nullptr);

// Destructor
    ~Image();

// Blit Functions
    void                              BlitImage(        VkCommandBuffer *activeCmdBuffer, VkImageLayout srcImageLayout,
                                                        VkImage          dstImage,        VkImageLayout dstImageLayout,
                                                  const VkImageBlit*     imageBlit,       VkFilter      imageFilter);

// Create Functions
    bool                              Create(void);
    void                              CreateImageSubresourceRange(void);
    void                              CreateBufferImageCopy(int32_t offsetX, int32_t offsetY, uint32_t extentWidth, uint32_t extentHeight, uint32_t miplevel, uint32_t layer, uint32_t layerCount);

// Copy Functions
    void                              CopyBufferToImage(VkCommandBuffer *activeCmdBuffer, VkBuffer srcBuffer);
    void                              CopyImageToBuffer(VkCommandBuffer *activeCmdBuffer, VkBuffer srcBuffer);

// Modify Functions
    void                              ModifyImageSubresourceRange(uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void                              ModifyImageLayout(VkCommandBuffer *activeCmdBuffer, VkImageLayout newImageLayout);

// Release Functions
    void                              Release(void);

// Get Functions
    inline VkImage &                  GetImage(void)                            { FUN_ENTRY(GL_LOG_TRACE); return mVkImage;          }
    inline VkFormat                   GetFormat(void)                     const { FUN_ENTRY(GL_LOG_TRACE); return mVkFormat;         }
    inline VkImageTarget              GetImageTarget(void)                const { FUN_ENTRY(GL_LOG_TRACE); return mVkImageTarget;    }
    inline VkImageLayout              GetImageLayout(void)                const { FUN_ENTRY(GL_LOG_TRACE); return mVkImageLayout;    }
    inline VkBufferImageCopy *        GetBufferImageCopy(void)                  { FUN_ENTRY(GL_LOG_TRACE); return &mVkBufferImageCopy;      }
    inline VkImageSubresourceRange    GetImageSubresourceRange(void)      const { FUN_ENTRY(GL_LOG_TRACE); return mVkImageSubresourceRange; }
    inline uint32_t                   GetMipLevels(void)                  const { FUN_ENTRY(GL_LOG_TRACE); return mMipLevels;        }
    inline uint32_t                   GetLayers(void)                     const { FUN_ENTRY(GL_LOG_TRACE); return mLayers;           }

// Set Functions
    inline void                       SetContext(const vkContext_t *vkContext)  { FUN_ENTRY(GL_LOG_TRACE); mVkContext     = vkContext; }
    inline void                       SetFormat(VkFormat format)                { FUN_ENTRY(GL_LOG_TRACE); mVkFormat      = format;    }
    inline void                       SetImage(VkImage image)                   { FUN_ENTRY(GL_LOG_TRACE); mVkImage       = image;
                                                                                                       mDelete        = false;     }
    inline void                       SetImageUsage(VkImageUsageFlagBits usage) { FUN_ENTRY(GL_LOG_TRACE); mVkImageUsage  = usage;     }
    inline void                       SetImageTarget(VkImageTarget target)      { FUN_ENTRY(GL_LOG_TRACE); mVkImageTarget = target;    }
    inline void                       SetImageTiling(VkImageTiling tiling)      { FUN_ENTRY(GL_LOG_TRACE); mVkImageTiling = tiling;    }
    inline void                       SetImageLayout(VkImageLayout layout)      { FUN_ENTRY(GL_LOG_TRACE); mVkImageLayout = layout;    }
    inline void                       SetWidth(uint32_t width)                  { FUN_ENTRY(GL_LOG_TRACE); mWidth         = width;     }
    inline void                       SetHeight(uint32_t height)                { FUN_ENTRY(GL_LOG_TRACE); mHeight        = height;    }
    inline void                       SetMipLevels(uint32_t levels)             { FUN_ENTRY(GL_LOG_TRACE); mMipLevels     = levels;    }
};

}

#endif // __VKIMAGE_H__
