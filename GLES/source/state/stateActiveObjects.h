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
 *  @file       stateActiveObjects.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Active Resources
 *
 */

#ifndef __STATEACTIVEOBJECTS_H__
#define __STATEACTIVEOBJECTS_H__

#include "resources/shaderProgram.h"
#include "resources/bufferObject.h"
#include "resources/texture.h"

#define GL_BUFFER_TARGET_TO_TYPE(__target__)  ((__target__) == GL_ARRAY_BUFFER ? BUFFER_OBJECT_TARGET_ARRAY : BUFFER_OBJECT_TARGET_ELEMENT)
#define GL_TEXTURE_TARGET_TO_TYPE(__target__) ((__target__) == GL_TEXTURE_2D ? 0 : 1)
#define GL_TEXTURE_ENUM_TO_UNIT(__enum__)     ((__enum__) - GL_TEXTURE0)

class StateActiveObjects {
private:
      typedef enum {
        BUFFER_OBJECT_TARGET_ARRAY = 0,
        BUFFER_OBJECT_TARGET_ELEMENT,
        BUFFER_OBJECT_TARGET_ALL
      } BufferObjectTarget_t;

      BufferObject*             mActiveBufferObjects[BUFFER_OBJECT_TARGET_ALL];
      ShaderProgram*            mActiveShaderProgram;
      GLuint                    mActiveFramebufferObjectID;
      GLuint                    mActiveRenderbufferObjectID;
      GLenum                    mActiveTextureUnit;
      Texture *                 mActiveTextures[2][GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS];

public:
      StateActiveObjects();
      ~StateActiveObjects();

// Get Functions
      inline Texture*           GetActiveTexture(GLenum target)                            { FUN_ENTRY(GL_LOG_TRACE); return mActiveTextures[GL_TEXTURE_TARGET_TO_TYPE(target)][GL_TEXTURE_ENUM_TO_UNIT(mActiveTextureUnit)]; }
      inline Texture*           GetActiveTexture(GLenum target, int j)                     { FUN_ENTRY(GL_LOG_TRACE); return mActiveTextures[GL_TEXTURE_TARGET_TO_TYPE(target)][j]; }
      inline ShaderProgram*     GetActiveShaderProgram(void)                               { FUN_ENTRY(GL_LOG_TRACE); return mActiveShaderProgram; }
      inline BufferObject*      GetActiveBufferObject(BufferObjectTarget_t target)         { FUN_ENTRY(GL_LOG_TRACE); return mActiveBufferObjects[target]; }
      inline BufferObject*      GetActiveBufferObject(GLenum target)                       { FUN_ENTRY(GL_LOG_TRACE); return GetActiveBufferObject(GL_BUFFER_TARGET_TO_TYPE(target)); }
      inline uint32_t           GetActiveFramebufferObjectID(void)                  const  { FUN_ENTRY(GL_LOG_TRACE); return mActiveFramebufferObjectID; }
      inline uint32_t           GetActiveRenderbufferObjectID(void)                 const  { FUN_ENTRY(GL_LOG_TRACE); return mActiveRenderbufferObjectID; }
      inline GLenum             GetActiveTextureUnit(void)                          const  { FUN_ENTRY(GL_LOG_TRACE); return mActiveTextureUnit; }

// Set Functions
      inline void               SetActiveTexture(GLenum target, Texture *tex)              { FUN_ENTRY(GL_LOG_TRACE); mActiveTextures[GL_TEXTURE_TARGET_TO_TYPE(target)][GL_TEXTURE_ENUM_TO_UNIT(mActiveTextureUnit)] = tex; }
      inline void               SetActiveTexture(GLenum target, int j, Texture *tex)       { FUN_ENTRY(GL_LOG_TRACE); mActiveTextures[GL_TEXTURE_TARGET_TO_TYPE(target)][j] = tex; }
      inline void               SetActiveTextureUnit(GLenum tex)                           { FUN_ENTRY(GL_LOG_TRACE); mActiveTextureUnit = tex; }
      inline void               SetActiveFramebufferObjectID(GLuint id)                    { FUN_ENTRY(GL_LOG_TRACE); mActiveFramebufferObjectID  = id; }
      inline void               SetActiveRenderbufferObjectID(GLuint id)                   { FUN_ENTRY(GL_LOG_TRACE); mActiveRenderbufferObjectID = id; }
      inline void               SetActiveShaderProgram(ShaderProgram *program)             { FUN_ENTRY(GL_LOG_TRACE); mActiveShaderProgram = program; }
      inline void               SetActiveBufferObject(BufferObjectTarget_t target,
                                                      BufferObject *bo)                    { FUN_ENTRY(GL_LOG_TRACE); mActiveBufferObjects[target] = bo; }
      inline void               SetActiveBufferObject(GLenum target,
                                                      BufferObject *bo)                    { FUN_ENTRY(GL_LOG_TRACE); SetActiveBufferObject(GL_BUFFER_TARGET_TO_TYPE(target), bo); }
      inline void               ResetActiveBufferObject(GLenum target)                     { FUN_ENTRY(GL_LOG_TRACE); SetActiveBufferObject(target, NULL); }

// Equals/Is Functions
      inline bool               EqualsActiveBufferObject(BufferObject *bo)                 { FUN_ENTRY(GL_LOG_TRACE); return GetActiveBufferObject(bo->GetTarget()) == bo; }

      inline bool               EqualsActiveTexture(GLenum target, int j, Texture *tex)    { FUN_ENTRY(GL_LOG_TRACE); return mActiveTextures[GL_TEXTURE_TARGET_TO_TYPE(target)][j] == tex; }
      inline bool               EqualsActiveRenderbufferObject(uint32_t rbo)               { FUN_ENTRY(GL_LOG_TRACE); return mActiveRenderbufferObjectID == rbo; }
      inline bool               IsDefaultFramebufferObjectActive(void)                     { FUN_ENTRY(GL_LOG_TRACE); return mActiveFramebufferObjectID  == 0;   }

};

#endif //__STATEACTIVEOBJECTS_H__
