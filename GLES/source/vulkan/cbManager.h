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
 *  @file       cbManager.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Command Buffers Manager Functionality in Vulkan
 *
 */

#ifndef __VKCBMANAGER_H__
#define __VKCBMANAGER_H__

#include "utils/globals.h"

typedef enum {
    CMD_BUFFER_INITIAL_STATE = 0,
    CMD_BUFFER_RECORDING_STATE,
    CMD_BUFFER_EXECUTABLE_STATE,
    CMD_BUFFER_SUBMITED_STATE
} cmdBufferState_t;

typedef enum {
    RESOURCE_TYPE_SHADER = 0,
    RESOURCE_TYPE_PIPELINE_LAYOUT,
    RESOURCE_TYPE_DESC_POOL,
    RESOURCE_TYPE_DESC_SET_LAYOUT,
    RESOURCE_TYPE_DESC_SET,
    RESOURCE_TYPE_LAST
} resourceType_t;

class resourceBase_t {
public:

    uint32_t                        mRefCount;
    resourceType_t                  mType;

    resourceBase_t(resourceType_t type) : mRefCount(1), mType(type)             { FUN_ENTRY(GL_LOG_TRACE); }
    virtual ~resourceBase_t()                                                   { FUN_ENTRY(GL_LOG_TRACE); }
};

template<typename T>
class referencedResource_t : public resourceBase_t {
public:

    T                               mResourcePtr;

    referencedResource_t(T resourcePtr, resourceType_t type)
    : resourceBase_t(type), mResourcePtr(resourcePtr)                           { FUN_ENTRY(GL_LOG_TRACE); }
};

class CommandBufferManager {
private:

    typedef struct State {
        vector<VkCommandBuffer>         commandBuffer;
        vector<cmdBufferState_t>        commandBufferState;
        vector<VkFence>                 fence;

        State()  { FUN_ENTRY(GL_LOG_TRACE); }
        ~State() { FUN_ENTRY(GL_LOG_TRACE); }
    } State;

    static CommandBufferManager    *mInstance;
    VkCommandPool                   mVkCmdPool;
    vkContext_t                    *mVkContext;

    uint32_t                        mActiveCmdBuffer;
    int32_t                         mLastSubmittedBuffer;

    State                           mVkCommandBuffers;

    VkCommandBuffer                 mVkAuxCommandBuffer;
    cmdBufferState_t                mVkAuxCommandBufferState;
    VkFence                         mVkAuxFence;

    std::vector<resourceBase_t *>   mReferencedResources;

    void FreeResources(void);

public:
// Constructor
    CommandBufferManager(vkContext_t *context = nullptr);

// Destructor
    ~CommandBufferManager();

// Get Functions
    static CommandBufferManager *GetCommandBufferManager(vkContext_t *context);

// Reset Functions
    static void                  ResetCommandBufferManager(void);

// Allocate Functions
    bool AllocateVkCmdBuffers(void);

// Begin Functions
    bool BeginVkAuxCommandBuffer(void);
    bool BeginVkDrawCommandBuffer(void);

// End Functions
    void EndVkAuxCommandBuffer(void);
    void EndVkDrawCommandBuffer(void);

// Submit Functions
    bool SubmitVkDrawCommandBuffer(void);
    bool SubmitVkAuxCommandBuffer(void);

// Wait Functions
    bool WaitLastSubmition(void);
    bool WaitVkAuxCommandBuffer(void);

// Get Functions
    inline VkCommandBuffer GetActiveCommandBuffer(void)                   const { FUN_ENTRY(GL_LOG_TRACE); return mVkCommandBuffers.commandBuffer[mActiveCmdBuffer]; }
    inline VkCommandBuffer GetAuxCommandBuffer(void)                      const { FUN_ENTRY(GL_LOG_TRACE); return mVkAuxCommandBuffer; }

// Resource Functions
    template<typename T>
    void RefResource(T resource, resourceType_t type)
    {
        FUN_ENTRY(GL_LOG_TRACE);

        int index = LocateResource(resource);
        if(index == -1) {
            mReferencedResources.push_back(new referencedResource_t<T>(resource, type));
        } else {
            ++mReferencedResources[index]->mRefCount;
        }
    }

    template<typename T>
    void UnrefResouce(T resource)
    {
        FUN_ENTRY(GL_LOG_TRACE);

        int index = LocateResource(resource);
        if(index != -1) {
            assert(mReferencedResources[index]->mRefCount);
            --mReferencedResources[index]->mRefCount;
        }
    }

    template<typename T>
    int LocateResource(T resource)
    {
        FUN_ENTRY(GL_LOG_TRACE);

        assert(resource);

        for(uint32_t i = 0; i < mReferencedResources.size(); ++i) {
            referencedResource_t<T> *res = (referencedResource_t<T> *)mReferencedResources[i];
            assert(res);
            if(res->mResourcePtr == resource) {
                return i;
            }
        }

        return -1;
    }
};

#endif // __VKCBMANAGER_H__
