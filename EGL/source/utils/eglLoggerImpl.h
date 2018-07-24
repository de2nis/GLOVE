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
 *  @file       eglLoggerImpl.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Logger Implementation Interface
 *
 */

#ifndef __EGLLOGGERIMPL_H__
#define __EGLLOGGERIMPL_H__

#include "eglLogger.h"

typedef enum {
    EGL_LOG_TRACE        = 0,
    EGL_LOG_DEBUG        = 1,
    EGL_LOG_INFO         = 2,
    EGL_LOG_WARN         = 3,
    EGL_LOG_ERROR        = 4,
    EGL_LOG_CRITICAL     = 5
}eglLogLevel_e;

class EGLLoggerImpl {
public:

    EGLLoggerImpl() { }
    virtual ~EGLLoggerImpl() { }

    virtual void          Initialize() = 0;
    virtual void          Destroy() = 0;
    virtual void          WriteLog(eglLogLevel_e level, const char *log) = 0;
};

#endif //__EGLLOGGERIMPL_H__
