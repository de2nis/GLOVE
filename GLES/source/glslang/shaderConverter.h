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
 *  @file       shaderConverter.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      ESSL code converter from an ESSL version to another. Currently supports ESSL 100 to ESSL 400.
 *
 */

#ifndef __SHADER_CONVERTER_H__
#define __SHADER_CONVERTER_H__

#include "resources/shaderReflection.h"
#include "glslang/glslang_utils.h"
#include "glslang/Include/ShHandle.h"
#include "glslangIoMapResolver.h"

class ShaderConverter {
public:
    typedef enum {
        INVALID_SHADER_CONVERSION,
        SHADER_CONVERSION_100_400
    } shader_conversion_type_t;

    ShaderConverter();
    ~ShaderConverter();

    void Convert(string& source, const uniformBlockMap_t &uniformBlockMap, ShaderReflection* reflection);
    void Initialize(shader_conversion_type_t conversionType, shader_type_t shaderType);

    void SetSlangProgram(glslang::TProgram* slangProgram)                       { FUN_ENTRY(GL_LOG_TRACE); mSlangProg = slangProgram; }
    void SetIoMapResolver(const GlslangIoMapResolver * ioMapResolver)           { FUN_ENTRY(GL_LOG_TRACE); mIoMapResolver = ioMapResolver; }

private:
    static const char * const   shaderVersion;
    static const char * const   shaderExtensions;
    static const char * const   shaderPrecision;
    static const char * const   shaderTexture2d;
    static const char * const   shaderTextureCube;
    static const char * const   shaderDepthRange;
    static const char * const   shaderLimitsBuiltIns;

    shader_conversion_type_t    mConversionType;
    shader_type_t               mShaderType;
    string                      mMemLayoutQualifier;
    glslang::TProgram*          mSlangProg;
    const GlslangIoMapResolver *mIoMapResolver;

    void Convert100To400(string& source,const uniformBlockMap_t &uniformBlockMap, ShaderReflection* reflection);
    void ProcessHeader(string& source, const uniformBlockMap_t &uniformBlockMap);
    void ProcessUniforms(string& source, const uniformBlockMap_t &uniformBlockMap);
    void ProcessVaryings(string& source);
    void ProcessVertexAttributes(string& source, ShaderReflection* reflection);
    void ConvertGLToVulkanDepthRange(string& source);

};

#endif // __SHADER_CONVERTER_H__
