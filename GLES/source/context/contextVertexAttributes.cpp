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

/**
 *  @file       contextVertexAttributes.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Vertex Attributes
 *
 */

#include "context.h"

void
Context::GetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    GenericVertexAttributes *genericVertexAttributes = mResourceManager.GetGenericVertexAttributes();

    switch(pname) {
    case GL_VERTEX_ATTRIB_ARRAY_ENABLED:        *params = static_cast<GLfloat>(genericVertexAttributes->GetVertexAttribActive(index)); break;
    case GL_VERTEX_ATTRIB_ARRAY_SIZE:           *params = static_cast<GLfloat>(genericVertexAttributes->GetVertexAttribSize(index)); break;
    case GL_VERTEX_ATTRIB_ARRAY_STRIDE:         *params = static_cast<GLfloat>(genericVertexAttributes->GetVertexAttribStride(index)); break;
    case GL_VERTEX_ATTRIB_ARRAY_TYPE:           *params = static_cast<GLfloat>(genericVertexAttributes->GetVertexAttribType(index)); break;
    case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:     *params = static_cast<GLfloat>(genericVertexAttributes->GetVertexAttribNormalized(index)); break;
    case GL_CURRENT_VERTEX_ATTRIB:              genericVertexAttributes->GetGenericVertexAttribute(index, (float *)params); break;
    case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: {
        const BufferObject *vbo = genericVertexAttributes->GetVertexAttribVbo(index);
        *params = vbo ? static_cast<GLfloat>(mResourceManager.GetBufferID(vbo)) : 0.0f;
    } break;
    default:                                    RecordError(GL_INVALID_ENUM); break;
    }
}

void
Context::GetVertexAttribiv(GLuint index, GLenum pname, GLint* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    GenericVertexAttributes *genericVertexAttributes = mResourceManager.GetGenericVertexAttributes();

    switch(pname) {
    case GL_VERTEX_ATTRIB_ARRAY_ENABLED:        *params = static_cast<GLint>(genericVertexAttributes->GetVertexAttribActive(index)); break;
    case GL_VERTEX_ATTRIB_ARRAY_SIZE:           *params = static_cast<GLint>(genericVertexAttributes->GetVertexAttribSize(index)); break;
    case GL_VERTEX_ATTRIB_ARRAY_STRIDE:         *params = static_cast<GLint>(genericVertexAttributes->GetVertexAttribStride(index)); break;
    case GL_VERTEX_ATTRIB_ARRAY_TYPE:           *params = static_cast<GLint>(genericVertexAttributes->GetVertexAttribType(index)); break;
    case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:     *params = static_cast<GLint>(genericVertexAttributes->GetVertexAttribNormalized(index)); break;
    case GL_CURRENT_VERTEX_ATTRIB:              genericVertexAttributes->GetGenericVertexAttribute(index, params); break;
    case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: {
        const BufferObject *vbo = genericVertexAttributes->GetVertexAttribVbo(index);
        *params = vbo ? static_cast<GLint>(mResourceManager.GetBufferID(vbo)) : 0;
    } break;
    default:                                    RecordError(GL_INVALID_ENUM); break;
    }
}

void
Context::GetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(pname != GL_VERTEX_ATTRIB_ARRAY_POINTER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    *pointer = (void *)mResourceManager.GetGenericVertexAttributes()->GetVertexAttribPointer(index);
}

void
Context::VertexAttrib1f(GLuint index, GLfloat x)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    float vals[4] = {x, 0.0f, 0.0f, 1.0f};
    mResourceManager.GetGenericVertexAttributes()->SetGenericVertexAttribute(index, vals);
    mPipeline->SetUpdateVertexAttribVBOs(true);
}

void
Context::VertexAttrib1fv(GLuint index, const GLfloat* values)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    float vals[4] = {values[0], 0.0f, 0.0f, 1.0f};
    mResourceManager.GetGenericVertexAttributes()->SetGenericVertexAttribute(index, vals);
    mPipeline->SetUpdateVertexAttribVBOs(true);
}

void
Context::VertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    float values[4] = {x, y, 0.0f, 1.0f};
    mResourceManager.GetGenericVertexAttributes()->SetGenericVertexAttribute(index, values);
    mPipeline->SetUpdateVertexAttribVBOs(true);
}

void
Context::VertexAttrib2fv(GLuint index, const GLfloat* values)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    float vals[4] = {values[0], values[1], 0.0f, 1.0f};
    mResourceManager.GetGenericVertexAttributes()->SetGenericVertexAttribute(index, vals);
    mPipeline->SetUpdateVertexAttribVBOs(true);
}

void
Context::VertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    float values[4] = {x, y, z, 1.0f};
    mResourceManager.GetGenericVertexAttributes()->SetGenericVertexAttribute(index, values);
    mPipeline->SetUpdateVertexAttribVBOs(true);
}

void
Context::VertexAttrib3fv(GLuint index, const GLfloat* values)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    float vals[4] = {values[0], values[1], values[2], 1.0f};
    mResourceManager.GetGenericVertexAttributes()->SetGenericVertexAttribute(index, vals);
    mPipeline->SetUpdateVertexAttribVBOs(true);
}

void
Context::VertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    float values[4] = {x, y, z, w};
    mResourceManager.GetGenericVertexAttributes()->SetGenericVertexAttribute(index, values);
    mPipeline->SetUpdateVertexAttribVBOs(true);
}

void
Context::VertexAttrib4fv(GLuint index, const GLfloat* values)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    mResourceManager.GetGenericVertexAttributes()->SetGenericVertexAttribute(index, values);
    mPipeline->SetUpdateVertexAttribVBOs(true);
}

void
Context::EnableVertexAttribArray(GLuint index)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    mResourceManager.GetGenericVertexAttributes()->EnableVertexAttribute(index, mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ARRAY_BUFFER));
    mPipeline->SetUpdateVertexAttribVBOs(true);
}

void
Context::DisableVertexAttribArray(GLuint index)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    GenericVertexAttributes *genericVertexAttributes = mResourceManager.GetGenericVertexAttributes();

    if(genericVertexAttributes->GetVertexAttribActive(index)) {
        genericVertexAttributes->DisableVertexAttribute(index);
        mPipeline->SetUpdateVertexAttribVBOs(true);
    }
}

void
Context::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(type != GL_BYTE && type != GL_UNSIGNED_BYTE && type != GL_SHORT && type != GL_UNSIGNED_SHORT && type != GL_FIXED && type != GL_FLOAT &&
       type != GL_INT  && type != GL_UNSIGNED_INT) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if((size < 1 || size > 4) || (stride < 0) || (index >= GLOVE_MAX_VERTEX_ATTRIBS)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    mResourceManager.GetGenericVertexAttributes()->SetVertexAttributePointer(index, size, type, normalized, stride, ptr, mStateManager.GetActiveObjectsState()->GetActiveBufferObject(GL_ARRAY_BUFFER));
    mPipeline->SetUpdateVertexAttribVBOs(true);
}
