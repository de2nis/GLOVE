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
 *  @file       stateManager.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State Manager Functionality
 *
 */

#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__

#include "stateFragmentOperations.h"
#include "stateFramebufferOperations.h"
#include "stateInputAssembly.h"
#include "stateRasterization.h"
#include "statePixelStorage.h"
#include "stateViewportTransformation.h"
#include "stateActiveObjects.h"
#include "stateHintAspects.h"
#include "vulkan/pipeline.h"

class StateManager {

private:
      StateActiveObjects                      mActiveObjectsState;
      StateFramebufferOperations              mFramebufferOperationsState;
      StateFragmentOperations                 mFragmentOperationsState;
      StateInputAssembly                      mInputAssemblyState;
      StatePixelStorage                       mPixelStorageState;
      StateRasterization                      mRasterizationState;
      StateViewportTransformation             mViewportTransformationState;
      StateHintAspects                        mHintAspectsState;

      GLenum                                  mError;
public:

       StateManager();
      ~StateManager();

// Is Functions
             GLboolean                        IsCapabilityEnabled(GLenum cap);

// Init Functions
             void                             InitVkPipelineStates(vulkanAPI::Pipeline *pipeline);

// Get Functions
      inline StateActiveObjects*              GetActiveObjectsState(void)             { FUN_ENTRY(GL_LOG_TRACE); return &mActiveObjectsState; }
      inline StateViewportTransformation*     GetViewportTransformationState(void)    { FUN_ENTRY(GL_LOG_TRACE); return &mViewportTransformationState; }
      inline StateInputAssembly*              GetInputAssemblyState(void)             { FUN_ENTRY(GL_LOG_TRACE); return &mInputAssemblyState; }
      inline StateRasterization*              GetRasterizationState(void)             { FUN_ENTRY(GL_LOG_TRACE); return &mRasterizationState; }
      inline StatePixelStorage*               GetPixelStorageState(void)              { FUN_ENTRY(GL_LOG_TRACE); return &mPixelStorageState; }
      inline StateFragmentOperations*         GetFragmentOperationsState(void)        { FUN_ENTRY(GL_LOG_TRACE); return &mFragmentOperationsState; }
      inline StateFramebufferOperations*      GetFramebufferOperationsState(void)     { FUN_ENTRY(GL_LOG_TRACE); return &mFramebufferOperationsState; }
      inline StateHintAspects*                GetHintAspectsState(void)               { FUN_ENTRY(GL_LOG_TRACE); return &mHintAspectsState; }
      inline ShaderProgram*                   GetActiveShaderProgram(void)            { FUN_ENTRY(GL_LOG_TRACE); return mActiveObjectsState.GetActiveShaderProgram(); }
      inline GLenum                           GetError(void)                          { FUN_ENTRY(GL_LOG_TRACE); return mError;  }

// Set Functions
      inline void                             SetError(GLenum error)                  { FUN_ENTRY(GL_LOG_TRACE); mError = error; }
};

#endif // __STATEMANAGER_H__
