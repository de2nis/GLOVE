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
 *  @file       bufferObject.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Buffer Object Functionality in GLOVE
 *
 */

#ifndef __BUFFEROBJECT_H__
#define __BUFFEROBJECT_H__

#include "vulkan/buffer.h"
#include "vulkan/memory.h"

class BufferObject {
private:
    const vkContext_t*      mVkContext;

    GLenum                  mUsage;
    GLenum                  mTarget;

    vulkanAPI::Memory*      mMemory;

protected:
    vulkanAPI::Buffer*      mBuffer;

public:
    explicit                BufferObject(const vkContext_t       *vkContext          = nullptr,
                                         const VkBufferUsageFlags vkBufferUsageFlags = VK_NULL_HANDLE,
                                         const VkSharingMode      vkSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
                                         const VkFlags            vkFlags            = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    virtual                ~BufferObject();

// Allocate Functions
    bool                    Allocate(size_t size, const void *data);

// Release Functions
    void                    Release(void);

// Update Functions        
    void                    UpdateData(size_t size, size_t offset, const void *data);

// Get Functions
    void                    GetData(size_t size,
                                    size_t offset, void *data)          const;
    inline GLenum           GetUsage(void)                              const   { FUN_ENTRY(GL_LOG_TRACE); return mUsage;  }
    inline GLenum           GetTarget(void)                             const   { FUN_ENTRY(GL_LOG_TRACE); return mTarget; }
    inline size_t           GetSize(void)                               const   { FUN_ENTRY(GL_LOG_TRACE); return mBuffer->GetSize(); }
    inline VkBuffer         GetVkBuffer(void)                                   { FUN_ENTRY(GL_LOG_TRACE); return mBuffer->GetVkBuffer(); }

// Set Functions
    inline void             SetTarget(GLenum target)                            { FUN_ENTRY(GL_LOG_TRACE); mTarget    = target; }
    inline void             SetUsage(GLenum usage)                              { FUN_ENTRY(GL_LOG_TRACE); mUsage     = usage; }
    inline void             SetContext(const vkContext_t *vkContext)            { FUN_ENTRY(GL_LOG_TRACE); mVkContext = vkContext;
                                                                                                       mBuffer->SetContext(vkContext);
                                                                                                       mMemory->SetContext(vkContext); }
// Has/Is Functions
    inline bool             HasData(void)                               const   { FUN_ENTRY(GL_LOG_TRACE); return mBuffer->GetVkBuffer() != VK_NULL_HANDLE; }

// Virtual Functions
    virtual bool            Allocate(VkFormat srcFormat, bool normalize, size_t size, const void *data);
};

class IndexBufferObject : public BufferObject
{

public:
    explicit                IndexBufferObject(const vkContext_t *vkContext = nullptr)        : BufferObject(vkContext, VK_BUFFER_USAGE_INDEX_BUFFER_BIT) { FUN_ENTRY(GL_LOG_TRACE); }

};

class TransferSrcBufferObject : public BufferObject
{

public:
    explicit                TransferSrcBufferObject(const vkContext_t *vkContext = nullptr)  : BufferObject(vkContext, VK_BUFFER_USAGE_TRANSFER_SRC_BIT) { FUN_ENTRY(GL_LOG_TRACE); }

};

class TransferDstBufferObject : public BufferObject
{

public:
    explicit                TransferDstBufferObject(const vkContext_t *vkContext = nullptr)  : BufferObject(vkContext, VK_BUFFER_USAGE_TRANSFER_DST_BIT) { FUN_ENTRY(GL_LOG_TRACE); }

};

class UniformBufferObject : public BufferObject
{
    void                    AllocateVkDescriptorBufferInfo(void);

public:
    explicit                UniformBufferObject(const vkContext_t *vkContext = nullptr)      : BufferObject(vkContext, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) { FUN_ENTRY(GL_LOG_TRACE); }

    bool                    Allocate(VkFormat srcFormat, bool normalize, size_t size, const void *data);
    VkDescriptorBufferInfo* GetBufferDescInfo(void)                             { FUN_ENTRY(GL_LOG_TRACE); return mBuffer->GetVkDescriptorBufferInfo(); }
};

class VertexBufferObject : public BufferObject
{

public:
    explicit                VertexBufferObject(const vkContext_t *vkContext = nullptr)       : BufferObject(vkContext, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) { FUN_ENTRY(GL_LOG_TRACE); }

};

#endif // __BUFFEROBJECT_H__
