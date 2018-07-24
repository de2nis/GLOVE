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

#include "shaderManager.h"

bool  LoadSource(const char *filename, char **shaderSource, int *length)
{
    FILE *file;
    file = fopen(filename, "r");
    if(!file) {
#ifdef DEBUG_ASSET_MANAGEMENT
        fprintf(stderr, "[shaderManager.c] [LoadSource()]: Error opening file %s\n", filename);
#endif
        return false;
    }

    fseek(file, 0, SEEK_END);
    unsigned int len = ftell(file) + 1;
    fseek(file, 0, SEEK_SET);

    if(!len){
        fclose(file);
        return false;
    }
    *length       = (int)len;
    *shaderSource = (char *) malloc((len) * sizeof(char));
    assert(*shaderSource);
    char *src = *shaderSource;

    size_t ret = fread(src, 1, len - 1, file);
    assert(ret == len - 1);
    src[len - 1] = '\0';

    fclose(file);

    return true;
}

bool  LoadShader(const char *filename, GLuint *shader, GLenum shaderType)
{
  int   length = 0, type = 0, status = 0;
  char *source = NULL;

  *shader      = glCreateShader(shaderType);

  if(!LoadSource(filename, &source, &length))
      return false;

  assert(source);
  assert(length);

  assert(glIsShader(*shader));
  glGetShaderiv(*shader, GL_SHADER_TYPE, &type);
  ASSERT_NO_GL_ERROR();
  assert(type == (GLint)shaderType);

  glShaderSource(*shader, 1, (const char **) &source, NULL);
  ASSERT_NO_GL_ERROR();
  glCompileShader(*shader);
  ASSERT_NO_GL_ERROR();

  glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
  ASSERT_NO_GL_ERROR();
  char *src = (char *)malloc(length * sizeof(char));
  glGetShaderSource(*shader, length, NULL, src);
  ASSERT_NO_GL_ERROR();
  free(src);

  glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
  ASSERT_NO_GL_ERROR();
  if(status == GL_FALSE) {

      glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &length);
      char *info = (char *)malloc(length * sizeof(char));
      glGetShaderInfoLog(*shader, length, NULL, info);
#ifdef DEBUG_OPENGLES
      fprintf(stderr, "[shaderManager.c] [LoadShader()]: Failed to compile vertex shader %s\n", filename);
      fprintf(stderr, "Compiler log:\n");
      fprintf(stderr, "***********\n%s\n***********\n", info);
#endif
      assert(0);

      free(info);
      DeleteShader(*shader);
  }
  free(source);

  glReleaseShaderCompiler();
  ASSERT_NO_GL_ERROR();

  return status!=0;
}

bool  LoadProgram(const GLuint vs, const GLuint fs, GLuint *prog)
{
    int   length = 0, status = 0;

    *prog = glCreateProgram();

    glAttachShader(*prog, vs);
    ASSERT_NO_GL_ERROR();
    glAttachShader(*prog, fs);
    ASSERT_NO_GL_ERROR();
    assert(glIsProgram(*prog));
    glLinkProgram(*prog);

    glGetProgramiv(*prog, GL_LINK_STATUS, &status);
    ASSERT_NO_GL_ERROR();
    if(status == GL_FALSE) {
        glGetProgramiv(*prog, GL_INFO_LOG_LENGTH, &length);
        char *info = (char *)malloc(length * sizeof(char));
        glGetProgramInfoLog(*prog, length, NULL, info);
#ifdef DEBUG_OPENGLES
        fprintf(stderr, "[shaderManager.c] [LoadProgram()]: Failed to link shaders\n");
        fprintf(stderr, "Compiler log:\n");
        fprintf(stderr, "***********\n%s\n***********\n", info);
#endif
        assert(0);
        free(info);
        DeleteProgram(*prog);
    }

    glDetachShader(*prog, vs);
    ASSERT_NO_GL_ERROR();
    glDetachShader(*prog, fs);
    ASSERT_NO_GL_ERROR();
    DeleteShader(vs);
    DeleteShader(fs);

    return status!=0;
}

bool LoadProgramBinary(const char* myBinaryFileName, GLenum binaryFormat, GLuint *prog)
{
    GLint   length = 0, status = 0;

    GLint   binaryLength;
    void*   binary;

    FILE*   infile;

    *prog = glCreateProgram();
    ASSERT_NO_GL_ERROR();
    //
    //  Read the program binary
    //
    infile = fopen(myBinaryFileName, "rb");
    if(!infile) {
#ifdef DEBUG_ASSET_MANAGEMENT
        fprintf(stderr, "Cannot load Program Binary: invalid input file!\n");
#endif
        return false;
    }

    fseek(infile, 0, SEEK_END);
    binaryLength = (GLint)ftell(infile);
    binary = (void*)malloc(binaryLength);
    fseek(infile, 0, SEEK_SET);
    fread(binary, binaryLength, 1, infile);
    fclose(infile);
    //
    //  Load the binary into the program object -- no need to link!
    //
    glProgramBinaryOES(*prog, binaryFormat, binary, binaryLength);
    ASSERT_NO_GL_ERROR();
    free(binary);

    glGetProgramiv(*prog, GL_LINK_STATUS, &status);
    ASSERT_NO_GL_ERROR();
    if(status == GL_FALSE) {
        glGetProgramiv(*prog, GL_INFO_LOG_LENGTH, &length);
        char *info = (char *)malloc(length * sizeof(char));
        glGetProgramInfoLog(*prog, length, NULL, info);
#ifdef DEBUG_OPENGLES
        fprintf(stderr, "[shaderManager.c] [LoadProgram()]: Failed to link shaders\n");
        fprintf(stderr, "Compiler log:\n");
        fprintf(stderr, "***********\n%s\n***********\n", info);
#endif
        assert(0);
        free(info);
        DeleteProgram(*prog);
    }

    return status!=0;
}

bool SaveProgramBinary(GLuint *prog, char *fileName)
{
    GLint    binaryLength;
    void*    binary;
    FILE*    outfile;
    uint32_t binaryFormat = 0;

    //
    //  Retrieve the binary from the program object
    //
    glGetProgramiv(*prog, GL_PROGRAM_BINARY_LENGTH_OES, &binaryLength);
    binary = (void*)malloc(binaryLength);
    glGetProgramBinaryOES(*prog, binaryLength, &binaryLength, &binaryFormat, binary);
    ASSERT_NO_GL_ERROR();
    //
    //  Cache the program binary for future runs
    //
    outfile = fopen(fileName, "wb");
    fwrite(binary, binaryLength, 1, outfile);
    fclose(outfile);
    free(binary);

    return true;
}
void  DeleteShader   (const GLuint sh)
{
    glDeleteShader(sh);
    ASSERT_NO_GL_ERROR();
}

void  DeleteProgram   (const GLuint prog)
{
    glDeleteProgram(prog);
    ASSERT_NO_GL_ERROR();
}
