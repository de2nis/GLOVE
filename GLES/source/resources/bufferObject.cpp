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
 *  @file       bufferObject.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Buffer Object Functionality in GLOVE
 *
 *  @scope
 *
 *  Buffer Objects store an array of unformatted memory allocated by the OpenGL
 *  ES context (GPU). These can be used to store vertex data, pixel data retrieved
 *  from images or the framebuffer, and a variety of other things.
 *
 */

#include "bufferObject.h"

BufferObject::BufferObject(const vkContext_t *vkContext, const VkBufferUsageFlags vkBufferUsageFlags, const VkSharingMode vkSharingMode, const VkFlags vkFlags)
: mVkContext(vkContext), mUsage(GL_STATIC_DRAW), mTarget(GL_INVALID_VALUE)
{
    FUN_ENTRY(GL_LOG_TRACE);

    mBuffer = new vulkanAPI::Buffer(vkContext, vkBufferUsageFlags, vkSharingMode);
    mMemory = new vulkanAPI::Memory(vkContext, vkFlags);
}

BufferObject::~BufferObject()
{
    FUN_ENTRY(GL_LOG_TRACE);

    delete mBuffer;
    delete mMemory;
}

void
BufferObject::Release()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mBuffer->Release();
    mMemory->Release();
}

bool
BufferObject::Allocate(VkFormat srcFormat, bool normalize, size_t size, const void *data)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mBuffer->SetSize(size);
    if(mBuffer->GetFlags() == VK_NULL_HANDLE)
        mBuffer->SetFlags(mTarget == GL_ELEMENT_ARRAY_BUFFER ? VK_BUFFER_USAGE_INDEX_BUFFER_BIT : VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    return mBuffer->CreateVkBuffer()                                       &&
           mMemory->GetBufferMemoryRequirements(mBuffer->GetVkBuffer())    &&
           mMemory->Allocate()                                             &&
           mMemory->SetData(srcFormat, normalize, size, 0, data)           &&
           mMemory->BindBufferMemory(mBuffer->GetVkBuffer());
}

bool
BufferObject::Allocate(size_t size, const void *data)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return Allocate(VK_FORMAT_UNDEFINED, false, size, data);
}

void
BufferObject::GetData(size_t size, size_t offset, void *data) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mMemory->GetData(size, offset, data);
}

void
BufferObject::UpdateData(size_t size, size_t offset, const void *data)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mMemory->UpdateData(size, offset, data);
}

bool
UniformBufferObject::Allocate(VkFormat srcFormat, bool normalize, size_t size, const void *data)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    bool res = BufferObject::Allocate(srcFormat, normalize, size, data);

    if(res) {
        AllocateVkDescriptorBufferInfo();
    }

    return res;
}

void
UniformBufferObject::AllocateVkDescriptorBufferInfo()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mBuffer->CreateVkDescriptorBufferInfo();
}
