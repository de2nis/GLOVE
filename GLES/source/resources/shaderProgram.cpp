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
 *  @file       shaderProgram.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shader Program Functionality in GLOVE
 *
 *  @scope
 *
 *  A Shader Program represents fully processed executable code, in the OpenGL ES
 *  Shading Language (ESSL) for one or more Shader stages. 
 *
 */

#include "shaderProgram.h"
#include "context/context.h"

ShaderProgram::ShaderProgram(const vkContext_t *vkContext)
: mGlContext(nullptr)
{
    FUN_ENTRY(GL_LOG_TRACE);

    mVkContext = vkContext;

    mShaders[0] = NULL;
    mShaders[1] = NULL;

    mStagesIDs[0] = -1;
    mStagesIDs[1] = -1;

    mMinDepthRange = 1.f;
    mMaxDepthRange = 0.f;

    mVkShaderModules[0] = VK_NULL_HANDLE;
    mVkShaderModules[1] = VK_NULL_HANDLE;

    mVkDescSetLayout = VK_NULL_HANDLE;
    mVkDescSetLayoutBind = NULL;
    mVkDescPool = VK_NULL_HANDLE;
    mVkDescSet = VK_NULL_HANDLE;
    mVkPipelineLayout = VK_NULL_HANDLE;
    mVkPipelineCache = VK_NULL_HANDLE;

    mStageCount = 0;

    mUpdateDescriptorSets = false;
    mUpdateDescriptorData = false;
    mMarkForDeletion = false;
    mLinked = false;
    mIsPrecompiled = false;
    mValidated = false;
    mActiveVertexVkBuffersCount = 0;

    SetPipelineVertexInputStateInfo();
}

ShaderProgram::~ShaderProgram()
{
    FUN_ENTRY(GL_LOG_TRACE);

    ReleaseVkObjects();
    ReleaseVkPipelineCache();
}

bool
ShaderProgram::SetPipelineShaderStage(uint32_t &pipelineShaderStageCount, int *pipelineShaderStagesIDs, VkPipelineShaderStageCreateInfo *pipelineShaderStages)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    bool linked = true;

    pipelineShaderStageCount = GetStageCount();
    if(pipelineShaderStageCount == 1) {
        pipelineShaderStages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipelineShaderStages[0].stage  = GetShaderStage();
        pipelineShaderStages[0].module = GetShaderModule();
        pipelineShaderStages[0].pName  = "main\0";
        pipelineShaderStagesIDs[0]     = GetStagesIDs(0);

        if(GetShaderModule() == VK_NULL_HANDLE) {
            linked = false;
        }
    } else if (pipelineShaderStageCount == 2) {
        pipelineShaderStages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipelineShaderStages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
        pipelineShaderStages[0].module = GetVertexShaderModule();
        pipelineShaderStages[0].pName  = "main\0";
        pipelineShaderStagesIDs[0]     = GetStagesIDs(0);

        if(GetVertexShaderModule() == VK_NULL_HANDLE) {
            linked = false;
        }

        pipelineShaderStages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipelineShaderStages[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        pipelineShaderStages[1].module = GetFragmentShaderModule();
        pipelineShaderStages[1].pName  = "main\0";
        pipelineShaderStagesIDs[1]     = GetStagesIDs(1);

        if(GetFragmentShaderModule() == VK_NULL_HANDLE) {
            linked = false;
        }
    } else {
        linked = false;
    }

    return linked;
}

void
ShaderProgram::SetPipelineVertexInputStateInfo(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mVkPipelineVertexInput.sType                            = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    mVkPipelineVertexInput.pNext                            = NULL;
    mVkPipelineVertexInput.flags                            = 0;
    mVkPipelineVertexInput.vertexBindingDescriptionCount    = 0;
    mVkPipelineVertexInput.pVertexBindingDescriptions       = mVkVertexInputBinding;
    mVkPipelineVertexInput.vertexAttributeDescriptionCount  = 0;
    mVkPipelineVertexInput.pVertexAttributeDescriptions     = mVkVertexInputAttribute;
}

int
ShaderProgram::GetInfoLogLength(void) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return mShaderCompiler->GetProgramInfoLog() ? (int)strlen(mShaderCompiler->GetProgramInfoLog()) + 1 : 0;
}

Shader *
ShaderProgram::IsShaderAttached(Shader *shader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if((shader->GetShaderType() == SHADER_TYPE_VERTEX   && mShaders[0] == shader) ||
       (shader->GetShaderType() == SHADER_TYPE_FRAGMENT && mShaders[1] == shader)) {
        return (Shader *)shader;
    }

    return nullptr;
}

void
ShaderProgram::AttachShader(Shader *shader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    shader->RefShader();
    mShaders[shader->GetShaderType() == SHADER_TYPE_VERTEX ? 0 : 1] = shader;
}

