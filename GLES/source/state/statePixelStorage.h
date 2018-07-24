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
 *  @file       statePixelStorage.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Pixel Storage (data pack and unpack) 
 *
 */

#ifndef __STATEPIXELSTORAGE_H__
#define __STATEPIXELSTORAGE_H__

#include "GLES2/gl2.h"
#include "utils/glLogger.h"

class StatePixelStorage {

private:
      GLint                   mPixelStorePack;
      GLint                   mPixelStoreUnpack;

public:
      StatePixelStorage();
      ~StatePixelStorage();

// Get Functions
      inline GLint            GetPixelStorePack(void)                    const  { FUN_ENTRY(GL_LOG_TRACE); return mPixelStorePack; }
      inline GLint            GetPixelStoreUnpack(void)                  const  { FUN_ENTRY(GL_LOG_TRACE); return mPixelStoreUnpack; }

// Set Functions
      inline void             SetPixelStorePack(GLint pixelStorePack)           { FUN_ENTRY(GL_LOG_TRACE); mPixelStorePack   = pixelStorePack;}
      inline void             SetPixelStoreUnpack(GLint pixelStoreUnpack)       { FUN_ENTRY(GL_LOG_TRACE); mPixelStoreUnpack = pixelStoreUnpack;}
};

#endif //__STATEPIXELSTORAGE_H__
