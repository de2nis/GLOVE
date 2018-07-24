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
 *  @file       glslangLinker.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      ESSL Shaders linker to a GL program, based on glslang TProgram
 *
 */

#ifndef __GLSLANGLINKER_H__
#define __GLSLANGLINKER_H__

#include "utils/globals.h"

#include "glslang/Include/ShHandle.h"
#include "glslang/Public/ShaderLang.h"
#include "SPIRV/GlslangToSpv.h"
#include "SPIRV/doc.h"
#include "SPIRV/disassemble.h"
#include "resources/shaderProgram.h"
#include "glslangIoMapResolver.h"

class GlslangLinker {
private:
    glslang::TProgram* mSlangProgram;
    glslang::TProgram* mSlangProgram400;
    GlslangIoMapResolver mIoMapResolver;

    void CleanUp();

public:
    GlslangLinker();
    ~GlslangLinker();

    void GenerateSPV(std::vector<unsigned int>& vertSPV, std::vector<unsigned int>& fragSPV);
    const char* GetInfoLog();
    bool LinkProgram(glslang::TShader* vertShader, glslang::TShader* fragShader);
    bool ValidateProgram(glslang::TShader* vertShader, glslang::TShader* fragShader);

    inline glslang::TProgram* GetSlangProgram()                           const { FUN_ENTRY(GL_LOG_TRACE); return mSlangProgram; }
    inline glslang::TProgram* GetSlangProgram400()                        const { FUN_ENTRY(GL_LOG_TRACE); return mSlangProgram400; }
    inline const GlslangIoMapResolver* GetIoMapResolver()                 const { FUN_ENTRY(GL_LOG_TRACE); return &mIoMapResolver; }
};

#endif // __GLSLANGLINKER_H__
