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
 *  @file       platformFactory.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Abstract Factory to create platform related objects (Childs of PlatformResources and PlatformWindowInterface).
 *
 */

#ifndef __PLATFORM_FACTORY_H__
#define __PLATFORM_FACTORY_H__

#include "platformWindowInterface.h"
#include "platformResources.h"

class PlatformFactory
{
public:
    enum PlatformType {
        UNKNOWN_PLATFORM,
        WSI_XCB,
        WSI_ANDROID,
        WSI_PLANE_DISPLAY
    };

private:
    PlatformType                       mPlatformType;
    static PlatformFactory            *mInstance;

    PlatformFactory();
    ~PlatformFactory();

    inline PlatformType                GetPlatformType()                            { FUN_ENTRY(EGL_LOG_TRACE); return mPlatformType; }

public:
    static PlatformFactory            *GetInstance();
    static void                        DestroyInstance();

    static void                        ChoosePlatform();
    static PlatformWindowInterface    *GetWindowInterface();
    static PlatformResources          *GetResources();

    void                               SetPlatformType(PlatformType platformType)   { FUN_ENTRY(EGL_LOG_TRACE); mPlatformType = platformType; }
};

#endif // __PLATFORM_FACTORY_H__