void
ShaderProgram::DetachShader(Shader *shader)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!shader || !IsShaderAttached(shader)) {
        return;
    }

    if(shader->GetShaderType() == SHADER_TYPE_VERTEX) {
        mShaders[0] = nullptr;
    } else if(shader->GetShaderType() == SHADER_TYPE_FRAGMENT) {
        mShaders[1] = nullptr;
    }

    shader->UnrefShader();
}

void
ShaderProgram::DetachAndDeleteShaders()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    Shader *shaderPtr;

    shaderPtr = GetVertexShader();
    if(shaderPtr) {
        DetachShader(shaderPtr);
        if(shaderPtr->GetMarkForDeletion()) {
            ((Context *)mGlContext)->DeleteShader(shaderPtr);
        }
    }

    shaderPtr = GetFragmentShader();
    if(shaderPtr) {
        DetachShader(shaderPtr);
        if(shaderPtr->GetMarkForDeletion()) {
            ((Context *)mGlContext)->DeleteShader(shaderPtr);
        }
    }
}

void
ShaderProgram::SetShaderModules(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mStageCount = HasVertexShader() + HasFragmentShader();
    assert(mStageCount == 0 || mStageCount == 1 || mStageCount == 2);

    if(mStageCount == 1) {

        Shader* shader = HasVertexShader() ? GetVertexShader() : GetFragmentShader();

        if(mVkShaderModules[0] != VK_NULL_HANDLE) {
            mVkContext->mCommandBufferManager->UnrefResouce(mVkShaderModules[0]);
            mVkShaderModules[0] = VK_NULL_HANDLE;
        }

        mVkShaderModules[0] = shader->CreateVkShaderModule();

        if(mVkShaderModules[0] != VK_NULL_HANDLE) {
            mVkContext->mCommandBufferManager->RefResource(mVkShaderModules[0], RESOURCE_TYPE_SHADER);
        }

        mVkShaderStages[0]  = HasVertexShader() ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;

    } else if(mStageCount == 2) {

        Shader* shader = GetVertexShader();

        if(mVkShaderModules[0] != VK_NULL_HANDLE) {
            mVkContext->mCommandBufferManager->UnrefResouce(mVkShaderModules[0]);
            mVkShaderModules[0] = VK_NULL_HANDLE;
        }

        mVkShaderModules[0] = shader->CreateVkShaderModule();

        if(mVkShaderModules[0] != VK_NULL_HANDLE) {
            mVkContext->mCommandBufferManager->RefResource(mVkShaderModules[0], RESOURCE_TYPE_SHADER);
        }

        mShaderSPVsize[0]  = shader->GetSPV().size();
        mShaderSPVdata[0]  = shader->GetSPV().data();
        mVkShaderStages[0] = VK_SHADER_STAGE_VERTEX_BIT;

        shader = GetFragmentShader();

        if(mVkShaderModules[1] != VK_NULL_HANDLE) {
            mVkContext->mCommandBufferManager->UnrefResouce(mVkShaderModules[1]);
            mVkShaderModules[1] = VK_NULL_HANDLE;
        }

        mVkShaderModules[1] = shader->CreateVkShaderModule();

        if(mVkShaderModules[1] != VK_NULL_HANDLE) {
            mVkContext->mCommandBufferManager->RefResource(mVkShaderModules[1], RESOURCE_TYPE_SHADER);
        }

        mShaderSPVsize[1]  = shader->GetSPV().size();
        mShaderSPVdata[1]  = shader->GetSPV().data();
        mVkShaderStages[1] = VK_SHADER_STAGE_FRAGMENT_BIT;
    }
}

uint32_t
ShaderProgram::SerializeShadersSpirv(void *binary)
{
    uint8_t *rawDataPtr = reinterpret_cast<uint8_t *>(binary);
    uint32_t *u32DataPtr = NULL;
    uint32_t vsSpirvSize = 4 * mShaderSPVsize[0];
    uint32_t fsSpirvSize = 4 * mShaderSPVsize[1];

    u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
    *u32DataPtr = vsSpirvSize;
    rawDataPtr += sizeof(uint32_t);
    memcpy(rawDataPtr, mShaderSPVdata[0], vsSpirvSize);
    rawDataPtr += vsSpirvSize;

    u32DataPtr = reinterpret_cast<uint32_t *>(rawDataPtr);
    *u32DataPtr = fsSpirvSize;
    rawDataPtr += sizeof(uint32_t);
    memcpy(rawDataPtr, mShaderSPVdata[1], fsSpirvSize);

    return 2 * sizeof(uint32_t) + vsSpirvSize + fsSpirvSize;
}

