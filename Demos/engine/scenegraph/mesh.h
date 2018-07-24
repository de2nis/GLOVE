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

#ifndef __MESH_H_
#define __MESH_H_

#include "camera.h"
#include "../renderer/shaderprogram.h"
#include "../renderer/framebufferobject.h"

typedef struct openGL_mesh_t
{
    GLint      mVerticesNum;
    GLint      mPrimitivesNum;
    GLint      mTexturesNum;
    GLint      mVertexComponentsNum;

    GLuint     mVerticesVbo;
    GLuint     mUVsVbo;
    GLuint     mColorsVbo;
    GLuint     mIndicesEbo;

    mat4x4     mModelMatrix;
    GLfloat    mSpinAngle;
    Texture   *mTexture[2];

} openGL_mesh_t;

void DeleteMesh(struct openGL_mesh_t *mesh);
void RotateMesh(struct openGL_mesh_t *mesh, const vec3 axis);
void DrawMesh(struct openGL_program_t *program, struct openGL_mesh_t *mesh);
void TransformMesh(struct openGL_program_t *program, struct openGL_mesh_t *mesh, struct openGL_camera_t *camera);
void InitMesh(struct openGL_mesh_t *mesh, const int vertex_components_num, const int primitives_num,
              const GLfloat vertex_buffer_data[], const int vertex_buffer_data_size,
              const GLfloat uv_buffer_data[], const int uv_buffer_data_size,
              const GLfloat color_buffer_data[], const int color_buffer_data_size,
              const GLuint  index_buffer_dat [], const int index_buffer_data_size,
              const char*   textures[] , const int textures_size);

#endif // __MESH_H_
