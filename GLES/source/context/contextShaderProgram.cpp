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
 *  @file       contextShaderProgram.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Shader Programs
 *
 */

#include "context.h"

void
Context::AttachShader(GLuint program, GLuint shader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }

    Shader *shaderPtr = GetShaderPtr(shader);
    if(!shaderPtr) {
        return;
    }

    if((progPtr->HasFragmentShader() && shaderPtr->GetShaderType() == SHADER_TYPE_FRAGMENT) ||
       (progPtr->HasVertexShader()   && shaderPtr->GetShaderType() == SHADER_TYPE_VERTEX)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(progPtr->IsShaderAttached(shaderPtr)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    progPtr->AttachShader(shaderPtr);
    progPtr->SetStagesIDs(shaderPtr->GetShaderType() == SHADER_TYPE_VERTEX ? 0 : 1, shader);
}

void
Context::BindAttribLocation(GLuint program, GLuint index, const char *name)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }

    if(index >= GLOVE_MAX_VERTEX_ATTRIBS) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!memcmp((void *)name, (void *)"gl_", 3)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    progPtr->SetCustomAttribsLayout(name, index);
}

GLuint
Context::CreateProgram(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    GLuint         res     = mResourceManager.AllocateShaderProgram();
    ShaderProgram *progPtr = mResourceManager.GetShaderProgram(res);
    progPtr->SetVkContext(mVkContext);
    progPtr->SetGlContext(this);
    progPtr->SetShaderCompiler(mShaderCompiler);

    return mResourceManager.PushShadingObject((ShadingNamespace_t){SHADER_PROGRAM_ID, res});
}

void
Context::DeleteProgram(GLuint program)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!program) {
        return;
    }

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }

    if(progPtr != mStateManager.GetActiveShaderProgram()) {
        progPtr->DetachAndDeleteShaders();
        mResourceManager.EraseShadingObject(program);
        mResourceManager.DeallocateShaderProgram(progPtr);
    } else {
        progPtr->MarkForDeletion();
    }
}

void
Context::DetachShader(GLuint program, GLuint shader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    Shader *shaderPtr = GetShaderPtr(shader);
    if(!shaderPtr) {
        return;
    }

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }

    if(!progPtr->IsShaderAttached(shaderPtr)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    progPtr->DetachShader(shaderPtr);

    if(!shaderPtr->GetRefCount() && shaderPtr->GetMarkForDeletion()) {
        mResourceManager.EraseShadingObject(shader);
        mResourceManager.DeallocateShader(shaderPtr);
    }
}

void
Context::GetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(maxcount < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }

    int shaderCount = progPtr->HasVertexShader() + progPtr->HasFragmentShader();
    if(!shaderCount) {
        if(count) *count = 0;
        return;
    }

    if(maxcount == 1 && shaderCount) {
        shaders[0] = GetShaderId(progPtr->GetVertexShader() ? progPtr->GetVertexShader() : progPtr->GetFragmentShader());
        if(count) *count = 1;
    } else if (maxcount >= 2 && shaderCount == 1) {
        shaders[0] = GetShaderId(progPtr->GetVertexShader() ? progPtr->GetVertexShader() : progPtr->GetFragmentShader());
        if(count) *count = 1;
    } else if (maxcount >= 2 && shaderCount == 2) {
        shaders[0] = GetShaderId(progPtr->GetVertexShader());
        shaders[1] = GetShaderId(progPtr->GetFragmentShader());
        if(count) *count = 2;
    } else {
        NOT_REACHED();
    }
}

int
Context::GetAttribLocation(GLuint program, const char* name)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return -1;
    }

    if(progPtr->IsLinked() == false) {
        RecordError(GL_INVALID_OPERATION);
        return -1;
    }

    return progPtr->GetAttributeLocation(name);
}

void
Context::GetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(bufsize < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }

    if(!progPtr->IsLinked() || index >= (GLuint)progPtr->GetNumberOfActiveAttributes()) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    const ShaderResourceInterface::attribute *attribute = progPtr->GetVertexAttribute(index);
    GLint len = std::max(std::min((int)attribute->name.length(), bufsize - 1), 0);
    if(length) {
        *length = len;
    }

    if(len) {
        memcpy((void *)name, (void *)attribute->name.c_str(), len);
        name[len] = '\0';
    }

    if(type) {
        *type = attribute->glType;
    }

    if(size) {
        *size = 1;
    }
}

