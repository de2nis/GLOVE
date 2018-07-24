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
 *  @file       parser_helpers.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      String parser helper functions
 *
 */

#ifndef __PARSER_HELPERS_H__
#define __PARSER_HELPERS_H__

#include <iostream>

#define STRINGIFY(expr)         #expr
#define STRINGIFY_MACRO(expr)   STRINGIFY(expr)

using namespace std;

bool                    IsChar(char c);
bool                    IsWhiteSpace(char c);
string::size_type       SkipWhiteSpaces(const string &source, string::size_type pos);
string::size_type       FindToken(const string &token, const string &source, string::size_type pos);
string                  GetNextToken(const string &source, string::size_type start);
bool                    IsPrecisionQualifier(const string &token);
bool                    CanTypeBeInUniformBlock(const string &token);

#endif // __PARSER_HELPERS_H__
