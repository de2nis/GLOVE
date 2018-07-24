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
 *  @file       rect.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      (Image) Rectangle and Pixel Convertions in GLOVE
 *
 *  @scope
 *
 *  Defines a rectangle object for image transfer operations
 *  and pixel convertion functions for converting buffers
 *  between different formats
 *
 */

#include "rect.h"
#include "utils/glLogger.h"

#include <string.h>

#ifdef NDEBUG
#   define NOT_REACHED()                                printf("You shouldn't be here. (function %s at line %d of file %s)\n", __func__, __LINE__, __FILE__)
#   define NOT_FOUND_ENUM(inv_enum)                     printf("Invalid enum: %#04x. (function %s at line %d of file %s)\n", inv_enum, __func__, __LINE__, __FILE__)
#else
#include <assert.h>
#   define NOT_REACHED()                                assert(0 && "You shouldn't be here.")
#   define NOT_FOUND_ENUM(inv_enum)                     { printf("Invalid enum: %#04x\n", inv_enum); assert(0); }
#endif // NDEBUG

Rect::Rect(int _x, int _y, int _width, int _height)
: x(_x), y(_y), width(_width), height(_height)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

Rect::~Rect()
{
    FUN_ENTRY(GL_LOG_TRACE);
}

ImageRect::ImageRect()
: Rect(0, 0, 0, 0), mNumElements(1), mSizeElement(1), mAlignment(1)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

ImageRect::ImageRect(const Rect& rect, int numElements, int sizeElement, int alignment)
: Rect(rect.x, rect.y, rect.width, rect.height), mNumElements(numElements), mSizeElement(sizeElement), mAlignment(alignment)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

ImageRect::ImageRect(int _x, int _y, int _width, int _height, int numElements, int sizeElement, int alignment)
: Rect(_x, _y, _width, _height), mNumElements(numElements), mSizeElement(sizeElement), mAlignment(alignment)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

ImageRect::~ImageRect()
{
    FUN_ENTRY(GL_LOG_TRACE);
}

// converts a buffer between different types
// for example, a buffer containing uint8_t to uint16_t
template<typename SourceType, typename DestType>
bool ConvertBuffer(const void *srcData, void *dstData, size_t elemCount) {

    assert(sizeof(DestType) >= sizeof(SourceType));

    if(srcData == nullptr){
        return false;
    }

    const SourceType* srcBuffer = static_cast<const SourceType*>(srcData);
    DestType* dstBuffer = static_cast<DestType*>(dstData);

    for(size_t i = 0; i < elemCount; ++i) {
        dstBuffer[i] = static_cast<DestType>(srcBuffer[i]);
    }
    return true;
}
template bool ConvertBuffer<uint8_t, uint16_t>(const void *, void *, size_t);

// inverts an image
void
InvertImageYAxis(uint8_t *image, const ImageRect* rect)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(rect->height < 2) {
        return;
    }

    assert(rect->mNumElements > 0);

    // size of an entire row in bytes
    const uint32_t rowStride = rect->GetRectAlignedRowInBytes();

    uint8_t *tmpRow = new uint8_t[rowStride];
    uint8_t *srcRow = static_cast<uint8_t *>(image);
    uint8_t *dstRow = static_cast<uint8_t *>(image) + (rect->height - 1) * rowStride;

    // switch rows
    for(uint32_t i = 0; i < (uint32_t)(rect->height / 2); ++i) {
        memcpy(tmpRow, dstRow, rowStride);
        memcpy(dstRow, srcRow, rowStride);
        memcpy(srcRow, tmpRow, rowStride);

        srcRow += rowStride;
        dstRow -= rowStride;
    }

    delete[] tmpRow;
}

