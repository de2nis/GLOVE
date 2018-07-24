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
 */

#ifndef __RECT_H__
#define __RECT_H__

#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include <cmath>
#include <algorithm>
#include "utils/color.hpp"

class Rect {

public:

    int   x;
    int   y;
    int   width;
    int   height;

    Rect(int _x = 0, int _y = 0, int _width = 0, int _height = 0);

    virtual ~Rect();
};

class ImageRect : public Rect {
private:
    inline unsigned int GetRowInBytes(int _width) const
    {
       int unalignedRowSize = mNumElements * mSizeElement * _width;
       return mAlignment*(unalignedRowSize/mAlignment + ((unalignedRowSize % mAlignment) ? 1 : 0));
    }

public:
    int   mNumElements;
    int   mSizeElement;
    int   mAlignment;

    ImageRect();
    ImageRect(const Rect& rect, int numElements, int sizeElement, int alignment);
    ImageRect(int _x, int _y, int _width, int _height, int numElements, int sizeElement, int alignment);
    ~ImageRect();

// Get Functions
    inline unsigned int GetRectBufferSize(void)                           const { return GetRectAlignedRowInBytes() * height;}
    inline unsigned int GetRectAlignedRowInBytes(void)                    const { return GetRowInBytes(width); }
    inline unsigned int GetDataRowSize(void)                              const { return width * mNumElements * mSizeElement; }
    inline unsigned int GetStartRowIndex(unsigned int rowStride)          const { return (x * mNumElements * mSizeElement) + (y * rowStride); }
    inline unsigned int GetPixelByteOffset(void)                          const { return mNumElements * mSizeElement; }
};

template<typename SourceType, typename DestType>
bool                    ConvertBuffer(const void *srcData, void *dstData, size_t elemCount);
void                    InvertImageYAxis(uint8_t *image, const ImageRect* rect);
void                    CopyPixelsNoConvertion(
                        const ImageRect* srcRect,
                        const void* srcData,
                        const ImageRect* dstRect,
                        void* dstData);
void                    CopyPixelsConvert(
                        const ImageRect* srcRect,
                        const void* srcData,
                        const ImageRect* dstRect,
                        void* dstData,
                        Color (*SrcColorFunPtr)(const uint8_t*),
                        void (*DstColorFunPtr)(class Color&, uint8_t*));
void                    ConvertPixels(GLenum srcFormat , GLenum dstFormat,
                        ImageRect* srcRect,
                        const void* srcData,
                        ImageRect* dstRect,
                        void* dstData);

#endif // __RECT_H__
