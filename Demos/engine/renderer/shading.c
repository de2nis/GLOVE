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

#include "shading.h"

void InitShading(struct  openGL_shading_t *shading, const GLint shading_types_num, const GLfloat value)
{
    shading->mType             = 0;
    shading->mTypesNum         = shading_types_num;
    shading->mValue            = value;
}