void
Context::GetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(index >= progPtr->GetNumberOfActiveUniforms() || bufsize < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    const ShaderResourceInterface::uniform *uniform = progPtr->GetUniform((uint32_t)index);
    assert(uniform);
    GLint len = static_cast<GLint>(std::max(std::min((int)uniform->reflectionName.length(), bufsize-1), 0));
    if(length) {
        *length = len;
    }

    if(len) {
        memcpy((void *)name, (void *)uniform->reflectionName.c_str(), len);
        name[len] = '\0';
    }

    if(type) {
        *type = uniform->glType;
    }

    if(size) {
        *size = uniform->arraySize > 1 ? uniform->arraySize : 1;
    }
}

void
Context::GetProgramBinaryOES(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    (void)binaryFormat;

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }

    progPtr->GetBinaryData(binary, length);

    if(bufSize < *length) {
        *length = 0;
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    return;
}

void
Context::GetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(bufsize < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }

    char *log = progPtr->GetInfoLog();
    if(log) {
        int len = progPtr->GetInfoLogLength();
        int returnedLen = std::max(std::min(bufsize, len) - 1, 0);

        if(length) {
            *length = returnedLen;
        }

        if(returnedLen) {
            memcpy((void *)infolog, (void *)log, returnedLen);
            infolog[returnedLen - 1] = '\0';
        }

        delete[] log;
    } else {
        if(length) {
            *length = 0;
        }
    }
}

ShaderProgram *
Context::GetProgramPtr(GLuint program)
{
    FUN_ENTRY(GL_LOG_TRACE);

    if(!program || program >= mResourceManager.GetShadingObjectCount() || !mResourceManager.ShadingObjectExists(program)) {
        RecordError(GL_INVALID_VALUE);
        return nullptr;
    }

    ShadingNamespace_t progId = mResourceManager.GetShadingObject(program);
    if(!progId.arrayIndex || progId.type != SHADER_PROGRAM_ID) {
        RecordError(GL_INVALID_OPERATION);
        return nullptr;
    }

    return mResourceManager.GetShaderProgram(progId.arrayIndex);
}

void
Context::GetProgramiv(GLuint program, GLenum pname, GLint* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(pname != GL_DELETE_STATUS && pname != GL_LINK_STATUS && pname != GL_VALIDATE_STATUS &&
       pname != GL_INFO_LOG_LENGTH && pname != GL_ATTACHED_SHADERS && pname != GL_ACTIVE_ATTRIBUTES &&
       pname != GL_ACTIVE_ATTRIBUTE_MAX_LENGTH && pname != GL_ACTIVE_UNIFORMS &&
       pname != GL_ACTIVE_UNIFORM_MAX_LENGTH && pname != GL_PROGRAM_BINARY_LENGTH_OES) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!IsProgram(program)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    switch(pname) {
    case GL_DELETE_STATUS:               *params = progPtr->GetMarkForDeletion() ? GL_TRUE : GL_FALSE; break;
    case GL_LINK_STATUS:                 *params = progPtr->IsLinked() ? GL_TRUE : GL_FALSE; break;
    case GL_VALIDATE_STATUS:             *params = progPtr->IsValidated() ? GL_TRUE : GL_FALSE; break;
    case GL_INFO_LOG_LENGTH:             *params = progPtr->GetInfoLogLength(); break;
    case GL_ATTACHED_SHADERS:            *params = (bool)progPtr->GetVertexShader() + (bool)progPtr->GetFragmentShader(); break;
    case GL_ACTIVE_ATTRIBUTES:           *params = progPtr->GetNumberOfActiveAttributes(); break;
    case GL_ACTIVE_ATTRIBUTE_MAX_LENGTH: *params = progPtr->GetActiveAttribMaxLen(); break;
    case GL_ACTIVE_UNIFORMS:             *params = progPtr->GetNumberOfActiveUniforms(); break;
    case GL_ACTIVE_UNIFORM_MAX_LENGTH:   *params = progPtr->GetActiveUniformMaxLen(); break;
    case GL_PROGRAM_BINARY_LENGTH_OES:   *params = progPtr->GetBinaryLength(); break;
    default:                             RecordError(GL_INVALID_ENUM); return; break;
    }
}

