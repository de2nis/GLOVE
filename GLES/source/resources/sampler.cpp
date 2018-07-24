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
 *  @file       sampler.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      (Texture) Sampler Functionality in GLOVE
 *
 *  @scope
 *
 *  A Sampler object stores the sampling parameters for a texture access
 *  inside of a shader.
 *
 */

#include "sampler.h"

Sampler::Sampler()
: mMinFilter(GL_NEAREST_MIPMAP_LINEAR), mMagFilter(GL_LINEAR), mWrapS(GL_REPEAT), mWrapT(GL_REPEAT)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

Sampler::~Sampler()
{
    FUN_ENTRY(GL_LOG_TRACE);
}
