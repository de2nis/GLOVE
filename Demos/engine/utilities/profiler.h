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

#ifndef __PROFILER_H_
#define __PROFILER_H_

#include "debug.h"
#include "../../eglut/eglutint.h"
#include <sys/time.h>

double GpuTimer(const char *title);
void GpuViewer(void);

#endif // __PROFILER_H_