uint32_t
ShaderProgram::DeserializeShadersSpirv(const void *binary)
{
    const uint8_t *rawDataPtr = reinterpret_cast<const uint8_t *>(binary);
    const uint32_t *u32DataPtr = NULL;
    std::vector<uint32_t> &vsSpirvData = GetVertexShader()->GetSPV();
    std::vector<uint32_t> &fsSpirvData = GetFragmentShader()->GetSPV();
    uint32_t vsSpirvSize = 0;
    uint32_t fsSpirvSize = 0;

    u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
    vsSpirvSize = *u32DataPtr;\
    rawDataPtr += sizeof(uint32_t);
    u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
    std::copy(u32DataPtr, u32DataPtr + vsSpirvSize /4, back_inserter(vsSpirvData));
    rawDataPtr += vsSpirvSize;

    u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
    fsSpirvSize = *u32DataPtr;
    rawDataPtr += sizeof(uint32_t);
    u32DataPtr = reinterpret_cast<const uint32_t *>(rawDataPtr);
    std::copy(u32DataPtr, u32DataPtr + fsSpirvSize /4, back_inserter(fsSpirvData));
    rawDataPtr += fsSpirvSize;

    return 2 * sizeof(uint32_t) + vsSpirvSize + fsSpirvSize;
}

const ShaderResourceInterface::attribute *
ShaderProgram::GetVertexAttribute(int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    return mShaderResourceInterface.GetVertexAttribute(index);
}

uint32_t
ShaderProgram::GetNumberOfActiveAttributes(void) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    return mShaderResourceInterface.GetLiveAttributes();
}

int
ShaderProgram::GetAttributeType(int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    return mShaderResourceInterface.GetAttributeType(index);
}

int
ShaderProgram::GetAttributeLocation(const char *name) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    return mShaderResourceInterface.GetAttributeLocation(name);
}

VkPipelineCache
ShaderProgram::GetVkPipelineCache(void)
{
    FUN_ENTRY(GL_LOG_TRACE);

    if(mVkPipelineCache == VK_NULL_HANDLE) {
        CreateVkPipelineCache(nullptr, 0);
    }

    return mVkPipelineCache;
}

const string &
ShaderProgram::GetAttributeName(int index) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return mShaderResourceInterface.GetAttributeName(index);
}

void
ShaderProgram::Validate()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mValidated = true;

    if(!mLinked) {
        mValidated = false;
        return;
    }
}

bool
ShaderProgram::ValidateProgram(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    bool validated;
    const Shader *vs = mShaders[0];
    const Shader *fs = mShaders[1];

    if((!vs || !fs) ||
       (!vs->IsCompiled() || !fs->IsCompiled())) {
        return false;
    }

    if(GLOVE_DUMP_INPUT_SHADER_REFLECTION) {
        mShaderCompiler->EnableDumpInputShaderReflection();
    }
    validated = mShaderCompiler->ValidateProgram();

    return validated;
}

bool
ShaderProgram::LinkProgram()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!(mLinked = ValidateProgram())) {
        return false;
    }

    if(GLOVE_SAVE_SHADER_SOURCES_TO_FILES) {
        mShaderCompiler->EnableSaveSourceToFiles();
    }

    if(GLOVE_DUMP_PROCESSED_SHADER_SOURCE) {
        mShaderCompiler->EnableDumpProcessedShaderSource();
    }

    if(GLOVE_SAVE_SPIRV_BINARY_TO_FILES) {
        mShaderCompiler->EnableSaveBinaryToFiles();
    }

    if(GLOVE_SAVE_SPIRV_TEXT_TO_FILE) {
        mShaderCompiler->EnableSaveSpvTextToFile();
    }

    if(GLOVE_DUMP_VULKAN_SHADER_REFLECTION) {
        mShaderCompiler->EnableDumpVulkanShaderReflection();
    }

    ResetVulkanVertexInput();

    mShaderCompiler->PrepareReflection();
    UpdateAttributeInterface();
    mLinked = mShaderCompiler->LinkProgram(*this);
    BuildShaderResourceInterface();

    /// A program object will fail to link if the number of active vertex attributes exceeds GL_MAX_VERTEX_ATTRIBS
    /// A link error will be generated if an attempt is made to utilize more than the space available for fragment shader uniform variables.
    if(GetNumberOfActiveUniforms() > GLOVE_MAX_VERTEX_UNIFORM_VECTORS ||
       GetNumberOfActiveUniforms() > GLOVE_MAX_FRAGMENT_UNIFORM_VECTORS ||
       GetNumberOfActiveAttributes() > GLOVE_MAX_VERTEX_ATTRIBS) {
        mLinked = false;
        return false;
    }

    if(GLOVE_DUMP_VULKAN_SHADER_REFLECTION) {
        printf("Glove's vulkan shader reflection:\n");
        mShaderCompiler->DumpUniforms();
        mShaderResourceInterface.DumpGloveShaderVertexInputInterface();
    }

    return mLinked;
}

void
ShaderProgram::PrepareVertexAttribBufferObjects(size_t vertCount, uint32_t firstVertex, GenericVertexAttributes *genericVertAttribs)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(firstVertex) {
        genericVertAttribs->CleanupVertexAttributes();
    }

    // store the location-binding associations for faster lookup
    std::map<uint32_t, uint32_t> vboLocationBindings;

    // reset active buffers
    memset(mActiveVertexVkBuffers, VK_NULL_HANDLE, sizeof(VkBuffer) * mActiveVertexVkBuffersCount);
    mActiveVertexVkBuffersCount = 0;

    GenerateVertexAttribProperties(vertCount, firstVertex, genericVertAttribs, vboLocationBindings);
    GenerateVertexInputProperties(genericVertAttribs, vboLocationBindings);
}

