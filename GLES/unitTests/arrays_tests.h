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

#ifndef __ARRAYS_TESTS_H__
#define __ARRAYS_TESTS_H__

#include "gtest/gtest.h"
#include "resources/shader.h"
#include "utils/globals.h"
#include "utils/arrays.hpp"

namespace Testing {

class ObjectArrayTest : public ::testing::Test {
protected:
    void SetUp(void);
    void TearDown(void);

    class ObjectArray<Shader> ShaderArray;
};

} //end of namespace

#endif // __ARRAYS_TESTS_H__
