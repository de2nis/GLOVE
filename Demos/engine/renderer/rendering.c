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

#include "rendering.h"

void InitRendering(struct openGL_rendering_t  *rendering, const GLfloat color[])
{
    rendering->mDepth               = 1.0f;
    rendering->mBackgroundColor[0]  = color[0];
    rendering->mBackgroundColor[1]  = color[1];
    rendering->mBackgroundColor[2]  = color[2];
    rendering->mBackgroundColor[3]  = color[3];

    rendering->mMaterialType        = OPAQUE;
    rendering->mProjectionType      = PERSPECTIVE;
}
