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
 *  @file       stateHintAspects.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to implementation-specific hints
 *
 */

#ifndef __STATEHINTASPECTS_H__
#define __STATEHINTASPECTS_H__

#include "GLES2/gl2.h"
#include "utils/glLogger.h"

#define GL_HINT_TARGET_TO_TYPE(__target__) ((__target__) == GL_GENERATE_MIPMAP_HINT ? 0 : 0)

class StateHintAspects {

private:
      GLenum                  mHintModes[1];

public:
      StateHintAspects();
      ~StateHintAspects();

// Get Functions
      inline GLenum           GetMode(GLenum target)                      const { FUN_ENTRY(GL_LOG_TRACE); return mHintModes[GL_HINT_TARGET_TO_TYPE(target)]; }

// Update Functions
      inline void             SetMode(GLenum target, GLenum mode)               { FUN_ENTRY(GL_LOG_TRACE); mHintModes[GL_HINT_TARGET_TO_TYPE(target)] = mode; }

};

#endif //__STATEHINTASPECTS_H__
