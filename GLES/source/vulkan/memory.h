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
 *  @file       memory.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Device Memory Allocation Functionality in Vulkan
 *
 */

#ifndef __VKMEMORY_H__
#define __VKMEMORY_H__

#include "utils/globals.h"
#include "utils.h"

namespace vulkanAPI {

class Memory {

private:

    const
    vkContext_t *                     mVkContext;

    VkDeviceMemory                    mVkMemory;
    const
    VkMemoryMapFlags                  mVkMemoryFlags;
    VkFlags                           mVkFlags;
    VkMemoryRequirements              mVkRequirements;

public:
// Constructor
    Memory(const vkContext_t *vkContext = nullptr, VkFlags flags = 0);

// Destructor
    ~Memory();

// Allocate Functions
    bool                              Allocate(void);

// Release Functions
    void                              Release(void);

// Bind Functions
    bool                              BindBufferMemory(VkBuffer &buffer);
    bool                              BindImageMemory(VkImage &image);

// Get Functions
    void                              GetImageMemoryRequirements(VkImage &image);
    bool                              GetBufferMemoryRequirements(VkBuffer &buffer);
    bool                              GetData(VkDeviceSize size, VkDeviceSize offset, void *data) const;
    VkResult                          GetMemoryTypeIndexFromProperties(uint32_t *typeIndex);

// Set/Update Functions
    bool                              SetData(VkFormat srcFormat, bool normalize, VkDeviceSize size, VkDeviceSize offset, const void *data);
    void                              UpdateData(VkDeviceSize size, VkDeviceSize offset, const void *data);

    inline void                       SetContext(const vkContext_t *vkContext)  { FUN_ENTRY(GL_LOG_TRACE); mVkContext = vkContext; }
};

}

#endif // __VKMEMORY_H__
