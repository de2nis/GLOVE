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

#include "render_to_texture_filter_invert.h"

static  openGL_mesh_t      mesh_cube;
static  openGL_mesh_t      mesh_screen_quad;
static  openGL_program_t   program_cube;
static  openGL_program_t   program_screen_quad;
static  openGL_camera_t    camera;
static  openGL_viewport_t  viewport;
static  openGL_rendering_t rendering;
static  openGL_shading_t   shading_cube;
static  openGL_shading_t   shading_screen_quad;
static  openGL_fbo_t       fbo;
static  const char        *win_name;

bool InitGL()
{
  // Print GPU specifications
    GpuViewer();

#ifdef BINARY_PROG
// Load Precompilde binary program - CUBE
    if(!LoadProgramBinary(BINARY_PROGRAM_SHADER_CUBE_NAME, 1, &program_cube.mID))
        return false;
// Load Precompilde binary program - QUAD
    if(!LoadProgramBinary(BINARY_PROGRAM_SHADER_QUAD_NAME, 1, &program_screen_quad.mID))
        return false;
#else
// Initialize Shader Program - CUBE
    if(!LoadShader(VERTEX_SHADER_CUBE_NAME    , &program_cube.mVertexShader  , GL_VERTEX_SHADER))
        return false;
    if(!LoadShader(FRAGMENT_SHADER_CUBE_NAME  , &program_cube.mFragmentShader, GL_FRAGMENT_SHADER))
        return false;
    if(!LoadProgram(program_cube.mVertexShader, program_cube.mFragmentShader , &program_cube.mID))
        return false;

// Initialize Shader Program - SCREEN QUAD
    if(!LoadShader(VERTEX_SHADER_QUAD_NAME, &program_screen_quad.mVertexShader, GL_VERTEX_SHADER))
        return false;
    if(!LoadShader(FRAGMENT_SHADER_QUAD_NAME, &program_screen_quad.mFragmentShader, GL_FRAGMENT_SHADER))
        return false;
    if(!LoadProgram(program_screen_quad.mVertexShader, program_screen_quad.mFragmentShader, &program_screen_quad.mID))
        return false;
#endif
// Initialize Mesh - CUBE
    InitMesh      (&mesh_cube       , 3, 12, cube_vertex_buffer_data, sizeof(cube_vertex_buffer_data),
                                             cube_uv_buffer_data    , sizeof(cube_uv_buffer_data)    ,
                                             cube_color_buffer_data , sizeof(cube_color_buffer_data) ,
                                             cube_index_buffer_data , sizeof(cube_index_buffer_data) ,
                                             diffuse_textures, 2);

// Initialize Mesh - SCREEN QUAD
    InitMesh      (&mesh_screen_quad, 2, 2 , squad_vertex_buffer_data, sizeof(squad_vertex_buffer_data),
                                             squad_uv_buffer_data    , sizeof(squad_uv_buffer_data)    ,
                                             squad_color_buffer_data , sizeof(squad_color_buffer_data) ,
                                             squad_index_buffer_data , sizeof(squad_index_buffer_data) ,
                                             diffuse_textures, 0);

// Initialize Camera
    InitCamera    (&camera);

// Basic Rendering Setup, (a) Init Depth & (b) Init Background Color
    InitRendering (&rendering, COLOR_WHITE);

// Basic Shading Setup, (a) Init Shading Count & (b) Init uniform value
    InitShading   (&shading_cube, 1, 0.0f);

// Basic Shading Setup, (a) Init Shading Count & (b) Init uniform value
    InitShading   (&shading_screen_quad, 1, 0.0f);

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
    glUseProgram(program_cube.mID);
    program_cube.mLocationPos    = glGetAttribLocation (program_cube.mID, "v_posCoord_in");
    program_cube.mLocationUV     = glGetAttribLocation (program_cube.mID, "v_textCoord_in");
    program_cube.mLocationColor  = glGetAttribLocation (program_cube.mID, "v_colorCoord_in");
    program_cube.mLocationMVP    = glGetUniformLocation(program_cube.mID, "uniform_mvp");
    glUniform1i(glGetUniformLocation(program_cube.mID, "uniform_texture0"), 0);
    glUniform1i(glGetUniformLocation(program_cube.mID, "uniform_texture1"), 1);
    glUniform1f(glGetUniformLocation(program_cube.mID, "uniform_mix_value"), 1.0);

    glUseProgram(program_screen_quad.mID);
    program_screen_quad.mLocationPos = glGetAttribLocation (program_screen_quad.mID, "v_posCoord_in");
    glUniform1i(glGetUniformLocation(program_screen_quad.mID, "uniform_texture"), 0);
    glUniform2f(glGetUniformLocation(program_screen_quad.mID, "uniform_resolution_div"), 1.0/(float)WIDTH, 1.0/(float)HEIGHT);

    InitFBO(&fbo, WIDTH, HEIGHT);

    mesh_screen_quad.mTexturesNum = 1;
    mesh_screen_quad.mTexture[0]  = fbo.mTextureColor;

#ifdef INFO_DISPLAY
    printf("[Shading    Mode] [%s] [Total Time] [%d sec]\n", shading_titles[shading_screen_quad.mType], KILL_APP_PERIOD);
#endif

// Check for opengGL-relate Errors
    ASSERT_NO_GL_ERROR();

    return true;
}

void DrawGL()
{
// Set Viewport
    glViewport(0, 0, viewport.mWidth, viewport.mHeight);

// Draw to framebuffer object
    glBindFramebuffer(GL_FRAMEBUFFER, fbo.mID);
// Clear Depth & Color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Rotate (model) around the Y axis
    RotateMesh    (&mesh_cube, ROT_AXIS_Y);
// Compute transformation matrix = model * world * projection * view
    TransformMesh (&program_cube, &mesh_cube, &camera);
// Draw Mesh to FBO
    DrawMesh      (&program_cube, &mesh_cube);

// Draw to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
// Clear Depth & Color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Draw Mesh to screen
    DrawMesh(&program_screen_quad, &mesh_screen_quad);

// Check for opengGL-relate Errors
    ASSERT_NO_GL_ERROR();
}

void IdleGL(void)
{
    static float  mix_value         = 0.025f;
    static double totalTimeScript   = 0.0;

    double timePerFrame = GpuTimer(win_name);

    totalTimeScript    += timePerFrame;
    if(totalTimeScript >= (float)KILL_APP_PERIOD)
        KeyboardGL(ESC_KEY);

    eglutPostRedisplay();
}

void DestroyGL(void)
{
// Delete FBO
    DeleteFBO     (&fbo);

// Delete Program
    DeleteProgram (program_cube.mID);
    DeleteProgram (program_screen_quad.mID);

// Delete Mesh
    DeleteMesh    (&mesh_cube);
    DeleteMesh    (&mesh_screen_quad);
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

    glUseProgram(program_screen_quad.mID);
    glUniform2f(glGetUniformLocation(program_screen_quad.mID, "uniform_resolution_div"), 1.0/(float)viewport.mWidth, 1.0/(float)viewport.mHeight);

// Check for opengGL-relate Errors
    ASSERT_NO_GL_ERROR();
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
