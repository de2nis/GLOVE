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
 *  @file       resourceManager.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Resource Manager Functionality in GLOVE
 *
 */

#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include "resources/bufferObject.h"
#include "resources/framebuffer.h"
#include "resources/shaderProgram.h"
#include "resources/renderbuffer.h"
#include "resources/shader.h"
#include "resources/texture.h"
#include "utils/arrays.hpp"

typedef enum {
    NO_ID,
    SHADER_ID,
    SHADER_PROGRAM_ID
} shadingNamespaceType_t;

typedef struct {
    shadingNamespaceType_t                 type;
    uint32_t                               arrayIndex;
} ShadingNamespace_t;

class ResourceManager {
private:

    typedef ObjectArray<Texture>               TextureArray;
    typedef ObjectArray<BufferObject>          BufferArray;
    typedef ObjectArray<Shader>                ShaderArray;
    typedef ObjectArray<ShaderProgram>         ShaderProgramArray;
    typedef ObjectArray<Renderbuffer>          RenderbufferArray;
    typedef ObjectArray<Framebuffer>           FramebufferArray;
    typedef map<uint32_t, ShadingNamespace_t>  shadingPoolIDs_t;

    BufferArray                mBuffers;
    RenderbufferArray          mRenderbuffers;
    FramebufferArray           mFramebuffers;
    TextureArray               mTextures;

    static uint32_t            mShadingObjectCount;
    shadingPoolIDs_t           mShadingObjectPool;
    ShaderArray                mShaders;
    ShaderProgramArray         mShaderPrograms;

    GenericVertexAttributes*   mGenericVertexAttributes;
    Texture                *   mDefaultTexture2D;
    Texture                *   mDefaultTextureCubeMap;

public:
    ResourceManager();
    ~ResourceManager();

// Allocate/Deallocate Functions
    inline GLuint              AllocateTexture(void)                            { FUN_ENTRY(GL_LOG_TRACE); return mTextures.Allocate(); }
    inline GLuint              AllocateBuffer(void)                             { FUN_ENTRY(GL_LOG_TRACE); return mBuffers.Allocate(); }
    inline GLuint              AllocateRenderbuffer(void)                       { FUN_ENTRY(GL_LOG_TRACE); return mRenderbuffers.Allocate(); }
    inline GLuint              AllocateFramebuffer(void)                        { FUN_ENTRY(GL_LOG_TRACE); return mFramebuffers.Allocate(); }
    inline GLuint              AllocateShader(void)                             { FUN_ENTRY(GL_LOG_TRACE); return mShaders.Allocate(); }
    inline GLuint              AllocateShaderProgram(void)                      { FUN_ENTRY(GL_LOG_TRACE); return mShaderPrograms.Allocate(); }
    inline void                DeallocateTexture(uint32_t index)                { FUN_ENTRY(GL_LOG_TRACE); mTextures.Deallocate(index); }
    inline void                DeallocateBuffer(uint32_t index)                 { FUN_ENTRY(GL_LOG_TRACE); mBuffers.Deallocate(index); }
    inline void                DeallocateRenderbuffer(uint32_t index)           { FUN_ENTRY(GL_LOG_TRACE); mRenderbuffers.Deallocate(index); }
    inline void                DeallocateFramebuffer(uint32_t index)            { FUN_ENTRY(GL_LOG_TRACE); mFramebuffers.Deallocate(index); }
    inline void                DeallocateShader(Shader *shader)                 { FUN_ENTRY(GL_LOG_TRACE); mShaders.Deallocate(mShaders.GetObjectId(shader)); }
    inline void                DeallocateShaderProgram(ShaderProgram *program)  { FUN_ENTRY(GL_LOG_TRACE); mShaderPrograms.Deallocate(mShaderPrograms.GetObjectId(program)); }

// Get Functions
    inline
    GenericVertexAttributes *  GetGenericVertexAttributes(void)                 { FUN_ENTRY(GL_LOG_TRACE); return mGenericVertexAttributes; }
    inline Texture *           GetTexture(GLuint index)                         { FUN_ENTRY(GL_LOG_TRACE); return mTextures.GetObject(index); }
    inline Texture *           GetDefaultTexture(GLenum target)                 { FUN_ENTRY(GL_LOG_TRACE); return target == GL_TEXTURE_2D ? mDefaultTexture2D : mDefaultTextureCubeMap; }
    inline Framebuffer *       GetFramebuffer(GLuint index)                     { FUN_ENTRY(GL_LOG_TRACE); return mFramebuffers.GetObject(index); }
    inline Renderbuffer *      GetRenderbuffer(GLuint index)                    { FUN_ENTRY(GL_LOG_TRACE); return mRenderbuffers.GetObject(index); }
    inline BufferObject *      GetBuffer(GLuint index)                          { FUN_ENTRY(GL_LOG_TRACE); return mBuffers.GetObject(index); }
    inline uint32_t            GetTextureID(const Texture *tex)                 { FUN_ENTRY(GL_LOG_TRACE); return (tex == mDefaultTexture2D) || (tex == mDefaultTextureCubeMap) ? 0 : mTextures.GetObjectId(tex);}
    inline uint32_t            GetBufferID(const BufferObject *bo)              { FUN_ENTRY(GL_LOG_TRACE); return mBuffers.GetObjectId(bo); }
    inline Shader *            GetShader(GLuint index)                          { FUN_ENTRY(GL_LOG_TRACE); return mShaders.GetObject(index); }
    inline ShaderProgram *     GetShaderProgram(GLuint index)                   { FUN_ENTRY(GL_LOG_TRACE); return mShaderPrograms.GetObject(index); }
    inline uint32_t            GetShaderID(const Shader *shader)                { FUN_ENTRY(GL_LOG_TRACE); return mShaders.GetObjectId(shader); }
    inline uint32_t            GetShaderProgramID(const ShaderProgram *program) { FUN_ENTRY(GL_LOG_TRACE); return mShaderPrograms.GetObjectId(program); }
    inline uint32_t            GetShadingObjectCount(void)                const { FUN_ENTRY(GL_LOG_TRACE); return mShadingObjectCount; }
    inline ShadingNamespace_t  GetShadingObject(GLuint index)                   { FUN_ENTRY(GL_LOG_TRACE); return mShadingObjectPool[index]; }

// Map Functions
    inline uint32_t            PushShadingObject(ShadingNamespace_t obj)        { FUN_ENTRY(GL_LOG_TRACE); mShadingObjectPool[mShadingObjectCount] = obj; return mShadingObjectCount++;}
    inline void                EraseShadingObject(GLuint index)                 { FUN_ENTRY(GL_LOG_TRACE); mShadingObjectPool.erase(index); }