void
Context::GetUniformiv(GLuint program, GLint location, GLint *params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!progPtr->IsLinked() || location < 0) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    const ShaderResourceInterface::uniform *uniform = progPtr->GetUniformAtLocation(location);
    if(!uniform) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    size_t size = GlslTypeToSize(uniform->glType);
    progPtr->GetUniformData(location, size, (void *)params);
}

void
Context::GetUniformfv(GLuint program, GLint location, GLfloat *params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!progPtr->IsLinked() || location < 0) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    const ShaderResourceInterface::uniform *uniform = progPtr->GetUniformAtLocation(location);
    if(!uniform) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    size_t size = GlslTypeToSize(uniform->glType);
    switch(uniform->glType) {
    case GL_FLOAT:
    case GL_FLOAT_VEC2:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_VEC4:
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT4:
        progPtr->GetUniformData(location, size, (void *)params);
        break;

    case GL_BOOL: {
        glsl_bool_t bData;
        progPtr->GetUniformData(location, size, (void *)&bData);
        params[0] = (float)bData;
        break;
    }

    case GL_BOOL_VEC2: {
        glsl_bool_t bData[2];
        progPtr->GetUniformData(location, size, (void *)bData);
        params[0] = bData[0];
        params[1] = bData[1];
        break;
    }
    case GL_BOOL_VEC3: {
        glsl_bool_t bData[3];
        progPtr->GetUniformData(location, size, (void *)bData);
        params[0] = bData[0];
        params[1] = bData[1];
        params[2] = bData[2];
        break;
    }
    case GL_BOOL_VEC4: {
        glsl_bool_t bData[4];
        progPtr->GetUniformData(location, size, (void *)bData);
        params[0] = bData[0];
        params[1] = bData[1];
        params[2] = bData[2];
        params[3] = bData[3];
        break;
    }

    case GL_INT: {
        glsl_int_t iData;
        progPtr->GetUniformData(location, size, (void *)&iData);
        params[0] = (float)iData;
        break;
    }

    case GL_INT_VEC2: {
        glsl_int_t iData[2];
        progPtr->GetUniformData(location, size, (void *)iData);
        params[0] = iData[0];
        params[1] = iData[1];
        break;
    }
    case GL_INT_VEC3: {
        glsl_int_t iData[3];
        progPtr->GetUniformData(location, size, (void *)iData);
        params[0] = iData[0];
        params[1] = iData[1];
        params[2] = iData[2];
        break;
    }
    case GL_INT_VEC4: {
        glsl_int_t iData[4];
        progPtr->GetUniformData(location, size, (void *)iData);
        params[0] = iData[0];
        params[1] = iData[1];
        params[2] = iData[2];
        params[3] = iData[3];
        break;
    }

    default:
        RecordError(GL_INVALID_OPERATION);
        break;
    }
}

int
Context::GetUniformLocation(GLuint program, const char *name)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!memcmp((void *)name, (void *)"gl_", 3)) {
        return -1;
    }

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return -1;
    }

    if(progPtr->IsLinked() == false) {
        RecordError(GL_INVALID_OPERATION);
        return -1;
    }

    return progPtr->GetUniformLocation(name);
}

GLboolean
Context::IsProgram(GLuint program)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return mResourceManager.IsShadingObject(program, SHADER_PROGRAM_ID);
}

bool
Context::SetPipelineProgramShaderStages(ShaderProgram *progPtr)
{
    FUN_ENTRY(GL_LOG_TRACE);

    if(!progPtr->HasStages() || progPtr->HasStagesUpdated(mPipeline->GetShaderStageIDs())) {

        if(!progPtr->SetPipelineShaderStage(mPipeline->GetShaderStageCount(),
                                            mPipeline->GetShaderStageIDs(),
                                            mPipeline->GetShaderStages())) {
            return false;
        }

        mPipeline->SetCache(progPtr->GetVkPipelineCache());
        mPipeline->SetLayout(progPtr->GetVkPipelineLayout());
        mPipeline->SetVertexInputState(progPtr->GetVkPipelineVertexInput());
    }

    return true;
}