void ShaderProgram::GenerateVertexAttribProperties(size_t vertCount, uint32_t firstVertex, GenericVertexAttributes *genericVertAttribs, std::map<uint32_t, uint32_t>& vboLocationBindings)
{
    // store attribute locations containing the same VkBuffer and stride
    // as they are directly associated with vertex input bindings
    typedef std::pair<VkBuffer, uint32_t> BUFFER_STRIDE_PAIR;
    std::map<BUFFER_STRIDE_PAIR, std::vector<uint32_t>> unique_buffer_stride_map;

    for(uint32_t i = 0; i < mShaderResourceInterface.GetLiveAttributes(); ++i) {
        const uint32_t location = mShaderResourceInterface.GetAttributeLocation(i);
        assert(location < GLOVE_MAX_VERTEX_ATTRIBS);

        if(genericVertAttribs->GetVertexAttribActive(location)) {
            /// Calculate stride if not given from user
            if(!genericVertAttribs->GetVertexAttribStride(location)) {
                // gva->stride = gva->nElements * glDataTypeToBpp(gva->dataType);
                // shaders only accept floats
                size_t stride = genericVertAttribs->GetVertexAttribSize(location) * sizeof(glsl_float_t);
                genericVertAttribs->SetVertexAttribStride(location, stride);
            }

            /// Create new vbo if user passed pointer to data
            /// This happens when vertex data are located in user space, instead of stored in a Vertex Buffer Objects
            if(!genericVertAttribs->GetVertexAttribVbo(location)) {
                BufferObject *vbo = new VertexBufferObject(mVkContext);
                vbo->Allocate(genericVertAttribs->GetVertexAttribFormat(location),
                              genericVertAttribs->GetVertexAttribNormalized(location),
                              (firstVertex + vertCount) * genericVertAttribs->GetVertexAttribStride(location),
                              (const void *)genericVertAttribs->GetVertexAttribPointer(location));

                genericVertAttribs->SetVertexAttribVbo(location, vbo);
                genericVertAttribs->SetVertexAttribOffset(location, 0);
                genericVertAttribs->SetVertexAttribInternalVBO(location, true);
            }
        } else {
            /// Use the generic vertex attribute value registered to that location
            BufferObject *vbo = new VertexBufferObject(mVkContext);
            float genericValue[4];
            genericVertAttribs->GetGenericVertexAttribute(location, genericValue);
            vbo->Allocate(4 * sizeof(float), (const void *)genericValue);

            genericVertAttribs->SetVertexAttribVbo(location, vbo);
            genericVertAttribs->SetVertexAttribFormat(location, GlAttribTypeToVkFormat(mShaderResourceInterface.GetAttributeType(i)));
            genericVertAttribs->SetVertexAttribStride(location, 0);
            genericVertAttribs->SetVertexAttribInternalVBO(location, true);
        }

        assert(genericVertAttribs->GetVertexAttribVbo(location));

        // store each location
        VkBuffer bo = genericVertAttribs->GetVertexAttribVbo(location)->GetVkBuffer();
        uint32_t stride = genericVertAttribs->GetVertexAttribStride(location);
        BUFFER_STRIDE_PAIR p = {bo, stride};
        unique_buffer_stride_map[p].push_back(location);
    }

    // generate unique bindings for each VKbuffer/stride pair
    int current_binding = 0;
    for(const auto& iter : unique_buffer_stride_map) {
        VkBuffer bo = iter.first.first;
        for(const auto& loc_str_iter : iter.second) {
                vboLocationBindings[loc_str_iter] = current_binding;
        }
        mActiveVertexVkBuffers[current_binding] = bo;
        ++current_binding;
    }
    mActiveVertexVkBuffersCount = current_binding;
}

void
ShaderProgram::GenerateVertexInputProperties(GenericVertexAttributes *genericVertAttribs, const std::map<uint32_t, uint32_t>& vboLocationBindings)
{
    // create vertex input bindings and attributes
    for(uint32_t i = 0; i < mShaderResourceInterface.GetLiveAttributes(); ++i) {
        const uint32_t location = mShaderResourceInterface.GetAttributeLocation(i);

        assert(vboLocationBindings.find(location) != vboLocationBindings.end());
        const uint32_t binding = vboLocationBindings.at(location);

        mVkVertexInputBinding[binding].binding = binding;
        mVkVertexInputBinding[binding].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        mVkVertexInputBinding[binding].stride = genericVertAttribs->GetVertexAttribStride(location);

        mVkVertexInputAttribute[i].binding = binding;
        mVkVertexInputAttribute[i].format = VkIntFormatToVkFloatFormat(genericVertAttribs->GetVertexAttribFormat(location));
        mVkVertexInputAttribute[i].location = location;
        mVkVertexInputAttribute[i].offset = genericVertAttribs->GetVertexAttribOffset(location);
    }

    mVkPipelineVertexInput.vertexBindingDescriptionCount = mActiveVertexVkBuffersCount;
    mVkPipelineVertexInput.vertexAttributeDescriptionCount = mShaderResourceInterface.GetLiveAttributes();
}

