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
 *  @file       rendering_api.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      It connects EGL to the client (rendering) API at runtime.
 *
 */

#ifndef __RENDERING_API_H__
#define __RENDERING_API_H__

#include "EGL/egl.h"
#include "rendering_api_interface.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum rendering_api_return {
    RENDERING_API_INIT_SUCCESS,
    RENDERING_API_ALREADY_INIT,
    RENDERING_API_INPUT_ERROR,
    RENDERING_API_NOT_FOUND
} rendering_api_return_e;

typedef struct {
    EGLNativeDisplayType nativeDisplay;
} eglDisplay_t;

rendering_api_interface_t *RENDERING_API_get_gles1_interface();
rendering_api_interface_t *RENDERING_API_get_gles2_interface();
rendering_api_interface_t *RENDERING_API_get_vg_interface();
rendering_api_return_e     RENDERING_API_init_api(EGLenum api, uint32_t client_version, rendering_api_interface_t ** api_interface_ret);
void                       RENDERING_API_terminate_gles1_api();
void                       RENDERING_API_terminate_gles2_api();
void                       RENDERING_API_terminate_vg_api();

#ifdef __cplusplus
}
#endif

#endif // __RENDERING_API_H__
