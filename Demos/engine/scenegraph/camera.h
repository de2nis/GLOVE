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

#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "../utilities/debug.h"
#include "../utilities/linmath.h"

typedef struct openGL_camera_t
{
   GLfloat   mFov;       // in radians

   // Ortho
   GLfloat   mLeft;
   GLfloat   mRight;
   GLfloat   mBottom;
   GLfloat   mTop;

   GLfloat   mNear;
   GLfloat   mFar;

   vec3      mEye;
   vec3      mOrigin;
   vec3      mUp;

   GLfloat   mSpinAngle;

   mat4x4    mProjectionMatrix;
   mat4x4    mViewMatrix;
   mat4x4    mWorldMatrix;

} openGL_camera_t;

void InitCamera(struct openGL_camera_t *camera);
void RotateCamera(struct openGL_camera_t   *camera, const vec3 axis);

#endif // __CAMERA_H_