// converts and copies pixels between two buffers with different formats
// e.g., copies RGB565 pixels to BGRA8888
void
CopyPixelsConvert(
            const ImageRect* srcRect,
            const void* srcData,
            const ImageRect* dstRect,
            void* dstData,
            Color (*SrcColorFunPtr)(const uint8_t*),
            void (*DstColorFunPtr)(Color&, uint8_t*))
{

    // size of an entire row in bytes
    const uint32_t srcRowStride = srcRect->GetRectAlignedRowInBytes();
    const uint32_t dstRowStride = dstRect->GetRectAlignedRowInBytes();

    // rectangle offset in the memory block
    const uint32_t srcCurrentRowIndex = srcRect->GetStartRowIndex(srcRowStride);
    const uint32_t dstCurrentRowIndex = dstRect->GetStartRowIndex(dstRowStride);

    // obtain ptr locations with the byte offset
    const uint8_t* srcPtr = static_cast<const uint8_t*>(srcData) + srcCurrentRowIndex;
    uint8_t* dstPtr = static_cast<uint8_t*>(dstData) + dstCurrentRowIndex;

    // perform the convertion
    for(int row = 0; row < srcRect->height; ++row) {
        for(int col = 0; col < srcRect->width; ++col) {
            const uint32_t srcIndex = col * srcRect->GetPixelByteOffset();
            const uint32_t dstIndex = col * dstRect->GetPixelByteOffset();
            Color color = SrcColorFunPtr(&srcPtr[srcIndex]);
            DstColorFunPtr(color, &dstPtr[dstIndex]);
        }
        // offset by the number of bytes per row
        dstPtr = dstPtr + dstRowStride;
        srcPtr = srcPtr + srcRowStride;
    }
}

// copies pixels between two buffers
// buffers must have the same format but may have different alignment
void
CopyPixelsNoConvertion(
            const ImageRect* srcRect,
            const void* srcData,
            const ImageRect* dstRect,
            void* dstData)
{
    assert(srcRect->mNumElements == dstRect->mNumElements);

    // size of an entire row in bytes
    uint32_t srcRowStride = srcRect->GetRectAlignedRowInBytes();
    uint32_t dstRowStride = dstRect->GetRectAlignedRowInBytes();

    // rectangle offset in the memory block
    uint32_t srcCurrentRowIndex = srcRect->GetStartRowIndex(srcRowStride);
    uint32_t dstCurrentRowIndex = dstRect->GetStartRowIndex(dstRowStride);

    // obtain ptr locations with the byte offset
    const uint8_t* srcPtr = static_cast<const uint8_t*>(srcData) + srcCurrentRowIndex;
    uint8_t* dstPtr = static_cast<uint8_t*>(dstData) + dstCurrentRowIndex;

    // get the buffer size for each row containing the actual data
    // (i.e., without any padding applied)
    const uint32_t dataRowSize = srcRect->GetDataRowSize();

    // copy each row separately
    for(int row = 0; row < srcRect->height; ++row) {
        memcpy(static_cast<void*>(dstPtr), static_cast<const void*>(srcPtr), dataRowSize);
        // offset by the number of bytes per row
        srcPtr += srcRowStride;
        dstPtr += dstRowStride;
    }
}

