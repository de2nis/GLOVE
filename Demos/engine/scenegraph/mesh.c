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

#include "mesh.h"

void InitMesh(struct openGL_mesh_t *mesh, const int vertex_components_num, const int primitives_num,
              const GLfloat vertex_buffer_data[], const int vertex_buffer_data_size,
              const GLfloat uv_buffer_data[], const int uv_buffer_data_size,
              const GLfloat color_buffer_data[], const int color_buffer_data_size,
              const GLuint  index_buffer_data[], const int index_buffer_data_size,
              const char* textures[], const int textures_size)
{
    mesh->mVertexComponentsNum = vertex_components_num;
    mesh->mPrimitivesNum  = primitives_num;
    mesh->mVerticesNum  = mesh->mPrimitivesNum * 3;
    mesh->mTexturesNum  = textures_size;

    for (size_t i = 0; i < (size_t)mesh->mTexturesNum; i++) {
        Texture *tex = LoadGLTexture(textures[i]);
        if(tex == NULL) {
            return;
        } else {
            mesh->mTexture[i] = tex;
        }
    }

    mesh->mSpinAngle = 1.0f;
    mat4x4_identity(mesh->mModelMatrix);

    glGenBuffers(1, &mesh->mVerticesVbo); assert(mesh->mVerticesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVerticesVbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data_size, vertex_buffer_data, GL_STATIC_DRAW);

    mesh->mUVsVbo = 0;
    if(uv_buffer_data_size > 0) {
        glGenBuffers(1, &mesh->mUVsVbo); assert(mesh->mUVsVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->mUVsVbo);
        glBufferData(GL_ARRAY_BUFFER, uv_buffer_data_size, uv_buffer_data, GL_STATIC_DRAW);
    }

    mesh->mColorsVbo = 0;
    if(color_buffer_data_size > 0) {
        glGenBuffers(1, &mesh->mColorsVbo); assert(mesh->mColorsVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->mColorsVbo);
        glBufferData(GL_ARRAY_BUFFER, color_buffer_data_size, color_buffer_data, GL_STATIC_DRAW);
    }

    mesh->mIndicesEbo = 0;
    if(index_buffer_data_size > 0) {
        glGenBuffers(1, &mesh->mIndicesEbo); assert(mesh->mIndicesEbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndicesEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data_size, index_buffer_data, GL_STATIC_DRAW);
    }
    ASSERT_NO_GL_ERROR();
}

void RotateMesh(struct openGL_mesh_t *mesh, const vec3 axis)
{
    mat4x4 M;
    mat4x4_dup(M, mesh->mModelMatrix);
    mat4x4_rotate(mesh->mModelMatrix  , M, axis[0], axis[1], axis[2], (float)degreesToRadians(mesh->mSpinAngle));
}

void TransformMesh(struct openGL_program_t *program, struct openGL_mesh_t *mesh, struct openGL_camera_t *camera)
{
    mat4x4 VP, WVP, MWVP;
    mat4x4_mul(VP  , camera->mProjectionMatrix, camera->mViewMatrix);
    mat4x4_mul(WVP , VP                       , camera->mWorldMatrix);
    mat4x4_mul(MWVP, WVP, mesh->mModelMatrix);

    glUseProgram(program->mID);
    glUniformMatrix4fv(program->mLocationMVP, 1, GL_FALSE, (const float *)&MWVP[0][0]);
    ASSERT_NO_GL_ERROR();
}

void DrawMesh(struct openGL_program_t *program, struct openGL_mesh_t *mesh)
{
    glUseProgram(program->mID);

    // Load Textures
    for (size_t i = 0; i < (size_t)mesh->mTexturesNum; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mesh->mTexture[i]->texID);
    }
    // Bind Vertex VBO
    glBindBuffer (GL_ARRAY_BUFFER, mesh->mVerticesVbo);
    glEnableVertexAttribArray (program->mLocationPos);
    glVertexAttribPointer (program->mLocationPos, mesh->mVertexComponentsNum, GL_FLOAT, GL_FALSE, mesh->mVertexComponentsNum * sizeof(float), 0);

    // Bind Texture VBO
    if(mesh->mUVsVbo > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh->mUVsVbo);
        glEnableVertexAttribArray(program->mLocationUV);
        glVertexAttribPointer    (program->mLocationUV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    }

    // Bind Color VBO
    if(mesh->mColorsVbo > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh->mColorsVbo);
        glEnableVertexAttribArray(program->mLocationColor);
        glVertexAttribPointer(program->mLocationColor, mesh->mVertexComponentsNum, GL_FLOAT, GL_FALSE, mesh->mVertexComponentsNum * sizeof(float), 0);
    }

    if(mesh->mIndicesEbo > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndicesEbo);
        glDrawElements(GL_TRIANGLES, mesh->mVerticesNum, GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, mesh->mVerticesNum);
    }

    glDisableVertexAttribArray(program->mLocationPos);
    if(mesh->mUVsVbo > 0)
        glDisableVertexAttribArray(program->mLocationUV);
    if(mesh->mColorsVbo > 0)
        glDisableVertexAttribArray(program->mLocationColor);
    ASSERT_NO_GL_ERROR();
}

void DeleteMesh(struct openGL_mesh_t *mesh)
{
    glDeleteBuffers(1, &mesh->mVerticesVbo);
    if(glIsBuffer(mesh->mUVsVbo))
        glDeleteBuffers (1, &mesh->mUVsVbo);
    if(glIsBuffer(mesh->mColorsVbo))
        glDeleteBuffers (1, &mesh->mColorsVbo);
    if(glIsBuffer(mesh->mIndicesEbo))
        glDeleteBuffers (1, &mesh->mIndicesEbo);

    for (size_t i = 0; i < (size_t)mesh->mTexturesNum; i++) {
        if(glIsTexture(mesh->mTexture[i]->texID)) {
            glDeleteTextures(1, &mesh->mTexture[i]->texID);
            free(mesh->mTexture[i]);
        }
    }
    ASSERT_NO_GL_ERROR();
}
