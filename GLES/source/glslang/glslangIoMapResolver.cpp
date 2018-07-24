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
 *  @file       glslangIoMapResolver.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shader I/O gathering functionality based on glslang TIoMapResolver
 *
 */

#include "glslangIoMapResolver.h"
#include "utils/glLogger.h"

#include <algorithm>

void
GlslangIoMapResolver::FillInVaryingInfo(VaryingInfo *varyingInfo, const glslang::TType& type, const char *name)
{
    varyingInfo->name = name;
    varyingInfo->type = type.getBasicString();
    varyingInfo->hasLocation = type.getQualifier().hasLocation();
    varyingInfo->location = type.getQualifier().hasLocation() ? type.getQualifier().layoutLocation : -1;
}

bool
GlslangIoMapResolver::validateBinding(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{
    return true;
}

int
GlslangIoMapResolver::resolveBinding(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{
    return -1;
}


int
GlslangIoMapResolver::resolveSet(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{
    return -1;
}

int
GlslangIoMapResolver::resolveUniformLocation(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{
    return -1;
}

bool
GlslangIoMapResolver::validateInOut(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{
    return true;
}

int
GlslangIoMapResolver::resolveInOutLocation(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{
    return -1;
}

int
GlslangIoMapResolver::resolveInOutComponent(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{
    return -1;
}

int
GlslangIoMapResolver::resolveInOutIndex(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{
    return -1;
}

void
GlslangIoMapResolver::notifyBinding(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{

}

void
GlslangIoMapResolver::notifyInOut(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live)
{
    FUN_ENTRY(GL_LOG_TRACE);

    if(glslang::EvqVaryingIn == type.getQualifier().storage) {
        VaryingInfo varyingInfo;

        FillInVaryingInfo(&varyingInfo, type, name);
        mVaryingINMap.push_back(varyingInfo);
    }

    if(glslang::EvqVaryingOut == type.getQualifier().storage) {
        VaryingInfo varyingInfo;

        FillInVaryingInfo(&varyingInfo, type, name);
        mVaryingOUTMap.push_back(varyingInfo);
    }
}

void
GlslangIoMapResolver::endNotifications(EShLanguage stage)
{

}

void
GlslangIoMapResolver::beginNotifications(EShLanguage stage)
{

}

void
GlslangIoMapResolver::beginResolve(EShLanguage stage)
{

}

void
GlslangIoMapResolver::endResolve(EShLanguage stage)
{

}

void
GlslangIoMapResolver::Reset()
{
    mVaryingINMap.clear();
    mVaryingOUTMap.clear();
}

const char *
GlslangIoMapResolver::GetVaryingInName(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_TRACE);
    return (index > mVaryingINMap.size() - 1) ? nullptr : mVaryingINMap[index].name;
}

const char *
GlslangIoMapResolver::GetVaryingInType(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_TRACE);
    return (index > mVaryingINMap.size() - 1) ? nullptr : mVaryingINMap[index].type;
}

bool
GlslangIoMapResolver::GetVaryingInHasLocation(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_TRACE);
    return (index > mVaryingINMap.size() - 1) ? false : mVaryingINMap[index].hasLocation;
}

int
GlslangIoMapResolver::GetVaryingInLocation(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_TRACE);
    return (index > mVaryingINMap.size() - 1) ? -1 : mVaryingINMap[index].location;
}

const char *
GlslangIoMapResolver::GetVaryingOutName(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_TRACE);
    return (index > mVaryingOUTMap.size() - 1) ? nullptr : mVaryingOUTMap[index].name;
}

const char *
GlslangIoMapResolver::GetVaryingOutType(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_TRACE);
    return (index > mVaryingOUTMap.size() - 1) ? nullptr : mVaryingOUTMap[index].type;
}

bool
GlslangIoMapResolver::GetVaryingOutHasLocation(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_TRACE);
    return (index > mVaryingOUTMap.size() - 1) ? false : mVaryingOUTMap[index].hasLocation;
}

int
GlslangIoMapResolver::GetVaryingOutLocation(uint32_t index) const
{
    FUN_ENTRY(GL_LOG_TRACE);
    return (index > mVaryingOUTMap.size() - 1) ? -1 : mVaryingOUTMap[index].location;
}
