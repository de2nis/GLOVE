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
 *  @file       glslangIoMapResolver.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shader I/O gathering functionality based on glslang TIoMapResolver
 *
 */

#ifndef __GLSLANGIOMAPRESOLVER_H__
#define __GLSLANGIOMAPRESOLVER_H__

#include <vector>

#include "glslang/Public/ShaderLang.h"
#include "glslang/Include/Types.h"

class GlslangIoMapResolver : public glslang::TIoMapResolver
{
private:
    using VaryingInfo  = struct {
        const char *                name;
        const char *                type;
        bool                        hasLocation;
        int                         location;
    };

    using VaryingMap = std::vector<VaryingInfo>;
    using VaryingMapIter = VaryingMap::const_iterator;

    VaryingMap                      mVaryingINMap;
    VaryingMap                      mVaryingOUTMap;

    void                            FillInVaryingInfo(VaryingInfo *varyinginfo, const glslang::TType& type, const char *name);

public:

    GlslangIoMapResolver() {}
    ~GlslangIoMapResolver() {}

    bool                            validateBinding(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    int                             resolveBinding(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    int                             resolveSet(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    int                             resolveUniformLocation(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    bool                            validateInOut(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    int                             resolveInOutLocation(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    int                             resolveInOutComponent(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    int                             resolveInOutIndex(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    void                            notifyBinding(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    void                            notifyInOut(EShLanguage stage, const char* name, const glslang::TType& type, bool is_live) override;
    void                            endNotifications(EShLanguage stage) override;
    void                            beginNotifications(EShLanguage stage) override;
    void                            beginResolve(EShLanguage stage) override;
    void                            endResolve(EShLanguage stage) override;

    void                            Reset();

    inline uint32_t                 GetNumLiveVaryingInVariables() const { return mVaryingINMap.size(); }
    const char *                    GetVaryingInName(uint32_t index) const;
    const char *                    GetVaryingInType(uint32_t index) const;
    bool                            GetVaryingInHasLocation(uint32_t index) const;
    int                             GetVaryingInLocation(uint32_t index) const;

    inline uint32_t                 GetNumLiveVaryingOutVariables() const { return mVaryingOUTMap.size(); }
    const char *                    GetVaryingOutName(uint32_t index) const;
    const char *                    GetVaryingOutType(uint32_t index) const;
    bool                            GetVaryingOutHasLocation(uint32_t index) const;
    int                             GetVaryingOutLocation(uint32_t index) const;
};

#endif // __GLSLANGIOMAPRESOLVER_H__
