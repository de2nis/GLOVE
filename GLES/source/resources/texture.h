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
 *  @file       texture.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Texture Functionality in GLOVE
 *
 */

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "rect.h"
#include "sampler.h"
#include "bufferObject.h"
#include "vulkan/sampler.h"
#include "vulkan/imageView.h"
#include "vulkan/cbManager.h"
#include "utils/GlToVkConverter.h"

class Texture {

    struct State {
        GLint                      width;
        GLint                      height;
        GLenum                     format;
        GLenum                     type;
        void                       *data;

        State() : width(-1), height(-1), format(GL_INVALID_VALUE), type(GL_INVALID_VALUE),
            data(NULL) { FUN_ENTRY(GL_LOG_TRACE); }
        ~State() { FUN_ENTRY(GL_LOG_TRACE); if(data) {delete [] (uint8_t *)data; data = NULL;}}
    };
    typedef State                  State_t;
    typedef map<uint32_t, State_t> StateMap_t;

private:
    const vkContext_t *         mVkContext;

    GLenum                      mFormat;
    GLenum                      mTarget;
    GLenum                      mType;
    GLenum                      mInternalFormat;

    // NOTE: Formats are forced into GL_RGBA8_OES. Keep here the original format requested by the user
    GLenum                      mExplicitType;
    GLenum                      mExplicitInternalFormat;

    GLint                       mMipLevelsCount;
    GLint                       mLayersCount;

    Rect                        mDims;
    Sampler                     mParameters;
    StateMap_t*                 mState;

    vulkanAPI::Image*           mImage;
    vulkanAPI::Memory*          mMemory;
    vulkanAPI::Sampler*         mSampler;
    vulkanAPI::ImageView*       mImageView;

    static int                  mDefaultInternalAlignment;

