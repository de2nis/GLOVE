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
 *  @file       rendering_api.c
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      It connects EGL to the client (rendering) API at runtime.
 *
 */

#include "utils/egl_defs.h"
#include "rendering_api.h"
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdbool.h>

typedef struct rendering_api_library_info {
    bool initialized;
    void *handle;
} rendering_api_library_info_t;

static rendering_api_library_info_t gles1_library_info = { false, NULL };
static rendering_api_library_info_t gles2_library_info = { false, NULL };
static rendering_api_library_info_t vg_library_info    = { false, NULL };

static rendering_api_interface_t *gles1_interface = NULL;
static rendering_api_interface_t *gles2_interface = NULL;
static rendering_api_interface_t *vg_interface    = NULL;

static void rendering_api_cache_interface(rendering_api_interface_t* api_interface, uint32_t client_version, EGLenum api);
static rendering_api_return_e rendering_api_get_api_interface(const char *library_name,
                                                              const char *api_interface_name,
                                                              rendering_api_interface_t **api_interface_ret,
                                                              rendering_api_library_info_t *library_info);

static void rendering_terminate_api(rendering_api_interface_t *api_interface, rendering_api_library_info_t *library_info);

static void rendering_api_cache_interface(rendering_api_interface_t* api_interface, uint32_t client_version, EGLenum api)
{
    switch(api) {
        case EGL_OPENGL_ES_API:
            if(EGL_GL_VERSION_1 == client_version) {
                gles1_interface = api_interface;

            } else {
                gles2_interface = api_interface;

            }
            break;
        case EGL_OPENVG_API:
            vg_interface = api_interface;
            break;
        default:
            NOT_REACHED();
    }
}

static rendering_api_return_e rendering_api_get_api_interface(const char *library_name,
                                                              const char *api_interface_name,
                                                              rendering_api_interface_t **api_interface_ret,
                                                              rendering_api_library_info_t *library_info)
{
    rendering_api_interface_t *api_interface = NULL;
    char *error = NULL;

    if(true == library_info->initialized) {
        return RENDERING_API_ALREADY_INIT;
    }

    library_info->handle = dlopen(library_name, RTLD_NOW);
    if(!library_info->handle) {
        fprintf(stderr, "%s\n", dlerror());
        return RENDERING_API_NOT_FOUND;
    }

    dlerror();

    api_interface = (rendering_api_interface_t *) dlsym(library_info->handle, api_interface_name);

    error = dlerror();
    if(error)  {
        fprintf(stderr, "%s\n", error);
        return RENDERING_API_NOT_FOUND;
    }

    library_info->initialized = true;
    *api_interface_ret = api_interface;

    return RENDERING_API_INIT_SUCCESS;
}

rendering_api_return_e RENDERING_API_init_api(EGLenum api, uint32_t client_version, rendering_api_interface_t **api_interface_ret)
{
    rendering_api_interface_t *api_interface = NULL;
    const char *api_library_name = NULL;
    const char *api_interface_name = NULL;
    rendering_api_library_info_t *library_info = NULL;

    if(    EGL_OPENGL_ES_API == api && (EGL_GL_VERSION_1 != client_version &&
                                        EGL_GL_VERSION_2 != client_version)) {
        return RENDERING_API_INPUT_ERROR;
    }

    switch(api) {
        case EGL_OPENGL_ES_API:
            if(EGL_GL_VERSION_1 == client_version) {
                api_library_name = "libGLESv1_CM.so";
                api_interface_name = "GLES1Interface";
                api_interface = gles1_interface;
                library_info = &gles1_library_info;
            } else {
                api_library_name = "libGLESv2.so";
                api_interface_name = "GLES2Interface";
                api_interface = gles2_interface;
                library_info = &gles2_library_info;
            }
            break;
        case EGL_OPENVG_API:
            api_library_name = "libVG.so";
            api_interface_name = "VGInterface";
            api_interface = vg_interface;
            library_info = &vg_library_info;
            break;
        default:
            NOT_REACHED();
    }

    rendering_api_return_e ret = rendering_api_get_api_interface(api_library_name, api_interface_name, &api_interface, library_info);

    if(RENDERING_API_INIT_SUCCESS == ret && api_interface->init_API_cb) {
        api_interface->state = api_interface->init_API_cb();
        rendering_api_cache_interface(api_interface, client_version, api);
    }

    *api_interface_ret = api_interface;

    return ret;
}

static void rendering_terminate_api(rendering_api_interface_t *api_interface, rendering_api_library_info_t *library_info)
{
    if(api_interface->terminate_API_cb) {
        api_interface->terminate_API_cb();
    }

    dlclose(library_info->handle);
}


rendering_api_interface_t *RENDERING_API_get_gles1_interface()
{
    return gles1_interface;
}

rendering_api_interface_t *RENDERING_API_get_gles2_interface()
{
    return gles2_interface;
}

rendering_api_interface_t *RENDERING_API_get_vg_interface()
{
    return vg_interface;
}

void RENDERING_API_terminate_gles1_api()
{
    rendering_terminate_api(gles1_interface, &gles1_library_info);
}

void RENDERING_API_terminate_gles2_api()
{
    rendering_terminate_api(gles2_interface, &gles2_library_info);
}

void RENDERING_API_terminate_vg_api()
{
    rendering_terminate_api(vg_interface, &vg_library_info);
}
