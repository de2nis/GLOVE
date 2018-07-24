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
 *  @file       glUtils.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpengGL ES Utility Functions
 *
 */

#ifndef __GLUTILS_H__
#define __GLUTILS_H__

#include <cstdio>
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

const char *            GlAttribTypeToString(GLenum type);
GLenum                  GlFormatToGlInternalFormat(GLenum format, GLenum type);
GLenum                  GlInternalFormatToGlType(GLenum internalFormat);
GLenum                  GlInternalFormatToGlFormat(GLenum internalFormat);
size_t                  GlInternalFormatTypeToNumElements(GLenum format, GLenum type);
size_t                  GlTypeToElementSize(GLenum type);
void                    GlFormatToStorageBits(GLenum format, GLint     *r_, GLint     *g_, GLint     *b_, GLint     *a_, GLint     *d_, GLint     *s_);
void                    GlFormatToStorageBits(GLenum format, GLfloat   *r_, GLfloat   *g_, GLfloat   *b_, GLfloat   *a_, GLfloat   *d_, GLfloat   *s_);
void                    GlFormatToStorageBits(GLenum format, GLboolean *r_, GLboolean *g_, GLboolean *b_, GLboolean *a_, GLboolean *d_, GLboolean *s_);
bool                    GlFormatIsDepth(GLenum format);
bool                    GlFormatIsStencil(GLenum format);
bool                    GlFormatIsColor(GLenum format);

#endif // __GLUTILS_H__
