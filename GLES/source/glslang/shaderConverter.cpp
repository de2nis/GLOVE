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
 *  @file       shaderConverter.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      ESSL code converter from an ESSL version to another. Currently supports ESSL 100 to ESSL 400.
 *
 */

#include "shaderConverter.h"
#include "resources/shaderProgram.h"
#include "utils/glLogger.h"

const char * const ShaderConverter::shaderVersion    = "#version 400\n";
const char * const ShaderConverter::shaderExtensions = "#extension GL_ARB_shading_language_420pack : enable\n"
                                                       "#extension GL_ARB_separate_shader_objects : enable\n"
                                                       "#extension GL_OES_EGL_image_external : enable\n"
                                                       "\n";

const char * const ShaderConverter::shaderPrecision  = "#ifdef GL_ES\n"
                                                       "    precision highp float;\n"
                                                       "#endif\n"
                                                       "\n";

const char * const ShaderConverter::shaderTexture2d  = "/// GL_KHR_vulkan_glsl removed texture2D(), texture2Dproj()\n"
                                                       "#define texture2D texture\n"
                                                       "#define texture2Dproj texture\n"
                                                       "#define samplerExternalOES sampler2D\n"
                                                       "\n";

const char * const ShaderConverter::shaderTextureCube= "/// GL_KHR_vulkan_glsl removed textureCube()\n"
                                                      "#define textureCube texture\n"
                                                      "\n";

const char * const ShaderConverter::shaderDepthRange = "/// GL_KHR_vulkan_glsl removed gl_DepthRange as well\n"
                                                       "struct gl_DepthRangeParameters {\n"
                                                       "    float near;\n"
                                                       "    float far;\n"
                                                       "    float diff;\n"
                                                       "\n"
                                                       "    ///std140 will force this struct to 16 bytes anyway. Pad it for peace of mind\n"
                                                       "    float pad;\n"
                                                       "};\n"
                                                       "uniform gl_DepthRangeParameters " STRINGIFY_MACRO(VULKAN_DEPTH_RANGE) ";\n"
                                                       "\n"
                                                       "#define gl_DepthRange " STRINGIFY_MACRO(VULKAN_DEPTH_RANGE) "\n"
                                                       "\n";

const char * const ShaderConverter::shaderLimitsBuiltIns = "#define gl_MaxVertexAttribs "              STRINGIFY_MACRO(GLOVE_MAX_VERTEX_ATTRIBS) "\n"
                                                           "#define gl_MaxVertexUniformVectors "       STRINGIFY_MACRO(GLOVE_MAX_VERTEX_UNIFORM_VECTORS) "\n"
                                                           "#define gl_MaxVaryingVectors "             STRINGIFY_MACRO(GLOVE_MAX_VARYING_VECTORS) "\n"
                                                           "#define gl_MaxVertexTextureImageUnits "    STRINGIFY_MACRO(GLOVE_MAX_VERTEX_TEXTURE_IMAGE_UNITS) "\n"
                                                           "#define gl_MaxCombinedTextureImageUnits "  STRINGIFY_MACRO(GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS) "\n"
                                                           "#define gl_MaxTextureImageUnits "          STRINGIFY_MACRO(GLOVE_MAX_TEXTURE_IMAGE_UNITS) "\n"
                                                           "#define gl_MaxFragmentUniformVectors "     STRINGIFY_MACRO(GLOVE_MAX_FRAGMENT_UNIFORM_VECTORS) "\n"
                                                           "#define gl_MaxDrawBuffers "                STRINGIFY_MACRO(GLOVE_MAX_DRAW_BUFFERS) "\n"
                                                           "\n";

ShaderConverter::ShaderConverter()
: mConversionType(INVALID_SHADER_CONVERSION),
  mShaderType(INVALID_SHADER),
  mMemLayoutQualifier("std140")
{
    FUN_ENTRY(GL_LOG_TRACE);
}

ShaderConverter::~ShaderConverter()
{
    FUN_ENTRY(GL_LOG_TRACE);
}

void
ShaderConverter::Convert(std::string& source, const uniformBlockMap_t &uniformBlockMap, ShaderReflection* reflection)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    switch(mConversionType) {
        case SHADER_CONVERSION_100_400 : Convert100To400(source, uniformBlockMap, reflection); break;
        case INVALID_SHADER_CONVERSION : NOT_REACHED(); break;
        default: break;
    }
}

