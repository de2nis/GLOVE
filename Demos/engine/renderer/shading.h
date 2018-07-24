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

#ifndef __SHADING_H_
#define __SHADING_H_

#include "../utilities/debug.h"

typedef struct openGL_shading_t
{
    GLint            mType;
    GLint            mTypesNum;
    GLfloat          mValue;

} openGL_shading_t;

void InitShading(struct openGL_shading_t *shading, const GLint shading_types_num, const GLfloat value);

#endif //__SHADING_H_
