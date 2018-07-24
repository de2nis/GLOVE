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

#include "arrays_tests.h"

namespace Testing {

size_t ZERO = 0;

// Code here will be called immediately after the constructor (right
// before each test).
void ObjectArrayTest::SetUp(void) {
    return;
}

// Code here will be called immediately after each test (right
// before the destructor).
void ObjectArrayTest::TearDown() {
    return;
}

// Objects declared here can be used by all tests.


TEST_F(ObjectArrayTest, CreateShaders)
{
    for(size_t i=1; i<11; i++) {
        ASSERT_EQ(i, ShaderArray.Allocate());
        ASSERT_FALSE(ShaderArray.ObjectExists(i));
        ASSERT_NE(ZERO, ShaderArray.GetObjectId(ShaderArray.GetObject(i)));
        ASSERT_TRUE(ShaderArray.ObjectExists(i));
    }

    for(size_t i=1; i<11; i++) {
        ShaderArray.Deallocate(i);
    }
}

TEST_F(ObjectArrayTest, DeleteShaders)
{
    for(size_t i=1; i<11; i++) {
        ASSERT_EQ(i, ShaderArray.Allocate());
        ASSERT_FALSE(ShaderArray.ObjectExists(i));
        ASSERT_NE(ZERO, ShaderArray.GetObjectId(ShaderArray.GetObject(i)));
        ASSERT_TRUE(ShaderArray.ObjectExists(i));
    }

    for(size_t i=1; i<11; i+=2) {
        ASSERT_TRUE(ShaderArray.Deallocate(i));
        ASSERT_FALSE(ShaderArray.ObjectExists(i));
    }

    for(size_t i=2; i<11; i+=2) {
        ASSERT_FALSE(ShaderArray.ObjectExists(i-1));
        ASSERT_TRUE(ShaderArray.ObjectExists(i));
    }

}

} //end of namespace
