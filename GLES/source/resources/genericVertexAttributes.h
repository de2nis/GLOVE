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
 *  @file       genericVertexAttributes.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Generic Vertex Attributes Functionality in GLOVE
 *
 */

#ifndef __GENERICVERTEXATTRIBUTES_H__
#define __GENERICVERTEXATTRIBUTES_H__

#include "bufferObject.h"
#include "utils/parser_helpers.h"
#include "utils/glsl_types.h"

class GenericVertexAttributes {
private:
    typedef struct {
        bool                            active;
        int                             nElements;
        int                             dataType;
        bool                            normalized;
        size_t                          stride;
        uint32_t                        offset;
        uintptr_t                       ptr;
        BufferObject *                  vbo;
        VkFormat                        vkFormat;
        GenericVec4                     genericValue;
        bool                            internalVBO;
    } vertexAttrib_t;

    vertexAttrib_t                      mGenericVertexAttributes[GLOVE_MAX_VERTEX_ATTRIBS];

    void                                CleanupIfInternalVBO(uint32_t location);

public:
    GenericVertexAttributes();
    ~GenericVertexAttributes();

    void                                CleanupVertexAttributes();
    void                                DisableVertexAttribute(uint32_t location);
    void                                EnableVertexAttribute(uint32_t location, BufferObject *vbo);
    static uint32_t                     LocationIndexPerType(GLenum type);

    VkBuffer                            GetVkBuffer(uint32_t location)                            const { FUN_ENTRY(GL_LOG_TRACE); return GetVertexAttribVbo(location)->GetVkBuffer(); }
    bool                                GetVertexAttribActive(uint32_t location)                  const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].active; }
    int                                 GetVertexAttribSize(uint32_t location)                    const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].nElements; }
    int                                 GetVertexAttribType(uint32_t location)                    const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].dataType; }
    bool                                GetVertexAttribNormalized(uint32_t location)              const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].normalized; }
    size_t                              GetVertexAttribStride(uint32_t location)                  const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].stride; }
    uint32_t                            GetVertexAttribOffset(uint32_t location)                  const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].offset; }
    uintptr_t                           GetVertexAttribPointer(uint32_t location)                 const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].ptr; }
    BufferObject *                      GetVertexAttribVbo(uint32_t location)                     const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].vbo; }
    VkFormat                            GetVertexAttribFormat(uint32_t location)                  const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].vkFormat; }
    bool                                GetVertexAttribInternalVBO(uint32_t location)             const { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes[location].internalVBO; }
    template<typename T> void           GetGenericVertexAttribute(uint32_t location, T *ptr)      const;

    template<typename T> void           SetGenericVertexAttribute(uint32_t location, const T *ptr);
    void                                SetVertexAttribVbo(uint32_t location, BufferObject *vbo);
    void                                SetVertexAttributePointer(uint32_t location, size_t nElements, int type, bool normalized, size_t stride, const void *ptr, BufferObject *activeVBO);
    void                                SetVertexAttribActive(uint32_t location, bool active)           { FUN_ENTRY(GL_LOG_TRACE); mGenericVertexAttributes[location].active = active; }
    void                                SetVertexAttribSize(uint32_t location, int nElements)           { FUN_ENTRY(GL_LOG_TRACE); mGenericVertexAttributes[location].nElements = nElements; }
    void                                SetVertexAttribType(uint32_t location, int dataType)            { FUN_ENTRY(GL_LOG_TRACE); mGenericVertexAttributes[location].dataType = dataType; }
    void                                SetVertexAttribNormalized(uint32_t location, bool normalized)   { FUN_ENTRY(GL_LOG_TRACE); mGenericVertexAttributes[location].normalized = normalized; }
    void                                SetVertexAttribStride(uint32_t location, size_t stride)         { FUN_ENTRY(GL_LOG_TRACE); mGenericVertexAttributes[location].stride = stride; }
    void                                SetVertexAttribOffset(uint32_t location, uint32_t offset)       { FUN_ENTRY(GL_LOG_TRACE); mGenericVertexAttributes[location].offset = offset; }
    void                                SetVertexAttribPointer(uint32_t location, uintptr_t ptr)        { FUN_ENTRY(GL_LOG_TRACE); mGenericVertexAttributes[location].ptr = ptr; }
    void                                SetVertexAttribFormat(uint32_t location, VkFormat vkFormat)     { FUN_ENTRY(GL_LOG_TRACE); mGenericVertexAttributes[location].vkFormat = vkFormat; }
    void                                SetVertexAttribInternalVBO(uint32_t location, bool internalVBO) { FUN_ENTRY(GL_LOG_TRACE); mGenericVertexAttributes[location].internalVBO = internalVBO; }
};

template<typename T> void
GenericVertexAttributes::SetGenericVertexAttribute(uint32_t location, const T *ptr)
{
    FUN_ENTRY(GL_LOG_TRACE);

    assert(ptr);
    assert(location >= 0 && location < GLOVE_MAX_VERTEX_ATTRIBS);

    if(typeid(ptr) == typeid(const float *)) {
        mGenericVertexAttributes[location].genericValue.v.fData[0] = ptr[0];
        mGenericVertexAttributes[location].genericValue.v.fData[1] = ptr[1];
        mGenericVertexAttributes[location].genericValue.v.fData[2] = ptr[2];
        mGenericVertexAttributes[location].genericValue.v.fData[3] = ptr[3];
    } else {
        NOT_REACHED();
    }
}

template<typename T> void
GenericVertexAttributes::GetGenericVertexAttribute(uint32_t location, T *ptr) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    ptr[0] = mGenericVertexAttributes[location].genericValue.v.fData[0];
    ptr[1] = mGenericVertexAttributes[location].genericValue.v.fData[1];
    ptr[2] = mGenericVertexAttributes[location].genericValue.v.fData[2];
    ptr[3] = mGenericVertexAttributes[location].genericValue.v.fData[3];
}

#endif // __GENERICVERTEXATTRIBUTES_H__