bool
ShaderProgram::CreateVkPipelineCache(const void *initialData, size_t initialDataSize)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkPipelineCacheCreateInfo info;
    info.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    info.pNext           = NULL;
    info.flags           = 0;
    info.initialDataSize = initialDataSize;
    info.pInitialData    = initialData;

    VkResult err = vkCreatePipelineCache(mVkContext->vkDevice, &info, NULL, &mVkPipelineCache);
    assert(!err);

    return (err != VK_ERROR_OUT_OF_HOST_MEMORY && err != VK_ERROR_OUT_OF_DEVICE_MEMORY);
}

void
ShaderProgram::UsePrecompiledBinary(const void *binary, size_t binarySize)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mLinked = true;

    ResetVulkanVertexInput();

    uint32_t reflectionOffset = mShaderCompiler->DeserializeReflection(binary);
    uint32_t spirvOffset = DeserializeShadersSpirv(reinterpret_cast<const uint8_t *>(binary) + reflectionOffset);
    const uint8_t *vulkanDataPtr = reinterpret_cast<const uint8_t *>(binary) + reflectionOffset + spirvOffset;

    BuildShaderResourceInterface();

    if(mVkPipelineCache != VK_NULL_HANDLE) {
        vkDestroyPipelineCache(mVkContext->vkDevice, mVkPipelineCache, nullptr);
        mVkPipelineCache = VK_NULL_HANDLE;
    }

    CreateVkPipelineCache(vulkanDataPtr, binarySize - reflectionOffset);

    mIsPrecompiled = true;
}

void
ShaderProgram::GetBinaryData(void *binary, GLsizei *binarySize)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    uint32_t reflectionOffset = mShaderCompiler->SerializeReflection(binary);

    uint8_t *spirvDataPtr = reinterpret_cast<uint8_t *>(binary) + reflectionOffset;
    uint32_t spirvOffset = SerializeShadersSpirv(spirvDataPtr);

    uint8_t *vulkanDataPtr = reinterpret_cast<uint8_t *>(binary) + reflectionOffset + spirvOffset;
    size_t vulkanDataSize = *binarySize;

    if(mVkPipelineCache) {
        vkGetPipelineCacheData(mVkContext->vkDevice, mVkPipelineCache, &vulkanDataSize, reinterpret_cast<void *>(vulkanDataPtr));
        *binarySize = vulkanDataSize + reflectionOffset + spirvOffset;
    } else {
        *binarySize = 0;
    }
}

GLsizei
ShaderProgram::GetBinaryLength(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    size_t vkPipelineCacheDataLength = 0;
    uint32_t spirvSize = 2 * sizeof(uint32_t) + 4 * (mShaderSPVsize[0] + mShaderSPVsize[1]);

    if(mVkPipelineCache) {
        vkGetPipelineCacheData(mVkContext->vkDevice, mVkPipelineCache, &vkPipelineCacheDataLength, nullptr);
    }

    return vkPipelineCacheDataLength + mShaderResourceInterface.GetReflectionSize() + spirvSize;
}

char *
ShaderProgram::GetInfoLog(void) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    char *log = NULL;

    if(mShaderCompiler) {
        uint32_t len = strlen(mShaderCompiler->GetProgramInfoLog()) + 1;
        log = new char[len];

        memcpy(log, mShaderCompiler->GetProgramInfoLog(), len);
        log[len - 1] = '\0';
    }

    return log;
}

void
ShaderProgram::GetUniformData(uint32_t location, size_t size, void *ptr) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(ptr);
    assert(IsLinked());

    mShaderResourceInterface.GetUniformClientData(location, size, ptr);
}

void
ShaderProgram::SetUniformData(uint32_t location, size_t size, const void *ptr)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mLinked);
    assert(size);
    assert(ptr);

    mShaderResourceInterface.SetUniformClientData(location, size, ptr);
    mUpdateDescriptorData = true;
}

void
ShaderProgram::SetSampler(uint32_t location, int count, const int *textureUnit)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mShaderResourceInterface.SetSampler(location, count, textureUnit);
    mUpdateDescriptorSets = true;
}

