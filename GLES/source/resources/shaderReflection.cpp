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
 *  @file       shaderReflection.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shader Reflection Functionality in GLOVE
 *
 */

#include "shaderReflection.h"

ShaderReflection::ShaderReflection()
{
    FUN_ENTRY(GL_LOG_TRACE);
}

ShaderReflection::~ShaderReflection()
{
    FUN_ENTRY(GL_LOG_TRACE);
}

void
ShaderReflection::ResetReflection(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    memset(&mReflectionData, 0, sizeof(reflectionData));
}

uint32_t
ShaderReflection::SerializeReflection(void *binary) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    uint8_t *rawDataPtr = reinterpret_cast<uint8_t *>(binary);
    uint32_t *u32DataPtr = NULL;

    u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
    *u32DataPtr = mReflectionData.mLiveAttributes;
    rawDataPtr += sizeof(uint32_t);

    for(uint32_t i = 0; i < mReflectionData.mLiveAttributes; ++i) {
        memcpy(rawDataPtr, mReflectionData.mAttributeReflection[i].name, GLSLANG_MAX_ATTRIBUTE_NAME_LENGTH);
        rawDataPtr += GLSLANG_MAX_ATTRIBUTE_NAME_LENGTH;
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mAttributeReflection[i].type;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mAttributeReflection[i].location;
        rawDataPtr += sizeof(uint32_t);
    }

    rawDataPtr += (GLSLANG_MAX_ATTRIBUTES - mReflectionData.mLiveAttributes) * sizeof(attribute);

    u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
    *u32DataPtr = mReflectionData.mLiveUniforms;
    rawDataPtr += sizeof(uint32_t);

    for(uint32_t i = 0; i < mReflectionData.mLiveUniforms; ++i) {
        memcpy(rawDataPtr, mReflectionData.mUniformReflection[i].reflectionName, GLSLANG_MAX_UNIFORM_NAME_LENGTH);
        rawDataPtr += GLSLANG_MAX_UNIFORM_NAME_LENGTH;
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mUniformReflection[i].location;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mUniformReflection[i].blockIndex;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mUniformReflection[i].arraySize;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mUniformReflection[i].glType;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mUniformReflection[i].offset;
        rawDataPtr += sizeof(uint32_t);
    }

    rawDataPtr += (GLSLANG_MAX_UNIFORMS - mReflectionData.mLiveUniforms) * sizeof(uniform);

    u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
    *u32DataPtr = mReflectionData.mLiveUniformBlocks;
    rawDataPtr += sizeof(uint32_t);

    for(uint32_t i = 0; i < mReflectionData.mLiveUniformBlocks; ++i) {
        memcpy(rawDataPtr, mReflectionData.mUniformBlockReflection[i].glslBlockName, GLSLANG_MAX_UNIFORM_BLOCK_NAME_LENGTH);
        rawDataPtr += GLSLANG_MAX_UNIFORM_BLOCK_NAME_LENGTH;
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mUniformBlockReflection[i].binding;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mUniformBlockReflection[i].blockSize;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
        *u32DataPtr = mReflectionData.mUniformBlockReflection[i].blockStage;
        rawDataPtr += sizeof(uint32_t);
        *rawDataPtr = mReflectionData.mUniformBlockReflection[i].isOpaque;
        rawDataPtr += sizeof(bool);
    }

    return sizeof(reflectionData);
}

