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
 *  @file       egl_defs.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      A header file that includes several global variables.
 *
 */

#ifndef __EGL_DEFS_H__
#define __EGL_DEFS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define EGL_GL_VERSION_1                               1
#define EGL_GL_VERSION_2                               2

#define EGL_FENCE_WAIT_TIMEOUT                         UINT64_MAX

#ifndef EGL_SUPPORT_ONLY_PBUFFER_SURFACE
#   define EGL_SUPPORT_ONLY_PBUFFER_SURFACE            0
#else
#   undef EGL_SUPPORT_ONLY_PBUFFER_SURFACE
#   define EGL_SUPPORT_ONLY_PBUFFER_SURFACE            1
#endif // EGL_SUPPORT_ONLY_PBUFFER_SURFACE

// Compiler
#ifdef __GNUC__
#   define ASSERT_ONLY                                  __attribute__((unused))
#   define COMPILER_WARN_UNUSED_RESULT                  __attribute__((warn_unused_result))
#   define FORCE_INLINE                                 __attribute__((always_inline))
#else
#   define ASSERT_ONLY
#   define COMPILER_WARN_UNUSED_RESULT
#endif // __GNUC__

// Code
#ifdef NDEBUG
#   define NOT_REACHED()                                printf("You shouldn't be here. (function %s at line %d of file %s)\n", __func__, __LINE__, __FILE__)
#   define NOT_IMPLEMENTED()                            printf("Function %s (line %d of file %s) not implemented yet.\n", __func__, __LINE__, __FILE__)
#else
#   define NOT_REACHED()                                assert(0 && "You shouldn't be here.")
#   define NOT_IMPLEMENTED()                            assert(0 && "Function not implemented yet.")
#endif // NDEBUG

#define ARRAY_SIZE(array)                               (int)(sizeof(array) / sizeof(array[0]))

#endif // __EGL_DEFS_H__
