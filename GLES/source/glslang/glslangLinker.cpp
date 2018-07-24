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
 *  @file       glslangLinker.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      ESSL Shaders linker to a GL program, based on glslang TProgram
 *
 */

#include "glslangLinker.h"
#include "utils/glLogger.h"

GlslangLinker::GlslangLinker()
: mSlangProgram(nullptr), mSlangProgram400(nullptr)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

GlslangLinker::~GlslangLinker()
{
    FUN_ENTRY(GL_LOG_TRACE);

    CleanUp();
}

void
GlslangLinker::CleanUp()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mSlangProgram) {
        delete mSlangProgram;
        mSlangProgram = nullptr;
    }

    if(mSlangProgram400) {
        delete mSlangProgram400;
        mSlangProgram400 = nullptr;
    }
}

void
GlslangLinker::GenerateSPV(std::vector<unsigned int>& vertSPV, std::vector<unsigned int>& fragSPV)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    glslang::GlslangToSpv(*mSlangProgram400->getIntermediate(EShLangVertex), vertSPV);
    glslang::GlslangToSpv(*mSlangProgram400->getIntermediate(EShLangFragment), fragSPV);
}

const char*
GlslangLinker::GetInfoLog()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return mSlangProgram->getInfoLog();
}

bool
GlslangLinker::LinkProgram(glslang::TShader* slangVertShader, glslang::TShader* slangFragShader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    bool result;

    mSlangProgram400 = new glslang::TProgram();
    assert(mSlangProgram400);

    mSlangProgram400->addShader(slangVertShader);
    mSlangProgram400->addShader(slangFragShader);

    result = mSlangProgram400->link(EShMsgDefault);
    if(!result) {
        printf("%s\n", mSlangProgram400->getInfoLog());
        return result;
    }

    return mSlangProgram400->buildReflection();
}

bool
GlslangLinker::ValidateProgram(glslang::TShader* slangVertShader, glslang::TShader* slangFragShader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    bool result;

    mSlangProgram = new glslang::TProgram();
    assert(mSlangProgram);

    mSlangProgram->addShader(slangVertShader);
    mSlangProgram->addShader(slangFragShader);

    result = mSlangProgram->link(EShMsgDefault);
    if(!result) {
        return result;
    }

    result = mSlangProgram->buildReflection();

    mIoMapResolver.Reset();
    mSlangProgram->mapIO(&mIoMapResolver);

    return result;
}