void
ShaderProgram::ReleaseVkObjects(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkPipelineLayout != VK_NULL_HANDLE) {
        mVkContext->mCommandBufferManager->UnrefResouce(mVkPipelineLayout);
        mVkPipelineLayout = VK_NULL_HANDLE;
    }

    if(mVkDescSetLayout != VK_NULL_HANDLE) {
        mVkContext->mCommandBufferManager->UnrefResouce(mVkDescSetLayout);
        mVkDescSetLayout = VK_NULL_HANDLE;
    }

    if(mVkDescSet != VK_NULL_HANDLE) {
        vkFreeDescriptorSets(mVkContext->vkDevice, mVkDescPool, 1, &mVkDescSet);
        mVkDescSet = VK_NULL_HANDLE;
    }

    if(mVkDescPool != VK_NULL_HANDLE) {
        mVkContext->mCommandBufferManager->UnrefResouce(mVkDescPool);
        mVkDescPool = VK_NULL_HANDLE;
    }

    if(mVkShaderModules[0] != VK_NULL_HANDLE) {
        mVkContext->mCommandBufferManager->UnrefResouce(mVkShaderModules[0]);
        mVkShaderModules[0] = VK_NULL_HANDLE;
    }
    if(mVkShaderModules[1] != VK_NULL_HANDLE) {
        mVkContext->mCommandBufferManager->UnrefResouce(mVkShaderModules[1]);
        mVkShaderModules[1] = VK_NULL_HANDLE;
    }
}

void
ShaderProgram::ReleaseVkPipelineCache(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVkPipelineCache != VK_NULL_HANDLE) {
        vkDestroyPipelineCache(mVkContext->vkDevice, mVkPipelineCache, nullptr);
        mVkPipelineCache = VK_NULL_HANDLE;
    }
}

bool
ShaderProgram::CreateDescriptorSetLayout(uint32_t nLiveUniformBlocks)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mVkDescSetLayoutBind) {
        delete mVkDescSetLayoutBind;
        mVkDescSetLayoutBind = nullptr;
    }

    if(nLiveUniformBlocks) {
        mVkDescSetLayoutBind = new VkDescriptorSetLayoutBinding[nLiveUniformBlocks];
        assert(mVkDescSetLayoutBind);

        for(uint32_t i = 0; i < mShaderResourceInterface.GetLiveUniformBlocks(); ++i) {
            mVkDescSetLayoutBind[i].binding = mShaderResourceInterface.GetUniformBlockBinding(i);
            mVkDescSetLayoutBind[i].descriptorType = mShaderResourceInterface.IsUniformBlockOpaque(i) ? VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            mVkDescSetLayoutBind[i].descriptorCount = 1;
            mVkDescSetLayoutBind[i].stageFlags = mShaderResourceInterface.GetUniformBlockBlockStage(i) == (SHADER_TYPE_VERTEX | SHADER_TYPE_FRAGMENT) ? VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT :
                                                 mShaderResourceInterface.GetUniformBlockBlockStage(i) == SHADER_TYPE_VERTEX ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
            mVkDescSetLayoutBind[i].pImmutableSamplers = NULL;
        }
    }

    VkDescriptorSetLayoutCreateInfo descLayoutInfo;
    memset((void *)&descLayoutInfo, 0, sizeof(descLayoutInfo));
    descLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descLayoutInfo.pNext = NULL;
    descLayoutInfo.flags = 0;
    descLayoutInfo.bindingCount = nLiveUniformBlocks;
    descLayoutInfo.pBindings = mVkDescSetLayoutBind;

    if(vkCreateDescriptorSetLayout(mVkContext->vkDevice, &descLayoutInfo, 0, &mVkDescSetLayout) != VK_SUCCESS) {
        assert(0);
        return false;
    }
    assert(mVkDescSetLayout != VK_NULL_HANDLE);

    mVkContext->mCommandBufferManager->RefResource(mVkDescSetLayout, RESOURCE_TYPE_DESC_SET_LAYOUT);

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    memset((void *) &pipelineLayoutCreateInfo, 0, sizeof(pipelineLayoutCreateInfo));
    pipelineLayoutCreateInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext                  = NULL;
    pipelineLayoutCreateInfo.flags                  = 0;
    pipelineLayoutCreateInfo.setLayoutCount         = 1;
    pipelineLayoutCreateInfo.pSetLayouts            = &mVkDescSetLayout;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges    = NULL;

    if(vkCreatePipelineLayout(mVkContext->vkDevice, &pipelineLayoutCreateInfo, 0, &mVkPipelineLayout) != VK_SUCCESS) {
        assert(0);
        return false;
    }
    assert(mVkPipelineLayout != VK_NULL_HANDLE);

    mVkContext->mCommandBufferManager->RefResource(mVkPipelineLayout, RESOURCE_TYPE_PIPELINE_LAYOUT);

    return true;
}

