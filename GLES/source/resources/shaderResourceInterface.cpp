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
 *  @file       shaderResourceInterface.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shader Resource Interface Functionality in GLOVE
 *
 *  @scope
 *
 *  A Shader-resource interface accesses shader (reflection) information.
 *
 */

#include "shaderResourceInterface.h"
#include "utils/glUtils.h"
#include "utils/glLogger.h"

ShaderResourceInterface::ShaderResourceInterface()
: mLiveAttributes(0), mLiveUniforms(0), mLiveUniformBlocks(0),
  mActiveAttributeMaxLength(0), mActiveUniformMaxLength(0), mReflectionSize(0)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

ShaderResourceInterface::~ShaderResourceInterface()
{
    FUN_ENTRY(GL_LOG_TRACE);

    CleanUp();
}

void
ShaderResourceInterface::CleanUp(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mLiveAttributes = 0;
    mLiveUniforms = 0;
    mLiveUniformBlocks = 0;

    mAttributeInterface.clear();
    mUniformInterface.clear();
    mUniformBlockInterface.clear();
}

void
ShaderResourceInterface::CreateInterface(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    CleanUp();

    mLiveAttributes = mShaderReflection->GetLiveAttributes();
    mLiveUniforms = mShaderReflection->GetLiveUniforms();
    mLiveUniformBlocks = mShaderReflection->GetLiveUniformBlocks();


    mAttributeInterface.reserve(mLiveAttributes);
    mUniformInterface.reserve(mLiveUniforms);
    mUniformBlockInterface.reserve(mLiveUniformBlocks);

    for(uint32_t i = 0; i < mLiveAttributes; ++i) {
        mAttributeInterface.emplace_back(mShaderReflection->GetAttributeName(i),
                                         mShaderReflection->GetAttributeType(i),
                                         mShaderReflection->GetAttributeLocation(i));
    }

    for(uint32_t i = 0; i < mLiveUniforms; ++i) {
        mUniformInterface.emplace_back(mShaderReflection->GetUniformReflectionName(i),
                                       mShaderReflection->GetUniformLocation(i),
                                       mShaderReflection->GetUniformBlockIndex(i),
                                       mShaderReflection->GetUniformArraySize(i),
                                       mShaderReflection->GetUniformType(i),
                                       mShaderReflection->GetUniformOffset(i));
    }

    for(uint32_t i = 0; i < mShaderReflection->GetLiveUniformBlocks(); ++i) {
        mUniformBlockInterface.emplace_back(mShaderReflection->GetUniformBlockGlslBlockName(i),
                                            mShaderReflection->GetUniformBlockBinding(i),
                                            mShaderReflection->GetUniformBlockBlockSize(i),
                                            mShaderReflection->GetUniformBlockBlockStage(i),
                                            mShaderReflection->GetUniformBlockOpaque(i));
    }
}

void
ShaderResourceInterface::AllocateUniformClientData(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(auto& uni : mUniformInterface) {
        size_t clientDataSize = uni.arraySize * GlslTypeToSize(uni.glType);
        uint8_t* clientData = new uint8_t[clientDataSize];
        memset((void *)clientData, 0, clientDataSize);

        mUniformDataInterface.insert(make_pair(uni.reflectionName, uniformData()));
        map<std::string, uniformData>::iterator it = mUniformDataInterface.find(uni.reflectionName);
        assert(it != mUniformDataInterface.end());
        it->second.pClientData = clientData;
    }
}

bool
ShaderResourceInterface::AllocateUniformBufferObjects(const vkContext_t *vkContext)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(auto &uniBlock : mUniformBlockInterface) {
        if(!uniBlock.isOpaque) {
            assert(uniBlock.blockSize);

            mUniformBlockDataInterface.insert(make_pair(uniBlock.glslBlockName, uniformBlockData()));
            map<std::string, uniformBlockData>::iterator it = mUniformBlockDataInterface.find(uniBlock.glslBlockName);
            assert(it != mUniformBlockDataInterface.end());
            it->second.pBufferObject = new UniformBufferObject(vkContext);
            it->second.pBufferObject->BufferObject::Allocate(uniBlock.blockSize, NULL);
        }
    }

    return true;
}

UniformBufferObject *
ShaderResourceInterface::GetUniformBufferObject(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    map<std::string, uniformBlockData>::const_iterator itBlock = mUniformBlockDataInterface.find(mUniformBlockInterface[index].glslBlockName);
    return itBlock->second.pBufferObject;
}