void
Context::LinkProgram(GLuint program)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }
    progPtr->LinkProgram();
    progPtr->SetShaderModules();

    mPipeline->SetUpdatePipeline(progPtr->IsLinked());
    if(SetPipelineProgramShaderStages(progPtr)) {
        mPipeline->Create(mWriteFBO->GetVkRenderPass());
    }
}

void
Context::ProgramBinaryOES(GLuint program, GLenum binaryFormat, const void *binary, GLint length)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(   GLOVE_HOST_X86_BINARY != binaryFormat
       && GLOVE_HOST_ARM_BINARY != binaryFormat
       && GLOVE_DEV_BINARY != binaryFormat) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    ShaderProgram *progPtr = GetProgramPtr(program);
    if(!progPtr) {
        return;
    }

    GLuint vs = CreateShader(GL_VERTEX_SHADER);
    GLuint fs = CreateShader(GL_FRAGMENT_SHADER);
    AttachShader(program, vs);
    AttachShader(program, fs);

    progPtr->UsePrecompiledBinary(binary, length);
    progPtr->SetShaderModules();
}

void
Context::Uniform1f(GLint location, GLfloat x)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT && uniform->glType != GL_BOOL)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(uniform->glType == GL_FLOAT) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, sizeof(float), (void *)&x);
    } else {
        glsl_bool_t bf = (bool)x;
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, sizeof(glsl_bool_t), (void *)&bf);
    }
}

void
Context::Uniform1fv(GLint location, GLsizei count, const GLfloat *v)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT && uniform->glType != GL_BOOL) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    if(uniform->glType == GL_FLOAT) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, count * sizeof(float), (void *)v);
    } else {
        glsl_bool_t *bv = new glsl_bool_t[count];
        for(int i = 0; i < count; ++i) {
            bv[i] = (bool)v[i];
        }
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, count * sizeof(glsl_bool_t), (void *)bv);
        delete[] bv;
    }
}

void
Context::Uniform1i(GLint location, GLint x)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_INT        && uniform->glType != GL_BOOL &&
        uniform->glType != GL_SAMPLER_2D && uniform->glType != GL_SAMPLER_CUBE)
      ) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(uniform->glType == GL_SAMPLER_2D || uniform->glType == GL_SAMPLER_CUBE) {
        mStateManager.GetActiveShaderProgram()->SetSampler(location, 1, &x);
    } else if(uniform->glType == GL_INT) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, sizeof(int), &x);
    } else {
        glsl_bool_t bx = (bool)x;
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, sizeof(glsl_bool_t), (void *)&bx);
    }
}

void
Context::Uniform1iv(GLint location, GLsizei count, const GLint *v)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform ) ||
       (uniform->glType != GL_INT && uniform->glType != GL_BOOL &&
        uniform->glType != GL_SAMPLER_2D && uniform->glType != GL_SAMPLER_CUBE) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    if(uniform->glType == GL_SAMPLER_2D || uniform->glType == GL_SAMPLER_CUBE) {
        mStateManager.GetActiveShaderProgram()->SetSampler(location, count, v);
    } else if(uniform->glType == GL_INT) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, count * sizeof(int), (void *)v);
    } else {
        glsl_bool_t *bv = new glsl_bool_t[count];
        for(int i = 0; i < count; ++i) {
            bv[i] = (bool)v[i];
        }
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, count * sizeof(glsl_bool_t), (void *)bv);
        delete[] bv;
    }
}

void
Context::Uniform2f(GLint location, GLfloat x, GLfloat y)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT_VEC2 && uniform->glType != GL_BOOL_VEC2)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(uniform->glType == GL_FLOAT_VEC2) {
        float v[2] = {x, y};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 2 * sizeof(float), (void *)v);
    } else {
        glsl_bool_t bv[2] = {(bool)x, (bool)y};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 2 * sizeof(glsl_bool_t), (void *)bv);
    }
}