bool
ShaderProgram::CreateDescriptorPool(uint32_t nLiveUniformBlocks)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkDescriptorPoolSize *descTypeCounts = new VkDescriptorPoolSize[nLiveUniformBlocks];
    assert(descTypeCounts);
    memset((void *)descTypeCounts, 0, nLiveUniformBlocks * sizeof(*descTypeCounts));

    for(uint32_t i = 0; i < mShaderResourceInterface.GetLiveUniformBlocks(); ++i) {
        descTypeCounts[i].descriptorCount = 1;
        descTypeCounts[i].type = mShaderResourceInterface.IsUniformBlockOpaque(i) ? VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        assert(descTypeCounts[i].type == mVkDescSetLayoutBind[i].descriptorType);
    }

    VkDescriptorPoolCreateInfo descriptorPoolInfo;
    memset((void *)&descriptorPoolInfo, 0, sizeof(descriptorPoolInfo));
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.pNext = NULL;
    descriptorPoolInfo.poolSizeCount = nLiveUniformBlocks;
    descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    descriptorPoolInfo.maxSets = 1;
    descriptorPoolInfo.pPoolSizes = descTypeCounts;

    if(vkCreateDescriptorPool(mVkContext->vkDevice, &descriptorPoolInfo, 0, &mVkDescPool) != VK_SUCCESS) {
        assert(0);
        return false;
    }
    assert(mVkDescPool != VK_NULL_HANDLE);

    mVkContext->mCommandBufferManager->RefResource(mVkDescPool, RESOURCE_TYPE_DESC_POOL);

    delete[] descTypeCounts;

    return true;
}

bool
ShaderProgram::CreateDescriptorSet(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    VkDescriptorSetAllocateInfo descAllocInfo;
    memset((void *)&descAllocInfo, 0, sizeof(descAllocInfo));
    descAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descAllocInfo.pNext = NULL;
    descAllocInfo.descriptorPool = mVkDescPool;
    descAllocInfo.descriptorSetCount = 1;
    descAllocInfo.pSetLayouts = &mVkDescSetLayout;

    if(vkAllocateDescriptorSets(mVkContext->vkDevice, &descAllocInfo, &mVkDescSet) != VK_SUCCESS) {
        assert(0);
        return false;
    }
    assert(mVkDescSet != VK_NULL_HANDLE);

    return true;
}

bool
ShaderProgram::AllocateVkDescriptoSet(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    const uint32_t nLiveUniformBlocks = mShaderResourceInterface.GetLiveUniformBlocks();

    ReleaseVkObjects();

    if(!CreateDescriptorSetLayout(nLiveUniformBlocks)) {
        assert(0);
        return false;
    }

    if(!nLiveUniformBlocks) {
        return true;
    }

    if(!CreateDescriptorPool(nLiveUniformBlocks)) {
        assert(0);
        return false;
    }

    if(!CreateDescriptorSet()) {
        assert(0);
        return false;
    }

    delete[] mVkDescSetLayoutBind;
    mVkDescSetLayoutBind = nullptr;

    return true;
}

void
ShaderProgram::UpdateBuiltInUniformData(float minDepthRange, float maxDepthRange)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mMinDepthRange == minDepthRange && mMaxDepthRange == maxDepthRange) {
        return;
    }

    mMinDepthRange = minDepthRange;
    mMaxDepthRange = maxDepthRange;

    int location;
    if((location = GetUniformLocation("gl_DepthRange.near")) != -1) {
        SetUniformData(location, sizeof(float), &mMinDepthRange);
    }

    if((location = GetUniformLocation("gl_DepthRange.far")) != -1) {
        SetUniformData(location, sizeof(float), &mMaxDepthRange);
    }

    if((location = GetUniformLocation("gl_DepthRange.diff")) != -1) {
        float diffDepthRange = mMaxDepthRange - mMinDepthRange;
        SetUniformData(location, sizeof(float), &diffDepthRange);
    }
}

void
ShaderProgram::UpdateDescriptorSet(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mVkDescSet);
    assert(mGlContext);
    assert(mVkContext);

    if(mShaderResourceInterface.GetLiveUniformBlocks() == 0) {
        return;
    }

    /// Transfer any new local uniform data into the buffer objects
    if(mUpdateDescriptorData) {
        mShaderResourceInterface.UpdateUniformBufferData();
        mUpdateDescriptorData = false;
    }

    /// This can be true only in three occasions:
    /// 1. This is a freshly linked shader. So the descriptor sets need to be created
    /// 2. There has been an update in a sampler via the glUniform1i()
    /// 3. glBindTexture has been called
    if(!mUpdateDescriptorSets) {
        return;
    }

    UpdateSamplerDescriptors();

    mUpdateDescriptorSets = false;
}

