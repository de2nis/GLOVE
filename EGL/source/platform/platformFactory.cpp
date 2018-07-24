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
 *  @file       platformFactory.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Abstract Factory to create platform related objects (Childs of PlatformResources and PlatformWindowInterface).
 *
 */

#include "platformFactory.h"
#include "platform/vulkan/vulkanWindowInterface.h"
#include "platform/vulkan/vulkanResources.h"
#include "platform/vulkan/WSIXcb.h"
#include "platform/vulkan/WSIPlaneDisplay.h"

PlatformFactory *PlatformFactory::mInstance = nullptr;

PlatformFactory::PlatformFactory()
: mPlatformType(UNKNOWN_PLATFORM)
{
    FUN_ENTRY(EGL_LOG_TRACE);
}

PlatformFactory::~PlatformFactory()
{
    FUN_ENTRY(EGL_LOG_TRACE);
}

PlatformFactory *
PlatformFactory::GetInstance()
{
    FUN_ENTRY(EGL_LOG_TRACE);

    if(!mInstance) {
        mInstance = new PlatformFactory();
    }

    return mInstance;
}

void
PlatformFactory::DestroyInstance()
{
    FUN_ENTRY(EGL_LOG_TRACE);

    if(mInstance) {
        delete mInstance;
        mInstance = nullptr;
    }
}

void
PlatformFactory::ChoosePlatform()
{
    FUN_ENTRY(EGL_LOG_TRACE);

    PlatformFactory *platformFactory = PlatformFactory::GetInstance();

#ifdef VK_USE_PLATFORM_XCB_KHR
    platformFactory->SetPlatformType(PlatformFactory::WSI_XCB);
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
    platformFactory->SetPlatformType(PlatformFactory::WSI_ANDROID);
#endif 
}

PlatformWindowInterface *
PlatformFactory::GetWindowInterface()
{
    FUN_ENTRY(EGL_LOG_TRACE);

    PlatformFactory *platformFactory = GetInstance();
    PlatformType platformType = platformFactory->GetPlatformType();

    switch(platformType) {
        case WSI_XCB: {
            VulkanWindowInterface *windowInterface = new VulkanWindowInterface();
            WSIXcb *vulkanWSI = new WSIXcb();
            windowInterface->SetWSI(vulkanWSI);
            return windowInterface;
        }

        case WSI_ANDROID:
            NOT_IMPLEMENTED();
            return nullptr;

        case WSI_PLANE_DISPLAY: {
            VulkanWindowInterface *windowInterface = new VulkanWindowInterface();
            WSIPlaneDisplay *vulkanWSI = new WSIPlaneDisplay();
            windowInterface->SetWSI(vulkanWSI);
            return windowInterface;
        }
        case UNKNOWN_PLATFORM:
        default:
            NOT_REACHED();
    }

    return nullptr;
}

PlatformResources *
PlatformFactory::GetResources()
{
    FUN_ENTRY(EGL_LOG_TRACE);

    PlatformFactory *platformFactory = GetInstance();
    PlatformType platformType = platformFactory->GetPlatformType();

    switch(platformType) {
        case WSI_XCB:
            return new VulkanResources();

        case WSI_ANDROID:
            NOT_IMPLEMENTED();
            return nullptr;

        case WSI_PLANE_DISPLAY:
            return new VulkanResources();

        case UNKNOWN_PLATFORM:
        default:
            NOT_REACHED();
    }

    return nullptr;
}
