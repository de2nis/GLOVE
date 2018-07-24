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
 *  @file       eglLogger.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      A Logging Library
 *
 *  @section
 *
 *  eglLogger is an easy-to-use monitor interface that can be used for
 *  developing, debugging and running OpenGL ES applications.
 *
 */

#ifndef __EGLLOGGER_H__
#define __EGLLOGGER_H__

#include "eglLoggerImpl.h"

#include <stdio.h>
#include <string.h>

#ifdef TRACE_BUILD
#   define STR_HELPER(x)                               #x
#   define STR(x)                                      STR_HELPER(x)
#   define PROJECT_LENGTH                              strlen(STR(PROJECT_PATH)) -1
#   define EGL_SOURCE_FILE_NAME                        &__FILE__[ PROJECT_LENGTH ]
#   define FUN_ENTRY(__lvl__)                          EGLLogger::FunEntry(__lvl__, EGL_SOURCE_FILE_NAME, __func__, __LINE__)
#else
#   define FUN_ENTRY(__lvl__)
#endif

#ifdef DEBUG_DEPTH
#   undef DEBUG_DEPTH
#endif // DEBUG_DEPTH
#define DEBUG_DEPTH                                    EGL_LOG_DEBUG


class EGLLogger {
private:
    static const int      funcWidth = 54;
    static EGLLogger     *mInstance;
    static EGLLoggerImpl *mLoggerImpl;

    EGLLogger();
    virtual ~EGLLogger();

    static void           SetLoggerImpl();

    static EGLLogger     *GetInstance();
    static void           DestroyInstance();

    inline int            CalculateSpacesBefore(eglLogLevel_e level);
    inline int            CalculateSpacesAfter(const char *func, eglLogLevel_e level);
    void                  WriteFunEntry(eglLogLevel_e level, const char* filename, const char *func, int line);

public:
    static void           Shutdown();
    static void           FunEntry(eglLogLevel_e level, const char* filename, const char *func, int line);
    static void           Log(eglLogLevel_e level, const char* log);
};

#endif //__EGLLOGGER_H__
