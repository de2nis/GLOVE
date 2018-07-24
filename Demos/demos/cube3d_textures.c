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

#include "cube3d_textures.h"

static  openGL_mesh_t      mesh_cube;
static  openGL_program_t   program;
static  openGL_camera_t    camera;
static  openGL_viewport_t  viewport;
static  openGL_rendering_t rendering;
static  openGL_shading_t   shading;
static  const char        *win_name;

bool InitGL()
{
  // Print GPU specifications
    GpuViewer();

#ifdef BINARY_PROG
// Load Precompilde binary program
    if(!LoadProgramBinary(BINARY_PROGRAM_SHADER_NAME, 1, &program.mID))
        return false;
#else
// Initialize Shader Program
    if(!LoadShader(VERTEX_SHADER_NAME, &program.mVertexShader, GL_VERTEX_SHADER))
        return false;
    if(!LoadShader(FRAGMENT_SHADER_NAME, &program.mFragmentShader, GL_FRAGMENT_SHADER))
        return false;
    if(!LoadProgram(program.mVertexShader, program.mFragmentShader, &program.mID))
        return false;
#endif
// Initialize Mesh
    InitMesh      (&mesh_cube, 3, 12, cube_vertex_buffer_data, sizeof(cube_vertex_buffer_data),
                                      cube_uv_buffer_data    , sizeof(cube_uv_buffer_data)    ,
                                      cube_color_buffer_data , sizeof(cube_color_buffer_data) ,
                                      cube_index_buffer_data , sizeof(cube_index_buffer_data) ,
                                      diffuse_textures, 2);

// Initialize Camera
    InitCamera    (&camera);

// Basic Rendering Setup, (a) Init Depth & (b) Init Background Color
    InitRendering (&rendering, COLOR_BLACK);

// Basic Shading Setup, (a) Init Shading Count & (b) Init uniform value
    InitShading   (&shading  , 1, 0.0f);

// Culling Setup
    glEnable      (GL_CULL_FACE);
    glCullFace    (GL_BACK);
    glFrontFace   (GL_CW);

// Depth Testing Setup
    glEnable      (GL_DEPTH_TEST);
    glDepthFunc   (GL_LEQUAL);
    glClearDepthf (rendering.mDepth);

// Set Clear Color
    glClearColor  (rendering.mBackgroundColor[0], rendering.mBackgroundColor[1], rendering.mBackgroundColor[2], rendering.mBackgroundColor[3]);

// Upload shader uniforms
    glUseProgram(program.mID);
    program.mLocationPos    = glGetAttribLocation (program.mID, "v_posCoord_in");
    program.mLocationUV     = glGetAttribLocation (program.mID, "v_textCoord_in");
    program.mLocationColor  = glGetAttribLocation (program.mID, "v_colorCoord_in");
    program.mLocationMVP    = glGetUniformLocation(program.mID, "uniform_mvp");
    glUniform1i(glGetUniformLocation(program.mID, "uniform_texture0"), 0);
    glUniform1i(glGetUniformLocation(program.mID, "uniform_texture1"), 1);

#ifdef INFO_DISPLAY
    printf("[Shading    Mode] [%s] [Total Time] [%d sec]\n", shading_titles[shading.mType], KILL_APP_PERIOD);
    printf("[Material   Mode] [%s] \n", material_titles[rendering.mMaterialType]);
    printf("[Projection Mode] [%s] \n", projection_titles[rendering.mProjectionType]);
#endif

// Check for opengGL-relate Errors
    ASSERT_NO_GL_ERROR();

    return true;
}

