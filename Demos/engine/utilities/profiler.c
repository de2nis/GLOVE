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

#include "profiler.h"

void GpuViewer()
{
#ifdef INFO_DISPLAY
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const GLubyte *oglVersion  = glGetString(GL_VERSION);
    const GLubyte *oglVendor   = glGetString(GL_VENDOR);
    const GLubyte *oglRenderer = glGetString(GL_RENDERER);
    const GLubyte *oglExt      = glGetString(GL_EXTENSIONS);

    printf("--------------------------- \n");
    printf("OpenGL ES Vendor : %s\n", oglVendor);
    printf("OpenGL ES Version: %s\n", oglVersion);
    printf("GLSL   ES Version: %s\n", glslVersion);
    printf("OpenGL ES Ext    : %s\n", oglExt);
    printf("Graphics API     : %s\n", oglRenderer);
    printf("--------------------------- \n");
    eglutGetSetup();
    printf("--------------------------- \n");
#endif
}

double GpuTimer						(const char *title)
{
    struct timeval tim;
    static double t0                = 0.0;
    static double totalTimeFPS      = 0.0;
    static int    frames            = 0;

    double t1                = 0.0;
    double timePerFrame      = 0.0;
    static double totalTimePerFrame = 0.0;

    char  str[80];

    if(t0 == 0.0) {
        gettimeofday(&tim, NULL);
        t0 = tim.tv_sec + (tim.tv_usec / 1000000.0);
    }

    // Get time
    gettimeofday(&tim, NULL);
    t1                 = tim.tv_sec + (tim.tv_usec / 1000000.0);
    timePerFrame       = t1 - t0;
    totalTimeFPS      += timePerFrame;
    t0                 = t1;

    // Count fps (ms)
    ++frames;
    totalTimePerFrame += timePerFrame*1000;
    if(totalTimeFPS >= (float)FPS_TIME_PERIOD) {
        float fps = (float)frames / (float)totalTimeFPS;
        float ms  = (float)totalTimePerFrame/(float)frames;
        sprintf(str, "%s (%6.3f fps) (%6.3f ms)\n", title, fps, ms);
#ifdef FPS_DISPLAY
        _eglutStoreName(str);
#elif  INFO_DISPLAY
        printf("%s", str);
#endif

        frames            = 0;
        totalTimeFPS      = 0.0;
        totalTimePerFrame = 0.0;
    }

    return timePerFrame;
}