void
ShaderConverter::Convert100To400(std::string& source, const uniformBlockMap_t &uniformBlockMap, ShaderReflection* reflection)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    ProcessHeader(source, uniformBlockMap);
    ProcessUniforms(source, uniformBlockMap);
    ProcessVaryings(source);
    ProcessVertexAttributes(source, reflection);

    if(mShaderType == SHADER_TYPE_VERTEX) {
        ConvertGLToVulkanDepthRange(source);
    }
}

void
ShaderConverter::Initialize(shader_conversion_type_t conversionType, shader_type_t shaderType)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mConversionType = conversionType;
    mShaderType     = shaderType;
}

void
ShaderConverter::ProcessHeader(std::string& source, const uniformBlockMap_t &uniformBlockMap)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    /// Do not add vulkan_DepthRange declaration if gl_DepthRange is not active in the input shader
    const bool depthRangeActive = uniformBlockMap.find(string("gl_DepthRange")) != uniformBlockMap.cend();
    const string sourceHeader = string(shaderVersion) +
                                string(shaderExtensions) +
                                string(shaderPrecision) +
                                string(shaderTexture2d) +
                                string(shaderTextureCube) +
                                (depthRangeActive ? string(shaderDepthRange) : string("")) +
                                string(shaderLimitsBuiltIns);

    /// If #version is present
    size_t found = source.find("#version");
    if(found != string::npos) {
        size_t f1 = source.find("\n", found);
        source.replace(found, f1 - found, sourceHeader);
    } else {
        source.insert(0, sourceHeader);
    }
}

void
ShaderConverter::ProcessUniforms(std::string& source, const uniformBlockMap_t &uniformBlockMap)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    /// Start of dead uniform blocks where the active end
    uint32_t unusedBlockBindings = uniformBlockMap.size();
    uniformBlockMap_t::const_iterator uniBlockIt;
    string layoutSyntax;
    string blockSyntax;

    /// Convert uniforms into uniform blocks
    string token;
    const string uniformLiteralStr("uniform");

    size_t found = FindToken(uniformLiteralStr, source, 0);
    while(found != string::npos) {
        size_t f1 = found;
        found = SkipWhiteSpaces(source, found + uniformLiteralStr.length());

        /// Either type or precision qualifier
        token = GetNextToken(source, found);
        if(IsPrecisionQualifier(token)) {
            found = SkipWhiteSpaces(source, found + token.length());
            token = GetNextToken(source, found);
        }

        /// Definitely type now
        if(!CanTypeBeInUniformBlock(token)) {
            /// Sampler type
            found = SkipWhiteSpaces(source, found + token.length());
            /// Variable name
            token = GetNextToken(source, found);

            uniBlockIt = uniformBlockMap.find(token);
            if(uniBlockIt != uniformBlockMap.cend()) {
                layoutSyntax = "layout(binding = " + to_string(uniBlockIt->second.binding) + string(") ");
            } else {
                layoutSyntax = "layout(binding = " + to_string(unusedBlockBindings) + string(") ");
                ++unusedBlockBindings;
            }
            source.insert(f1, layoutSyntax);

            found = FindToken(uniformLiteralStr, source, found + token.length() + layoutSyntax.length());
            continue;
        }

        found = SkipWhiteSpaces(source, found + token.length());

        /// Variable name
        token = GetNextToken(source, found);

        if(!token.compare(STRINGIFY_MACRO(VULKAN_DEPTH_RANGE))) {
            token = std::string("gl_DepthRange");
        }

        /// Construct uniform block
        uniBlockIt = uniformBlockMap.find(token);
        if(uniBlockIt != uniformBlockMap.cend()) {
            const uniformBlock_t &block = uniBlockIt->second;
            layoutSyntax = "layout(" + mMemLayoutQualifier + ", binding = " + to_string(block.binding) + string(") ");
            source.insert(f1, layoutSyntax);
            f1 += layoutSyntax.length();

            blockSyntax = block.glslBlockName + string(" {");
            source.insert(f1 + strlen("uniform\0") + 1, blockSyntax);
        } else {
            /// inactive uniform
            layoutSyntax = "layout(" + mMemLayoutQualifier + ", binding = " + to_string(unusedBlockBindings) + string(") ");
            source.insert(f1, layoutSyntax);
            f1 += layoutSyntax.length();

            blockSyntax = string("uni") + to_string(unusedBlockBindings) + string(" {");
            source.insert(f1 + strlen("uniform\0") + 1, blockSyntax);
            ++unusedBlockBindings;
        }

        /// Close brackets and add block name
        found = source.find(";", found);
        blockSyntax = string("};");
        source.insert(found + 1, blockSyntax);

        found = FindToken(uniformLiteralStr, source, found);
    }
}

