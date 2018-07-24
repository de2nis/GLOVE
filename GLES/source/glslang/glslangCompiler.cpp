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
 *  @file       glslangCompiler.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      ESSL code compiler based on glslang TShader
 *
 */

#include "glslangCompiler.h"
#include "utils/glLogger.h"

GlslangCompiler::GlslangCompiler()
: mSlangShader(nullptr), mSlangShader400(nullptr)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

GlslangCompiler::~GlslangCompiler()
{
    FUN_ENTRY(GL_LOG_TRACE);

    CleanUpShader(mSlangShader);
    CleanUpShader(mSlangShader400);
}

void
GlslangCompiler::CleanUpShader(glslang::TShader* shader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(shader) {
        delete shader;
        shader = nullptr;
    }
}

bool
GlslangCompiler::CompileShader(const char* const* source, TBuiltInResource* resources, EShLanguage language)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    CleanUpShader(mSlangShader);

    mSlangShader = new glslang::TShader(language);
    assert(mSlangShader);

    mSlangShader->setStrings(source, 1);

    bool result = mSlangShader->parse(resources, 100, ENoProfile, false, false, EShMsgDefault);
    if(!result) {
        printf("%s\n", mSlangShader->getInfoLog());
    }

    return result;
}

bool
GlslangCompiler::CompileShader400(const char* const* source, TBuiltInResource* resources, EShLanguage language)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    CleanUpShader(mSlangShader400);

    mSlangShader400 = new glslang::TShader(language);
    assert(mSlangShader400);

    mSlangShader400->setStrings(source, 1);
    bool result = mSlangShader400->parse(resources, 400, EEsProfile, false, false, (EShMessages)(EShMsgVulkanRules | EShMsgSpvRules));
    if(!result) {
        printf("%s\n", mSlangShader400->getInfoLog());
    }

    return result;
}

const char*
GlslangCompiler::GetInfoLog()
{
    FUN_ENTRY(GL_LOG_TRACE);

    return mSlangShader->getInfoLog();
}
