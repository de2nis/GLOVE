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

#include "tgaLoader.h"

int LoadTGA(Texture * texture, const char *filename)
{
    FILE * fTGA;												// File pointer to texture file
    fTGA = fopen(filename, "rb");				// Open file for reading

    if(fTGA == NULL)
        return 0;													// Exit function

    if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)					// Attempt to read 12 byte header from file
    {
        if(fTGA != NULL)
            fclose(fTGA);
        return 0;
    }

    if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)			// See if header matches the predefined header of an Uncompressed TGA image
    {
        LoadUncompressedTGA(texture, filename, fTGA);									// If so, jump to Uncompressed TGA loading code
    }
    else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)// See if header matches the predefined header of an RLE compressed TGA image
    {
        LoadCompressedTGA(texture, filename, fTGA);										// If so, jump to Compressed TGA loading code
    }
    else																														// If header matches neither type
    {
        fclose(fTGA);
        return 0;																											// Exit function
    }

    return 1;																												// All went well, continue on
}

int LoadUncompressedTGA(Texture * texture, const char *filename, FILE *fTGA)	// Load an uncompressed TGA
{
    if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Read TGA header
    {
        if(fTGA != NULL)
            fclose(fTGA);
        return 0;
    }

    texture->width  = tga.header[1] * 256 + tga.header[0];	// Determine The TGA Width	(highbyte*256+lowbyte)
    texture->height = tga.header[3] * 256 + tga.header[2];	// Determine The TGA Height	(highbyte*256+lowbyte)
    texture->bpp		= tga.header[4];												// Determine the bits per pixel
    tga.Width				= texture->width;												// Copy width into local structure
    tga.Height			= texture->height;											// Copy height into local structure
    tga.Bpp					= texture->bpp;													// Copy BPP into local structure

    if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))	// Make sure all information is valid
    {
        if(fTGA != NULL)
            fclose(fTGA);
        return 0;
    }

    if(texture->bpp == 24)												// If the BPP of the image is 24...
        texture->type	= GL_RGB;											// Set Image type to GL_RGB
    else																					// Else if its 32 BPP
        texture->type	= GL_RGBA;										// Set image type to GL_RGBA

    tga.bytesPerPixel	= (tga.Bpp / 8);															// Compute the number of BYTES per pixel
    tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);	// Compute the total amout ofmemory needed to store data
    texture->imageData	= (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

    if(texture->imageData == NULL)								// If no space was allocated
    {
        fclose(fTGA);
        return 0;
    }

    if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)	// Attempt to read image data
    {
        if(texture->imageData != NULL)							// If imagedata has data in it
        {
            free(texture->imageData);									// Delete data from memory
        }
        fclose(fTGA);																// Close file
        return 0;
    }

    // Byte Swapping Optimized By Steve Thomas
    for(GLuint cswap = 0; cswap < tga.imageSize; cswap += tga.bytesPerPixel)
    {
        unsigned char tmp = texture->imageData[cswap];
        texture->imageData[cswap] = texture->imageData[cswap + 2];
        texture->imageData[cswap + 1] = tmp;
    }

    fclose(fTGA);																// Close file
    return 1;																		// Return success
}

