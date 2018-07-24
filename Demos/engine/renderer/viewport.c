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

#include "viewport.h"

void SetViewport(struct openGL_viewport_t *viewport,  const GLint x,  const GLint y, const GLint width, const GLint height)
{
    viewport->mOriginX     = x;
    viewport->mOriginY     = y;
    viewport->mWidth       = width;
    viewport->mHeight      = height;

    // Update aspect ratio since width or/and heigt has/have been modified
    viewport->mAspectRatio = (viewport->mHeight - viewport->mOriginY > 0.0f) ? (GLfloat)(viewport->mWidth - viewport->mOriginX) / (GLfloat)(viewport->mHeight - viewport->mOriginY)
        : (GLfloat)(viewport->mWidth - viewport->mOriginX);
    // Set the viewport since width or/and heigt has/have been modified
    glViewport(viewport->mOriginX, viewport->mOriginY, viewport->mWidth, viewport->mHeight);

    // Check for opengGL-relate Errors
    ASSERT_NO_GL_ERROR();
}
