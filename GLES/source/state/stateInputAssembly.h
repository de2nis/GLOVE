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
 *  @file       stateInputAssembly.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Input Assembly
 *
 */

#ifndef __STATEINPUTASSEMBLY_H__
#define __STATEINPUTASSEMBLY_H__

#include "GLES2/gl2.h"
#include "utils/glLogger.h"

class StateInputAssembly {

private:
      GLenum                  mPrimitiveMode;
      GLboolean               mPrimitiveRestartEnabled; // glEnable((GL_PRIMITIVE_RESTART_FIXED_INDEX) - OpenGL ES 3.0

public:
      StateInputAssembly();
      ~StateInputAssembly();

// Get Functions
      inline GLenum           GetPrimitiveMode(void)                      const { FUN_ENTRY(GL_LOG_TRACE); return mPrimitiveMode; }
      inline GLboolean        GetPrimitiveRestartEnabled(void)            const { FUN_ENTRY(GL_LOG_TRACE); return mPrimitiveRestartEnabled; }

// Update Functions
      inline bool             UpdatePrimitiveMode(GLenum pMode)                 { FUN_ENTRY(GL_LOG_TRACE); bool res = mPrimitiveMode != pMode;
                                                                                                       mPrimitiveMode = pMode;
                                                                                                       return res; }
};

#endif //__STATEINPUTASSEMBLY_H__
