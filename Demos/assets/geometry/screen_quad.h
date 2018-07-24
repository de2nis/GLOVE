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

#ifndef __SCREENQUAD_H_
#define __SCREENQUAD_H_

static const GLfloat squad_vertex_buffer_data[] = {
    1.0f, -1.0f,
   -1.0f, -1.0f,
   -1.0f,  1.0f,

    1.0f,  1.0f,
    1.0f, -1.0f,
   -1.0f,  1.0f
};

static const GLfloat squad_uv_buffer_data   [] = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,

    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f
};

static const GLfloat squad_color_buffer_data[] = {};
static const GLuint  squad_index_buffer_data[] = {};

#endif // __SCREENQUAD_H_
