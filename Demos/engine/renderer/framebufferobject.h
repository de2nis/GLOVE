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

#ifndef __FRAMEBUFFEROBJECT_H_
#define __FRAMEBUFFEROBJECT_H_

#include "../utilities/debug.h"
#include "../asset_manager/tgaLoader.h"

typedef struct openGL_fbo_t
{
    GLuint       mID;
    Texture     *mTextureColor;
    Texture     *mTextureDepth;

} openGL_fbo_t;

bool InitFBO(struct openGL_fbo_t *fbo, const GLint width, const GLint height);
void DeleteFBO(struct openGL_fbo_t *fbo);
bool CheckFBOStatus(void);

#endif //__FRAMEBUFFEROBJECT_H_
