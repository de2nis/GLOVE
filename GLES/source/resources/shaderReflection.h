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
 *  @file       shaderReflection.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shader Reflection Functionality in GLOVE
 *
 */

#ifndef __SHADERREFLECTION_H__
#define __SHADERREFLECTION_H__

#include "utils/globals.h"
#include "utils/glsl_types.h"

#define GLSLANG_MAX_ATTRIBUTES                    GLOVE_MAX_VERTEX_ATTRIBS
#define GLSLANG_MAX_ATTRIBUTE_NAME_LENGTH         50

#define GLSLANG_MAX_UNIFORMS                      GLOVE_MAX_COMBINED_UNIFORM_VECTORS
#define GLSLANG_MAX_UNIFORM_NAME_LENGTH           50

#define GLSLANG_MAX_UNIFORM_BLOCKS                GLOVE_MAX_COMBINED_UNIFORM_VECTORS
#define GLSLANG_MAX_UNIFORM_BLOCK_NAME_LENGTH     50

class ShaderReflection {
private:
    typedef struct {
        char   name[GLSLANG_MAX_ATTRIBUTE_NAME_LENGTH];
        GLenum type;
        int    location;
    } attribute;

    typedef struct {
        char     reflectionName[GLSLANG_MAX_UNIFORM_NAME_LENGTH];
        uint32_t location;
        uint32_t blockIndex;
        int32_t  arraySize;
        GLenum   glType;
        size_t   offset;
    } uniform;

    typedef struct {
        char          glslBlockName[GLSLANG_MAX_UNIFORM_BLOCK_NAME_LENGTH];
        uint32_t      binding;
        size_t        blockSize;
        shader_type_t blockStage;
        bool          isOpaque;
    } uniformBlock;

    typedef struct {
        uint32_t     mLiveAttributes;
        uint32_t     mLiveUniforms;
        uint32_t     mLiveUniformBlocks;
        attribute    mAttributeReflection[GLSLANG_MAX_ATTRIBUTES];
        uniform      mUniformReflection[GLSLANG_MAX_UNIFORMS];
        uniformBlock mUniformBlockReflection[GLSLANG_MAX_UNIFORM_BLOCKS];
    } reflectionData;

    reflectionData mReflectionData;

public:
    ShaderReflection();
    ~ShaderReflection();

    void ResetReflection(void);
    int GetAttributeLocation(const char *name) const;
    uint32_t DeserializeReflection(const void *binary);
    uint32_t SerializeReflection(void *binary) const;
    uint32_t GetReflectionSize(void)                                          const { FUN_ENTRY(GL_LOG_TRACE); return sizeof(reflectionData); }
    void ReadReflection() const;

    inline uint32_t GetLiveAttributes(void)                                   const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mLiveAttributes; }
    inline uint32_t GetLiveUniforms(void)                                     const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mLiveUniforms; }
    inline uint32_t GetLiveUniformBlocks(void)                                const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mLiveUniformBlocks; }

    inline const char* GetAttributeName(uint32_t index)                       const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mAttributeReflection[index].name; }
    inline GLenum GetAttributeType(uint32_t index)                            const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mAttributeReflection[index].type; }
    inline int GetAttributeLocation(uint32_t index)                           const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mAttributeReflection[index].location; }

    inline const char* GetUniformReflectionName(uint32_t index)               const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformReflection[index].reflectionName; }
    inline int GetUniformLocation(uint32_t index)                             const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformReflection[index].location; }
    inline int GetUniformBlockIndex(uint32_t index)                           const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformReflection[index].blockIndex; }
    inline int GetUniformArraySize(uint32_t index)                            const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformReflection[index].arraySize; }
    inline uint32_t GetUniformType(uint32_t index)                            const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformReflection[index].glType; }
    inline GLenum GetUniformOffset(uint32_t index)                            const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformReflection[index].offset; }

    inline const char* GetUniformBlockGlslBlockName(uint32_t index)           const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformBlockReflection[index].glslBlockName; }
    inline uint32_t GetUniformBlockBinding(uint32_t index)                    const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformBlockReflection[index].binding; }
    inline size_t GetUniformBlockBlockSize(uint32_t index)                    const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformBlockReflection[index].blockSize; }
    inline shader_type_t GetUniformBlockBlockStage(uint32_t index)            const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformBlockReflection[index].blockStage; }
    inline bool GetUniformBlockOpaque(uint32_t index)                         const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionData.mUniformBlockReflection[index].isOpaque; }

    inline void SetLiveAttributes(uint32_t LiveAttributes)                          { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mLiveAttributes = LiveAttributes; }
    inline void SetLiveUniforms(uint32_t LiveUniforms)                              { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mLiveUniforms = LiveUniforms; }
    inline void SetLiveUniformBlocks(uint32_t LiveUniformBlocks)                    { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mLiveUniformBlocks = LiveUniformBlocks; }

    inline void SetAttributeName(const char* name, uint32_t index)                  { FUN_ENTRY(GL_LOG_TRACE); strcpy(mReflectionData.mAttributeReflection[index].name, name); }
    inline void SetAttributeType(GLenum type, uint32_t index)                       { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mAttributeReflection[index].type = type; }
    inline void SetAttributeLocation(int location, uint32_t index)                  { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mAttributeReflection[index].location = location; }

    inline void SetUniformReflectionName(const char* name, uint32_t index)          { FUN_ENTRY(GL_LOG_TRACE); strcpy(mReflectionData.mUniformReflection[index].reflectionName, name); }
    inline void SetUniformLocation(int location, uint32_t index)                    { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mUniformReflection[index].location = location; }
    inline void SetUniformBlockIndex(int blockIndex, uint32_t index)                { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mUniformReflection[index].blockIndex = blockIndex; }
    inline void SetUniformArraySize(int arraySize, uint32_t index)                  { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mUniformReflection[index].arraySize = arraySize; }
    inline void SetUniformType(GLenum type, uint32_t index)                         { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mUniformReflection[index].glType = type; }
    inline void SetUniformOffset(GLenum offset, uint32_t index)                     { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mUniformReflection[index].offset = offset; }

    inline void SetUniformBlockGlslBlockName(const char* name, uint32_t index)      { FUN_ENTRY(GL_LOG_TRACE); strcpy(mReflectionData.mUniformBlockReflection[index].glslBlockName, name); }
    inline void SetUniformBlockBinding(uint32_t binding, uint32_t index)            { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mUniformBlockReflection[index].binding = binding; }
    inline void SetUniformBlockBlockSize(size_t blockSize, uint32_t index)          { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mUniformBlockReflection[index].blockSize = blockSize; }
    inline void SetUniformBlockBlockStage(shader_type_t blockStage, uint32_t index) { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mUniformBlockReflection[index].blockStage = blockStage; }
    inline void SetUniformBlockOpaque(bool opaque, uint32_t index)                  { FUN_ENTRY(GL_LOG_TRACE); mReflectionData.mUniformBlockReflection[index].isOpaque = opaque; }
};

#endif //__SHADERREFLECTION_H__
