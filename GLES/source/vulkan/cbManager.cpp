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
 *  @file       cbManager.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Command Buffers Manager Functionality in Vulkan
 *
 *  @section
 *
 *  Command buffers are objects used to record commands which can be
 *  subsequently submitted to a device queue for execution. There are
 *  two levels of command buffers - primary command buffers, which can
 *  execute secondary command buffers, and which are submitted to queues,
 *  and secondary command buffers, which can be executed by primary command
 *  buffers, and which are not directly submitted to queues.
 *  Command buffers are represented by VkCommandBuffer.
 *
 */

#include "cbManager.h"

CommandBufferManager *CommandBufferManager::mInstance = nullptr;

CommandBufferManager::CommandBufferManager(vkContext_t *context)
: mVkContext(context)
{
    FUN_ENTRY(GL_LOG_TRACE);

    mActiveCmdBuffer    = 0;
    mLastSubmittedBuffer= GLOVE_NO_BUFFER_TO_WAIT;

    mVkCmdPool          = VK_NULL_HANDLE;
    mVkAuxCommandBuffer = VK_NULL_HANDLE;
    mVkAuxFence         = VK_NULL_HANDLE;
}

CommandBufferManager::~CommandBufferManager()
{
    FUN_ENTRY(GL_LOG_TRACE);

    assert(this == mInstance);

    for(uint32_t i = 0; i < mReferencedResources.size(); ++i) {
        mReferencedResources[i]->mRefCount = 0;
    }

    FreeResources();

    mReferencedResources.clear();

    if(mVkContext->vkDevice != VK_NULL_HANDLE ) {

        vkDeviceWaitIdle(mVkContext->vkDevice);

        for(uint32_t i = 0; i < mVkCommandBuffers.fence.size(); ++i) {
            if(mVkCommandBuffers.fence[i] != VK_NULL_HANDLE) {
                vkDestroyFence(mVkContext->vkDevice, mVkCommandBuffers.fence[i], NULL);
            }
        }

        if(mVkAuxFence != VK_NULL_HANDLE) {
            vkDestroyFence(mVkContext->vkDevice, mVkAuxFence, NULL);
        }

        vkFreeCommandBuffers(mVkContext->vkDevice, mVkCmdPool, mVkCommandBuffers.commandBuffer.size(), mVkCommandBuffers.commandBuffer.data());
        mVkCommandBuffers.commandBuffer.clear();
        mVkCommandBuffers.commandBufferState.clear();
        mVkCommandBuffers.fence.clear();
        memset((void *)&mVkCommandBuffers, 0, mVkCommandBuffers.commandBuffer.size()*sizeof(State));

        if(mVkAuxCommandBuffer != VK_NULL_HANDLE) {
            vkFreeCommandBuffers(mVkContext->vkDevice, mVkCmdPool, 1, &mVkAuxCommandBuffer);
            mVkAuxCommandBuffer = VK_NULL_HANDLE;
        }

        if(mVkCmdPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(mVkContext->vkDevice, mVkCmdPool, NULL);
            mVkCmdPool = VK_NULL_HANDLE;
        }
    }
}

CommandBufferManager *
CommandBufferManager::GetCommandBufferManager(vkContext_t *context)
{
    FUN_ENTRY(GL_LOG_TRACE);

    if(mInstance == nullptr) {
        mInstance = new CommandBufferManager(context);
    }

    return mInstance;
}

void
CommandBufferManager::ResetCommandBufferManager(void)
{
    FUN_ENTRY(GL_LOG_TRACE);

    delete mInstance;

    mInstance = nullptr;
}

bool
CommandBufferManager::AllocateVkCmdBuffers(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkResult err;

    VkCommandPoolCreateInfo cmdPoolInfo;
    memset((void *)&cmdPoolInfo, 0 ,sizeof(cmdPoolInfo));
    cmdPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext            = NULL;
    cmdPoolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdPoolInfo.queueFamilyIndex = mVkContext->vkGraphicsQueueNodeIndex;

    err = vkCreateCommandPool(mVkContext->vkDevice, &cmdPoolInfo, NULL, &mVkCmdPool);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    mVkCommandBuffers.commandBuffer.resize(GLOVE_NUM_VK_COMMAND_BUFFERS);
    mVkCommandBuffers.commandBufferState.resize(GLOVE_NUM_VK_COMMAND_BUFFERS);
    mVkCommandBuffers.fence.resize(GLOVE_NUM_VK_COMMAND_BUFFERS);

    VkCommandBufferAllocateInfo cmdAllocInfo;
    cmdAllocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdAllocInfo.pNext              = NULL;
    cmdAllocInfo.commandPool        = mVkCmdPool;
    cmdAllocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdAllocInfo.commandBufferCount = GLOVE_NUM_VK_COMMAND_BUFFERS;

    err = vkAllocateCommandBuffers(mVkContext->vkDevice, &cmdAllocInfo, mVkCommandBuffers.commandBuffer.data());
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    cmdAllocInfo.commandBufferCount = 1;
    err = vkAllocateCommandBuffers(mVkContext->vkDevice, &cmdAllocInfo, &mVkAuxCommandBuffer);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    mVkAuxCommandBufferState = CMD_BUFFER_INITIAL_STATE;

    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;

    for(uint32_t i = 0; i < GLOVE_NUM_VK_COMMAND_BUFFERS; ++i) {
        mVkCommandBuffers.commandBufferState[i] = CMD_BUFFER_INITIAL_STATE;

        err = vkCreateFence(mVkContext->vkDevice, &fenceInfo, NULL, &mVkCommandBuffers.fence[i]);
        assert(!err);

        if(err != VK_SUCCESS) {
            return false;
        }
    }

    err = vkCreateFence(mVkContext->vkDevice, &fenceInfo, NULL, &mVkAuxFence);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    return true;
}