void
Context::Uniform2fv(GLint location, GLsizei count, const GLfloat *v)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT_VEC2 && uniform->glType != GL_BOOL_VEC2) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    if(uniform->glType == GL_FLOAT_VEC2) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 2 * count * sizeof(float), (void *)v);
    } else {
        glsl_bool_t *bv = new glsl_bool_t[2 * count];
        for(int i = 0; i < 2 * count; ++i) {
            bv[i] = (bool)v[i];
        }
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 2 * count * sizeof(glsl_bool_t), (void *)bv);
        delete[] bv;
    }
}

void
Context::Uniform2i(GLint location, GLint x, GLint y)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
        (uniform->glType != GL_INT_VEC2 && uniform->glType != GL_BOOL_VEC2)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(uniform->glType == GL_INT_VEC2) {
        int v[2] = {x, y};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 2 * sizeof(int), (void *)v);
    } else {
        glsl_bool_t bv[2] = {(bool)x, (bool)y};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 2 * sizeof(glsl_bool_t), (void *)bv);
    }
}

void
Context::Uniform2iv(GLint location, GLsizei count, const GLint *v)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_INT_VEC2 && uniform->glType != GL_BOOL_VEC2) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    if(uniform->glType == GL_INT_VEC2) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 2 * count * sizeof(int), (void *)v);
    } else if(uniform->glType == GL_BOOL_VEC2) {
        glsl_bool_t *bv = new glsl_bool_t[2 * count];
        for(int i = 0; i < 2 * count; ++i) {
            bv[i] = (bool)v[i];
        }
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 2 * count * sizeof(glsl_bool_t), (void *)bv);
        delete[] bv;
    }
}

void
Context::Uniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT_VEC3 && uniform->glType != GL_BOOL_VEC3)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(uniform->glType == GL_FLOAT_VEC3) {
        float v[3] = {x, y, z};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 3 * sizeof(float), (void *)v);
    } else {
        glsl_bool_t bv[3] = {(bool)x, (bool)y, (bool)z};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 3 * sizeof(glsl_bool_t), (void *)bv);
    }
}

void
Context::Uniform3fv(GLint location, GLsizei count, const GLfloat *v)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT_VEC3 && uniform->glType != GL_BOOL_VEC3) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    if(uniform->glType == GL_FLOAT_VEC3) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 3 * count * sizeof(float), (void *)v);
    } else {
        glsl_bool_t *bv = new glsl_bool_t[3 * count];
        for(int i = 0; i < 3 * count; ++i) {
            bv[i] = (bool)v[i];
        }
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 3 * count * sizeof(glsl_bool_t), (void *)bv);
        delete[] bv;
    }
}

void
Context::Uniform3i(GLint location, GLint x, GLint y, GLint z)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_INT_VEC3 && uniform->glType != GL_BOOL_VEC3)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(uniform->glType == GL_INT_VEC3) {
        int v[3] = {x, y, z};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 3 * sizeof(int), (void *)v);
    } else {
        glsl_bool_t bv[3] = {(bool)x, (bool)y, (bool)z};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 3 * sizeof(glsl_bool_t), (void *)bv);
    }
}

void
Context::Uniform3iv(GLint location, GLsizei count, const GLint *v)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_INT_VEC3 && uniform->glType != GL_BOOL_VEC3) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    if(uniform->glType == GL_INT_VEC3) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 3 * count * sizeof(int), (void *)v);
    } else {
        glsl_bool_t *bv = new glsl_bool_t[3 * count];
        for(int i = 0; i < 3 * count; ++i) {
            bv[i] = (bool)v[i];
        }
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 3 * count * sizeof(glsl_bool_t), (void *)bv);
        delete[] bv;
    }
}

void
Context::Uniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT_VEC4 && uniform->glType != GL_BOOL_VEC4)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(uniform->glType == GL_FLOAT_VEC4) {
        float v[4] = {x, y, z, w};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 4 * sizeof(float), (void *)v);
    } else {
        glsl_bool_t bv[4] = {(bool)x, (bool)y, (bool)z, (bool)w};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 4 * sizeof(glsl_bool_t), (void *)bv);
    }
}

