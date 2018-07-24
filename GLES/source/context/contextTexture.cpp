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
 *  @file       contextTexture.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API calls related to Textures
 *
 */

#include "context.h"

#define ISPOWEROFTWO(x)                                 ((x != 0) && !(x & (x - 1)))

void
Context::ActiveTexture(GLenum texture)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(texture < GL_TEXTURE0 || texture >= GL_TEXTURE0 + GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    mStateManager.GetActiveObjectsState()->SetActiveTextureUnit(texture);
}

void
Context::BindTexture(GLenum target, GLuint texture)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    Texture *tex = nullptr;
    if(texture)
    {
        tex = mResourceManager.GetTexture(texture);

        if(tex->GetTarget() == GL_INVALID_VALUE) {
            tex->SetVkContext(mVkContext);
            tex->SetTarget(target);
            tex->SetVkFormat(VK_FORMAT_R8G8B8A8_UNORM);
            tex->SetVkImageUsage(static_cast<VkImageUsageFlagBits>(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT));
            tex->SetVkImageTarget(target == GL_TEXTURE_2D ? vulkanAPI::Image::VK_IMAGE_TARGET_2D : vulkanAPI::Image::VK_IMAGE_TARGET_CUBE);
            tex->InitState();
        } else if(tex->GetTarget() != target) {
            RecordError(GL_INVALID_OPERATION);
            return;
        }
    } else {
        tex = mResourceManager.GetDefaultTexture(target);
    }

    mStateManager.GetActiveObjectsState()->SetActiveTexture(target, tex);
    if(mStateManager.GetActiveShaderProgram() != nullptr) {
        mStateManager.GetActiveShaderProgram()->EnableUpdateOfDescriptorSets();
    }
}

void
Context::DeleteTextures(GLsizei n, const GLuint* textures)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(n < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    while (n-- != 0) {
        uint32_t texture = *textures++;

        if (texture && mResourceManager.TextureExists(texture)) {
            Texture *tex = mResourceManager.GetTexture(texture);

            if(tex == mWriteFBO->GetColorAttachmentTexture()) {
                mWriteFBO->SetColorAttachmentTexture(nullptr);
                mWriteFBO->SetColorAttachmentType(GL_NONE);
                mWriteFBO->SetColorAttachmentName(0);
            }

            if(tex == mWriteFBO->GetDepthAttachmentTexture()) {
                mWriteFBO->SetDepthAttachmentTexture(nullptr);
                mWriteFBO->SetDepthAttachmentType(GL_NONE);
                mWriteFBO->SetDepthAttachmentName(0);
            }

            if(tex == mWriteFBO->GetStencilAttachmentTexture()) {
                mWriteFBO->SetStencilAttachmentTexture(nullptr);
                mWriteFBO->SetStencilAttachmentType(GL_NONE);
                mWriteFBO->SetStencilAttachmentName(0);
            }

            GLenum target = tex->GetTarget();
            for(int i = 0; i < GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS && target != GL_INVALID_VALUE; ++i) {
                if(mStateManager.GetActiveObjectsState()->EqualsActiveTexture(target, i, tex)) {
                    mStateManager.GetActiveObjectsState()->SetActiveTexture(target, i, mResourceManager.GetDefaultTexture(target));
                }
            }

            mResourceManager.DeallocateTexture(texture);
        }
    }
}

void
Context::GenerateMipmap(GLenum target)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    Texture *activeTexture = mStateManager.GetActiveObjectsState()->GetActiveTexture(target);

    if(!ISPOWEROFTWO(activeTexture->GetWidth()) || !ISPOWEROFTWO(activeTexture->GetHeight())) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(activeTexture->IsCompressed()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    if(activeTexture->GetTarget() == GL_TEXTURE_CUBE_MAP && !activeTexture->IsCompleted()) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    activeTexture->GenerateMipmaps(mStateManager.GetHintAspectsState()->GetMode(GL_GENERATE_MIPMAP_HINT));
}

void
Context::GenTextures(GLsizei n, GLuint* textures)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(n < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    while (n != 0) {
        *textures++ = mResourceManager.AllocateTexture();
        --n;
    }
}

void
Context::TexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    TexParameteri(target, pname, (GLint)param);
}

void
Context::TexParameterfv(GLenum target, GLenum pname, const GLfloat* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    TexParameteri(target, pname, (GLint)params[0]);
}

