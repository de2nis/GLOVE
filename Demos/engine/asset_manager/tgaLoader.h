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

#ifndef __TGALOADER_H__
#define __TGALOADER_H__

#include "../utilities/debug.h"

typedef	struct
{
    unsigned char	* imageData;							// Image Data (Up To 32 Bits)
    unsigned int	bpp;											// Image Color Depth In Bits Per Pixel
    unsigned int	width;										// Image Width
    unsigned int	height;										// Image Height
    unsigned int	texID;										// Texture ID Used To Select A Texture
    unsigned int	type;											// Image Type (GL_RGB, GL_RGBA)
} Texture;

typedef struct
{
    unsigned char Header[12];							// TGA File Header
} TGAHeader;

typedef struct
{
    unsigned char		header[6];						// First 6 Useful Bytes From The Header
    unsigned int		bytesPerPixel;				// Holds Number Of Bytes Per Pixel Used In The TGA File
    unsigned int		imageSize;						// Used To Store The Image Size When Setting Aside Ram
    unsigned int		temp;									// Temporary Variable
    unsigned int		type;
    unsigned int		Height;								//Height of Image
    unsigned int		Width;								//Width ofImage
    unsigned int		Bpp;									// Bits Per Pixel
} TGA;

TGAHeader tgaheader;                             // TGA header
TGA tga;                                                   // TGA image data

static unsigned char uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
static unsigned char cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header

int          LoadUncompressedTGA	(Texture *, const char *, FILE *);					// Load an Uncompressed file
int          LoadCompressedTGA		(Texture *, const char *, FILE *);					// Load a Compressed file
int          LoadTGA							(Texture * texture, const char *filename);
Texture *LoadGLTexture				(const char *filename);

#endif //__TGALOADER_H__
