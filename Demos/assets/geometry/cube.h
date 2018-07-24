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

#ifndef __CUBE_H_
#define __CUBE_H_

static const GLfloat cube_vertex_buffer_data[] = {
    -1.0f, 1.0f,-1.0f,  // Left side - triangle 1
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,

    -1.0f,-1.0f, 1.0f,  // Left side - triangle 2
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,

    -1.0f, 1.0f,-1.0f,  // Back side - triangle 1
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,

    -1.0f, 1.0f,-1.0f,  // Back side - triangle 2
    -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,

    -1.0f, 1.0f,-1.0f,  // Top side - triangle 1
     1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,

    -1.0f, 1.0f,-1.0f,  // Top side - triangle 2
     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,

    -1.0f,-1.0f,-1.0f,  // Bottom side - triangle 1
    -1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,

    -1.0f,-1.0f,-1.0f,  // Bottom side - triangle 2
     1.0f,-1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,

     1.0f,-1.0f,-1.0f,  // Right side - triangle 1
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,

     1.0f, 1.0f, 1.0f,  // Right side - triangle 2
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,

    -1.0f,-1.0f, 1.0f,  // Front side - triangle 1
    -1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,

    -1.0f, 1.0f, 1.0f,  // Front side - triangle 2
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
};

static const GLfloat cube_uv_buffer_data    [] = {
    0.0f, 1.0f,  // Left side - triangle 1
    1.0f, 1.0f,
    1.0f, 0.0f,

    1.0f, 0.0f,  // Left side - triangle 2
    0.0f, 0.0f,
    0.0f, 1.0f,

    1.0f, 1.0f,  // Back side - triangle 1
    0.0f, 0.0f,
    0.0f, 1.0f,

    1.0f, 1.0f,  // Back side - triangle 2
    1.0f, 0.0f,
    0.0f, 0.0f,

    0.0f, 0.0f,  // Top side - triangle 1
    0.0f, 1.0f,
    1.0f, 1.0f,

    0.0f, 0.0f,  // Top side - triangle 2
    1.0f, 1.0f,
    1.0f, 0.0f,

    0.0f, 1.0f,  // Bottom side - triangle 1
    1.0f, 1.0f,
    1.0f, 0.0f,

    0.0f, 1.0f,  // Bottom side - triangle 2
    1.0f, 0.0f,
    0.0f, 0.0f,

    1.0f, 0.0f,  // Right side - triangle 1
    0.0f, 0.0f,
    0.0f, 1.0f,

    0.0f, 1.0f,  // Right side - triangle 2
    1.0f, 1.0f,
    1.0f, 0.0f,

    0.0f, 0.0f,  // Front side - triangle 1
    0.0f, 1.0f,
    1.0f, 0.0f,

    0.0f, 1.0f,  // Front side - triangle 2
    1.0f, 1.0f,
    1.0f, 0.0f,
};

static const GLfloat cube_color_buffer_data [] = {
    1.0f,  0.0f,  0.0f,         // LEFT
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,

    0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,         // BACK
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  0.0f,

    1.0f,  0.0f,  0.0f,         // TOP
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,

    1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  1.0f,

    1.0f,  0.0f,  0.0f,         // BOTTOM
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,

    1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  1.0f,

    1.0f,  0.0f,  0.0f,         // RIGHT
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,

    0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,         // FRONT
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,

    0.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
};

static const GLuint  cube_index_buffer_data [] = {};

#endif // __CUBE_H_