void
Context::TexParameteri(GLenum target, GLenum pname, GLint param)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(pname != GL_TEXTURE_WRAP_S     && pname != GL_TEXTURE_WRAP_T &&
       pname != GL_TEXTURE_MIN_FILTER && pname != GL_TEXTURE_MAG_FILTER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    Texture *activeTexture = mStateManager.GetActiveObjectsState()->GetActiveTexture(target);

    switch(pname) {
    case GL_TEXTURE_WRAP_S:
        if(param != GL_CLAMP_TO_EDGE && param != GL_REPEAT && param != GL_MIRRORED_REPEAT) {
            RecordError(GL_INVALID_ENUM);
            return;
        }
        activeTexture->SetWrapS(param);
        break;
    case GL_TEXTURE_WRAP_T:
        if(param != GL_CLAMP_TO_EDGE && param != GL_REPEAT && param != GL_MIRRORED_REPEAT) {
            RecordError(GL_INVALID_ENUM);
            return;
        }
        activeTexture->SetWrapT(param);
        break;
    case GL_TEXTURE_MIN_FILTER:
        if(param != GL_NEAREST && param != GL_LINEAR && param != GL_NEAREST_MIPMAP_NEAREST &&
           param != GL_NEAREST_MIPMAP_LINEAR && param != GL_LINEAR_MIPMAP_LINEAR && param != GL_LINEAR_MIPMAP_NEAREST) {
            RecordError(GL_INVALID_ENUM);
            return;
        }
        activeTexture->SetMinFilter(param);
        break;
    case GL_TEXTURE_MAG_FILTER:
        if(param != GL_NEAREST && param != GL_LINEAR) {
            RecordError(GL_INVALID_ENUM);
            return;
        }
        activeTexture->SetMagFilter(param);
        break;
    default:
        break;
    }
}

void
Context::TexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    TexParameteri(target, pname, params[0]);
}

void
Context::GetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(pname != GL_TEXTURE_WRAP_S     && pname != GL_TEXTURE_WRAP_T &&
       pname != GL_TEXTURE_MIN_FILTER && pname != GL_TEXTURE_MAG_FILTER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    Texture *activeTexture = mStateManager.GetActiveObjectsState()->GetActiveTexture(target);

    switch(pname) {
    case GL_TEXTURE_WRAP_S:                     *params = static_cast<GLfloat>(activeTexture->GetWrapS());      break;
    case GL_TEXTURE_WRAP_T:                     *params = static_cast<GLfloat>(activeTexture->GetWrapT());      break;
    case GL_TEXTURE_MIN_FILTER:                 *params = static_cast<GLfloat>(activeTexture->GetMinFilter());  break;
    case GL_TEXTURE_MAG_FILTER:                 *params = static_cast<GLfloat>(activeTexture->GetMagFilter());  break;
    default:                                    break;
    }
}

void
Context::GetTexParameteriv(GLenum target, GLenum pname, GLint* params)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(pname != GL_TEXTURE_WRAP_S     && pname != GL_TEXTURE_WRAP_T &&
       pname != GL_TEXTURE_MIN_FILTER && pname != GL_TEXTURE_MAG_FILTER) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    Texture *activeTexture = mStateManager.GetActiveObjectsState()->GetActiveTexture(target);

    switch(pname) {
    case GL_TEXTURE_WRAP_S:                     *params = activeTexture->GetWrapS();      break;
    case GL_TEXTURE_WRAP_T:                     *params = activeTexture->GetWrapT();      break;
    case GL_TEXTURE_MIN_FILTER:                 *params = activeTexture->GetMinFilter();  break;
    case GL_TEXTURE_MAG_FILTER:                 *params = activeTexture->GetMagFilter();  break;
    default:                                    break;
    }
}

GLboolean
Context::IsTexture(GLuint texture)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return (texture && mResourceManager.TextureExists(texture) && mResourceManager.GetTexture(texture)->GetTarget() != GL_INVALID_VALUE) ? GL_TRUE : GL_FALSE;
}

