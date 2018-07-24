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

#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

#include "../engine/glcore/common.h"

#define ARGS_NUM 7

static openGL_program_t  program;

static char *vs_source = NULL;
static char *fs_source = NULL;
static char *out_file  = NULL;

static void PrintUsage    ();
static void CompileShaders();
static bool ReadArguments (int argc, char *argv[]);

static void PrintUsage()
{
    printf("Correct Usage: ./offline_shader_compiler -v <vertex_shader_source> -f <fragment_shader_source> -o <output_file>\n");
}

static bool ReadArguments(int argc, char *argv[])
{
    signed char c;

    if(argc != ARGS_NUM) {
        PrintUsage();
        return false;
    }

    while ((c = getopt(argc, argv, "v:f:o:")) != -1) {
        switch (c) {
        case 'v':
            vs_source = optarg;
            break;
        case 'f':
            fs_source = optarg;
            break;
        case 'o':
            out_file = optarg;
            break;
        case '?':
            if (optopt == 'v')
                printf ("Option -%c requires an argument.\n", optopt);
            else if (optopt == 'f')
                printf ("Option -%c requires an argument.\n", optopt);
            else if (optopt == 'o')
                printf ("Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                printf ("Unknown option `-%c'.\n", optopt);
            else
                printf ("Unknown option character `\\x%x'.\n", optopt);
            PrintUsage();

            return false;
        default:
            abort ();
        }
    }

    return true;
}

static void CompileShaders(void)
{
// Load Vertex Shader
    if(!LoadShader(vs_source, &program.mVertexShader  , GL_VERTEX_SHADER))
        return;
// Load Fragment Shader
    if(!LoadShader(fs_source, &program.mFragmentShader, GL_FRAGMENT_SHADER))
        return;
// Load Program
    if(!LoadProgram(program.mVertexShader, program.mFragmentShader, &program.mID))
        return;
// Load Binary Program
    SaveProgramBinary(&program.mID, out_file);

    printf ("Generation of Binary Shader Program '%s' Completed Successfully\n", out_file);
}

void DestroyGL(void)
{
// Detach Shaders
    glDetachShader(program.mID, program.mVertexShader);
    glDetachShader(program.mID, program.mFragmentShader);

// Delete Shaders
    DeleteShader  (program.mVertexShader);
    DeleteShader  (program.mFragmentShader);

// Delete Program
    DeleteProgram (program.mID);
}

int main(int argc, char **argv)
{
    if(!ReadArguments(argc, argv))
      return 0;

    eglutInit(argc, (const char **)argv);
    int win = eglutCreateWindow("");

    CompileShaders();

    DestroyGL();

    eglutDestroyWindow(win);
    _eglutFini();

    return 1;
}
