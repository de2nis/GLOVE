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
 *  @file       glsl_types.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Basic Types in GLSL (ESSL)
 *
 */

#ifndef __GLSL_TYPES_H__
#define __GLSL_TYPES_H__

typedef uint32_t                            glsl_bool_t;
typedef int32_t                             glsl_int_t;
typedef uint32_t                            glsl_uint_t;
typedef float                               glsl_float_t;

typedef struct { glsl_bool_t  b[2]; }       glsl_bvec2_t;
typedef struct { glsl_int_t   i[2]; }       glsl_ivec2_t;
typedef struct { glsl_uint_t  u[2]; }       glsl_uvec2_t;
typedef struct { glsl_float_t f[2]; }       glsl_vec2_t;

typedef struct { glsl_bool_t  b[3]; }       glsl_bvec3_t;
typedef struct { glsl_int_t   i[3]; }       glsl_ivec3_t;
typedef struct { glsl_uint_t  u[3]; }       glsl_uvec3_t;
typedef struct { glsl_float_t f[3]; }       glsl_vec3_t;

typedef struct { glsl_bool_t  b[4]; }       glsl_bvec4_t;
typedef struct { glsl_int_t   i[4]; }       glsl_ivec4_t;
typedef struct { glsl_uint_t  u[4]; }       glsl_uvec4_t;
typedef struct { glsl_float_t f[4]; }       glsl_vec4_t;

typedef struct { glsl_vec2_t fm[2]; }       glsl_mat2_t;
typedef struct { glsl_vec3_t fm[3]; }       glsl_mat3_t;
typedef struct { glsl_vec4_t fm[4]; }       glsl_mat4_t;

typedef uint32_t                            glsl_sampler_t;

typedef enum {
    INVALID_SHADER       = 0,
    SHADER_TYPE_VERTEX   = 1 << 0,
    SHADER_TYPE_FRAGMENT = 1 << 1
} shader_type_t;

class GenericVec4
{
public:
    union {
        glsl_bool_t                     bData[4];
        glsl_int_t                      iData[4];
        glsl_float_t                    fData[4];
    } v;

    GenericVec4()
    {
        v.fData[0] = 0.0f;
        v.fData[1] = 0.0f;
        v.fData[2] = 0.0f;
        v.fData[3] = 1.0f;
    }
};

inline size_t GlslTypeToSize(GLenum type)
{
    switch(type) {
    case GL_BOOL:                           return sizeof(glsl_bool_t);
    case GL_INT:                            return sizeof(glsl_int_t);
    case GL_FLOAT:                          return sizeof(glsl_float_t);

    case GL_BOOL_VEC2:                      return sizeof(glsl_bvec2_t);
    case GL_INT_VEC2:                       return sizeof(glsl_ivec2_t);
    case GL_FLOAT_VEC2:                     return sizeof(glsl_vec2_t);

    case GL_BOOL_VEC3:                      return sizeof(glsl_bvec3_t);
    case GL_INT_VEC3:                       return sizeof(glsl_ivec3_t);
    case GL_FLOAT_VEC3:                     return sizeof(glsl_vec3_t);

    case GL_BOOL_VEC4:                      return sizeof(glsl_bvec4_t);
    case GL_INT_VEC4:                       return sizeof(glsl_ivec4_t);
    case GL_FLOAT_VEC4:                     return sizeof(glsl_vec4_t);

    case GL_FLOAT_MAT2:                     return sizeof(glsl_mat2_t);
    case GL_FLOAT_MAT3:                     return sizeof(glsl_mat3_t);
    case GL_FLOAT_MAT4:                     return sizeof(glsl_mat4_t);

    case GL_SAMPLER_2D:
    case GL_SAMPLER_CUBE:                   return sizeof(glsl_sampler_t);
    default:                                return 0;
    }
}

#endif // __GLSL_TYPES_H__
