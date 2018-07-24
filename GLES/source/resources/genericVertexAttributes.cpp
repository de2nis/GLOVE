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
 *  @file       genericVertexAttributes.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Generic Vertex Attributes Functionality in GLOVE
 *
 *  Each vertex is specified with multiple generic vertex attributes. Each
 *  attribute is specified with one, two, three, or four scalar values.
 *  Generic vertex attributes can be accessed from within vertex shaders
 *  and used to compute values for consumption by later processing stages.
 */

#include "genericVertexAttributes.h"
#include "utils/GlToVkConverter.h"

GenericVertexAttributes::GenericVertexAttributes()
{
    FUN_ENTRY(GL_LOG_TRACE);

    for(uint32_t i = 0; i < GLOVE_MAX_VERTEX_ATTRIBS; ++i) {
        mGenericVertexAttributes[i].active      = false;
        mGenericVertexAttributes[i].vkFormat    = VK_FORMAT_UNDEFINED;
        mGenericVertexAttributes[i].nElements   = 0;
        mGenericVertexAttributes[i].normalized  = false;
        mGenericVertexAttributes[i].stride      = 0;
        mGenericVertexAttributes[i].ptr         = 0;
        mGenericVertexAttributes[i].vbo         = nullptr;
        mGenericVertexAttributes[i].offset      = 0;
        mGenericVertexAttributes[i].internalVBO = false;
    }
}

GenericVertexAttributes::~GenericVertexAttributes()
{
    FUN_ENTRY(GL_LOG_TRACE);

    CleanupVertexAttributes();
}

void
GenericVertexAttributes::CleanupIfInternalVBO(uint32_t location)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mGenericVertexAttributes[location].internalVBO) {

        delete mGenericVertexAttributes[location].vbo;

        mGenericVertexAttributes[location].vbo         = nullptr;
        mGenericVertexAttributes[location].internalVBO = false;
    }
}

void
GenericVertexAttributes::CleanupVertexAttributes()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(uint32_t i = 0; i < GLOVE_MAX_VERTEX_ATTRIBS; ++i) {
        if(mGenericVertexAttributes[i].vbo) {
            CleanupIfInternalVBO(i);
        }
    }
}

void
GenericVertexAttributes::EnableVertexAttribute(uint32_t location, BufferObject *vbo)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(location < GLOVE_MAX_VERTEX_ATTRIBS);

    CleanupIfInternalVBO(location);

    mGenericVertexAttributes[location].active = true;
    mGenericVertexAttributes[location].vbo    = vbo;
}

void
GenericVertexAttributes::DisableVertexAttribute(uint32_t location)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(location < GLOVE_MAX_VERTEX_ATTRIBS);

    CleanupIfInternalVBO(location);

    mGenericVertexAttributes[location].active = false;
}

void
GenericVertexAttributes::SetVertexAttributePointer(uint32_t location, size_t nElements, int type, bool normalized, size_t stride, const void *ptr, BufferObject *vbo)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(location < GLOVE_MAX_VERTEX_ATTRIBS);

    mGenericVertexAttributes[location].dataType  = type;
    mGenericVertexAttributes[location].nElements = nElements;
    SetVertexAttribFormat(location, GlAttribPointerToVkFormat(nElements, type));
    SetVertexAttribNormalized(location, normalized);
    SetVertexAttribStride(location, stride);
    SetVertexAttribPointer(location, reinterpret_cast<uintptr_t>(ptr));
    SetVertexAttribOffset(location, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ptr)));

    if(mGenericVertexAttributes[location].active && mGenericVertexAttributes[location].vbo) {
        CleanupIfInternalVBO(location);
    }

    mGenericVertexAttributes[location].vbo = vbo;
}

void
GenericVertexAttributes::SetVertexAttribVbo(uint32_t location, BufferObject *vbo)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mGenericVertexAttributes[location].vbo && mGenericVertexAttributes[location].internalVBO) {
        delete mGenericVertexAttributes[location].vbo;
    }
    mGenericVertexAttributes[location].vbo = vbo;
}

uint32_t
GenericVertexAttributes::LocationIndexPerType(GLenum type)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    switch(type) {
    case GL_BOOL:
    case GL_INT:
    case GL_FLOAT:
    case GL_BOOL_VEC2:
    case GL_INT_VEC2:
    case GL_FLOAT_VEC2:
    case GL_BOOL_VEC3:
    case GL_INT_VEC3:
    case GL_FLOAT_VEC3:
    case GL_BOOL_VEC4:
    case GL_INT_VEC4:
    case GL_FLOAT_VEC4:         return 1;
    case GL_FLOAT_MAT2:         return 2;
    case GL_FLOAT_MAT3:         return 3;
    case GL_FLOAT_MAT4:         return 4;
    default: NOT_REACHED();     return 0;
    }
}
