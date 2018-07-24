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
 *  @file       shaderResourceInterface.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shader Resource Interface Functionality in GLOVE
 *
 */

#ifndef __SHADERRESOURCEINTERFACE_H__
#define __SHADERRESOURCEINTERFACE_H__

#include "shaderReflection.h"
#include "bufferObject.h"

class ShaderResourceInterface {
public:
    struct attribute {
        string                      name;
        GLenum                      glType;
        uint32_t                    location;

        attribute(string n, GLenum type, uint32_t loc)
         : name(n),
           glType(type),
           location(loc)
        {
            FUN_ENTRY(GL_LOG_TRACE);
        }
    };
    typedef struct attribute        attribute;
    typedef vector<attribute>       attributeInterface;

    struct uniform{
        string                      reflectionName;
        uint32_t                    location;
        uint32_t                    blockIndex;
        int32_t                     arraySize;
        GLenum                      glType;
        size_t                      offset;

        uniform(string name, uint32_t loc, uint32_t bIndex, int32_t size, GLenum type, size_t offset)
         : reflectionName(name),
           location(loc),
           blockIndex(bIndex),
           arraySize(size),
           glType(type),
           offset(offset)
        {
            FUN_ENTRY(GL_LOG_TRACE);
        }
    };
    typedef struct uniform uniform;
    typedef vector<uniform>         uniformInterface;

    struct uniformData {
        const uint8_t *             pClientData;
        bool                        clientDataDirty;

        uniformData()
         : pClientData(nullptr),
           clientDataDirty(false)
        {
            FUN_ENTRY(GL_LOG_TRACE);
        }

        ~uniformData()
        {
            FUN_ENTRY(GL_LOG_TRACE);

            if(pClientData) {
                delete[] pClientData;
                pClientData = nullptr;
            }
        }
    };
    typedef struct uniformData uniformData;
    typedef map<string, uniformData> uniformDataInterface;

    struct uniformBlock {
        string                      glslBlockName;
        uint32_t                    binding;
        size_t                      blockSize;
        shader_type_t               blockStage;
        bool                        isOpaque;

        uniformBlock(string blockName, uint32_t bind, size_t bSize, shader_type_t shaderType, bool opaque)
         : glslBlockName(blockName),
           binding(bind),
           blockSize(bSize),
           blockStage(shaderType),
           isOpaque(opaque)
        {
            FUN_ENTRY(GL_LOG_TRACE);
        }
    };
    typedef struct uniformBlock uniformBlock;
    typedef vector<uniformBlock>    uniformBlockInterface;

    struct uniformBlockData {
        UniformBufferObject *       pBufferObject;

        uniformBlockData()
         : pBufferObject(nullptr)
        {
            FUN_ENTRY(GL_LOG_TRACE);
        }

        ~uniformBlockData()
        {
            FUN_ENTRY(GL_LOG_TRACE);

            if(pBufferObject) {
                delete pBufferObject;
                pBufferObject = nullptr;
            }
        }
    };
    typedef struct uniformBlockData uniformBlockData;
    typedef map<string, uniformBlockData> uniformBlockDataInterface;

    typedef map<string, uint32_t>   attribsLayout_t;

private:
    uint32_t mLiveAttributes;
    uint32_t mLiveUniforms;
    uint32_t mLiveUniformBlocks;

    size_t mActiveAttributeMaxLength;
    size_t mActiveUniformMaxLength;
    uint32_t mReflectionSize;

    ShaderReflection* mShaderReflection;

    attributeInterface mAttributeInterface;

    uniformInterface mUniformInterface;
    uniformDataInterface mUniformDataInterface;

    uniformBlockInterface mUniformBlockInterface;
    uniformBlockDataInterface mUniformBlockDataInterface;

    attribsLayout_t mCustomAttributesLayout;

    uint32_t OccupiedLocationsPerType(GLenum type);
    void CleanUp(void);

public:
    ShaderResourceInterface();
    ~ShaderResourceInterface();

