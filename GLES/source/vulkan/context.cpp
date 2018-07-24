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
 *  @file       context.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Context Functionality via Vulkan
 *
 *  @section
 *
 *  Before using Vulkan, an application must initialize it by loading the Vulkan
 *  commands, and creating a VkInstance object. Once Vulkan is initialized, devices
 *  and queues are the primary objects used to interact with a Vulkan implementation.
 *
 */

#include "context.h"
#include "cbManager.h"

namespace vulkanAPI {

#define ARRAY_SIZE(array)   (int)(sizeof(array) / sizeof(array[0]))

#ifdef VK_USE_PLATFORM_XCB_KHR
static const char *requiredInstanceExtensions[] =     {VK_KHR_SURFACE_EXTENSION_NAME,
                                                       VK_KHR_XCB_SURFACE_EXTENSION_NAME};
#endif

static const char  *requiredDeviceExtensions[]      = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                                       "VK_KHR_maintenance1"};
static       char **enabledInstanceLayers           = NULL;

static vkContext_t GloveVkContext;

static bool InitVkLayers(uint32_t* nLayers);
static bool CheckVkInstanceExtensions(void);
static bool CheckVkDeviceExtensions(void);
static bool CreateVkInstance(void);
static bool EnumerateVkGpus(void);
static bool InitVkQueueFamilyIndex(void);
static bool CreateVkDevice(void);
static bool CreateVkCommandBuffers(void);
static bool CreateVkSemaphores(void);
static void InitVkQueue(void);

static bool
InitVkLayers(uint32_t* nLayers)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkResult res;
    uint32_t layerCount = 0;
    VkLayerProperties *vkLayerProperties = NULL;

    do {
        res = vkEnumerateInstanceLayerProperties(&layerCount, NULL);

        if(!layerCount || res) {
            break;
        }

        vkLayerProperties = (VkLayerProperties *)realloc(vkLayerProperties, layerCount * sizeof(VkLayerProperties));
        if(!vkLayerProperties) {
            return false;
        }

        res = vkEnumerateInstanceLayerProperties(&layerCount, vkLayerProperties);
    } while(res == VK_INCOMPLETE);

    if(layerCount) {
        enabledInstanceLayers = (char**)malloc(layerCount * sizeof(char*));
    }

    *nLayers = layerCount;

    for(uint32_t i = 0; i < layerCount; ++i) {
        enabledInstanceLayers[i] = (char*)malloc(VK_MAX_EXTENSION_NAME_SIZE*sizeof(char));
        strcpy(enabledInstanceLayers[i], vkLayerProperties[i].layerName);
    }

    if(vkLayerProperties) {
        free(vkLayerProperties);
        vkLayerProperties = nullptr;
    }
    return true;
}

static bool
CheckVkInstanceExtensions(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkResult res;
    uint32_t extensionCount = 0;
    VkExtensionProperties *vkExtensionProperties = NULL;

    do {
        res = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);

        if(!extensionCount || res) {
            break;
        }

        vkExtensionProperties = (VkExtensionProperties *)realloc(vkExtensionProperties, extensionCount * sizeof(*vkExtensionProperties));
        if(!vkExtensionProperties) {
            return false;
        }

        res = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, vkExtensionProperties);
    } while(res == VK_INCOMPLETE);

    bool extensionsAvailable[ARRAY_SIZE(requiredInstanceExtensions)] = {false};
    for(uint32_t i = 0; i < extensionCount; ++i) {
        for(uint32_t j = 0; j < ARRAY_SIZE(requiredInstanceExtensions); ++j) {
            if(!strcmp(requiredInstanceExtensions[j], vkExtensionProperties[i].extensionName)) {
                extensionsAvailable[j] = true;
                break;
            }
        }
    }

    if(vkExtensionProperties) {
        free(vkExtensionProperties);
        vkExtensionProperties = nullptr;
    }

    for(uint32_t j = 0; j < ARRAY_SIZE(requiredInstanceExtensions); ++j) {
        if(!extensionsAvailable[j]) {
            return false;
        }
    }

    return true;
}