    inline bool                TextureExists(GLuint index)                const { FUN_ENTRY(GL_LOG_TRACE); return mTextures.ObjectExists(index); }
    inline bool                BufferExists(GLuint index)                 const { FUN_ENTRY(GL_LOG_TRACE); return mBuffers.ObjectExists(index); }
    inline bool                RenderbufferExists(GLuint index)           const { FUN_ENTRY(GL_LOG_TRACE); return mRenderbuffers.ObjectExists(index); }
    inline bool                FramebufferExists(GLuint index)            const { FUN_ENTRY(GL_LOG_TRACE); return mFramebuffers.ObjectExists(index); }
    inline bool                ShadingObjectExists(GLuint index)          const { FUN_ENTRY(GL_LOG_TRACE); return mShadingObjectPool.find(index) != mShadingObjectPool.end(); }

    inline GLboolean           IsShadingObject(GLuint index,
                                             shadingNamespaceType_t type) const { FUN_ENTRY(GL_LOG_TRACE); if(!index || index >= mShadingObjectCount || !ShadingObjectExists(index)) { return GL_FALSE; }
                                                                                                       ShadingNamespace_t shadId = mShadingObjectPool.find(index)->second;
                                                                                                       return (shadId.arrayIndex && shadId.type == type) ? GL_TRUE : GL_FALSE;}
    uint32_t                   FindShaderID(const Shader *shader);
    uint32_t                   FindShaderProgramID(const ShaderProgram *program);

    void                       CreateDefaultTextures(void);

};

#endif //__RESOURCEMANAGER_H__
