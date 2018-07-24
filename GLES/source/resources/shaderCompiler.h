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
 *  @file       shaderCompiler.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shader Compiler Definition
 *
 */

#ifndef __SHADERCOMPILER_H__
#define __SHADERCOMPILER_H__

#include "shaderReflection.h"

class ShaderProgram;

class ShaderCompiler {

public:
    ShaderCompiler() {}
    virtual ~ShaderCompiler() {}

    virtual bool CompileVertexShader(const char* const* source) = 0;
    virtual bool CompileFragmentShader(const char* const* source) = 0;
    virtual const char* GetProgramInfoLog(void) = 0;
    virtual const char* GetShaderInfoLog(shader_type_t shaderType) = 0;
    virtual bool LinkProgram(ShaderProgram& shaderProgram) = 0;
    virtual void PrepareReflection(void) = 0;
    virtual uint32_t SerializeReflection(void* binary) = 0;
    virtual uint32_t DeserializeReflection(const void* binary) = 0;
    virtual bool ValidateProgram(void) = 0;
    virtual void DumpUniforms(void) = 0;
    virtual ShaderReflection* GetShaderReflection(void) = 0;
    virtual void EnableDumpInputShaderReflection(void) = 0;
    virtual void EnableDumpVulkanShaderReflection(void) = 0;
    virtual void EnableDumpProcessedShaderSource(void) = 0;
    virtual void EnableSaveBinaryToFiles(void) = 0;
    virtual void EnableSaveSourceToFiles(void) = 0;
    virtual void EnableSaveSpvTextToFile(void) = 0;
};

#endif // __SHADERCOMPILER_H__