static bool
CheckVkDeviceExtensions(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkResult res;
    uint32_t extensionCount = 0;
    VkExtensionProperties *vkExtensionProperties = NULL;

    do {
        res = vkEnumerateDeviceExtensionProperties(GloveVkContext.vkGpus[0], NULL, &extensionCount, NULL);

        if(!extensionCount || res) {
            break;
        }

        vkExtensionProperties = (VkExtensionProperties *)realloc(vkExtensionProperties, extensionCount * sizeof(*vkExtensionProperties));
        if(!vkExtensionProperties) {
            return false;
        }

        res = vkEnumerateDeviceExtensionProperties(GloveVkContext.vkGpus[0], NULL, &extensionCount, vkExtensionProperties);
    } while(res == VK_INCOMPLETE);

    bool extensionsAvailable[ARRAY_SIZE(requiredDeviceExtensions)] = {false};
    for(uint32_t i = 0; i < extensionCount; ++i) {
        for(uint32_t j = 0; j < ARRAY_SIZE(requiredDeviceExtensions); ++j) {
            if(!strcmp(requiredDeviceExtensions[j], vkExtensionProperties[i].extensionName)) {
                extensionsAvailable[j] = true;
                break;
            }
        }
    }

    if(vkExtensionProperties) {
        free(vkExtensionProperties);
        vkExtensionProperties = nullptr;
    }

    for(uint32_t j = 0; j < ARRAY_SIZE(requiredDeviceExtensions); ++j) {
        if(!extensionsAvailable[j]) {
            printf("\n%s extension is mandatory for GLOVE\n", requiredDeviceExtensions[j]);
            printf("Please link GLOVE to a Vulkan driver which supports the latter\n");
            return false;
        }
    }

    return true;
}

static bool
CreateVkInstance(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    uint32_t enabledLayerCount = 0;
    if(GLOVE_VK_VALIDATION_LAYERS) {
        if(InitVkLayers(&enabledLayerCount) == false) {
            return false;
        }
    }

    VkApplicationInfo applicationInfo;
    applicationInfo.sType             = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext             = NULL;
    applicationInfo.pApplicationName  = "GLOVE (GL Over Vulkan)\0";
    applicationInfo.applicationVersion= 1;
    applicationInfo.pEngineName       = "GLOVE (GL Over Vulkan)\0";
    applicationInfo.engineVersion     = 1;
    applicationInfo.apiVersion        = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceInfo;
    memset((void *)&instanceInfo, 0 ,sizeof(instanceInfo));
    instanceInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext                    = NULL;
    instanceInfo.flags                    = 0;
    instanceInfo.pApplicationInfo         = &applicationInfo;
    instanceInfo.enabledLayerCount        = enabledLayerCount;
    instanceInfo.ppEnabledLayerNames      = enabledInstanceLayers;
    instanceInfo.enabledExtensionCount    = ARRAY_SIZE(requiredInstanceExtensions);
    instanceInfo.ppEnabledExtensionNames  = requiredInstanceExtensions;

    VkResult err = vkCreateInstance(&instanceInfo, NULL, &GloveVkContext.vkInstance);
    assert(!err);

    for(uint32_t i = 0; i < enabledLayerCount; ++i) {
       free(enabledInstanceLayers[i]);
    }
    free(enabledInstanceLayers);

    return (err == VK_SUCCESS);
}

static bool
EnumerateVkGpus(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    uint32_t gpuCount;

    assert(GloveVkContext.vkInstance != VK_NULL_HANDLE);

    VkResult err;
    err = vkEnumeratePhysicalDevices(GloveVkContext.vkInstance, &gpuCount, NULL);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    GloveVkContext.vkGpus.resize(gpuCount);

    err = vkEnumeratePhysicalDevices(GloveVkContext.vkInstance, &gpuCount, GloveVkContext.vkGpus.data());
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    vkGetPhysicalDeviceMemoryProperties(GloveVkContext.vkGpus[0], &GloveVkContext.vkDeviceMemoryProperties);

    return true;
}

