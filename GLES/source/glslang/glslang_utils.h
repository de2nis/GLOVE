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
 *  @file       glslang_utils.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      glslang Utility Header File
 *
 */

#ifndef __GLSLANG_UTILS_H__
#define __GLSLANG_UTILS_H__

#include "utils/globals.h"
#include "utils/glsl_types.h"

namespace glslang {
    class TShader;
    class TProgram;
}

typedef struct {
    string                          name;
    int32_t                         arraySize;                  /// Array size. 1 for non arrays
} aggregate_t;
typedef map<string, aggregate_t>    aggregateMap_t;

struct uniformBlock_t {
    string                          name;                       /// For debug
    string                          glslBlockName;              /// Block name that will be generated in final GLSL
    uint32_t                        binding;                    /// layout decoration
    bool                            isOpaque;                   /// true for opaque types (samplers)
    size_t                          blockSize;                  /// Uniform block's size in bytes (including inactive variables)
    shader_type_t                   blockStage;                 /// Uniform block's shader stage
    const aggregate_t *             pAggregate;

    uniformBlock_t():
        binding(0),
        isOpaque(false),
        blockSize(0),
        blockStage(INVALID_SHADER),
        pAggregate(nullptr)
    {
        FUN_ENTRY(GL_LOG_TRACE);
    }
    uniformBlock_t(string n, string gbn, uint32_t b, bool io, size_t bs, shader_type_t bStage, const aggregate_t *pAggr)
     : name(n),
       glslBlockName(gbn),
       binding(b),
       isOpaque(io),
       blockSize(bs),
       blockStage(bStage),
       pAggregate(pAggr)
    {
        FUN_ENTRY(GL_LOG_TRACE);
    }

    ~uniformBlock_t()
    {
        FUN_ENTRY(GL_LOG_TRACE);
    }
};
typedef struct uniformBlock_t       uniformBlock_t;
typedef map<string, uniformBlock_t> uniformBlockMap_t;

struct uniform_t {
    string                          variableName;               /// Uniform's name without aggregate's name
    string                          reflectionName;             /// Full reflection name
    GLenum                          glType;                     /// glType
    uint32_t                        location;                   /// Location assigned to uniform
    int32_t                         arraySize;                  /// Array size. 1 for non arrays
    int32_t                         indexIntoAggregateArray;    /// Array index in aggregate array (-1 if it aggregate is not an array or if does not belong to one)
    const aggregate_t *             pAggregate;                 /// Pointer to aggregate type (if belongs to one)
    uniformBlock_t *                pBlock;                     /// Pointer to uniformBlock_t this uniform belongs
    shader_type_t                   stage;                      /// Uniform's shader stage
    size_t                          offset;                     /// Offset from start of uniform block's data

    uniform_t()
     : glType(0),
       location(0),
       arraySize(0),
       indexIntoAggregateArray(-1),
       pAggregate(nullptr),
       pBlock(nullptr),
       stage(INVALID_SHADER),
       offset(0)
    {
        FUN_ENTRY(GL_LOG_TRACE);
        assert(0);
    }

    uniform_t(string vn, string rn, GLenum glt, uint32_t loc, int32_t as, int32_t ind, const aggregate_t *pag, uniformBlock_t *pub, shader_type_t stg, size_t off)
     : variableName(vn),
       reflectionName(rn),
       glType(glt),
       location(loc),
       arraySize(as),
       indexIntoAggregateArray(ind),
       pAggregate(pag),
       pBlock(pub),
       stage(stg),
       offset(off)
    {
        FUN_ENTRY(GL_LOG_TRACE);
    }

    ~uniform_t()
    {
        FUN_ENTRY(GL_LOG_TRACE);
    }
};
typedef struct uniform_t            uniform_t;

#endif // __GLSLANG_UTILS_H__
