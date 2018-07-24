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
 *  @file       buffer.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Resource Creation Functionality via Buffers in Vulkan
 *
 */

#ifndef __VKBUFFER_H__
#define __VKBUFFER_H__

#include "utils/globals.h"

namespace vulkanAPI {

class Buffer {

private:
    const
    vkContext_t *                     mVkContext;

    VkBuffer                          mVkBuffer;
    VkDescriptorBufferInfo            mVkDescriptorBufferInfo;
    VkSharingMode                     mVkBufferSharingMode;
    VkBufferUsageFlags                mVkBufferUsageFlags;
    VkDeviceSize                      mVkSize;
    VkDeviceSize                      mVkOffset;

public:
// Constructor
    Buffer(const vkContext_t *vkContext, const VkBufferUsageFlags vkBufferUsageFlags, const VkSharingMode vkSharingMode);

// Destructor
    ~Buffer();

// Create Functions
    bool                              CreateVkBuffer(void);
    void                              CreateVkDescriptorBufferInfo(void);

// Release Functions
    void                              Release(void);

// Get Functions
    inline VkBuffer &                 GetVkBuffer(void)                         { FUN_ENTRY(GL_LOG_TRACE); return mVkBuffer;                }
    inline VkDescriptorBufferInfo*    GetVkDescriptorBufferInfo(void)           { FUN_ENTRY(GL_LOG_TRACE); return &mVkDescriptorBufferInfo; }
    inline VkDeviceSize               GetSize(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mVkSize;                  }
    inline VkBufferUsageFlags         GetFlags(void)                    const   { FUN_ENTRY(GL_LOG_TRACE); return mVkBufferUsageFlags;      }

// Set Functions
    inline void                       SetSize(VkDeviceSize size)                { FUN_ENTRY(GL_LOG_TRACE); mVkSize             = size;      }
    inline void                       SetFlags(VkBufferUsageFlags flags)        { FUN_ENTRY(GL_LOG_TRACE); mVkBufferUsageFlags = flags;     }
    inline void                       SetContext(const vkContext_t *vkContext)  { FUN_ENTRY(GL_LOG_TRACE); mVkContext          = vkContext; }
};

}

#endif // __VKBUFFER_H__
