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
 *  @file       glLoggerImpl.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Logger Implementation Interface
 *
 */

#ifndef __GLLOGGERIMPL_H__
#define __GLLOGGERIMPL_H__

#include "glLogger.h"

typedef enum {
    GL_LOG_TRACE        = 0,
    GL_LOG_DEBUG        = 1,
    GL_LOG_INFO         = 2,
    GL_LOG_WARN         = 3,
    GL_LOG_ERROR        = 4,
    GL_LOG_CRITICAL     = 5
}glLogLevel_e;

class GLLoggerImpl {
public:

    GLLoggerImpl() { }
    virtual ~GLLoggerImpl() { }

    virtual void          Initialize() = 0;
    virtual void          Destroy() = 0;
    virtual void          WriteLog(glLogLevel_e level, const char *format) = 0;
};

#endif //__GLLOGGERIMPL_H__