// copies and converts pixels between buffers
void
ConvertPixels(GLenum srcFormat, GLenum dstFormat,
              ImageRect* srcRect,
              const void* srcData,
              ImageRect* dstRect,
              void* dstData)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    switch(srcFormat) {
    case GL_BGRA8_EXT:
    case GL_BGRA_EXT:
        switch(dstFormat) {
        case GL_BGRA8_EXT:
        case GL_BGRA_EXT:
            CopyPixelsNoConvertion(srcRect, srcData, dstRect, dstData);
            break;
        case GL_RGBA:
        case GL_RGBA8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromBGRA, &Color::ConvertToRGBA);
            break;
        case GL_LUMINANCE_ALPHA:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromBGRA, &Color::ConvertToLuminanceAlpha);
            break;
        case GL_LUMINANCE:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromBGRA, &Color::ConvertToLuminance);
            break;
        case GL_ALPHA:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromBGRA, &Color::ConvertToAlpha);
            break;
        case GL_RGB:
        case GL_RGB8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromBGRA, &Color::ConvertToRGB);
            break;

        default: NOT_FOUND_ENUM(dstFormat); break;
        } break;

    case GL_RGBA:
    case GL_RGBA8_OES: {
        switch(dstFormat) {
        case GL_RGBA:
        case GL_RGBA8_OES:
            CopyPixelsNoConvertion(srcRect, srcData, dstRect, dstData);
            break;
        case GL_RGB:
        case GL_RGB8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromRGBA, &Color::ConvertToRGB);
            break;
        case GL_ALPHA:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromRGBA, &Color::ConvertToAlpha);
            break;

        default: NOT_FOUND_ENUM(dstFormat); break;
        }
    } break;

    case GL_RGB:
    case GL_RGB8_OES: {
        switch(dstFormat) {
        case GL_RGB:
        case GL_RGB8_OES:
            CopyPixelsNoConvertion(srcRect, srcData, dstRect, dstData);
            break;
        case GL_RGBA8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromRGB, &Color::ConvertToRGBA);
            break;
        default: NOT_FOUND_ENUM(dstFormat); break;
        }
    } break;

    case GL_LUMINANCE_ALPHA: {
        switch(dstFormat) {
        case GL_LUMINANCE_ALPHA:
            CopyPixelsNoConvertion(srcRect, srcData, dstRect, dstData);
            break;
        case GL_LUMINANCE:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromLuminanceAlpha, &Color::ConvertToLuminance);
            break;
        case GL_RGBA:
        case GL_RGBA8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromLuminanceAlpha, &Color::ConvertToRGBA);
            break;
        default: NOT_FOUND_ENUM(dstFormat); break;
        }
    } break;

    case GL_LUMINANCE: {
        switch(dstFormat) {
        case GL_LUMINANCE:
            CopyPixelsNoConvertion(srcRect, srcData, dstRect, dstData);
            break;
        case GL_LUMINANCE_ALPHA:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromLuminance, &Color::ConvertToLuminanceAlpha);
            break;
        case GL_RGBA:
        case GL_RGBA8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromLuminance, &Color::ConvertToRGBA);
            break;
        default: NOT_FOUND_ENUM(dstFormat); break;
        }
    } break;

    case GL_ALPHA: {
        switch(dstFormat) {
        case GL_ALPHA:
            CopyPixelsNoConvertion(srcRect, srcData, dstRect, dstData);
            break;
        case GL_RGBA:
        case GL_RGBA8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::FromAlpha, &Color::ConvertToRGBA);
            break;
        default: NOT_FOUND_ENUM(dstFormat); break;
        }
    } break;

    case GL_RGBA4:
        switch(dstFormat) {
        case GL_RGBA4:
            CopyPixelsNoConvertion(srcRect, srcData, dstRect, dstData);
            break;
        case GL_RGBA:
        case GL_RGBA8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::From4444, &Color::ConvertToRGBA);
            break;
        default: NOT_FOUND_ENUM(dstFormat); break;
        }
        break;

    case GL_RGB5_A1:
        switch(dstFormat) {
        case GL_RGB5_A1:
            CopyPixelsNoConvertion(srcRect, srcData, dstRect, dstData);
            break;
        case GL_RGBA:
        case GL_RGBA8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::From5551, &Color::ConvertToRGBA);
            break;
        default: NOT_FOUND_ENUM(dstFormat); break;
        }
        break;

    case GL_RGB565:
        switch(dstFormat) {
        case GL_RGB565:
            CopyPixelsNoConvertion(srcRect, srcData, dstRect, dstData);
            break;
        case GL_RGBA:
        case GL_RGB8_OES:
        case GL_RGBA8_OES:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::From565, &Color::ConvertToRGBA);
            break;
        case GL_LUMINANCE:
            CopyPixelsConvert(srcRect, srcData, dstRect, dstData, &Color::From565, &Color::ConvertToLuminance);
            break;
        default: NOT_FOUND_ENUM(dstFormat); break;
        }
        break;

    default: NOT_FOUND_ENUM(srcFormat); break;
    }
}
