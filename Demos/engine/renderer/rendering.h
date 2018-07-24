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

#ifndef __RENDERING_H_
#define __RENDERING_H_

#include "../utilities/debug.h"
#include "../utilities/linmath.h"

enum projectionMatrix {
    ORTHOGRAPHIC = 0,
    PERSPECTIVE  = 1
};

enum materialType {
    OPAQUE       = 0,
    TRANSPARENT  = 1
};

typedef struct openGL_rendering_t
{
    GLint      mMaterialType;
    GLint      mProjectionType;

    GLfloat    mDepth;
    vec4       mBackgroundColor;

} openGL_rendering_t;

void InitRendering(struct openGL_rendering_t *rendering, const GLfloat color[]);

#endif // __RENDERING_H_
