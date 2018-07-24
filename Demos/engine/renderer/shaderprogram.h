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

#ifndef __SHADERPROGRAM_H_
#define __SHADERPROGRAM_H_

#include "../utilities/debug.h"

typedef struct openGL_program_t
{
    GLuint           mID;
    GLuint           mVertexShader;
    GLuint           mFragmentShader;

    GLint            mLocationMVP;
    GLint            mLocationPos;
    GLint            mLocationUV;
    GLint            mLocationColor;

} openGL_program_t;

#endif //__SHADERPROGRAM_H_