static bool
InitVkQueueFamilyIndex(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(GloveVkContext.vkGpus[0], &queueFamilyCount, NULL);
    assert(queueFamilyCount >= 1);
    if(!queueFamilyCount) {
        return false;
    }

    VkQueueFamilyProperties *queueProperties = new VkQueueFamilyProperties[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(GloveVkContext.vkGpus[0], &queueFamilyCount, queueProperties);

    uint32_t i;
    for(i = 0; i < queueFamilyCount; ++i) {
        if(queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            GloveVkContext.vkGraphicsQueueNodeIndex = i;
            break;
        }
    }

    delete[] queueProperties;
    return i < queueFamilyCount ? true : false;
}

static bool
CreateVkDevice(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    float queue_priorities[1] = {0.0};
    VkDeviceQueueCreateInfo queueInfo;
    queueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext            = NULL;
    queueInfo.flags            = 0;
    queueInfo.queueCount       = 1;
    queueInfo.pQueuePriorities = queue_priorities;
    queueInfo.queueFamilyIndex = GloveVkContext.vkGraphicsQueueNodeIndex;

    VkDeviceCreateInfo deviceInfo;
    deviceInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext                   = NULL;
    deviceInfo.flags                   = 0;
    deviceInfo.queueCreateInfoCount    = 1;
    deviceInfo.pQueueCreateInfos       = &queueInfo;
    deviceInfo.enabledLayerCount       = 0;
    deviceInfo.ppEnabledLayerNames     = NULL;
    deviceInfo.enabledExtensionCount   = ARRAY_SIZE(requiredDeviceExtensions);
    deviceInfo.ppEnabledExtensionNames = requiredDeviceExtensions;
    deviceInfo.pEnabledFeatures        = NULL;

    VkResult err = vkCreateDevice(GloveVkContext.vkGpus[0], &deviceInfo, NULL, &GloveVkContext.vkDevice);
    assert(!err);

    return (err == VK_SUCCESS);
}

static bool
CreateVkCommandBuffers(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    GloveVkContext.mCommandBufferManager = CommandBufferManager::GetCommandBufferManager(&GloveVkContext);
    if(!GloveVkContext.mCommandBufferManager) {
        return false;
    }

    if(!GloveVkContext.mCommandBufferManager->AllocateVkCmdBuffers()) {
        return false;
    }

    return true;
}

static bool
CreateVkSemaphores(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkResult err;

    GloveVkContext.vkSyncItems = new vkSyncItems_t;

    VkSemaphoreCreateInfo semaphoreCreateInfo;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = NULL;
    semaphoreCreateInfo.flags = 0;

    err = vkCreateSemaphore(GloveVkContext.vkDevice, &semaphoreCreateInfo, NULL, &GloveVkContext.vkSyncItems->vkDrawSemaphore);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    err = vkCreateSemaphore(GloveVkContext.vkDevice, &semaphoreCreateInfo, NULL, &GloveVkContext.vkSyncItems->vkAcquireSemaphore);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    err = vkCreateSemaphore(GloveVkContext.vkDevice, &semaphoreCreateInfo, NULL, &GloveVkContext.vkSyncItems->vkAuxSemaphore);
    assert(!err);

    if(err != VK_SUCCESS) {
        return false;
    }

    GloveVkContext.vkSyncItems->acquireSemaphoreFlag = true;
    GloveVkContext.vkSyncItems->drawSemaphoreFlag = false;
    GloveVkContext.vkSyncItems->auxSemaphoreFlag = false;

    return true;
}

static void
InitVkQueue(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    vkGetDeviceQueue(GloveVkContext.vkDevice,
                     GloveVkContext.vkGraphicsQueueNodeIndex,
                     0,
                     &GloveVkContext.vkQueue);
}

vkContext_t *
GetContext()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return &GloveVkContext;
}

bool
InitContext()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if( !CheckVkInstanceExtensions()  ||
        !CreateVkInstance()           ||
        !EnumerateVkGpus()            ||
        !InitVkQueueFamilyIndex()     ||
        !CheckVkDeviceExtensions()    ||
        !CreateVkDevice()             ||
        !CreateVkCommandBuffers()     ||
        !CreateVkSemaphores()          ) {
          return false;
    }
    InitVkQueue();

    return true;
}

void
TerminateContext()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    GloveVkContext.mCommandBufferManager->ResetCommandBufferManager();
    GloveVkContext.mCommandBufferManager = nullptr;

    if(GloveVkContext.vkSyncItems->vkAcquireSemaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(GloveVkContext.vkDevice, GloveVkContext.vkSyncItems->vkAcquireSemaphore, NULL);
        GloveVkContext.vkSyncItems->vkAcquireSemaphore = VK_NULL_HANDLE;
    }

    if(GloveVkContext.vkSyncItems->vkDrawSemaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(GloveVkContext.vkDevice, GloveVkContext.vkSyncItems->vkDrawSemaphore, NULL);
        GloveVkContext.vkSyncItems->vkDrawSemaphore = VK_NULL_HANDLE;
    }

    if(GloveVkContext.vkSyncItems->vkAuxSemaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(GloveVkContext.vkDevice, GloveVkContext.vkSyncItems->vkAuxSemaphore, NULL);
        GloveVkContext.vkSyncItems->vkAuxSemaphore = VK_NULL_HANDLE;
    }

    if(GloveVkContext.vkDevice != VK_NULL_HANDLE ) {
        vkDeviceWaitIdle(GloveVkContext.vkDevice);
        vkDestroyDevice(GloveVkContext.vkDevice, NULL);
        vkDestroyInstance(GloveVkContext.vkInstance, NULL);
    }

    SafeDelete(GloveVkContext.vkSyncItems);
}

};