void
CommandBufferManager::FreeResources(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(std::vector<resourceBase_t *>::iterator it = mReferencedResources.begin(); it != mReferencedResources.end();) {
        const resourceBase_t *resourceBase = *it;
        if(!resourceBase->mRefCount) {
            switch(resourceBase->mType) {
            case RESOURCE_TYPE_SHADER: {
                referencedResource_t<VkShaderModule> *resource = (referencedResource_t<VkShaderModule> *)resourceBase;
                vkDestroyShaderModule(mVkContext->vkDevice, resource->mResourcePtr, NULL);
                } break;
            case RESOURCE_TYPE_PIPELINE_LAYOUT: {
                referencedResource_t<VkPipelineLayout> *resource = (referencedResource_t<VkPipelineLayout> *)resourceBase;
                vkDestroyPipelineLayout(mVkContext->vkDevice, resource->mResourcePtr, NULL);
                } break;
            case RESOURCE_TYPE_DESC_POOL: {
                referencedResource_t<VkDescriptorPool> *resource = (referencedResource_t<VkDescriptorPool> *)resourceBase;
                vkDestroyDescriptorPool(mVkContext->vkDevice, resource->mResourcePtr, NULL);
                } break;
            case RESOURCE_TYPE_DESC_SET_LAYOUT: {
                referencedResource_t<VkDescriptorSetLayout> *resource = (referencedResource_t<VkDescriptorSetLayout> *)resourceBase;
                vkDestroyDescriptorSetLayout(mVkContext->vkDevice, resource->mResourcePtr, NULL);
                } break;
            default: NOT_REACHED(); break;
            }

            delete *it;
            it = mReferencedResources.erase(it);
        } else {
            ++it;
        }
    }
}

bool
CommandBufferManager::BeginVkDrawCommandBuffer(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] == CMD_BUFFER_RECORDING_STATE) {
        return true;
    }

    assert(mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] == CMD_BUFFER_INITIAL_STATE ||
           mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] == CMD_BUFFER_SUBMITED_STATE);

    VkCommandBufferBeginInfo cmdBeginInfo;
    cmdBeginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBeginInfo.pNext            = NULL;
    cmdBeginInfo.flags            = 0;
    cmdBeginInfo.pInheritanceInfo = NULL;

    VkResult err = vkBeginCommandBuffer(mVkCommandBuffers.commandBuffer[mActiveCmdBuffer], &cmdBeginInfo);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] = CMD_BUFFER_RECORDING_STATE;

    return true;
}

void
CommandBufferManager::EndVkDrawCommandBuffer(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] == CMD_BUFFER_EXECUTABLE_STATE ||
       mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] == CMD_BUFFER_INITIAL_STATE) {
        return;
    }

    assert(mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] == CMD_BUFFER_RECORDING_STATE);

    vkEndCommandBuffer(mVkCommandBuffers.commandBuffer[mActiveCmdBuffer]);

    mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] = CMD_BUFFER_EXECUTABLE_STATE;
}

