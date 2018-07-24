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
 *  @file       sampler.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Texture Mapping Functionality via Image Samplers in Vulkan
 *
 *  @section
 *
 *  Sampler objects represent the state of an image sampler which is used by the
 *  implementation to read image data and apply filtering and other transformations
 *  for the shader. Samplers are represented by VkSampler handles.
 *
 */

#include "sampler.h"

namespace vulkanAPI {

Sampler::Sampler(const vkContext_t *vkContext)
: mVkContext(vkContext),
  mVkSampler(VK_NULL_HANDLE),
  mVkMinFilter(VK_FILTER_NEAREST), mVkMagFilter(VK_FILTER_LINEAR),
  mVkMipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR),
  mVkAddressModeU(VK_SAMPLER_ADDRESS_MODE_REPEAT),
  mVkAddressModeV(VK_SAMPLER_ADDRESS_MODE_REPEAT),
  mVkAddressModeW(VK_SAMPLER_ADDRESS_MODE_REPEAT),
  mMinLod(0.0f), mMaxLod(0.0f), mMipLodBias(0.0f),
  mAnisotropyEnabled(VK_FALSE), mMaxAnisotropy(1.0),
  mCompareEnabled(VK_FALSE), mVkCompareOp(VK_COMPARE_OP_NEVER),
  mVkBorderColor(VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE),
  mUnnormalizedCoordinates(VK_FALSE),
  mUpdated(true)
{
    FUN_ENTRY(GL_LOG_TRACE);
}

Sampler::~Sampler()
{
    FUN_ENTRY(GL_LOG_TRACE);

    Release();
}

void
Sampler::Release(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkSampler != VK_NULL_HANDLE) {
        vkDestroySampler(mVkContext->vkDevice, mVkSampler, NULL);
        mVkSampler = VK_NULL_HANDLE;
    }

    mUpdated = true;
}

bool
Sampler::Create()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mUpdated) {
        return true;
    }

    Release();

    VkSamplerCreateInfo samplerInfo;
    samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.pNext                   = NULL;
    samplerInfo.flags                   = 0;

    samplerInfo.mipmapMode              = mVkMipmapMode;
    samplerInfo.minFilter               = mVkMinFilter;
    samplerInfo.magFilter               = mVkMagFilter;
    samplerInfo.addressModeU            = mVkAddressModeU;
    samplerInfo.addressModeV            = mVkAddressModeV;
    samplerInfo.addressModeW            = mVkAddressModeW;

    samplerInfo.minLod                  = mMinLod;
    samplerInfo.maxLod                  = mMaxLod;
    samplerInfo.mipLodBias              = mMipLodBias;
    samplerInfo.anisotropyEnable        = mAnisotropyEnabled;
    samplerInfo.maxAnisotropy           = mMaxAnisotropy;
    samplerInfo.compareEnable           = mCompareEnabled;
    samplerInfo.compareOp               = mVkCompareOp;
    samplerInfo.borderColor             = mVkBorderColor;
    samplerInfo.unnormalizedCoordinates = mUnnormalizedCoordinates;

    VkResult err = vkCreateSampler(mVkContext->vkDevice, &samplerInfo, NULL, &mVkSampler);
    assert(!err);

    mUpdated = false;

    return (err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY && err != VK_ERROR_TOO_MANY_OBJECTS);
}

}