void DrawGL()
{
// Set Viewport
    glViewport(0, 0, viewport.mWidth, viewport.mHeight);

    if(rendering.mMaterialType == OPAQUE)
    {
// Clear Depth & Color of Screen Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
// Enable Culling
        glDisable(GL_CULL_FACE);

// Disable Depth Test
        glDisable(GL_DEPTH_TEST);
        glDepthMask(false);

// Enable Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// Clear Screen Buffer Color
        glClear(GL_COLOR_BUFFER_BIT);
    }

// Rotate (model) around the Y axis
    RotateMesh(&mesh_cube, ROT_AXIS_Y);

// Uncomment if you want to Rotate Camera around any axis
    RotateCamera(&camera, ROT_AXIS_X);

// Compute transformation matrix = model * world * projection * view
    TransformMesh(&program, &mesh_cube, &camera);

// Draw Scene
    DrawMesh(&program, &mesh_cube);

    if(rendering.mMaterialType == TRANSPARENT)
    {
// Enable Culling
        glEnable(GL_CULL_FACE);
// Enable Depth Test
        glEnable(GL_DEPTH_TEST);
        glDepthMask(true);

// Disable Blending
        glDisable(GL_BLEND);
    }

// Check for opengGL-relate Errors
    ASSERT_NO_GL_ERROR();
}

void IdleGL(void)
{
    static float  mix_value         = 0.025f;
    static double totalTimeScript   = 0.0;

    double timePerFrame = GpuTimer(win_name);

    totalTimeScript   += timePerFrame;
    if(totalTimeScript>= (float)KILL_APP_PERIOD)
        KeyboardGL(ESC_KEY);

// Update Dynamic Values (Uniform)
    shading.mValue    += mix_value;
    if(shading.mValue  < 0.0f || shading.mValue > 1.0f)
        mix_value = -mix_value;
    glUniform1f(glGetUniformLocation(program.mID, "uniform_mix_value"), shading.mValue);

    eglutPostRedisplay();
}

void DestroyGL(void)
{
// Delete Program
    DeleteProgram (program.mID);
// Delete Mesh
    DeleteMesh    (&mesh_cube);
}

void ReshapeGL(int width, int height)
{
// Set viewport
    SetViewport(&viewport, 0, 0, width, height);

// Update the projection matrix since aspect ratio has been modified
    if(rendering.mProjectionType == PERSPECTIVE)
        mat4x4_perspective(camera.mProjectionMatrix, camera.mFov, viewport.mAspectRatio, camera.mNear, camera.mFar);
    else
        mat4x4_ortho      (camera.mProjectionMatrix, camera.mLeft, camera.mRight, camera.mBottom, camera.mTop, camera.mNear, camera.mFar);
}

void KeyboardGL(unsigned char key)
{
   if (key == ESC_KEY) // escape key
   {
// Close app
     DestroyGL();
     if (_eglut->current)
        eglutDestroyWindow(_eglut->current->index);
     _eglutFini();

      exit(0);
   }
   else if (key == 't')
   {
      rendering.mMaterialType = 1 - rendering.mMaterialType;
#ifdef INFO_DISPLAY
      printf("[Material Mode] [%s] \n", material_titles[rendering.mMaterialType]);
#endif
   }
   else if (key == 'p')
   {
// Change projection type (PERSPECTIVE, ORTHOGRAPHIC)
     rendering.mProjectionType = 1 - rendering.mProjectionType;
// Update the projection matrix since projection type has been modified
     if(rendering.mProjectionType == PERSPECTIVE)
         mat4x4_perspective(camera.mProjectionMatrix, camera.mFov, viewport.mAspectRatio, camera.mNear, camera.mFar);
     else
         mat4x4_ortho      (camera.mProjectionMatrix, camera.mLeft, camera.mRight, camera.mBottom, camera.mTop, camera.mNear, camera.mFar);
#ifdef INFO_DISPLAY
     printf("[Projection Mode] [%s] \n", projection_titles[rendering.mProjectionType]);
#endif
   }
}

int main(int argc, char **argv)
{
    win_name = EXECUTABLE_NAME(argv[0]);

    eglutInitWindowSize (WIDTH, HEIGHT);
    eglutInitAPIMask    (EGLUT_OPENGL_ES2_BIT);
    eglutInit           (argc, (const char **)argv);

    int win = eglutCreateWindow(win_name);

    eglutIdleFunc       (IdleGL);
    eglutDisplayFunc    (DrawGL);
    eglutReshapeFunc    (ReshapeGL);
    eglutKeyboardFunc   (KeyboardGL);

    if(InitGL())
      eglutMainLoop();
    DestroyGL      ();

    return 0;
}