    inline uint32_t GetLiveAttributes(void)                                     const { FUN_ENTRY(GL_LOG_TRACE); return mLiveAttributes; }
    inline uint32_t GetLiveUniforms(void)                                       const { FUN_ENTRY(GL_LOG_TRACE); return mLiveUniforms; }
    inline uint32_t GetLiveUniformBlocks(void)                                  const { FUN_ENTRY(GL_LOG_TRACE); return mLiveUniformBlocks; }

    inline size_t GetActiveUniformMaxLen(void)                                  const { FUN_ENTRY(GL_LOG_TRACE); return mActiveUniformMaxLength; }
    inline size_t GetActiveAttribMaxLen(void)                                   const { FUN_ENTRY(GL_LOG_TRACE); return mActiveAttributeMaxLength; }

    inline uint32_t GetReflectionSize(void)                                     const { FUN_ENTRY(GL_LOG_TRACE); return mReflectionSize; }

    int GetAttributeLocation(const char *name) const;
    inline int GetAttributeLocation(uint32_t index)                             const { FUN_ENTRY(GL_LOG_TRACE); return mAttributeInterface[index].location; }
    inline GLenum GetAttributeType(uint32_t index)                              const { FUN_ENTRY(GL_LOG_TRACE); return mAttributeInterface[index].glType; }
    int GetAttributeType(int index) const;
    const string & GetAttributeName(int index) const;

    inline int32_t GetUniformArraySize(uint32_t index)                          const { FUN_ENTRY(GL_LOG_TRACE); return mUniformInterface[index].arraySize; }
    inline GLenum GetUniformType(uint32_t index)                                const { FUN_ENTRY(GL_LOG_TRACE); return mUniformInterface[index].glType; }
    void GetUniformClientData(uint32_t location, size_t size, void *ptr) const;
	  const uint8_t* GetUniformClientData(uint32_t index) const;
	  UniformBufferObject * GetUniformBufferObject(uint32_t index) const;
    int GetUniformLocation(const char *name) const;

    inline uint32_t GetUniformBlockBinding(uint32_t index)                      const { FUN_ENTRY(GL_LOG_TRACE); return mUniformBlockInterface[index].binding; }
    inline shader_type_t GetUniformBlockBlockStage(uint32_t index)              const { FUN_ENTRY(GL_LOG_TRACE); return mUniformBlockInterface[index].blockStage; }
    inline bool IsUniformBlockOpaque(uint32_t index)                            const { FUN_ENTRY(GL_LOG_TRACE); return mUniformBlockInterface[index].isOpaque; }

    const ShaderResourceInterface::uniform * GetUniform(uint32_t index)         const { FUN_ENTRY(GL_LOG_TRACE); return index < mUniformInterface.size() ? mUniformInterface.data() + index : nullptr; }
    const ShaderResourceInterface::uniform * GetUniformAtLocation(uint32_t location) const;
    const ShaderResourceInterface::attribute * GetVertexAttribute(int index) const;

    void SetActiveAttributeMaxLength(void);
    void SetActiveUniformMaxLength(void);

    inline void SetCustomAttribsLayout(const char *name, int index)                   { FUN_ENTRY(GL_LOG_TRACE); mCustomAttributesLayout[std::string(name)] = index; }
    inline void SetReflectionSize(void)                                               { FUN_ENTRY(GL_LOG_TRACE); mReflectionSize = mShaderReflection->GetReflectionSize(); }

    void SetUniformClientData(uint32_t location, size_t size, const void *ptr);
	  void SetSampler(uint32_t location, int count, const int *textureUnit);

    void AllocateUniformClientData(void);
	  bool AllocateUniformBufferObjects(const vkContext_t *vkContext);
	  bool UpdateUniformBufferData(void);

    void UpdateAttributeInterface(void);
    void CreateInterface(void);
    inline void SetReflection(ShaderReflection* reflection)                           { FUN_ENTRY(GL_LOG_TRACE); mShaderReflection = reflection; };

    void DumpGloveShaderVertexInputInterface(void);
};

#endif // __SHADERRESOURCEINTERFACE_H__
