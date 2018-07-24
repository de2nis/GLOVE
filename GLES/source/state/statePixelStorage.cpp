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
 *  @file       statePixelStorage.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      State related to Pixel Storage (data pack and unpack) 
 *
 */

#include "statePixelStorage.h"

StatePixelStorage::StatePixelStorage()
: mPixelStorePack(4), mPixelStoreUnpack(4)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

StatePixelStorage::~StatePixelStorage()
{
    FUN_ENTRY(GL_LOG_TRACE);
}