void
ShaderConverter::ProcessVaryings(std::string& source)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    /// Handle varyings
    assert(mSlangProg);

    int location = 0;
    map<string, int> varyingsLocationMap;
    for(uint32_t out = 0; out < mIoMapResolver->GetNumLiveVaryingOutVariables(); ++out) {
        for(uint32_t in = 0; in < mIoMapResolver->GetNumLiveVaryingInVariables(); ++in) {
            const char *inName = mIoMapResolver->GetVaryingInName(in);
            const char *outName = mIoMapResolver->GetVaryingOutName(out);
            if(!strcmp(inName, outName)) {
                assert(varyingsLocationMap.find(string(inName)) == varyingsLocationMap.end());
                varyingsLocationMap[string(inName)] = location;
                location++;
            }
        }
    }

    const string varyingLiteralStr("varying");

    size_t found = FindToken(varyingLiteralStr, source, 0);
    while(found != string::npos) {
        size_t f1 = found;
        found = SkipWhiteSpaces(source, found + varyingLiteralStr.length());

        /// Either type or precision qualifier
        string token = GetNextToken(source, found);
        if(IsPrecisionQualifier(token)) {
            found = SkipWhiteSpaces(source, found + token.length());
            token = GetNextToken(source, found);
        }

        /// Definitely type now
        found = SkipWhiteSpaces(source, found + token.length());

        /// Variable name
        token = GetNextToken(source, found);

        if(varyingsLocationMap.find(token) != varyingsLocationMap.end()) {
            string layoutSyntax = string("layout(location = ") +
                                  to_string(varyingsLocationMap[token]) +
                                  (mShaderType == SHADER_TYPE_VERTEX ? string(") out") : string(") in"));
            source.replace(f1, varyingLiteralStr.length(), layoutSyntax);
        } else {
            source.replace(f1, varyingLiteralStr.length(), "");
        }

        found = FindToken(varyingLiteralStr, source, found);
    }

}

void
ShaderConverter::ProcessVertexAttributes(std::string& source, ShaderReflection* reflection)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!reflection->GetLiveAttributes()) {
        return;
    }

    /// Handle vertex attributes
    const string attributeLiteralStr("attribute");

    size_t found = FindToken(attributeLiteralStr, source, 0);
    while(found != string::npos) {
        size_t f1 = found;
        found = SkipWhiteSpaces(source, found + attributeLiteralStr.length());

        /// Either type or precision qualifier
        string token = GetNextToken(source, found);
        if(IsPrecisionQualifier(token)) {
            found = SkipWhiteSpaces(source, found + token.length());
            token = GetNextToken(source, found);
        }

        /// Definitely type now
        found = SkipWhiteSpaces(source, found + token.length());
        /// Variable name
        token = GetNextToken(source, found);

        int location = reflection->GetAttributeLocation(token.c_str());
        if(location >=0) {
            string layoutSyntax = string("layout(location = ") + to_string(location) + string(") in");
            source.replace(f1, attributeLiteralStr.length(), layoutSyntax);
        } else {
            source.replace(f1, attributeLiteralStr.length(), "");
        }

        found = FindToken(attributeLiteralStr, source, found);
    }

}

// This adds overhead in Vertex Shader. Is there any better way to convert GL to Vulkan depth range?
void
ShaderConverter::ConvertGLToVulkanDepthRange(string& source)
{
    // Find last "}"
    size_t pos = source.rfind("}");

    string GlToVkDepthRangeConversion = string("    gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;\n");
    source.insert(pos, GlToVkDepthRangeConversion);
}