bool
CommandBufferManager::SubmitVkDrawCommandBuffer(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] == CMD_BUFFER_INITIAL_STATE) {
        return true;
    }

    assert(mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] == CMD_BUFFER_EXECUTABLE_STATE);

    vector<VkSemaphore> pSems;
    vector<VkPipelineStageFlags> pFlags;
    if(mVkContext->vkSyncItems->auxSemaphoreFlag) {
        pSems.push_back(mVkContext->vkSyncItems->vkAuxSemaphore);
        pFlags.push_back(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    }
    if(mVkContext->vkSyncItems->acquireSemaphoreFlag) {
        pSems.push_back(mVkContext->vkSyncItems->vkAcquireSemaphore);
        pFlags.push_back(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    }
    if(mVkContext->vkSyncItems->drawSemaphoreFlag) {
        pSems.push_back(mVkContext->vkSyncItems->vkDrawSemaphore);
        pFlags.push_back(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    }

    VkSubmitInfo submitInfo;
    submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext                = NULL;
    submitInfo.pWaitDstStageMask    = pFlags.data();
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &mVkCommandBuffers.commandBuffer[mActiveCmdBuffer];
    submitInfo.waitSemaphoreCount   = pSems.size();
    submitInfo.pWaitSemaphores      = pSems.data();
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = &mVkContext->vkSyncItems->vkDrawSemaphore;

    mVkContext->vkSyncItems->drawSemaphoreFlag    = true;
    mVkContext->vkSyncItems->auxSemaphoreFlag     = false;
    mVkContext->vkSyncItems->acquireSemaphoreFlag = false;

    VkResult err = vkQueueSubmit(mVkContext->vkQueue, 1, &submitInfo, mVkCommandBuffers.fence[mActiveCmdBuffer]);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] = CMD_BUFFER_SUBMITED_STATE;

    mLastSubmittedBuffer = mActiveCmdBuffer;
    mActiveCmdBuffer = (mActiveCmdBuffer + 1) % GLOVE_NUM_VK_COMMAND_BUFFERS;

    mVkCommandBuffers.commandBufferState[mActiveCmdBuffer] = CMD_BUFFER_INITIAL_STATE;

    return true;
}

bool
CommandBufferManager::WaitLastSubmition(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkResult err;

    if(mLastSubmittedBuffer != GLOVE_NO_BUFFER_TO_WAIT) {

        err = vkWaitForFences(mVkContext->vkDevice, 1, &mVkCommandBuffers.fence[mLastSubmittedBuffer], VK_TRUE, GLOVE_FENCE_WAIT_TIMEOUT);
        assert(!err);

        if(err != VK_SUCCESS) {
            return false;
        }

        FreeResources();

        vkResetFences(mVkContext->vkDevice, 1, &mVkCommandBuffers.fence[mLastSubmittedBuffer]);
        assert(!err);

        if(err != VK_SUCCESS) {
            return false;
        }

        mLastSubmittedBuffer = GLOVE_NO_BUFFER_TO_WAIT;
    }

    return true;
}

bool
CommandBufferManager::BeginVkAuxCommandBuffer(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mVkAuxCommandBufferState == CMD_BUFFER_INITIAL_STATE);

    VkCommandBufferBeginInfo cmdBeginInfo;
    cmdBeginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBeginInfo.pNext            = NULL;
    cmdBeginInfo.flags            = 0;
    cmdBeginInfo.pInheritanceInfo = NULL;

    VkResult err = vkBeginCommandBuffer(mVkAuxCommandBuffer, &cmdBeginInfo);
    assert(!err);

    mVkAuxCommandBufferState = CMD_BUFFER_RECORDING_STATE;

    if(err != VK_SUCCESS) {
        return false;
    }
    return true;
}

void
CommandBufferManager::EndVkAuxCommandBuffer(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mVkAuxCommandBufferState == CMD_BUFFER_RECORDING_STATE);

    vkEndCommandBuffer(mVkAuxCommandBuffer);

    mVkAuxCommandBufferState = CMD_BUFFER_EXECUTABLE_STATE;
}

bool
CommandBufferManager::SubmitVkAuxCommandBuffer(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mVkAuxCommandBufferState == CMD_BUFFER_EXECUTABLE_STATE);

    vector<VkSemaphore> pSems;
    vector<VkPipelineStageFlags> pFlags;

    if(mVkContext->vkSyncItems->auxSemaphoreFlag) {
        pSems.push_back(mVkContext->vkSyncItems->vkAuxSemaphore);
        pFlags.push_back(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    }
    if(mVkContext->vkSyncItems->acquireSemaphoreFlag) {
        pSems.push_back(mVkContext->vkSyncItems->vkAcquireSemaphore);
        pFlags.push_back(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    }
    if(mVkContext->vkSyncItems->drawSemaphoreFlag) {
        pSems.push_back(mVkContext->vkSyncItems->vkDrawSemaphore);
        pFlags.push_back(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    }

    VkSubmitInfo submitInfo;
    submitInfo.sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext                  = NULL;
    submitInfo.pWaitDstStageMask      = pFlags.data();
    submitInfo.commandBufferCount     = 1;
    submitInfo.pCommandBuffers        = &mVkAuxCommandBuffer;
    submitInfo.waitSemaphoreCount     = pSems.size();
    submitInfo.pWaitSemaphores        = pSems.data();
    submitInfo.signalSemaphoreCount   = 1;
    submitInfo.pSignalSemaphores      = &mVkContext->vkSyncItems->vkAuxSemaphore;

    mVkContext->vkSyncItems->auxSemaphoreFlag     = true;
    mVkContext->vkSyncItems->drawSemaphoreFlag    = false;
    mVkContext->vkSyncItems->acquireSemaphoreFlag = false;

    VkResult err = vkQueueSubmit(mVkContext->vkQueue, 1, &submitInfo, mVkAuxFence);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    mVkAuxCommandBufferState = CMD_BUFFER_SUBMITED_STATE;

    return true;
}

bool
CommandBufferManager::WaitVkAuxCommandBuffer(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkResult err;

    assert(mVkAuxCommandBufferState == CMD_BUFFER_SUBMITED_STATE);

    err = vkWaitForFences(mVkContext->vkDevice, 1, &mVkAuxFence, VK_TRUE, GLOVE_FENCE_WAIT_TIMEOUT);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    FreeResources();

    mVkAuxCommandBufferState = CMD_BUFFER_INITIAL_STATE;

    err = vkResetFences(mVkContext->vkDevice, 1, &mVkAuxFence);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    return true;
}