void
Context::TexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && (target < GL_TEXTURE_CUBE_MAP_POSITIVE_X || target > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)) {
        RecordError(GL_INVALID_ENUM);
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

    if((target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) && (width != height)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(internalformat != GL_ALPHA     && internalformat != GL_RGB && internalformat != GL_RGBA &&
       internalformat != GL_LUMINANCE && internalformat != GL_LUMINANCE_ALPHA) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(level < 0 || border || (width < 0 || height < 0) ||
       ((width > GLOVE_MAX_TEXTURE_SIZE || height > GLOVE_MAX_TEXTURE_SIZE) && target == GL_TEXTURE_2D) ||
       ((width > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE || height > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE) && target != GL_TEXTURE_2D)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if((level > log2(GLOVE_MAX_TEXTURE_SIZE)          && target == GL_TEXTURE_2D) ||
       (level > log2(GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE) && target != GL_TEXTURE_2D)) {
        RecordError(GL_INVALID_VALUE);
        return;
     }

     if(internalformat != format) {
         RecordError(GL_INVALID_OPERATION);
         return;
     }

     if((type == GL_UNSIGNED_BYTE && format != GL_RGBA && format != GL_RGB &&
         format != GL_LUMINANCE_ALPHA && format != GL_LUMINANCE && format != GL_ALPHA) ||
         (type == GL_UNSIGNED_SHORT_5_6_5                                          && format != GL_RGB) ||
         ((type == GL_UNSIGNED_SHORT_4_4_4_4 || type == GL_UNSIGNED_SHORT_5_5_5_1) && format != GL_RGBA)) {
         RecordError(GL_INVALID_OPERATION);
         return;
     }

    if(!width || !height) {
        return;
    }

    GLint layer = (target == GL_TEXTURE_2D) ? 0 : target - GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    Texture *activeTexture = mStateManager.GetActiveObjectsState()->GetActiveTexture(target);

    // copy the buffer contents to the texture
    activeTexture->SetState(width, height, level, layer, format, type, mStateManager.GetPixelStorageState()->GetPixelStoreUnpack(), pixels);

    if(activeTexture->IsCompleted()) {
        // pass contents to the driver
        activeTexture->Allocate();
    }
}

void
Context::TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && (target < GL_TEXTURE_CUBE_MAP_POSITIVE_X || target > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(level < 0 || width < 0 || height < 0 || xoffset < 0 || yoffset < 0) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(format != GL_ALPHA && format != GL_RGB && format != GL_RGBA &&
       format != GL_LUMINANCE && format != GL_LUMINANCE_ALPHA) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(type != GL_UNSIGNED_BYTE && type != GL_UNSIGNED_SHORT_5_6_5 &&
       type != GL_UNSIGNED_SHORT_4_4_4_4 && type != GL_UNSIGNED_SHORT_5_5_5_1) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(level > log2(GLOVE_MAX_TEXTURE_SIZE)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if((type == GL_UNSIGNED_BYTE && format != GL_RGBA && format != GL_RGB &&
        format != GL_LUMINANCE_ALPHA && format != GL_LUMINANCE && format != GL_ALPHA) ||
        (type == GL_UNSIGNED_SHORT_5_6_5                                          && format != GL_RGB) ||
        ((type == GL_UNSIGNED_SHORT_4_4_4_4 || type == GL_UNSIGNED_SHORT_5_5_5_1) && format != GL_RGBA)) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    Texture *activeTexture = mStateManager.GetActiveObjectsState()->GetActiveTexture(target);
    if(activeTexture->GetWidth()  < (xoffset + width ) ||
       activeTexture->GetHeight() < (yoffset + height)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    // TODO:: We could pass a default subtexture instead
    if(pixels == nullptr) {
        return;
    }

    GLint layer = (target == GL_TEXTURE_2D) ? 0 : target - GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    GLenum srcInternalFormat = GlFormatToGlInternalFormat(format, type);
    GLenum dstInternalFormat = activeTexture->GetInternalFormat();
    ImageRect srcRect(0,       0,       width, height,
                      GlInternalFormatTypeToNumElements(srcInternalFormat, type),
                      GlTypeToElementSize(type),
                      mStateManager.GetPixelStorageState()->GetPixelStoreUnpack());
    ImageRect dstRect(xoffset, yoffset, width, height,
                      GlInternalFormatTypeToNumElements(dstInternalFormat, activeTexture->GetType()),
                      GlTypeToElementSize(activeTexture->GetType()),
                      Texture::GetDefaultInternalAlignment());

    // copy the buffer contents to the texture
    activeTexture->SetSubState(&srcRect, &dstRect, level, layer, srcInternalFormat, pixels);

    if(activeTexture->IsCompleted()) {
        // pass contents to the driver
        activeTexture->Allocate();
    }
}

void
Context::CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && (target < GL_TEXTURE_CUBE_MAP_POSITIVE_X || target > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(internalformat != GL_ALPHA     && internalformat != GL_RGB && internalformat != GL_RGBA &&
       internalformat != GL_LUMINANCE && internalformat != GL_LUMINANCE_ALPHA) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if((target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) && (width != height)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if((level < 0) || (border) || (width < 0 || height < 0) ||
       ((width > GLOVE_MAX_TEXTURE_SIZE           || height > GLOVE_MAX_TEXTURE_SIZE)          && target == GL_TEXTURE_2D) ||
       ((width > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE  || height > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE) && target != GL_TEXTURE_2D)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if((level > log2(GLOVE_MAX_TEXTURE_SIZE)          && target == GL_TEXTURE_2D) ||
       (level > log2(GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE) && target != GL_TEXTURE_2D)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(mWriteFBO != mSystemFBO && mWriteFBO->CheckStatus() != GL_FRAMEBUFFER_COMPLETE) {
        RecordError(GL_INVALID_FRAMEBUFFER_OPERATION);
        return;
    }

    Texture *fbTexture = mWriteFBO->GetColorAttachmentTexture();
    Texture *activeTexture = mStateManager.GetActiveObjectsState()->GetActiveTexture(target);

    const GLenum fbFormat = fbTexture->GetFormat();
    if((fbFormat == GL_ALPHA  && internalformat != GL_ALPHA) ||
       (fbFormat == GL_RGB    &&(internalformat != GL_LUMINANCE && internalformat != GL_RGB))) {
       RecordError(GL_INVALID_OPERATION);
       return;
    }

    // transfer the data to the cpu and upload it to a new texture
    GLenum srcInternalFormat = GlFormatToGlInternalFormat(fbFormat, fbTexture->GetType());
    GLenum dstInternalFormat = internalformat;
    GLenum dstType           = GlInternalFormatToGlType(dstInternalFormat);
    ImageRect srcRect(x, y, width, height,
                      GlInternalFormatTypeToNumElements(srcInternalFormat, fbTexture->GetType()),
                      GlTypeToElementSize(fbTexture->GetType()),
                      Texture::GetDefaultInternalAlignment());
    ImageRect dstRect(0, 0, width, height,
                      GlInternalFormatTypeToNumElements(dstInternalFormat, dstType),
                      GlTypeToElementSize(dstType),
                      Texture::GetDefaultInternalAlignment());

    const GLint    layer   = (target == GL_TEXTURE_2D) ? 0 : target - GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    const size_t stageSize = dstRect.GetRectBufferSize();
    uint8_t *stagePixels = new uint8_t[stageSize];
    srcRect.y = fbTexture->GetInvertedYOrigin(&srcRect);

    // copy the framebuffer contents to the temp buffer
    // and convert them to the texture's internal format
    fbTexture->CopyPixelsToHost(&srcRect, &dstRect, 0, layer, internalformat, (void *)stagePixels);

    // now copy the temp buffer contents to the texture
    activeTexture->SetState(width, height, level, layer, dstInternalFormat, dstType, Texture::GetDefaultInternalAlignment(), stagePixels);
    delete[] stagePixels;

    if(activeTexture->IsCompleted()) {
        // pass contents to the driver
        activeTexture->Allocate();
    }
}

void
Context::CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && (target < GL_TEXTURE_CUBE_MAP_POSITIVE_X || target > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if((target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) && (width != height)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    Texture *activeTexture = mStateManager.GetActiveObjectsState()->GetActiveTexture(target);
    if((level < 0) || (width < 0 || height < 0) || (xoffset < 0 || yoffset < 0) ||
       (activeTexture->GetWidth() < (xoffset + width) || activeTexture->GetHeight() < (yoffset + height)) ||
       ((width > GLOVE_MAX_TEXTURE_SIZE || height > GLOVE_MAX_TEXTURE_SIZE) && target == GL_TEXTURE_2D) ||
       ((width > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE || height > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE) && target != GL_TEXTURE_2D)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if((level > log2(GLOVE_MAX_TEXTURE_SIZE)          && target == GL_TEXTURE_2D) ||
       (level > log2(GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE) && target != GL_TEXTURE_2D)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(mWriteFBO != mSystemFBO && mWriteFBO->CheckStatus() != GL_FRAMEBUFFER_COMPLETE) {
        RecordError(GL_INVALID_FRAMEBUFFER_OPERATION);
        return;
    }

    Texture *fbTexture = mWriteFBO->GetColorAttachmentTexture();

    const GLenum fbFormat = fbTexture->GetFormat();
    const GLenum internalformat = activeTexture->GetInternalFormat();
    if((fbFormat == GL_ALPHA && internalformat != GL_ALPHA) ||
       (fbFormat == GL_RGB &&(internalformat != GL_LUMINANCE && internalformat != GL_RGB))) {
       RecordError(GL_INVALID_OPERATION);
       return;
    }
    GLint layer = (target == GL_TEXTURE_2D) ? 0 : target - GL_TEXTURE_CUBE_MAP_POSITIVE_X;

     GLenum srcInternalFormat = GlFormatToGlInternalFormat(fbFormat, fbTexture->GetType());
     GLenum dstInternalFormat = internalformat;
     ImageRect srcRect(x,       y,       width, height,
                       GlInternalFormatTypeToNumElements(srcInternalFormat, fbTexture->GetType()),
                       GlTypeToElementSize(fbTexture->GetType()),
                       Texture::GetDefaultInternalAlignment());
     ImageRect dstRect(xoffset, yoffset, width, height,
                       GlInternalFormatTypeToNumElements(dstInternalFormat, activeTexture->GetType()),
                       GlTypeToElementSize(activeTexture->GetType()),
                       Texture::GetDefaultInternalAlignment());

     const size_t stageSize = dstRect.GetRectBufferSize();
     uint8_t *stagePixels = new uint8_t[stageSize];
     srcRect.y = fbTexture->GetInvertedYOrigin(&srcRect);

     // copy the framebuffer subcontents to the temp buffer
     // and convert them to the texture's internal format
     fbTexture->CopyPixelsToHost(&srcRect, &dstRect, 0, layer, dstInternalFormat, (void *)stagePixels);

     srcRect = dstRect;
     srcRect.x = 0; srcRect.y = 0;
     // now copy the temp buffer contents to the texture
     // source and destination rectangles have now similar properties except from their x,y offsets
     activeTexture->SetSubState(&srcRect, &dstRect, level, layer, dstInternalFormat,
                                stagePixels);

     delete[] stagePixels;

     if(activeTexture->IsCompleted()) {
         // pass contents to the driver
         activeTexture->Allocate();
     }
}

void
Context::CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && (target < GL_TEXTURE_CUBE_MAP_POSITIVE_X || target > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(internalformat != GL_ALPHA     && internalformat != GL_RGB && internalformat != GL_RGBA &&
       internalformat != GL_LUMINANCE && internalformat != GL_LUMINANCE_ALPHA) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(level < 0 || border || (width < 0 || height < 0) ||
       ((width > GLOVE_MAX_TEXTURE_SIZE || height > GLOVE_MAX_TEXTURE_SIZE) && target == GL_TEXTURE_2D) ||
       ((width > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE || height > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE) && target != GL_TEXTURE_2D)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if((level > log2(GLOVE_MAX_TEXTURE_SIZE)          && target == GL_TEXTURE_2D) ||
       (level > log2(GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE) && target != GL_TEXTURE_2D)) {
        RecordError(GL_INVALID_VALUE);
        return;
     }

    if(width == 0 || height == 0) {
        return;
    }

    // TODO:
    //GL_INVALID_VALUE is generated if imageSize is not consistent with the format, dimensions, and contents of the specified compressed image data.
    //GL_INVALID_OPERATION is generated if parameter combinations are not supported by the specific compressed internal format as specified in the specific texture compression extension.

    NOT_IMPLEMENTED();
}

void
Context::CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(target != GL_TEXTURE_2D && (target < GL_TEXTURE_CUBE_MAP_POSITIVE_X || target > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(format != GL_ALPHA     && format != GL_RGB && format != GL_RGBA &&
       format != GL_LUMINANCE && format != GL_LUMINANCE_ALPHA) {
        RecordError(GL_INVALID_ENUM);
        return;
    }

    if(level < 0 || width < 0 || height < 0 ||
       ((width > GLOVE_MAX_TEXTURE_SIZE || height > GLOVE_MAX_TEXTURE_SIZE) && target == GL_TEXTURE_2D) ||
       ((width > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE || height > GLOVE_MAX_CUBE_MAP_TEXTURE_SIZE) && target != GL_TEXTURE_2D)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    Texture *tex = mStateManager.GetActiveObjectsState()->GetActiveTexture(target);
    if(tex->GetWidth()  < (xoffset + width ) ||
       tex->GetHeight() < (yoffset + height)) {
        RecordError(GL_INVALID_VALUE);
        return;
    }

    if(tex->GetFormat() != format) {
        RecordError(GL_INVALID_OPERATION);
        return;
    }

    // TODO:
    //GL_INVALID_VALUE is generated if imageSize is not consistent with the format, dimensions, and contents of the specified compressed image data.
    //GL_INVALID_OPERATION is generated if parameter combinations are not supported by the specific compressed internal format as specified in the specific texture compression extension.

    NOT_IMPLEMENTED();
}
