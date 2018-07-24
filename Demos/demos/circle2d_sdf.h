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

#ifndef __CIRCLE2D_SDF_H_
#define __CIRCLE2D_SDF_H_

#include "../engine/glcore/common.h"
#include "../assets/geometry/screen_quad.h"

#define VERTEX_SHADER_NAME          SOURCES_PATH SHADERS_PATH "full_screen.vert"
#define FRAGMENT_SHADER_NAME        SOURCES_PATH SHADERS_PATH "circle2d_sdf.frag"
#define BINARY_PROGRAM_SHADER_NAME  SOURCES_PATH SHADERS_PATH "circle2d_sdf.bin"

static const char* diffuse_textures [] = { };
static const char* shading_titles   [] = { "CIRCLE2D_SDF" };

#endif // __CIRCLE2D_SDF_H_
