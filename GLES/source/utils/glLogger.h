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
 *  @file       glLogger.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      A Logging Library
 *
 */

#ifndef __GLLOGGER_H__
#define __GLLOGGER_H__

#include "glLoggerImpl.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#ifdef __cplusplus
}
#endif

#ifdef TRACE_BUILD
#   define STR_HELPER(x)                                #x
#   define STR(x)                                       STR_HELPER(x)
#   define PROJECT_LENGTH                               strlen(STR(PROJECT_PATH)) -1
#   define GL_SOURCE_FILE_NAME                          &__FILE__[ PROJECT_LENGTH ]
#   define FUN_ENTRY(__lvl__)                           GLLogger::FunEntry(__lvl__, GL_SOURCE_FILE_NAME, __func__, __LINE__)
#   define GLOVE_PRINT(__lvl__, ...)                    GLLogger::Log(__lvl__, __VA_ARGS__)
#   define GLOVE_PRINT_ERR(...)                         fprintf(stderr, __VA_ARGS__);
#else
#   define FUN_ENTRY(__lvl__)
#   define GLOVE_PRINT(...)
#   define GLOVE_PRINT_ERR(...)
#endif

class GLLogger {
private:
    static const int      funcWidth = 55;
    static GLLogger      *mInstance;
    static GLLoggerImpl  *mLoggerImpl;

    GLLogger();
    virtual ~GLLogger();

    static void           SetLoggerImpl();

    static GLLogger      *GetInstance();
    static void           DestroyInstance();

    int                   CalculateSpacesBefore(glLogLevel_e level);
    int                   CalculateSpacesAfter(const char *func, glLogLevel_e level);
    void                  WriteFunEntry(glLogLevel_e level, const char* filename, const char *func, int line);

public:
    static void           Shutdown();
    static void           FunEntry(glLogLevel_e level, const char* filename, const char *func, int line);
    static void           Log(glLogLevel_e level, const char *format, ...);

};

#endif //__GLLOGGER_H__
