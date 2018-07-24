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

#include "framebufferobject.h"

bool InitFBO(struct openGL_fbo_t *fbo, const GLint width, const GLint height)
{
    fbo->mTextureColor = (Texture *)malloc(sizeof(Texture));
    fbo->mTextureColor->width  = width;
    fbo->mTextureColor->height = height;
    fbo->mTextureColor->type   = GL_RGBA;

    // Build the texture that will serve as the color attachment for the framebuffer.
    glGenTextures(1, &fbo->mTextureColor->texID);
    glBindTexture(GL_TEXTURE_2D, fbo->mTextureColor->texID);
    glTexImage2D(GL_TEXTURE_2D, 0, fbo->mTextureColor->type, fbo->mTextureColor->width, fbo->mTextureColor->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Build the texture that will serve as the depth attachment for the framebuffer.
    fbo->mTextureDepth = (Texture *)malloc(sizeof(Texture));
    fbo->mTextureDepth->width  = width;
    fbo->mTextureDepth->height = height;
    fbo->mTextureDepth->type   = GL_DEPTH_COMPONENT16;

    glGenRenderbuffers(1, &fbo->mTextureDepth->texID);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo->mTextureDepth->texID);
    glRenderbufferStorage(GL_RENDERBUFFER, fbo->mTextureDepth->type, fbo->mTextureDepth->width, fbo->mTextureDepth->height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Build the framebuffer.
    glGenFramebuffers(1, &fbo->mID);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->mID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D  , fbo->mTextureColor->texID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER, fbo->mTextureDepth->texID );
    if (!CheckFBOStatus())
        return false;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void DeleteFBO(struct openGL_fbo_t *fbo)
{
    glDeleteRenderbuffers(1, &fbo->mTextureDepth->texID);
    glDeleteFramebuffers(1, &fbo->mID);
}

bool CheckFBOStatus()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "[framebufferobject.c] [CheckFBOStatus()]: Status of framebuffer object is not COMPLETE!\n");
        switch (status) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            fprintf(stderr, "An attachment could not be bound to frame buffer object!");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            fprintf(stderr, "Attachments are missing! At least one image (texture) must be bound to the frame buffer object!");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            fprintf(stderr, "Attempt to use an unsupported format combinaton!");
            break;
        default:
            fprintf(stderr, "Unknown error while attempting to create frame buffer object!");
            break;
        }
        return false;
    }
    return true;
}
