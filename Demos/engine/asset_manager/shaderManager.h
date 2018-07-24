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

#ifndef __SHADERMANAGER_H_
#define __SHADERMANAGER_H_

#include "../utilities/debug.h"

bool  LoadSource        (const char *filename  , char   **shaderSource, int    *length);
bool  LoadShader        (const char *filename  , GLuint *shader       , GLenum  shaderType);
void  DeleteShader      (const GLuint sh);
bool  LoadProgram       (const GLuint vs       , const  GLuint fs     , GLuint *prog);
bool  LoadProgramBinary (const char* myBinaryFileName, GLenum binaryFormat, GLuint *prog);
bool  SaveProgramBinary (GLuint *prog, char *fileName);
void  DeleteProgram     (const GLuint prog);

#endif //  __SHADERMANAGER_H_