int LoadCompressedTGA(Texture * texture, const char *filename, FILE * fTGA)		// Load compressed TGAs
{
    if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Attempt to read header
    {
        if(fTGA != NULL)
            fclose(fTGA);
        return 0;
    }

    texture->width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
    texture->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
    texture->bpp	= tga.header[4];																	// Determine Bits Per Pixel
    tga.Width		= texture->width;																		// Copy width to local structure
    tga.Height		= texture->height;																// Copy width to local structure
    tga.Bpp			= texture->bpp;																			// Copy width to local structure

    if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))	//Make sure all texture info is ok
    {
        if(fTGA != NULL)
            fclose(fTGA);
        return 0;
    }

    if(texture->bpp == 24)													// If the BPP of the image is 24...
        texture->type	= GL_RGB;												// Set Image type to GL_RGB
    else																						// Else if its 32 BPP
        texture->type	= GL_RGBA;											// Set image type to GL_RGBA

    tga.bytesPerPixel	= (tga.Bpp / 8);							// Compute BYTES per pixel
    tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);	// Compute amout of memory needed to store image
    texture->imageData	= (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

    if(texture->imageData == NULL)									// If it wasnt allocated correctly..
    {
        fclose(fTGA);
        return 0;
    }

    GLuint pixelcount	= tga.Height * tga.Width;			// Nuber of pixels in the image
    GLuint currentpixel	= 0;												// Current pixel being read
    GLuint currentbyte	= 0;												// Current byte
    GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);			// Storage for 1 pixel

    do
    {
        GLubyte chunkheader = 0;											// Storage for "chunk" header

        if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)				// Read in the 1 byte header
        {
            if(fTGA != NULL)
                fclose(fTGA);
            if(texture->imageData != NULL)							// If there is stored image data
                free(texture->imageData);									// Delete image data
            return 0;
        }

        if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1 that follow the header
        {
            chunkheader++;														// add 1 to get number of following color values
            for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
            {
                if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel) // Try to read 1 pixel
                {
                    if(fTGA != NULL)
                        fclose(fTGA);

                    if(colorbuffer != NULL)								// See if colorbuffer has data in it
                        free(colorbuffer);									// If so, delete it

                    if(texture->imageData != NULL)				// See if there is stored Image data
                        free(texture->imageData);						// If so, delete it too

                    return 0;
                }
                                                                                                                                                    // write to memory
                texture->imageData[currentbyte			] = colorbuffer[2];                 // Flip R and B vcolor values around in the process
                texture->imageData[currentbyte + 1	] = colorbuffer[1];
                texture->imageData[currentbyte + 2	] = colorbuffer[0];

                if(tga.bytesPerPixel == 4)							// if its a 32 bpp image
                {
                    texture->imageData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
                }

                currentbyte += tga.bytesPerPixel;				// Increase thecurrent byte by the number of bytes per pixel
                currentpixel++;													// Increase current pixel by 1

                if(currentpixel > pixelcount)						// Make sure we havent read too many pixels
                {
                    if(fTGA != NULL)
                        fclose(fTGA);

                    if(colorbuffer != NULL)								// If there is data in colorbuffer
                        free(colorbuffer);									// Delete it

                    if(texture->imageData != NULL)				// If there is Image data
                        free(texture->imageData);						// delete it

                    return 0;
                }
            }
        }
        else																				// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
        {
            chunkheader -= 127;												// Subteact 127 to get rid of the ID bit
            if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)		// Attempt to read following color values
            {
                if(fTGA != NULL)
                    fclose(fTGA);
                if(colorbuffer != NULL)									// If there is data in the colorbuffer
                    free(colorbuffer);										// delete it

                if(texture->imageData != NULL)					// If thereis image data
                    free(texture->imageData);							// delete it

                return 0;
            }

            for(short counter = 0; counter < chunkheader; counter++)				// copy the color into the image data as many times as dictated by the header
            {
                texture->imageData[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
                texture->imageData[currentbyte + 1	] = colorbuffer[1];
                texture->imageData[currentbyte + 2	] = colorbuffer[0];

                if(tga.bytesPerPixel == 4)																		// If TGA images is 32 bpp
                {
                    texture->imageData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
                }

                currentbyte += tga.bytesPerPixel;															// Increase current byte by the number of bytes per pixel
                currentpixel++;																								// Increase pixel count by 1

                if(currentpixel > pixelcount)																	// Make sure we havent written too many pixels
                {
                    if(fTGA != NULL)
                        fclose(fTGA);

                    if(colorbuffer != NULL)											// If there is data in colorbuffer
                        free(colorbuffer);												// Delete it

                    if(texture->imageData != NULL)							// If there is Image data
                        free(texture->imageData);									// delete it

                    return 0;														// Return failed
                }
            }
        }
    }

    while(currentpixel < pixelcount);										// Loop while there are still pixels left
    fclose(fTGA);																				// Close the file
    free(colorbuffer);
    return 1;																						// return success
}

Texture *LoadGLTexture(const char *filename)
{
   Texture *texture = (Texture *)malloc(sizeof(Texture));
   if(!LoadTGA(texture, filename)) {
#ifdef DEBUG_ASSET_MANAGEMENT
            fprintf(stderr, "[tgaLoader.c] [LoadGLTexture()]: Error loading texture %s\n", filename);
#endif
      assert(0);

            return NULL;
   }

   glGenTextures	(1, &texture->texID);
   glBindTexture	(GL_TEXTURE_2D, texture->texID);
   glTexImage2D		(GL_TEXTURE_2D, 0, texture->type, texture->width, texture->height, 0, texture->type, GL_UNSIGNED_BYTE, texture->imageData);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     free(texture->imageData);

     ASSERT_NO_GL_ERROR();

   return texture;
}