void
Context::Uniform4fv(GLint location, GLsizei count, const GLfloat *v)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT_VEC4 && uniform->glType != GL_BOOL_VEC4) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    if(uniform->glType == GL_FLOAT_VEC4) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 4 * count * sizeof(float), (void *)v);
    } else {
        glsl_bool_t *bv = new glsl_bool_t[4 * count];
        for(int i = 0; i < 4 * count; ++i) {
            bv[i] = (bool)v[i];
        }
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 4 * count * sizeof(glsl_bool_t), (void *)bv);
        delete[] bv;
    }
}

void
Context::Uniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_INT_VEC4 && uniform->glType != GL_BOOL_VEC4)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(uniform->glType == GL_INT_VEC4) {
        int v[4] = {x, y, z, w};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 4 * sizeof(int), (void *)v);
    } else {
        glsl_bool_t bv[4] = {(bool)x, (bool)y, (bool)z, (bool)w};
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 4 * sizeof(glsl_bool_t), (void *)bv);
    }
}

void
Context::Uniform4iv(GLint location, GLsizei count, const GLint *v)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_INT_VEC4 && uniform->glType != GL_BOOL_VEC4) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    if(uniform->glType == GL_INT_VEC4) {
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 4 * count * sizeof(int), (void *)v);
    } else {
        glsl_bool_t *bv = new glsl_bool_t[4 * count];
        for(int i = 0; i < 4 * count; ++i) {
            bv[i] = (bool)v[i];
        }
        mStateManager.GetActiveShaderProgram()->SetUniformData(location, 4 * count * sizeof(glsl_bool_t), (void *)bv);
        delete[] bv;
    }
}

void
Context::UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(transpose != GL_FALSE) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT_MAT2) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    mStateManager.GetActiveShaderProgram()->SetUniformData(location, 2 * 2 * count * sizeof(float), (void *)value);
}

void
Context::UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(transpose != GL_FALSE) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT_MAT3) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    mStateManager.GetActiveShaderProgram()->SetUniformData(location, 3 * 3 * count * sizeof(float), (void *)value);

}

void
Context::UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(count < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(transpose != GL_FALSE) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(!mStateManager.GetActiveShaderProgram()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(location == -1) {
        return;
    }

    const ShaderResourceInterface::uniform *uniform = mStateManager.GetActiveShaderProgram()->GetUniformAtLocation(location);
    if((!uniform) ||
       (uniform->glType != GL_FLOAT_MAT4) ||
       (uniform->arraySize == 1 && count > 1)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(count > uniform->arraySize) {
        assert((GLint)uniform->location <= location);
        count = uniform->arraySize - (location - (GLint)uniform->location);
    }

    mStateManager.GetActiveShaderProgram()->SetUniformData(location, 4 * 4 * count * sizeof(float), (void *)value);
}

void
Context::UseProgram(GLuint program)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderProgram *progPtr = nullptr;

    if(program) {
        progPtr = GetProgramPtr(program);
        if(!progPtr) {
            return;
        }

        if(mStateManager.GetActiveShaderProgram() == progPtr) {
            return;
        }

        if(!progPtr->IsLinked()) {
            RecordError(GL_INVALID_OPERATION);
            return;
        }
    }

    if(mStateManager.GetActiveShaderProgram() && mStateManager.GetActiveShaderProgram()->GetMarkForDeletion()) {
        mStateManager.GetActiveShaderProgram()->DetachAndDeleteShaders();
        mResourceManager.EraseShadingObject(GetProgramId(mStateManager.GetActiveShaderProgram()));
        mResourceManager.DeallocateShaderProgram(mStateManager.GetActiveShaderProgram());
    }

    mStateManager.GetActiveObjectsState()->SetActiveShaderProgram(progPtr);

    mPipeline->SetUpdatePipeline(true);
}

void
Context::ValidateProgram(GLuint program)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ShaderProgram* progPtr = GetProgramPtr(program);

    if(!progPtr) {
        return;
    } else {
        if(!IsProgram(program)){
            RecordError(GL_INVALID_OPERATION);
            return;
        }
    }

    progPtr->Validate();

    if(!progPtr->IsValidated()){
        //TODO: INFO LOG HERE:
    }
}