bool
ShaderResourceInterface::UpdateUniformBufferData(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    map<std::string, uniformData>::iterator itUni;
    map<std::string, uniformBlockData>::iterator itBlock;

    for(auto& uniform : mUniformInterface) {
        itUni = mUniformDataInterface.find(uniform.reflectionName);
        if(!itUni->second.clientDataDirty) {
            continue;
        }

        const uniformBlock *pBlock = &mUniformBlockInterface[uniform.blockIndex];
        itBlock = mUniformBlockDataInterface.find(pBlock->glslBlockName);
        size_t size = uniform.arraySize * GlslTypeToSize(uniform.glType);


        assert(pBlock);
        assert(uniform.offset + size <= pBlock->blockSize);
        assert(itBlock->second.pBufferObject);

        itBlock->second.pBufferObject->UpdateData(size, uniform.offset, (void *)itUni->second.pClientData);
        itUni->second.clientDataDirty = false;
    }

    return true;
}

const ShaderResourceInterface::uniform *
ShaderResourceInterface::GetUniformAtLocation(uint32_t location) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(const auto &uniform : mUniformInterface) {
        if(location >= (uint32_t)uniform.location && location < (uint32_t)(uniform.location + uniform.arraySize)) {
            return &uniform;
        }
    }

    return nullptr;
}

int
ShaderResourceInterface::GetUniformLocation(const char *name) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(name);

    if(name[strlen(name) - 1] != ']') {
        for(auto &uniIt : mUniformInterface) {
            string testName = uniIt.reflectionName;
            if(testName.back() == ']' && !strcmp(testName.substr(0, testName.length() - 3).c_str(), name)) {
                return uniIt.location;
            } else if(testName.back() != ']' && !strcmp(testName.c_str(), name)) {
                return uniIt.location;
            }
        }
    } else {
        /// Adjust location according to the specific array index requested
        string requestedName = string(name);
        size_t leftBracketPos = requestedName.find_last_of("[");
        int index = std::stoi(requestedName.substr(leftBracketPos + 1));

        string ptrName = requestedName.substr(0, leftBracketPos);
        for(auto &uniIt : mUniformInterface) {
            if(!ptrName.compare(uniIt.reflectionName.substr(0, uniIt.reflectionName.length() - 3))) {
                if(index >= uniIt.arraySize) {
                    return -1;
                } else {
                    return uniIt.location + index;
                }
            }
        }
    }

    return -1;
}

void
ShaderResourceInterface::SetUniformClientData(uint32_t location, size_t size, const void *ptr)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderResourceInterface::uniform *uniform = nullptr;
    for(auto &uni: mUniformInterface) {
        if(location >= (uint32_t)uni.location && location < (uint32_t)(uni.location + uni.arraySize)) {
            uniform = &uni;
            break;
        }
    }
    assert(uniform);
    assert(uniform->location <= location);

    size_t arrayOffset = (location - uniform->location) * GlslTypeToSize(uniform->glType);
    assert(arrayOffset + size <= uniform->arraySize * GlslTypeToSize(uniform->glType));

    map<std::string, uniformData>::iterator it = mUniformDataInterface.find(uniform->reflectionName);
    memcpy((void *)(it->second.pClientData + arrayOffset), ptr, size);
    it->second.clientDataDirty = true;
}

void
ShaderResourceInterface::SetSampler(uint32_t location, int count, const int *textureUnit)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    while(count--) {
        assert((*textureUnit >= GL_TEXTURE0 && *textureUnit < GL_TEXTURE0 + GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS) ||
               (*textureUnit >= 0 && *textureUnit < GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS));

        const ShaderResourceInterface::uniform *uniformSampler = GetUniformAtLocation(location + count);
        map<std::string, uniformData>::iterator it = mUniformDataInterface.find(uniformSampler->reflectionName);

        assert(uniformSampler->location <= location);
        size_t arrayOffset = (location - uniformSampler->location) * GlslTypeToSize(uniformSampler->glType);

        /// Make sure textureUnit is inside [0, GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
        if(*textureUnit >= GL_TEXTURE0 && *textureUnit < GL_TEXTURE0 + GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
            *((glsl_sampler_t *)(it->second.pClientData + arrayOffset)) = (glsl_sampler_t)(*textureUnit - GL_TEXTURE0);
        } else {
            *((glsl_sampler_t *)(it->second.pClientData + arrayOffset)) = (glsl_sampler_t)(*textureUnit);
        }

        ++textureUnit;
    }
}