    bool                        AllocateVkMemory(void);
    void                        ReleaseVkResources(void);

public:
    Texture(const vkContext_t  *vkContext = nullptr,
            const VkFlags       vkFlags   = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    ~Texture();

// Generate Functions
    bool                    Allocate();
    void                    SetState(GLsizei width, GLsizei height, GLint level, GLint layer, GLenum format, GLenum type, GLint unpackAlignment, const void *pixels);
    void                    SetSubState(ImageRect *srcRect, ImageRect *dstRect, GLint miplevel, GLint layer, GLenum srcFormat, const void *srcData);
    void                    GenerateMipmaps(GLenum hintMipmapMode);

// Init Functions
    inline void             InitState(void)                                     { FUN_ENTRY(GL_LOG_TRACE); mLayersCount  = mTarget == GL_TEXTURE_2D ? TEXTURE_2D_LAYERS : TEXTURE_CUBE_MAP_LAYERS;
                                                                                                       mState        = new StateMap_t[mLayersCount]; }

// Helper Functions
    static int              GetDefaultInternalAlignment()                       { FUN_ENTRY(GL_LOG_TRACE); return mDefaultInternalAlignment; }
    inline float            GetInvertedYOrigin(const Rect* rect)                { FUN_ENTRY(GL_LOG_TRACE); return mDims.height - rect->height - rect->y; }
    void                    PrepareVkImageLayout(VkImageLayout newImageLayout);

// Create Functions
    bool                    CreateVkTexture(void);
    bool                    CreateVkImage(void);
    bool                    CreateVkImageView(void)                             { FUN_ENTRY(GL_LOG_TRACE); return mImageView->Create(mImage); }
    bool                    CreateVkSampler(void)                               { FUN_ENTRY(GL_LOG_TRACE); return mSampler->Create(); }
    void                    CreateVkImageSubResourceRange(void)                 { FUN_ENTRY(GL_LOG_TRACE); return mImage->CreateImageSubresourceRange(); }

// Copy Functions
     void                   CopyPixelsFromHost (ImageRect *srcRect, ImageRect *dstRect, GLint miplevel, GLint layer, GLenum srcFormat, const void *srcData);
     void                   CopyPixelsToHost   (ImageRect *srcRect, ImageRect *dstRect, GLint miplevel, GLint layer, GLenum dstFormat, void *dstData);
     void                   SubmitCopyPixels   (const Rect *rect, BufferObject *tbo, GLint miplevel, GLint layer, GLenum dstFormat, bool copyToImage);

// Get Functions
    inline GLenum           GetWrapS(void)                              const   { FUN_ENTRY(GL_LOG_TRACE); return mParameters.GetWrapS(); }
    inline GLenum           GetWrapT(void)                              const   { FUN_ENTRY(GL_LOG_TRACE); return mParameters.GetWrapT(); }
    inline GLenum           GetMinFilter(void)                          const   { FUN_ENTRY(GL_LOG_TRACE); return mParameters.GetMinFilter(); }
    inline GLenum           GetMagFilter(void)                          const   { FUN_ENTRY(GL_LOG_TRACE); return mParameters.GetMagFilter(); }
    inline int              GetWidth(void)                              const   { FUN_ENTRY(GL_LOG_TRACE); return mDims.width; }
    inline int              GetHeight(void)                             const   { FUN_ENTRY(GL_LOG_TRACE); return mDims.height; }
    inline GLenum           GetType(void)                               const   { FUN_ENTRY(GL_LOG_TRACE); return mType; }
    inline GLenum           GetExplicitType(void)                       const   { FUN_ENTRY(GL_LOG_TRACE); return mExplicitType; }
    inline GLenum           GetFormat(void)                             const   { FUN_ENTRY(GL_LOG_TRACE); return mFormat; }
    inline GLenum           GetTarget(void)                             const   { FUN_ENTRY(GL_LOG_TRACE); return mTarget; }
    inline GLenum           GetInternalFormat(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mInternalFormat; }
    inline GLenum           GetExplicitInternalFormat(void)             const   { FUN_ENTRY(GL_LOG_TRACE); return mExplicitInternalFormat; }
    inline GLint            GetLayersCount(void)                        const   { FUN_ENTRY(GL_LOG_TRACE); return mLayersCount; }
    inline GLint            GetMipLevelsCount(void)                     const   { FUN_ENTRY(GL_LOG_TRACE); return mMipLevelsCount; }

    inline vulkanAPI::Image* GetImage(void)                                     { FUN_ENTRY(GL_LOG_TRACE); return mImage; }

    inline VkSampler        GetVkSampler(void)                          const   { FUN_ENTRY(GL_LOG_TRACE); return mSampler->GetSampler(); }
    inline VkFormat         GetVkFormat(void)                           const   { FUN_ENTRY(GL_LOG_TRACE); return mImage->GetFormat(); }
    inline VkImageLayout    GetVkImageLayout(void)                      const   { FUN_ENTRY(GL_LOG_TRACE); return mImage->GetImageLayout(); }
    inline VkImageView      GetVkImageView(void)                        const   { FUN_ENTRY(GL_LOG_TRACE); return mImageView->GetImageView(); }

// Set Functions
    inline void             SetVkContext(const vkContext_t *vkContext)          { FUN_ENTRY(GL_LOG_TRACE); mVkContext = vkContext;
                                                                                                       mMemory->SetContext(vkContext);
                                                                                                       mSampler->SetContext(vkContext);
                                                                                                       mImageView->SetContext(vkContext);
                                                                                                       mImage->SetContext(vkContext); }
    inline void             SetWrapS(GLenum mode)                               { FUN_ENTRY(GL_LOG_TRACE); if(mParameters.UpdateWrapS(mode)) { \
                                                                                                       mSampler->SetAddressModeU(GlTexAddressToVkTexAddress(mode));}}
    inline void             SetWrapT(GLenum mode)                               { FUN_ENTRY(GL_LOG_TRACE); if(mParameters.UpdateWrapT(mode)) { \
                                                                                                       mSampler->SetAddressModeV(GlTexAddressToVkTexAddress(mode));}}
    inline void             SetMinFilter(GLenum mode)                           { FUN_ENTRY(GL_LOG_TRACE); if(mParameters.UpdateMinFilter(mode)){ \
                                                                                                       mSampler->SetMinFilter(GlTexFilterToVkTexFilter(mode)); \
                                                                                                       mSampler->SetMipmapMode(GlTexMipMapModeToVkMipMapMode(mode));
                                                                                                       mSampler->SetMaxLod((mode == GL_NEAREST || mode == GL_LINEAR) ? 0.0 : static_cast<float>(mMipLevelsCount-1));
                                                                                                       } }
    inline void             SetMagFilter(GLenum mode)                           { FUN_ENTRY(GL_LOG_TRACE); if(mParameters.UpdateMagFilter(mode)) { \
                                                                                                       mSampler->SetMagFilter(GlTexFilterToVkTexFilter(mode));
                                                                                                       } }
    inline void             SetWidth(int width)                                 { FUN_ENTRY(GL_LOG_TRACE); mDims.width  = width;  }
    inline void             SetHeight(int height)                               { FUN_ENTRY(GL_LOG_TRACE); mDims.height = height; }
    inline void             SetTarget(GLenum target)                            { FUN_ENTRY(GL_LOG_TRACE); mTarget      = target; }
    inline void             SetFormat(GLenum format)                            { FUN_ENTRY(GL_LOG_TRACE); mFormat      = format; }
    inline void             SetType(GLenum type)                                { FUN_ENTRY(GL_LOG_TRACE); mType        = type;   }
    inline void             SetExplicitType(GLenum type)                        { FUN_ENTRY(GL_LOG_TRACE); mExplicitType = type;  }
    inline void             SetInternalFormat(GLenum format)                    { FUN_ENTRY(GL_LOG_TRACE); mInternalFormat         = format;  }
    inline void             SetExplicitInternalFormat(GLenum format)            { FUN_ENTRY(GL_LOG_TRACE); mExplicitInternalFormat = format;  }

    inline void             SetVkFormat(VkFormat format)                        { FUN_ENTRY(GL_LOG_TRACE); mImage->SetFormat(format);      }
    inline void             SetVkImage(VkImage image)                           { FUN_ENTRY(GL_LOG_TRACE); mImage->SetImage(image);        }
    inline void             SetVkImageUsage(VkImageUsageFlagBits usage)         { FUN_ENTRY(GL_LOG_TRACE); mImage->SetImageUsage(usage);   }
    inline void             SetVkImageLayout(VkImageLayout layout)              { FUN_ENTRY(GL_LOG_TRACE); mImage->SetImageLayout(layout); }
    inline void             SetVkImageTiling(VkImageTiling tiling)              { FUN_ENTRY(GL_LOG_TRACE); mImage->SetImageTiling(tiling); }
    inline void             SetVkImageTarget(vulkanAPI::Image::VkImageTarget
                                                                     target)    { FUN_ENTRY(GL_LOG_TRACE); mImage->SetImageTarget(target); }

// Is Functions
    inline bool             IsCubeMap(void)                             const   { FUN_ENTRY(GL_LOG_TRACE); return mTarget  == GL_TEXTURE_CUBE_MAP; }
    inline bool             IsCompressed(void)                          const   { FUN_ENTRY(GL_LOG_TRACE); return (mFormat != GL_ALPHA           &&
                                                                                                               mFormat != GL_RGB             &&
                                                                                                               mFormat != GL_RGBA            &&
                                                                                                               mFormat != GL_LUMINANCE       &&
                                                                                                               mFormat != GL_LUMINANCE_ALPHA &&
                                                                                                               mFormat != GL_BGRA8_EXT); }
           bool             IsCompleted(void);

};

#endif // __TEXTURE_H__
