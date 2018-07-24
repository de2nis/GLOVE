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

#ifndef __TEXTURE2D_COLOR_H_
#define __TEXTURE2D_COLOR_H_

#include "../engine/glcore/common.h"
#include "../assets/geometry/screen_quad.h"

#define VERTEX_SHADER_NAME          SOURCES_PATH SHADERS_PATH "full_screen.vert"
#define FRAGMENT_SHADER_NAME        SOURCES_PATH SHADERS_PATH "texture2d_color.frag"
#define BINARY_PROGRAM_SHADER_NAME  SOURCES_PATH SHADERS_PATH "texture2d_color.bin"

static const char* diffuse_textures [] = { "../assets/textures/tsi_256x256.tga" };
static const char* shading_titles   [] = { "TEXTURE_2D" };

#endif // __TEXTURE2D_COLOR_H_
