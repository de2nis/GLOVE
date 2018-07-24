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
 *  @file       memory.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Device Memory Allocation Functionality in Vulkan
 *
 *  @section
 *
 *  Device memory is memory that is visible to the device — for example the
 *  contents of the image or buffer objects, which can be natively used by
 *  the device. Memory properties of a physical device describe the memory
 *  heaps and memory types available.
 *
 */

#include "memory.h"
#include "context/context.h"

namespace vulkanAPI {

Memory::Memory(const vkContext_t *vkContext, VkFlags flags)
: mVkContext(vkContext), mVkMemory (VK_NULL_HANDLE), mVkMemoryFlags(0), mVkFlags(flags)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

Memory::~Memory()
{
    FUN_ENTRY(GL_LOG_TRACE);

    Release();
}

void
Memory::Release(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkMemory != VK_NULL_HANDLE) {
        vkFreeMemory(mVkContext->vkDevice, mVkMemory, NULL);
        mVkMemory = VK_NULL_HANDLE;
    }
}

bool
Memory::GetData(VkDeviceSize size, VkDeviceSize offset, void *data) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    void *pData;
    VkResult err = vkMapMemory(mVkContext->vkDevice, mVkMemory, offset, size, mVkMemoryFlags, &pData);
    assert(!err);

    if(err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY && err != VK_ERROR_MEMORY_MAP_FAILED)
    {
        memcpy(data, pData, size);
        vkUnmapMemory(mVkContext->vkDevice, mVkMemory);

        return true;
    }
    return false;
}

void
Memory::UpdateData(VkDeviceSize size, VkDeviceSize offset, const void *data)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    SetData(VK_FORMAT_UNDEFINED, false, size, offset, data);
}

bool
Memory::SetData(VkFormat srcFormat, bool normalize, VkDeviceSize size, VkDeviceSize offset, const void *data)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    void *pData = NULL;

    VkResult err = vkMapMemory(mVkContext->vkDevice, mVkMemory, offset, size ? size : mVkRequirements.size, mVkMemoryFlags, &pData);
    assert(!err);

    if(data) {
        if(srcFormat == VK_FORMAT_UNDEFINED  ||
           srcFormat == VK_FORMAT_R32_SFLOAT ||
           srcFormat == VK_FORMAT_R32G32_SFLOAT ||
           srcFormat == VK_FORMAT_R32G32B32_SFLOAT ||
           srcFormat == VK_FORMAT_R32G32B32A32_SFLOAT) {
            /// data is already float
            memcpy(pData, data, size);
        } else if(srcFormat == VK_FORMAT_R16_SSCALED ||
                  srcFormat == VK_FORMAT_R16G16_SSCALED ||
                  srcFormat == VK_FORMAT_R16G16B16_SSCALED ||
                  srcFormat == VK_FORMAT_R16G16B16A16_SSCALED) {
            /// fixed point to float conversion
            float *pDst         = (float *)pData;
            const GLfixed *pSrc = (GLfixed *)data;

            for(size_t i = 0; i < floor(size / (float)sizeof(float)); ++i) {
                pDst[i] = normalize ? GLOVE_FIXED_TO_FLOAT(pSrc[i]) / 255.0f : GLOVE_FIXED_TO_FLOAT(pSrc[i]);
            }
        } else {
            bool isUnsigned;
            float *pDst = (float *)pData;
            const size_t nBytes = nBytesOfVkIntFormat(srcFormat, &isUnsigned);

            /// if not float, the format will either be 1 or 2 bytes
            /// for every nBytes bytes, copy the corresponding 4 byte float
            switch(nBytes) {
            case 1:
                if(isUnsigned) {
                    const uint8_t *pSrc = (uint8_t *)data;
                    for(size_t i = 0; i < floor(size / (float)sizeof(float)); ++i) {
                        pDst[i] = normalize ? (float)pSrc[i] / 255.0f : (float)pSrc[i];
                    }
                } else {
                    const int8_t *pSrc = (int8_t *)data;
                    for(size_t i = 0; i < floor(size / (float)sizeof(float)); ++i) {
                        pDst[i] = normalize ? (float)pSrc[i] / 255.0f : (float)pSrc[i];
                    }
                }
                break;

            case 2:
                if(isUnsigned) {
                    const uint16_t *pSrc = (uint16_t *)data;
                    for(size_t i = 0; i < floor(size / (float)sizeof(float)); ++i) {
                        pDst[i] = normalize ? (float)pSrc[i] / 255.0f : (float)pSrc[i];
                    }
                } else {
                    const int16_t *pSrc = (int16_t *)data;
                    for(size_t i = 0; i < floor(size / (float)sizeof(float)); ++i) {
                        pDst[i] = normalize ? (float)pSrc[i] / 255.0f : (float)pSrc[i];
                    }
                }
                break;

            case 4:
                if(isUnsigned) {
                    const uint32_t *pSrc = (uint32_t *)data;
                    for(size_t i = 0; i < floor(size / (float)sizeof(float)); ++i) {
                        pDst[i] = normalize ? (float)pSrc[i] / 255.0f : (float)pSrc[i];
                    }
                } else {
                    const int32_t *pSrc = (int32_t *)data;
                    for(size_t i = 0; i < floor(size / (float)sizeof(float)); ++i) {
                        pDst[i] = normalize ? (float)pSrc[i] / 255.0f : (float)pSrc[i];
                    }
                }
                break;

            default:
                NOT_REACHED();
                break;
            }
        }
    } else {
        memset(pData, 0x0, size);
    }

    vkUnmapMemory(mVkContext->vkDevice, mVkMemory);

    return (err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY);
}

