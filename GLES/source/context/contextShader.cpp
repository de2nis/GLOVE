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
 *  @file       contextShader.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Shaders
 *
 */

#include "context.h"

void
Context::CompileShader(GLuint shader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!HasShaderCompiler()) {
        return;
    }

    Shader *shaderPtr = GetShaderPtr(shader);
    if(!shaderPtr || !shaderPtr->HasSource()) {
        return;
    }

    shaderPtr->CompileShader();
}

GLuint
Context::CreateShader(GLenum type)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
        RecordError(GL_INVALID_ENUM);
        return 0;
    }

    GLuint res     = mResourceManager.AllocateShader();
    Shader *shader = mResourceManager.GetShader(res);
    shader->SetShaderType(type == GL_VERTEX_SHADER ? SHADER_TYPE_VERTEX : SHADER_TYPE_FRAGMENT);
    shader->SetVkContext(mVkContext);
    shader->SetShaderCompiler(mShaderCompiler);

    return mResourceManager.PushShadingObject((ShadingNamespace_t){SHADER_ID, res});
}

void
Context::DeleteShader(GLuint shader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!shader) {
        return;
    }

    DeleteShader(GetShaderPtr(shader));
}

void
Context::DeleteShader(Shader *shaderPtr)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!shaderPtr) {
        return;
    }

    if(!shaderPtr->GetRefCount()) {
        mResourceManager.EraseShadingObject(GetShaderId(shaderPtr));
        mResourceManager.DeallocateShader(shaderPtr);
    } else {
        shaderPtr->MarkForDeletion();
    }
}

Shader *
Context::GetShaderPtr(GLuint shader)
{
    FUN_ENTRY(GL_LOG_TRACE);

    if(!shader || shader >= mResourceManager.GetShadingObjectCount() || !mResourceManager.ShadingObjectExists(shader)) {
        RecordError(GL_INVALID_VALUE);
        return nullptr;
    }

    ShadingNamespace_t shadId = mResourceManager.GetShadingObject(shader);
    if(!shadId.arrayIndex || shadId.type != SHADER_ID) {
        RecordError(GL_INVALID_OPERATION);
        return nullptr;
    }

    return mResourceManager.GetShader(shadId.arrayIndex);
}

void
Context::GetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    Shader *shaderPtr = GetShaderPtr(shader);
    if(!shaderPtr) {
        return;
    }

    switch(pname) {
    case GL_COMPILE_STATUS:         *params = shaderPtr->IsCompiled()           ? GL_TRUE : GL_FALSE; break;
    case GL_DELETE_STATUS:          *params = shaderPtr->GetMarkForDeletion()   ? GL_TRUE : GL_FALSE; break;
    case GL_INFO_LOG_LENGTH:        *params = shaderPtr->GetInfoLogLength();      break;
    case GL_SHADER_SOURCE_LENGTH:   *params = shaderPtr->GetShaderSourceLength(); break;
    case GL_SHADER_TYPE:            *params = shaderPtr->GetShaderType() == SHADER_TYPE_FRAGMENT ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER; break;
    default:                        RecordError(GL_INVALID_ENUM); break;
    }

    if(!HasShaderCompiler()) {
        return;
    }
}

void
Context::GetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(bufsize < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    Shader *shaderPtr = GetShaderPtr(shader);
    if(!shaderPtr) {
        return;
    }

    char *log = shaderPtr->GetInfoLog();

    if(log) {
        GLint len         = shaderPtr->GetInfoLogLength();
        GLint returnedLen = static_cast<GLint>(std::max(std::min(bufsize, len) - 1, 0));

        if(length) {
            *length = returnedLen;
        }

        if(returnedLen) {
            memcpy((void *)infolog, (void *)log, returnedLen);
            infolog[returnedLen] = '\0';
        }
        else {
            memcpy((void *)infolog, (void *)log, 1);
            infolog[0] = '\0';
        }

        delete[] log;
    } else {
        if(length) {
            *length = 0;
        }
    }
}

void
Context::GetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!HasShaderCompiler()) {
        return;
    }

    if(shadertype != GL_VERTEX_SHADER && shadertype != GL_FRAGMENT_SHADER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    switch(precisiontype) {
    case GL_LOW_FLOAT:
                            range[0]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<float>::max()))));
                            range[1]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<float>::max()))));
                            *precision = static_cast<GLint>(floor(-log2(std::numeric_limits<float>::epsilon())));
                            break;
    case GL_MEDIUM_FLOAT:
                            range[0]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<float>::max()))));
                            range[1]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<float>::max()))));
                            *precision = static_cast<GLint>(floor(-log2(std::numeric_limits<float>::epsilon())));
                            break;
    case GL_HIGH_FLOAT:
                            range[0]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<float>::max()))));
                            range[1]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<float>::max()))));
                            *precision = static_cast<GLint>(floor(-log2(std::numeric_limits<float>::epsilon())));
                            break;

    case GL_LOW_INT:        range[0]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<short>::max()))));
                            range[1]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<short>::max()))));
                            *precision = 0;
                            break;

    case GL_MEDIUM_INT:     range[0]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<int16_t>::max()))));
                            range[1]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<int16_t>::max()))));
                            *precision = 0;
                            break;

    case GL_HIGH_INT:       range[0]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<int32_t>::max()))));
                            range[1]   = static_cast<GLint>(floor(log2(abs(std::numeric_limits<int32_t>::max()))));
                            *precision = 0;
                            break;

    default:
        RecordError(GL_INVALID_ENUM);
        break;
    }
}

void
Context::GetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(bufsize < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    Shader *shaderPtr = GetShaderPtr(shader);
    if(!shaderPtr) {
        return;
    }

    char *src = shaderPtr->GetShaderSource();
    if(!src) {
        return;
    }
    
    GLint len         = shaderPtr->GetShaderSourceLength();
    GLint returnedLen = static_cast<GLint>(std::max(std::min(bufsize, len) - 1, 0));

    if(length) {
        *length = returnedLen;
    }

    if(returnedLen) {
        memcpy((void *)source, (void *)src, returnedLen);
        source[returnedLen] = '\0';
    }
    else {
        memcpy((void *)source, (void *)src, 1);
        source[0] = '\0';
    }

    delete[] src;
}

GLboolean
Context::IsShader(GLuint shader)
{
    FUN_ENTRY(GL_LOG_TRACE);

    return mResourceManager.IsShadingObject(shader, SHADER_ID);
}

void
Context::ShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    NOT_IMPLEMENTED();
}

void
Context::ShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!HasShaderCompiler()) {
        return;
    }

    Shader *shaderPtr = GetShaderPtr(shader);
    if(!shaderPtr) {
        return;
    }

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }
    shaderPtr->SetShaderSource(count, string, length);
}

void
Context::ReleaseShaderCompiler(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    HasShaderCompiler();

    NOT_IMPLEMENTED();
}
