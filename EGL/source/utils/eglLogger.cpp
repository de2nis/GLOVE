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
 *  @file       eglLogger.cpp
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

#include "eglLogger.h"
#include "simpleLoggerImpl.h"
#include <string.h>

EGLLogger *EGLLogger::mInstance = nullptr;
EGLLoggerImpl *EGLLogger::mLoggerImpl = nullptr;

EGLLogger::EGLLogger()
{
    mLoggerImpl->Initialize();
}

EGLLogger::~EGLLogger()
{
    mLoggerImpl->Destroy();
}

void
EGLLogger::SetLoggerImpl()
{
    if(!mLoggerImpl) {
        mLoggerImpl = new SimpleLoggerImpl();
    }
}

EGLLogger *
EGLLogger::GetInstance()
{
    SetLoggerImpl();

    if(!mInstance) {
        mInstance = new EGLLogger();
    }

    return mInstance;
}

void
EGLLogger::DestroyInstance()
{
    if(mInstance) {
        delete mInstance;
        mInstance = nullptr;
    }
}

int
EGLLogger::CalculateSpacesBefore(eglLogLevel_e level)
{
    return ( 8 - (int)level*4 );
}

int
EGLLogger::CalculateSpacesAfter(const char *func, eglLogLevel_e level)
{
    return  funcWidth - (int)strlen(func)  - 8 + (int)level*4;
}

void
EGLLogger::WriteFunEntry(eglLogLevel_e level, const char* filename, const char *func, int line)
{
    char log[200];
    snprintf(log, 200, "%*s%s()%*s  [ %s: %d  Context: c1 ]", CalculateSpacesBefore(level), "", func, CalculateSpacesAfter(func, level), "", filename, line);
    mLoggerImpl->WriteLog(level, log);
}

void
EGLLogger::FunEntry(eglLogLevel_e level, const char* filename, const char *func, int line)
{
    EGLLogger *eglLogger = EGLLogger::GetInstance();
    eglLogger->WriteFunEntry(level, filename, func, line);
}

void
EGLLogger::Log(eglLogLevel_e level, const char* log)
{
    mLoggerImpl->WriteLog(level, log);
}

void
EGLLogger::Shutdown()
{
    EGLLogger::DestroyInstance();
}
