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

#ifndef __COMMON_H_
#define __COMMON_H_

#include "../../eglut/eglutint.h"

#include "abstract.h"
#include "../renderer/rendering.h"
#include "../renderer/viewport.h"
#include "../renderer/shading.h"
#include "../scenegraph/mesh.h"
#include "../utilities/profiler.h"
#include "../asset_manager/shaderManager.h"

#define ESC_KEY            27
#define SOURCES_PATH       "../"
#define SHADERS_PATH       "assets/shaders/"
#define TEXTURES_PATH      "assets/textures/"
#define EXECUTABLE_NAME(name) ( (strchr(name, '/')) ? (strchr(name, '/') + 1) : (name) )

static const GLfloat COLOR_BLACK [] = { 0.0f, 0.0f, 0.0f, 1.0f };
static const GLfloat COLOR_WHITE [] = { 1.0f, 1.0f, 1.0f, 1.0f };
static const GLfloat COLOR_BLUE  [] = { 0.0f, 0.0f, 1.0f, 1.0f };
static const GLfloat COLOR_YELLOW[] = { 1.0f, 1.0f, 0.0f, 1.0f };

static const vec3    ROT_AXIS_X     = { 1.0f, 0.0f, 0.0f };
static const vec3    ROT_AXIS_Y     = { 0.0f, 1.0f, 0.0f };
static const vec3    ROT_AXIS_Z     = { 0.0f, 0.0f, 1.0f };

#endif // __COMMON_H_