void ShaderProgram::UpdateSamplerDescriptors()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    const uint32_t nLiveUniformBlocks = mShaderResourceInterface.GetLiveUniformBlocks();
    uint32_t nSamplers = 0;
    for(uint32_t i = 0; i < nLiveUniformBlocks; ++i) {
        if(mShaderResourceInterface.IsUniformBlockOpaque(i)) {
            nSamplers += mShaderResourceInterface.GetUniformArraySize(i);
        }
    }

    /// Get texture units from samplers
    uint32_t samp = 0;
    VkDescriptorImageInfo *textureDescriptors = NULL;
    if(nSamplers) {
        textureDescriptors = new VkDescriptorImageInfo[nSamplers];
        memset((void *)textureDescriptors, 0, nSamplers * sizeof(*textureDescriptors));

        for(uint32_t i = 0; i < mShaderResourceInterface.GetLiveUniforms(); ++i) {
            if(mShaderResourceInterface.GetUniformType(i) == GL_SAMPLER_2D || mShaderResourceInterface.GetUniformType(i) == GL_SAMPLER_CUBE) {
                for(int32_t j = 0; j < mShaderResourceInterface.GetUniformArraySize(i); ++j) {
                    const glsl_sampler_t textureUnit = *(glsl_sampler_t *)mShaderResourceInterface.GetUniformClientData(i);

                    /// Sampler might need an update
                    Texture *activeTexture = ((Context *)mGlContext)->GetStateManager()->GetActiveObjectsState()->GetActiveTexture(
                    mShaderResourceInterface.GetUniformType(i) == GL_SAMPLER_2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP, textureUnit); // TODO remove mGlContext

                    if( !activeTexture->IsCompleted() ) {
                        uint8_t pixels[4] = {0,0,0,255};
                        //activeTexture->SetVkImageUsage(VK_IMAGE_USAGE_SAMPLED_BIT);
                        //activeTexture->SetVkImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                        for(GLint layer = 0; layer < activeTexture->GetLayersCount(); ++layer) {
                            for(GLint level = 0; level < activeTexture->GetMipLevelsCount(); ++level) {
                                /// TODO:: how we handle this case should be investigated further
                                activeTexture->SetState(1, 1, level, layer, GL_RGBA, GL_UNSIGNED_BYTE, Texture::GetDefaultInternalAlignment(), pixels);
                            }
                        }
                        activeTexture->Allocate();
                    }
                    activeTexture->CreateVkSampler();

                    textureDescriptors[samp].sampler     = activeTexture->GetVkSampler();
                    textureDescriptors[samp].imageLayout = activeTexture->GetVkImageLayout();
                    textureDescriptors[samp].imageView   = activeTexture->GetVkImageView();

                    ++samp;
                }
            }
        }
    }
    assert(samp == nSamplers);

    samp = 0;
    VkWriteDescriptorSet *writes = new VkWriteDescriptorSet[nLiveUniformBlocks];
    memset((void*)writes, 0, nLiveUniformBlocks * sizeof(*writes));
    for(uint32_t i = 0; i < nLiveUniformBlocks; ++i) {
        writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[i].pNext = NULL;
        writes[i].dstSet = mVkDescSet;
        writes[i].dstBinding = mShaderResourceInterface.GetUniformBlockBinding(i);

        if(mShaderResourceInterface.IsUniformBlockOpaque(i)) {
            writes[i].pImageInfo = &textureDescriptors[samp];
            writes[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writes[i].descriptorCount = mShaderResourceInterface.GetUniformArraySize(i);
            samp += mShaderResourceInterface.GetUniformArraySize(i);
        } else {
            writes[i].descriptorCount = 1;
            writes[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            UniformBufferObject* pBufferObject = mShaderResourceInterface.GetUniformBufferObject(i);
            writes[i].pBufferInfo = pBufferObject->GetBufferDescInfo();
        }
    }
    assert(samp == nSamplers);

    vkUpdateDescriptorSets(mVkContext->vkDevice, nLiveUniformBlocks, writes, 0, NULL);

    delete[] writes;
    delete[] textureDescriptors;

    mUpdateDescriptorSets = false;
}

void
ShaderProgram::ResetVulkanVertexInput(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mVkPipelineVertexInput.vertexAttributeDescriptionCount = 0;
    mVkPipelineVertexInput.vertexBindingDescriptionCount = 0;
    mActiveVertexVkBuffersCount = 0;
    memset((void *)mActiveVertexVkBuffers, 0, sizeof(mActiveVertexVkBuffers));
}

void
ShaderProgram::UpdateAttributeInterface(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mShaderResourceInterface.SetReflection(mShaderCompiler->GetShaderReflection());
    mShaderResourceInterface.UpdateAttributeInterface();
    mShaderResourceInterface.SetReflectionSize();
    mShaderResourceInterface.SetReflection(nullptr);
}

void
ShaderProgram::BuildShaderResourceInterface(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mShaderResourceInterface.SetReflection(mShaderCompiler->GetShaderReflection());
    mShaderResourceInterface.CreateInterface();
    mShaderResourceInterface.SetReflection(nullptr);
    mShaderResourceInterface.AllocateUniformClientData();
    mShaderResourceInterface.AllocateUniformBufferObjects(mVkContext);

    mShaderResourceInterface.SetActiveUniformMaxLength();
    mShaderResourceInterface.SetActiveAttributeMaxLength();

    AllocateVkDescriptoSet();
    mUpdateDescriptorSets = true;
    mUpdateDescriptorData = true;
}
