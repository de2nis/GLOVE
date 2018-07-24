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
 *  @file       shader.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shader Functionality in GLOVE
 *
 *  @scope
 *
 *  A Shader is a user-defined program designed to run on some stage of a
 *  graphics processor. Its purpose is to execute one of the programmable
 *  stages of the rendering pipeline.
 *
 */

#include "shader.h"

Shader::Shader(const vkContext_t *vkContext)
: mVkContext(vkContext), mVkShaderModule(VK_NULL_HANDLE), mShaderCompiler(nullptr), mSource(nullptr),
  mSourceLength(0), mShaderType(INVALID_SHADER), mRefCounter(0), mMarkForDeletion(false), mCompiled(false)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

Shader::~Shader()
{
    FUN_ENTRY(GL_LOG_TRACE);

    FreeSources();
    DestroyVkShader();
}

int
Shader::GetInfoLogLength(void) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    return mShaderCompiler->GetShaderInfoLog(mShaderType) ? (int)strlen(mShaderCompiler->GetShaderInfoLog(mShaderType)) : 0;
}

void
Shader::FreeSources(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mSource) {
        delete[] mSource;
        mSource = nullptr;
        mSourceLength = 0;
    }
}

void
Shader::SetShaderSource(GLsizei count, const GLchar *const *source, const GLint *length)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    FreeSources();
    mCompiled = false;

    if(!source || !count) {
        return;
    }

    uint32_t *sourceLenghts = new uint32_t[count];
    mSourceLength = 0;

    /// Count total source length
    for(GLsizei i = 0; i < count; ++i) {
        /// All strings in source are considered to be null terminated
        if(length == NULL) {
            /// strlen does not include null termination character
            sourceLenghts[i] = strlen(source[i]);
        } else {
            if(length[i] < 0) {
                /// NULL terminated again
                sourceLenghts[i] = strlen(source[i]);
            } else {
                sourceLenghts[i] = length[i];
            }
        }

        mSourceLength += sourceLenghts[i];
    }

    if(!mSourceLength) {
        if(sourceLenghts) {
            delete[] sourceLenghts;
        }
        return;
    }

    mSource = new char[mSourceLength + 1];

    /// Concatenate sources into 1 string and null terminate it
    uint32_t currentLength = 0;
    for(GLsizei i = 0; i < count; ++i) {
        if(sourceLenghts[i]) {
            memcpy((void *)&mSource[currentLength], source[i], sourceLenghts[i]);
            currentLength += sourceLenghts[i];
        }
    }
    assert(currentLength == mSourceLength);
    mSource[currentLength] = '\0';
    delete[] sourceLenghts;

    if(GLOVE_SAVE_SHADER_SOURCES_TO_FILES) {
        mShaderCompiler->EnableSaveSourceToFiles();
    }

    if(GLOVE_DUMP_ORIGINAL_SHADER_SOURCE) {
        cout << "\n\nINPUT SOURCE:\n" << mSource << "\n\n";
    }
}

int
Shader::GetShaderSourceLength(void) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return (mSourceLength > 0) ? mSourceLength+1 : 0;
}

char *
Shader::GetShaderSource(void) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mSource) {
        return nullptr;
    } else {
        char *source = new char[GetShaderSourceLength()];
        memcpy(source, mSource, GetShaderSourceLength());

        return source;
    }
}

char *
Shader::GetInfoLog(void) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    char *log = nullptr;

    if(mShaderCompiler) {
        uint32_t len = strlen(mShaderCompiler->GetShaderInfoLog(mShaderType)) + 1;
        log = new char[len];

        memcpy(log, mShaderCompiler->GetShaderInfoLog(mShaderType), len);

        /// NULL terminate it
        if(log[len - 1] != '\0') {
            log[len] = '\0';
        }
    }

    return log;
}

void
Shader::RefShader()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ++mRefCounter;
}

void
Shader::UnrefShader()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mRefCounter >= 0);

    if(mRefCounter > 0) {
        --mRefCounter;
    }
}

bool
Shader::CompileShader(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mSource);
    assert(mShaderType == SHADER_TYPE_VERTEX || mShaderType == SHADER_TYPE_FRAGMENT);

    if(mShaderType == SHADER_TYPE_VERTEX) {
        mCompiled = mShaderCompiler->CompileVertexShader(&mSource);
    } else {
        mCompiled = mShaderCompiler->CompileFragmentShader(&mSource);
    }

    return mCompiled;
}

void
Shader::DestroyVkShader(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

     if(mVkShaderModule != VK_NULL_HANDLE) {
         mVkShaderModule = VK_NULL_HANDLE;
     }
}

VkShaderModule
Shader::CreateVkShaderModule(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mShaderType == SHADER_TYPE_VERTEX || mShaderType == SHADER_TYPE_FRAGMENT);

    if (mVkShaderModule != VK_NULL_HANDLE)
        return mVkShaderModule;

    DestroyVkShader();

    if(!mSpv.size()) {
        return VK_NULL_HANDLE;
    }

    VkShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = mSpv.size() * sizeof(uint32_t);
    moduleCreateInfo.pCode = mSpv.data();

    if(vkCreateShaderModule(mVkContext->vkDevice, &moduleCreateInfo, 0, &mVkShaderModule)) {
        return VK_NULL_HANDLE;
    }

    return mVkShaderModule;
}
