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
 *  @file       simpleLoggerImpl.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      A Logger implementation based on printf
 *
 */

#ifndef __SIMPLELOGGERIMPL_H__
#define __SIMPLELOGGERIMPL_H__

#include "eglLoggerImpl.h"

#define PRINTABLE_LOG_LEVEL EGL_LOG_DEBUG

class SimpleLoggerImpl : public EGLLoggerImpl {
public:
    SimpleLoggerImpl() { }
    ~SimpleLoggerImpl() { }

    void                   Initialize() override { }
    void                   Destroy() override    { }
    void                   WriteLog(eglLogLevel_e level, const char *log) override
    {
        if(level >= PRINTABLE_LOG_LEVEL) {
            printf("[EGL] %s\n", log);
        }
    }
};

#endif //__SIMPLELOGGERIMPL_H__