bool
Memory::GetBufferMemoryRequirements(VkBuffer &buffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    memset((void *)&mVkRequirements, 0, sizeof(mVkRequirements));
    vkGetBufferMemoryRequirements(mVkContext->vkDevice, buffer, &mVkRequirements);

    return true;
}

void
Memory::GetImageMemoryRequirements(VkImage &image)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    memset((void *)&mVkRequirements, 0, sizeof(mVkRequirements));
    vkGetImageMemoryRequirements(mVkContext->vkDevice, image, &mVkRequirements);
}

VkResult
Memory::GetMemoryTypeIndexFromProperties(uint32_t *typeIndex)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    uint32_t typeBitsShift = mVkRequirements.memoryTypeBits;

    // Search memtypes to find first index with those properties
    for(uint32_t i = 0; i < mVkContext->vkDeviceMemoryProperties.memoryTypeCount; i++) {
        if((typeBitsShift & 1) == 1) {
            // Type is available, does it match user properties?
            if ((mVkContext->vkDeviceMemoryProperties.memoryTypes[i].propertyFlags & mVkFlags) == mVkFlags) {
                *typeIndex = i;
                return VK_SUCCESS;
            }
        }
        typeBitsShift >>= 1;
    }

    typeBitsShift = mVkRequirements.memoryTypeBits;

    // Retry with properties = 0x0
    for(uint32_t i = 0; i < mVkContext->vkDeviceMemoryProperties.memoryTypeCount; i++) {
        if((typeBitsShift & 1) == 1) {
            // Type is available, does it match user properties?
            if ((mVkContext->vkDeviceMemoryProperties.memoryTypes[i].propertyFlags & 0) == 0) {
                *typeIndex = i;
                return VK_SUCCESS;
            }
        }
        typeBitsShift >>= 1;
    }

     // No memory types matched, return failure
     return VK_ERROR_FORMAT_NOT_SUPPORTED;
}

bool
Memory::BindBufferMemory(VkBuffer &buffer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkResult err = vkBindBufferMemory(mVkContext->vkDevice, buffer, mVkMemory, 0);
    assert(!err);

    return (err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY);
}

bool
Memory::BindImageMemory(VkImage &image)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkResult err = vkBindImageMemory(mVkContext->vkDevice, image, mVkMemory, 0);
    assert(!err);

    return (err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY);
}

bool
Memory::Allocate()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext           = NULL;
    allocInfo.memoryTypeIndex = 0;
    allocInfo.allocationSize  = mVkRequirements.size;

    VkResult err;
    err = GetMemoryTypeIndexFromProperties(&allocInfo.memoryTypeIndex);
    assert(!err);
    err = vkAllocateMemory(mVkContext->vkDevice, &allocInfo, NULL, &mVkMemory);
    assert(!err);

    return (err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY);
}

}
