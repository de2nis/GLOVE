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
 *  @file       contextStatePixelOperations.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Per-Pixel Operations
 *
 */

#include "context.h"

void
Context::PixelStorei(GLenum pname, GLint param)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(pname != GL_PACK_ALIGNMENT && pname != GL_UNPACK_ALIGNMENT) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(param != 1 && param != 2 && param != 4 && param != 8) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    switch(pname) {
    case GL_PACK_ALIGNMENT:     mStateManager.GetPixelStorageState()->SetPixelStorePack(param); break;
    case GL_UNPACK_ALIGNMENT:   mStateManager.GetPixelStorageState()->SetPixelStoreUnpack(param); break;
    default: NOT_REACHED();     break;
    }
}

void
Context::ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(width < 0 || height < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(format != GL_ALPHA     && format != GL_RGB && format != GL_RGBA &&
       format != GL_LUMINANCE && format != GL_LUMINANCE_ALPHA) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(type != GL_UNSIGNED_BYTE          && type != GL_UNSIGNED_SHORT_5_6_5 &&
       type != GL_UNSIGNED_SHORT_4_4_4_4 && type != GL_UNSIGNED_SHORT_5_5_5_1) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if( (type == GL_UNSIGNED_SHORT_5_6_5                                          && format != GL_RGB)  ||
       ((type == GL_UNSIGNED_SHORT_5_5_5_1 || type == GL_UNSIGNED_SHORT_4_4_4_4)  && format != GL_RGBA) ||
        (type == GL_UNSIGNED_BYTE                                                 && format != GL_RGBA)) {
        RecordError(GL_INVALID_OPERATION);
        return;
   }

   if(mWriteFBO != mSystemFBO && mWriteFBO->CheckStatus() != GL_FRAMEBUFFER_COMPLETE) {
       RecordError(GL_INVALID_FRAMEBUFFER_OPERATION);
       return;
   }

    Texture* activeTexture   = mWriteFBO->GetColorAttachmentTexture();
    GLenum srcInternalFormat = GlFormatToGlInternalFormat(activeTexture->GetFormat(), activeTexture->GetType());
    GLenum dstInternalFormat = GlFormatToGlInternalFormat(format, type);
    ImageRect srcRect(x, y, width, height,
                      GlInternalFormatTypeToNumElements(srcInternalFormat, activeTexture->GetType()),
                      GlTypeToElementSize(activeTexture->GetType()),
                      Texture::GetDefaultInternalAlignment());
    ImageRect dstRect(0, 0, width, height,
                      GlInternalFormatTypeToNumElements(dstInternalFormat, type),
                      GlTypeToElementSize(type),
                      mStateManager.GetPixelStorageState()->GetPixelStorePack());

    // The selected image has to be read with reverted y offset from Vulkan layer
    srcRect.y = activeTexture->GetInvertedYOrigin(&srcRect);
    activeTexture->CopyPixelsToHost(&srcRect, &dstRect, 0, 0, dstInternalFormat, pixels);

#if GLOVE_SAVE_READPIXELS_TO_FILE == true
    static int calls = 0;
    char fileName[64];
    snprintf(fileName, 64, "screen%d_%dx%d.rgba", calls++, width, height);
    FILE *fp = fopen(fileName, "w");
    if(fp) {
        fwrite(pixels, dstRect.GetRectBufferSize(), 1, fp);
        fclose(fp);
    }
#endif
}
