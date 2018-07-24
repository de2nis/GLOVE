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
 *  @file       rendering_api_interface.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      This header file provides the EGL interface to client APIs.
 *
 */

#ifndef __RENDERING_API_INTERFACE_H__
#define __RENDERING_API_INTERFACE_H__

#include <stdint.h>
#include <stdbool.h>
#include "EGL/egl.h"
#include "vulkan/vulkan.h"

typedef struct EGLSurfaceInterface_t {
    void *surface;
    void *images;
    uint32_t imageCount;
    uint32_t nextImageIndex;
    uint32_t surfaceColorFormat;
    uint32_t type;
    uint32_t width;
    uint32_t height;
    uint32_t depthSize;
    uint32_t stencilSize;
}EGLSurfaceInterface;

typedef void * api_state_t;
typedef void * api_context_t;

typedef api_state_t (*init_API_cb_t)();
typedef void (*terminate_API_cb_t)();
typedef api_context_t (*create_context_cb_t)();
typedef void (*set_write_surface_cb_t)(api_context_t api_context, EGLSurfaceInterface *eglSurfaceInterface);
typedef void (*set_read_surface_cb_t)(api_context_t api_context, EGLSurfaceInterface *eglSurfaceInterface);
typedef void (*delete_context_cb_t)(api_context_t api_context);
typedef void (*set_next_image_index_cb_t)(api_context_t api_context, uint32_t index);
typedef void (*finish_cb_t)(api_context_t api_context);

typedef struct rendering_api_interface {
    api_state_t state;
    init_API_cb_t init_API_cb;
    terminate_API_cb_t terminate_API_cb;
    create_context_cb_t create_context_cb;
    set_write_surface_cb_t set_write_surface_cb;
    set_read_surface_cb_t set_read_surface_cb;
    delete_context_cb_t delete_context_cb;
    set_next_image_index_cb_t set_next_image_index_cb;
    finish_cb_t finish_cb;
} rendering_api_interface_t;

typedef struct vkSyncItems_t {
    VkSemaphore                         vkAcquireSemaphore;
    bool                                acquireSemaphoreFlag;
    VkSemaphore                         vkDrawSemaphore;
    bool                                drawSemaphoreFlag;
    VkSemaphore                         vkAuxSemaphore;
    bool                                auxSemaphoreFlag;
} vkSyncItems_t;

typedef struct vkInterface {
    VkInstance                          vkInstance;
    VkPhysicalDevice                    *vkGpus;
    VkQueue                             vkQueue;
    uint32_t                            vkGraphicsQueueNodeIndex;
    VkDevice                            vkDevice;
    VkPhysicalDeviceMemoryProperties    vkDeviceMemoryProperties;
    vkSyncItems_t                       *vkSyncItems;
} vkInterface_t;

#endif // __RENDERING_API_INTERFACE_H__
