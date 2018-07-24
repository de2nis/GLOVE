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
 *  @file       glslangCompiler.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      ESSL code compiler based on glslang TShader
 *
 */

#ifndef __GLSLANGCOMPILER_H__
#define __GLSLANGCOMPILER_H__

#include "utils/glLogger.h"
#include "utils/parser_helpers.h"
#include "glslang/Include/ShHandle.h"
#include "glslang/Public/ShaderLang.h"
#include "SPIRV/GlslangToSpv.h"
#include "SPIRV/doc.h"
#include "SPIRV/disassemble.h"

class GlslangCompiler {
private:
    glslang::TShader* mSlangShader;
    glslang::TShader* mSlangShader400;

    void CleanUpShader(glslang::TShader* shader);

public:
    GlslangCompiler();
    ~GlslangCompiler();

    bool CompileShader(const char* const* source, TBuiltInResource* resources, EShLanguage language);
    bool CompileShader400(const char* const* source, TBuiltInResource* resources, EShLanguage language);
    const char* GetInfoLog();

    glslang::TShader* GetSlangShader()                                   const  { FUN_ENTRY(GL_LOG_TRACE); return mSlangShader; }
    glslang::TShader* GetSlangShader400()                                const  { FUN_ENTRY(GL_LOG_TRACE); return mSlangShader400; }
};

#endif // __GLSLANGCOMPILER_H__