const uint8_t*
ShaderResourceInterface::GetUniformClientData(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    map<std::string, uniformData>::const_iterator it = mUniformDataInterface.find(mUniformInterface[index].reflectionName);
    return it->second.pClientData;
}

void
ShaderResourceInterface::GetUniformClientData(uint32_t location, size_t size, void *ptr) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    const ShaderResourceInterface::uniform *uniform = GetUniformAtLocation(location);
    assert(uniform);
    assert(uniform->location <= location);

    size_t arrayOffset = (location - uniform->location) * GlslTypeToSize(uniform->glType);
    assert(arrayOffset + size <= uniform->arraySize * GlslTypeToSize(uniform->glType));

    map<std::string, uniformData>::const_iterator it = mUniformDataInterface.find(uniform->reflectionName);
    memcpy(ptr, (void *)(it->second.pClientData + arrayOffset), size);
}

void
ShaderResourceInterface::UpdateAttributeInterface(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mShaderReflection);

    uint32_t i;
    uint32_t nLiveAttributes = mShaderReflection->GetLiveAttributes();
    if(!nLiveAttributes) {
        return;
    }

    vector<int> locations(nLiveAttributes);
    for(i = 0; i < nLiveAttributes; ++i) {
        attribsLayout_t::const_iterator it = mCustomAttributesLayout.find(string(mShaderReflection->GetAttributeName(i)));
        locations[i] = it == mCustomAttributesLayout.end() ? -1 : it->second;
    }

    int acc = 0;
    for(i = 0; i < nLiveAttributes; ++i) {
        int location = 0;

        if((uint32_t)mShaderReflection->GetAttributeLocation(i) == GLOVE_INVALID_OFFSET) {
            if(locations[i] != -1) {
                location = locations[i];
            } else {
                location = locations[i] = acc;
                for(uint32_t j = 0; j < nLiveAttributes; ++j) {
                    if(locations[j] == acc) {
                        acc += OccupiedLocationsPerType(mShaderReflection->GetAttributeType(i));
                        j = 0;
                    }
                }
            }
            mShaderReflection->SetAttributeLocation(location, i);
        }
    }
}

uint32_t
ShaderResourceInterface::OccupiedLocationsPerType(GLenum type)
{
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

int
ShaderResourceInterface::GetAttributeLocation(const char *name) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(auto &it : mAttributeInterface) {
        if(!strcmp(it.name.c_str(), name)) {
            return it.location;
        }
    }

    return -1;
}

const string &
ShaderResourceInterface::GetAttributeName(int index) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(index >= 0 && index < (int)mAttributeInterface.size());

    return mAttributeInterface[index].name;
}

int
ShaderResourceInterface::GetAttributeType(int index) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(index >= 0 && index < (int)mAttributeInterface.size());

    return mAttributeInterface[index].glType;
}

const ShaderResourceInterface::attribute *
ShaderResourceInterface::GetVertexAttribute(int index) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(index >= 0 && index < (int)mAttributeInterface.size());

    return &(*(mAttributeInterface.cbegin() + index));
}

void
ShaderResourceInterface::SetActiveAttributeMaxLength(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mActiveAttributeMaxLength = 0;
    for(const auto &attribute : mAttributeInterface) {
        size_t len = attribute.name.length() + 1;
        if(len > mActiveAttributeMaxLength) {
            mActiveAttributeMaxLength = len;
        }
    }
}

void
ShaderResourceInterface::SetActiveUniformMaxLength(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mActiveUniformMaxLength = 0;
    for(const auto &uniform : mUniformInterface) {
        size_t len = uniform.reflectionName.length() + 1;
        if(len > mActiveUniformMaxLength) {
            mActiveUniformMaxLength = len;
        }
    }
}

void
ShaderResourceInterface::DumpGloveShaderVertexInputInterface(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    printf("Shader's vertex input interface:\n");

    for(const auto &vi : mAttributeInterface) {
        printf("%s: \n", vi.name.c_str());
#ifdef NDEBUG
        printf("  location: %d glsl type: %s\n", vi.location, GlAttribTypeToString(vi.glType));
#else
        printf("  location: %d glsl type: 0x%04x\n", vi.location, vi.glType);
#endif // NDEBUG
    }

    printf("\n\n");
}