uint32_t
ShaderReflection::DeserializeReflection(const void *binary)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    const uint8_t *rawDataPtr = reinterpret_cast<const uint8_t *>(binary);
    const uint32_t *u32DataPtr = NULL;

    u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
    mReflectionData.mLiveAttributes = *u32DataPtr;
    rawDataPtr += sizeof(uint32_t);

    for(uint32_t i = 0; i < mReflectionData.mLiveAttributes; ++i) {
        memcpy(mReflectionData.mAttributeReflection[i].name, rawDataPtr, GLSLANG_MAX_ATTRIBUTE_NAME_LENGTH);
        rawDataPtr += GLSLANG_MAX_ATTRIBUTE_NAME_LENGTH;
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mAttributeReflection[i].type = *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mAttributeReflection[i].location = *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
    }

    rawDataPtr += (GLSLANG_MAX_ATTRIBUTES - mReflectionData.mLiveAttributes) * sizeof(attribute);

    u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
    mReflectionData.mLiveUniforms = *u32DataPtr;
    rawDataPtr += sizeof(uint32_t);

    for(uint32_t i = 0; i < mReflectionData.mLiveUniforms; ++i) {
        memcpy(mReflectionData.mUniformReflection[i].reflectionName, rawDataPtr, GLSLANG_MAX_UNIFORM_NAME_LENGTH);
        rawDataPtr += GLSLANG_MAX_UNIFORM_NAME_LENGTH;
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mUniformReflection[i].location = *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mUniformReflection[i].blockIndex = *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mUniformReflection[i].arraySize = *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mUniformReflection[i].glType = *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mUniformReflection[i].offset = *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
    }

    rawDataPtr += (GLSLANG_MAX_UNIFORMS - mReflectionData.mLiveUniforms) * sizeof(uniform);

    u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
    mReflectionData.mLiveUniformBlocks = *u32DataPtr;
    rawDataPtr += sizeof(uint32_t);

    for(uint32_t i = 0; i < mReflectionData.mLiveUniformBlocks; ++i) {
        memcpy(mReflectionData.mUniformBlockReflection[i].glslBlockName, rawDataPtr, GLSLANG_MAX_UNIFORM_BLOCK_NAME_LENGTH);
        rawDataPtr += GLSLANG_MAX_UNIFORM_BLOCK_NAME_LENGTH;
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mUniformBlockReflection[i].binding = *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mUniformBlockReflection[i].blockSize = *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
        u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
        mReflectionData.mUniformBlockReflection[i].blockStage = (const shader_type_t) *u32DataPtr;
        rawDataPtr += sizeof(uint32_t);
        mReflectionData.mUniformBlockReflection[i].isOpaque = *rawDataPtr;
        rawDataPtr += sizeof(bool);
    }

    return sizeof(reflectionData);
}

void
ShaderReflection::ReadReflection() const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    printf("\nGL_ACTIVE_ATTRIBUTES: %d\n", mReflectionData.mLiveAttributes);
    for(uint32_t i = 0; i < mReflectionData.mLiveAttributes; ++i) {
        printf("%s (0x%x)\n", mReflectionData.mAttributeReflection[i].name, mReflectionData.mAttributeReflection[i].type);
        printf("location: %u\n", mReflectionData.mAttributeReflection[i].location);
    }

    printf("\nGL_ACTIVE_UNIFORMS: %d\n", mReflectionData.mLiveUniforms);
    for(uint32_t i = 0; i < mReflectionData.mLiveUniforms; ++i) {
        printf("%s (0x%x)\n", mReflectionData.mUniformReflection[i].reflectionName, mReflectionData.mUniformReflection[i].glType);
        printf("arraySize: %u, bufferOffset: %zu\n", mReflectionData.mUniformReflection[i].arraySize, mReflectionData.mUniformReflection[i].offset);
        printf("location: %u, blockIndex: %u\n", mReflectionData.mUniformReflection[i].location, mReflectionData.mUniformReflection[i].blockIndex);
    }

    printf("\nGL_ACTIVE_UNIFORM_BLOCKS: %d\n", mReflectionData.mLiveUniformBlocks);
    for(uint32_t i = 0; i < mReflectionData.mLiveUniformBlocks; ++i) {
        printf("%s , blockSize: %zu)\n", mReflectionData.mUniformBlockReflection[i].glslBlockName, mReflectionData.mUniformBlockReflection[i].blockSize);
        printf("blockStage: %u\n", mReflectionData.mUniformBlockReflection[i].blockStage);
        printf("binding: %u, isOpaque: %u\n", mReflectionData.mUniformBlockReflection[i].binding, mReflectionData.mUniformBlockReflection[i].isOpaque);
    }
}

int
ShaderReflection::GetAttributeLocation(const char *name) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(uint32_t i = 0; i < mReflectionData.mLiveAttributes; ++i) {
        if(!strcmp(mReflectionData.mAttributeReflection[i].name, name)) {
                return mReflectionData.mAttributeReflection[i].location;
        }
    }

    return -1;
}
