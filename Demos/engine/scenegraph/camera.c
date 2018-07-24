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

#include "camera.h"

void InitCamera(struct openGL_camera_t *camera)
{
    // World
    camera->mSpinAngle  = 1.0f;

    // Perspective
    camera->mFov        = (float)degreesToRadians(45.0f);
    camera->mNear       = 1.0f;
    camera->mFar        = 100.0f;

    // Orthographic
    camera->mLeft       = -2.0f;
    camera->mRight      =  2.0f;
    camera->mBottom     = -2.0f;
    camera->mTop        =  2.0f;

    // View
    camera->mEye[0]     = 0.0f;
    camera->mEye[1]     = 3.0f;
    camera->mEye[2]     = 5.0f;

    camera->mOrigin[0]  = 0.0f;
    camera->mOrigin[1]  = 0.0f;
    camera->mOrigin[2]  = 0.0f;

    camera->mUp[0]      = 0.0f;
    camera->mUp[1]      = 1.0f;
    camera->mUp[2]      = 0.0f;

    mat4x4_identity(camera->mWorldMatrix);
    mat4x4_look_at (camera->mViewMatrix, camera->mEye, camera->mOrigin, camera->mUp);
}

void RotateCamera(struct openGL_camera_t *camera, const vec3 axis)
{
    mat4x4 W;
    mat4x4_dup(W, camera->mWorldMatrix);
    mat4x4_rotate(camera->mWorldMatrix, W, axis[0], axis[1], axis[2], (float)degreesToRadians(camera->mSpinAngle));
}
