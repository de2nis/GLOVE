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
 *  @file       glslangShaderCompiler.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Shaders compilation and linking module. It implements ShaderCompiler interface, using glslang 
 *
 */

#include "glslangShaderCompiler.h"

bool GlslangShaderCompiler::mSlangInitialized = false;
static TBuiltInResource slangShaderResources;

GlslangShaderCompiler::GlslangShaderCompiler()
: mSlangVertCompiler(nullptr),
  mSlangFragCompiler(nullptr),
  mSlangProgLinker(nullptr),
  mShaderConverter(nullptr),
  mShaderReflection(nullptr),
  mDumpVulkanShaderReflection(false),
  mDumpInputShaderReflection(false),
  mDumpProcessedShaderSource(false),
  mSaveBinaryToFiles(false),
  mSaveSourceToFiles(false),
  mSaveSpvTextToFile(false)
{
    FUN_ENTRY(GL_LOG_TRACE);

    InitializeGlslang();
    mShaderReflection = new ShaderReflection();
}

GlslangShaderCompiler::~GlslangShaderCompiler()
{
    FUN_ENTRY(GL_LOG_TRACE);

    TerminateGlslang();
    CleanUp();
}

void
GlslangShaderCompiler::CleanUp()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mSlangVertCompiler) {
        delete mSlangVertCompiler;
        mSlangVertCompiler = nullptr;
    }

    if(mSlangFragCompiler) {
        delete mSlangFragCompiler;
        mSlangFragCompiler = nullptr;
    }

    if(mSlangProgLinker) {
        delete mSlangProgLinker;
        mSlangProgLinker = nullptr;
    }

    delete mShaderReflection;
}

bool
GlslangShaderCompiler::CompileVertexShader(const char* const* source)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mSlangInitialized);

    if(mSlangVertCompiler) {
        delete mSlangVertCompiler;
    }

    mVertSource = string(*source);
    mSlangVertCompiler = new GlslangCompiler();
    assert(mSlangVertCompiler);

    return mSlangVertCompiler->CompileShader(source, &slangShaderResources, EShLangVertex);
}

bool
GlslangShaderCompiler::CompileFragmentShader(const char* const* source)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mSlangInitialized);

    if(mSlangFragCompiler) {
        delete mSlangFragCompiler;
    }

    mFragSource = string(*source);
    mSlangFragCompiler = new GlslangCompiler();
    assert(mSlangFragCompiler);

    return mSlangFragCompiler->CompileShader(source, &slangShaderResources, EShLangFragment);
}

const char*
GlslangShaderCompiler::GetProgramInfoLog()
{
    FUN_ENTRY(GL_LOG_TRACE);

    assert(mSlangInitialized);

    return (mSlangProgLinker) ? mSlangProgLinker->GetInfoLog() : "";
}

const char*
GlslangShaderCompiler::GetShaderInfoLog(shader_type_t shaderType)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mSlangInitialized);

    if(shaderType == SHADER_TYPE_VERTEX) {
        if(!mSlangVertCompiler) {
            return "";
        }
        return mSlangVertCompiler->GetInfoLog();
    } else {
        if(!mSlangFragCompiler) {
            return "";
        }
        return mSlangFragCompiler->GetInfoLog();
    }
}

const char*
GlslangShaderCompiler::ConvertShader(ShaderProgram& program, shader_type_t shaderType)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mSaveSourceToFiles) {
        SaveShaderSourceToFile(&program, false, (shaderType == SHADER_TYPE_VERTEX) ? mVertSource.c_str() : mFragSource.c_str(), shaderType);
    }

    mShaderConverter = new ShaderConverter();
    mShaderConverter->Initialize(ShaderConverter::SHADER_CONVERSION_100_400, shaderType);
    mShaderConverter->SetSlangProgram(mSlangProgLinker->GetSlangProgram());
    mShaderConverter->SetIoMapResolver(mSlangProgLinker->GetIoMapResolver());
    mShaderConverter->Convert((shaderType == SHADER_TYPE_VERTEX) ? mVertSource400 : mFragSource400, GetUniformBlocks(), mShaderReflection);
    if(mSaveSourceToFiles) {
        SaveShaderSourceToFile(&program, true, (shaderType == SHADER_TYPE_VERTEX) ? mVertSource400.c_str() : mFragSource400.c_str(), shaderType);
    }

    if(mDumpProcessedShaderSource) {
        cout << "\n\nFINAL SOURCE:\n" << endl;
        if(shaderType == SHADER_TYPE_VERTEX) {
            cout << mVertSource400 << "\n\n" << endl;
        } else {
            cout << mFragSource400 << "\n\n" << endl;
        }
    }

    delete mShaderConverter;

    return (shaderType == SHADER_TYPE_VERTEX) ? mVertSource400.c_str() : mFragSource400.c_str();
}

void
GlslangShaderCompiler::PrepareReflection(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mSlangInitialized);

    mShaderReflection->ResetReflection();
    CompileAttributes(mSlangProgLinker->GetSlangProgram());
    CompileUniforms(mSlangProgLinker->GetSlangProgram());
}

bool
GlslangShaderCompiler::LinkProgram(ShaderProgram& shaderProgram)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mSlangInitialized);

    bool result;

    if(mSaveSourceToFiles) {
        SaveShaderSourceToFile(&shaderProgram, false, mVertSource.c_str(), SHADER_TYPE_VERTEX);
        SaveShaderSourceToFile(&shaderProgram, false, mFragSource.c_str(), SHADER_TYPE_FRAGMENT);
    }

    Shader* vertex = shaderProgram.GetVertexShader();
    assert(vertex);

    mVertSource400 = string(mVertSource);
    const char* source = ConvertShader(shaderProgram, SHADER_TYPE_VERTEX);
    result = mSlangVertCompiler->CompileShader400(&source, &slangShaderResources, EShLangVertex);
    if(!result) {
        return false;
    }

    Shader* fragment = shaderProgram.GetFragmentShader();
    assert(fragment);

    mFragSource400 = string(mFragSource);
    source = ConvertShader(shaderProgram, SHADER_TYPE_FRAGMENT);
    result = mSlangFragCompiler->CompileShader400(&source, &slangShaderResources, EShLangFragment);
    if(!result) {
        return false;
    }

    result = mSlangProgLinker->LinkProgram(mSlangVertCompiler->GetSlangShader400(),
                                           mSlangFragCompiler->GetSlangShader400());
    if(!result) {
        return false;
    }

    SetUniformBlocksSizes(mSlangProgLinker->GetSlangProgram400());
    SetUniformBlocksOffset(mSlangProgLinker->GetSlangProgram400());
    BuildUniformReflection();

    mSlangProgLinker->GenerateSPV(vertex->GetSPV(), fragment->GetSPV());

    if(mSaveBinaryToFiles) {
        SaveBinaryToFiles(&shaderProgram);
    }

    if(mSaveSpvTextToFile) {
        PrintReadableSPV(&shaderProgram);
    }

    if(mDumpVulkanShaderReflection) {
        printf("glslang's vulkan shader reflection:\n");
        DumpSlangProgramReflection(mSlangProgLinker->GetSlangProgram400());
    }

    return result;
}

void
GlslangShaderCompiler::PrintReadableSPV(ShaderProgram* program)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    mSlangProgLinker->GenerateSPV(mVertSpv, mFragSpv);
    if(!mVertSpv.empty()) {
        stringstream filename;
        filename << "vert_" << hex << (uintptr_t)program << ".spv.txt";

        ofstream out;
        out.open(filename.str(), ios::binary | ios::out);
        spv::Disassemble(out, mVertSpv);
        out.close();
    }

    if(!mFragSpv.empty()) {
        stringstream filename;
        filename << "frag_" << hex << (uintptr_t)program << ".spv.txt";

        ofstream out;
        out.open(filename.str(), ios::binary | ios::out);
        spv::Disassemble(out, mFragSpv);
        out.close();
    }
}

void
GlslangShaderCompiler::SaveBinaryToFiles(ShaderProgram* program)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mVertSpv.empty() || mFragSpv.empty()) {
        mSlangProgLinker->GenerateSPV(mVertSpv, mFragSpv);
    }
    stringstream filename;
    filename << "vert_" << hex << (uintptr_t)program << ".spv.bin";
    glslang::OutputSpvBin(mVertSpv, filename.str().c_str());

    filename.str(string());
    filename << "frag_" << hex << (uintptr_t)program << ".spv.bin";
    glslang::OutputSpvBin(mFragSpv, filename.str().c_str());
}

bool
GlslangShaderCompiler::ValidateProgram(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(mSlangInitialized);

    bool result;

    if(mSlangProgLinker) {
        delete mSlangProgLinker;
    }

    mSlangProgLinker = new GlslangLinker();
    assert(mSlangProgLinker);

    result = mSlangProgLinker->ValidateProgram(mSlangVertCompiler->GetSlangShader(),
                                               mSlangFragCompiler->GetSlangShader());

    if(mDumpInputShaderReflection) {
        printf("Glslang's input shader reflection:\n");
        DumpSlangProgramReflection(mSlangProgLinker->GetSlangProgram());
    }

    return result;
}

bool
GlslangShaderCompiler::InitializeGlslang()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(!mSlangInitialized) {
        mSlangInitialized = glslang::InitializeProcess();
        InitSlangShaderResources();
    }

    assert(mSlangInitialized);

    return mSlangInitialized;
}

void
GlslangShaderCompiler::InitSlangShaderResources(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    slangShaderResources.maxVertexAttribs              = GLOVE_MAX_VERTEX_ATTRIBS;
    slangShaderResources.maxVaryingVectors             = GLOVE_MAX_VARYING_VECTORS;
    slangShaderResources.maxVertexUniformVectors       = GLOVE_MAX_VERTEX_UNIFORM_VECTORS;
    slangShaderResources.maxFragmentUniformVectors     = GLOVE_MAX_FRAGMENT_UNIFORM_VECTORS;
    slangShaderResources.maxVertexTextureImageUnits    = GLOVE_MAX_VERTEX_TEXTURE_IMAGE_UNITS;
    slangShaderResources.maxTextureUnits               = GLOVE_MAX_TEXTURE_IMAGE_UNITS;
    slangShaderResources.maxCombinedTextureImageUnits  = GLOVE_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
}

void
GlslangShaderCompiler::TerminateGlslang()
{
    FUN_ENTRY(GL_LOG_DEBUG);

    if(mSlangInitialized) {
        glslang::FinalizeProcess();
        mSlangInitialized = false;
    }
}

void
GlslangShaderCompiler::CompileAttributes(const glslang::TProgram* prog)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    int location;

    const int nLiveAttributes = prog->getNumLiveAttributes();
    mShaderReflection->SetLiveAttributes(nLiveAttributes);
    if(!nLiveAttributes) {
        return;
    }

    for(int i = 0; i < nLiveAttributes; ++i) {
        if(GetAttributeHasLocation(prog, i)) {
            location = GetAttributeLocation(prog, i);
        } else {
            location = GLOVE_INVALID_OFFSET;
        }

        string attributeName = prog->getAttributeName(i);
        mShaderReflection->SetAttributeName(attributeName.c_str(), i);
        mShaderReflection->SetAttributeType(prog->getAttributeType(i), i);
        mShaderReflection->SetAttributeLocation((uint32_t)location, i);
    }
}

void
GlslangShaderCompiler::CompileUniforms(const glslang::TProgram* prog)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    /// Reset uniforms
    mAggregates.clear();
    mUniforms.clear();
    mUniformBlocks.clear();

    const int nLiveUniforms = prog->getNumLiveUniformVariables();
    if(!nLiveUniforms) {
        return;
    }

    /// NOTE: Reserve space in vector. This is very important when emplacing back uniform_t objects,
    /// as a change in vector's capacity can cause reallocation, which in turn will call uniform_t's destructor when we do not expect it
    mUniforms.reserve(nLiveUniforms);

    for(int i = 0; i < nLiveUniforms; ++i) {
        aggregate_t *pAggregate = nullptr;
        string uniformName = string(prog->getUniformName(i));
        int32_t arrayIndex = -1;

        /// Does this uniform belong to an aggregate type?
        const size_t dotPosition = uniformName.find_first_of(".");
        if(dotPosition != string::npos) {
            string aggregateName = string(uniformName, 0, dotPosition);

            /// Is this aggregate an array?
            const size_t firstOpenBracket = aggregateName.find_first_of("[");
            if(firstOpenBracket != string::npos) {
                /// Get array index
                const size_t firstCloseBracket = aggregateName.find_first_of("]");
                arrayIndex = stoi(string(aggregateName, firstOpenBracket + 1, firstCloseBracket - firstOpenBracket - 1));
                assert(arrayIndex >= 0);
                /// remove "[*]" from name
                aggregateName = string(aggregateName, 0, firstOpenBracket);
            }

            /// Remove any other "[*]" from uniformName too
            size_t lastOpenBracket = uniformName.find_last_of("[");
            while(lastOpenBracket > dotPosition && lastOpenBracket != string::npos){
                const size_t lastCloseBracket = uniformName.find_last_of("]");
                uniformName.erase(lastOpenBracket, lastCloseBracket - lastOpenBracket + 1);
                lastOpenBracket = uniformName.find_last_of("[");
            }

            /// Register aggregate and keep track of highest array index (if any)
            aggregateMap_t::iterator aggrIt = mAggregates.find(aggregateName);
            if(aggrIt != mAggregates.end()) {
                if(arrayIndex > -1 && aggrIt->second.arraySize < arrayIndex + 1) {
                    aggrIt->second.arraySize = arrayIndex + 1;
                }
                pAggregate = &aggrIt->second;
            } else {
                mAggregates[aggregateName] = {aggregateName, arrayIndex > -1 ? arrayIndex + 1: 1};
                pAggregate = &mAggregates[aggregateName];
            }
        }

        assert((!pAggregate && dotPosition == string::npos) || (pAggregate && dotPosition != string::npos));

        /// Create uniform
        const int arraySize = prog->getUniformArraySize(i);
        const GLenum glType = static_cast<GLenum>(prog->getUniformType(i));
        const shader_type_t stage = prog->getUniformStages(i) == (EShLangVertexMask | EShLangFragmentMask) ? static_cast<shader_type_t>(SHADER_TYPE_VERTEX | SHADER_TYPE_FRAGMENT) :
                                    prog->getUniformStages(i) == EShLangVertexMask ? SHADER_TYPE_VERTEX : SHADER_TYPE_FRAGMENT;

        mUniforms.emplace_back(string(uniformName, dotPosition != string::npos ? dotPosition + 1 : 0),  /// variableName
                               uniformName + (arraySize > 1 ? string("[0]") : string("")),              /// reflectionName
                               glType,                                                                  /// glType
                               GLOVE_MAX_COMBINED_UNIFORM_VECTORS,                                      /// location (not assigned yet)
                               arraySize,                                                               /// arraySize
                               arrayIndex,                                                              /// indexIntoAggregateArray
                               pAggregate,                                                              /// pAggregate
                               nullptr,                                                                 /// pBlock (not known yet)
                               stage,                                                                   /// stage
                               0);                                                                      /// offset (not known yet)
    }

    assert(mUniforms.size() == (uint32_t)nLiveUniforms);

    /// Generate locations for uniforms
    /// and bindings for uniform blocks
    uint32_t location = 0, binding = 0;
    for(const auto &aggr : mAggregates) {
        /// Each unique aggregate name will be encapsulated into a uniform block
        if(mUniformBlocks.find(aggr.second.name) == mUniformBlocks.end()) {
            mUniformBlocks[aggr.second.name] = {aggr.second.name,                                       /// Copy name for debugging
                                                string("uni") + to_string(binding),                     /// Construct uniform block's name
                                                binding,                                                /// Binding index
                                                false,                                                  /// Definitely not an opaque type
                                                0,                                                      /// blockSize (not known yet)
                                                INVALID_SHADER,                                         /// stage (not known yet)
                                                &aggr.second};                                          /// This block encapsulates an aggregate type (or an array of one)
            ++binding;
        }

        for(int32_t arrayIndex = 0; arrayIndex < aggr.second.arraySize; ++arrayIndex) {
            for(auto &uni : mUniforms) {
                if(uni.pAggregate == &aggr.second && (uni.indexIntoAggregateArray == arrayIndex || uni.indexIntoAggregateArray == -1)) {
                    uni.location = location;
                    assert(mUniformBlocks.find(aggr.second.name) != mUniformBlocks.end());
                    uni.pBlock = &mUniformBlocks.find(aggr.second.name)->second;
                    assert(uni.pBlock);
                    uni.pBlock->blockStage = static_cast<shader_type_t>(uni.pBlock->blockStage | uni.stage);
                    location += uni.arraySize;
                }
            }
        }
    }

    assert(mUniformBlocks.size() == mAggregates.size());

    /// For uniforms that do not belong to an aggregate type
    for(auto &uni : mUniforms) {
        if(!uni.pAggregate) {
            const bool isSampler = uni.glType == GL_SAMPLER_2D || uni.glType == GL_SAMPLER_CUBE;

            /// Each uniform that does not belong to an aggregate struct/array
            /// and is not of a sampler type, will be encapsulated into a uniform block
            if(mUniformBlocks.find(uni.variableName) == mUniformBlocks.end()) {
                mUniformBlocks[uni.variableName] = {uni.variableName,                                   /// Copy name for debugging
                                                    string("uni") + to_string(binding),                 /// Construct uniform block's name
                                                    binding,                                            /// Binding index
                                                    isSampler,                                          /// true for samplers
                                                    0,                                                  /// blockSize (not known yet)
                                                    uni.stage,                                          /// stage
                                                    nullptr};                                           /// This block encapsulates a base type (or an array of one)
                ++binding;
            }

            assert(uni.location == GLOVE_MAX_COMBINED_UNIFORM_VECTORS);
            assert(!uni.pBlock);
            uni.location = location;
            assert(mUniformBlocks.find(uni.variableName) != mUniformBlocks.end());
            uni.pBlock = &mUniformBlocks.find(uni.variableName)->second;
            assert(uni.pBlock);

            location += uni.arraySize ? uni.arraySize : 1;
        }
    }
}

void
GlslangShaderCompiler::SetUniformBlocksSizes(const glslang::TProgram* prog)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    /// Get uniform block sizes
    for(uint32_t index = 0; index < (uint32_t)prog->getNumLiveUniformBlocks(); ++index) {
        assert(prog->getUniformBlockSize(index) != -1);
        for(auto& block : mUniformBlocks) {
            if(!block.second.glslBlockName.compare(prog->getUniformBlockName(index))) {
                assert(!block.second.isOpaque);
                block.second.blockSize = prog->getUniformBlockSize(index);
            }
        }
    }
}

void
GlslangShaderCompiler::SetUniformBlocksOffset(const glslang::TProgram* prog)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    for(auto& block : mUniformBlocks) {
        if(block.second.pAggregate == nullptr) {
            continue;
        }

        const uniformBlock_t *pBlock = &block.second;
        size_t aggregateSize = pBlock->blockSize / pBlock->pAggregate->arraySize;
        for(auto& uni : mUniforms) {
            if(uni.pAggregate != pBlock->pAggregate) {
                continue;
            }

            assert(uni.pBlock);
            assert((uni.pBlock == pBlock && uni.pAggregate == pBlock->pAggregate) || (uni.pBlock != pBlock && uni.pAggregate != pBlock->pAggregate));

            /// Query uniform's offset in block from glslang
            size_t offset = GLOVE_INVALID_OFFSET;
            string uniName;
            if(uni.pAggregate->name.compare("gl_DepthRange")) {
                uniName = uni.pAggregate->name + string(".") + uni.variableName;
            } else {
                uniName = string(STRINGIFY_MACRO(VULKAN_DEPTH_RANGE)) + string(".") + uni.variableName;
            }

            for(uint32_t index = 0; index < (uint32_t)prog->getNumLiveUniformVariables(); ++index) {
                if(!strcmp(uniName.c_str(), prog->getUniformName(index))) {
                    offset = prog->getUniformBufferOffset(index);
                    break;
                }
            }
            assert(offset != GLOVE_INVALID_OFFSET);

            uni.offset = (uni.indexIntoAggregateArray > -1 ? uni.indexIntoAggregateArray : 0) * aggregateSize + offset;
        }
    }
}

void
GlslangShaderCompiler::BuildUniformReflection(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    const int nLiveUniforms = mUniforms.size();
    const int nLiveUniformBlocks = mUniformBlocks.size();
    mShaderReflection->SetLiveUniforms(nLiveUniforms);
    mShaderReflection->SetLiveUniformBlocks(nLiveUniformBlocks);

    int i = 0;
    for(auto& block : mUniformBlocks) {
        mShaderReflection->SetUniformBlockGlslBlockName(block.second.glslBlockName.c_str(), i);
        mShaderReflection->SetUniformBlockBinding(block.second.binding, i);
        mShaderReflection->SetUniformBlockBlockSize(block.second.blockSize, i);
        mShaderReflection->SetUniformBlockBlockStage(block.second.blockStage, i);
        mShaderReflection->SetUniformBlockOpaque(block.second.isOpaque, i);
        ++i;
    }

    i = 0;
    for(auto& uni : mUniforms) {
        const uniformBlock_t *pBlock = uni.pBlock;
        uint32_t index = 0;
        for(auto& block : mUniformBlocks) {
            if(!block.second.glslBlockName.compare(pBlock->glslBlockName)) {
                mShaderReflection->SetUniformReflectionName(uni.reflectionName.c_str(), i);
                mShaderReflection->SetUniformLocation(uni.location, i);
                mShaderReflection->SetUniformBlockIndex(index, i);
                mShaderReflection->SetUniformArraySize(uni.arraySize, i);
                mShaderReflection->SetUniformType(uni.glType, i);
                mShaderReflection->SetUniformOffset(uni.offset, i);

                break;
            }
            ++index;
        }
        ++i;
    }
}

bool
GlslangShaderCompiler::GetAttributeHasLocation(const glslang::TProgram* prog, int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    const glslang::TType *attributeType = prog->getAttributeTType(index);
    return attributeType->getQualifier().hasLocation();
}

int
GlslangShaderCompiler::GetAttributeLocation(const glslang::TProgram* prog, int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    const glslang::TType *attributeType = prog->getAttributeTType(index);
    return attributeType->getQualifier().hasLocation() ? attributeType->getQualifier().layoutLocation : -1;
}

bool
GlslangShaderCompiler::GetUniformHasLocation(const glslang::TProgram* prog, int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    const glslang::TType *uniformType = prog->getUniformTType(index);
    return uniformType->getQualifier().hasLocation();
}

int
GlslangShaderCompiler::GetUniformLocation(const glslang::TProgram* prog, int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    const glslang::TType *uniformType = prog->getUniformTType(index);
    return uniformType->getQualifier().hasLocation() ? uniformType->getQualifier().layoutLocation : -1;
}

bool
GlslangShaderCompiler::GetUniformHasBinding(const glslang::TProgram* prog, int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    const glslang::TType *uniformType = prog->getUniformTType(index);
    return uniformType->getQualifier().hasBinding();
}

int
GlslangShaderCompiler::GetUniformBinding(const glslang::TProgram* prog, int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    return prog->getUniformBinding(index);
}

bool
GlslangShaderCompiler::GetUniformHasSet(const glslang::TProgram* prog, int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    const glslang::TType *uniformType = prog->getUniformTType(index);
    return uniformType->getQualifier().hasSet();
}

int
GlslangShaderCompiler::GetUniformSet(const glslang::TProgram* prog, int index) const
{
    FUN_ENTRY(GL_LOG_TRACE);

    const glslang::TType *uniformType = prog->getUniformTType(index);
    return uniformType->getQualifier().hasSet() ? uniformType->getQualifier().layoutSet : -1;
}

void
GlslangShaderCompiler::DumpUniforms(void)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    printf("Shader resource interface:\n");

    printf("UNIFORMS\n");
    for(const auto &uni : mUniforms) {
        /// Print aggregate name
        if(uni.pAggregate) {
            printf("%s", uni.pAggregate->name.c_str());
            if(uni.indexIntoAggregateArray > -1) {
                printf("[%d]", uni.indexIntoAggregateArray);
            }
            printf(".");
        }

        /// Rest of variable's name without base aggregate
        /// If array size > 1 then append [0] in name
        printf("%s%s\n", uni.variableName.c_str(), uni.arraySize > 1 ? "[0]" : "");

        printf("  reflection name: %s\n", uni.reflectionName.c_str());
        printf("  GL type: 0x%04x\n", uni.glType);
        printf("  location: %u\n", uni.location);
        printf("  arraySize: %d\n", uni.arraySize > 1 ? uni.arraySize : 0);
        printf("  belongs to block: %s with offset: %zu", uni.pBlock ? uni.pBlock->glslBlockName.c_str() : "", uni.offset);
        printf("\n\n");
    }

    printf("UNIFORM BLOCKS\n");

    for(auto &it : mUniformBlocks) {
        printf("%s\n", it.second.glslBlockName.c_str());
        printf("  with size: %zu\n", it.second.blockSize);
        printf("  encapsulates aggregate type: %s\n", it.second.pAggregate ? it.second.pAggregate->name.c_str() : "(none)");
    }

    printf("\n\n");
}

void
GlslangShaderCompiler::DumpSlangProgramReflection(const glslang::TProgram *prog) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    assert(prog);

    const GlslangIoMapResolver *ioMapResolver = mSlangProgLinker->GetIoMapResolver();
    int i;
    printf("**********");
    printf("\nGL_ACTIVE_UNIFORMS_BLOCKS: %d\n", prog->getNumLiveUniformBlocks());
    for(i = 0; i < prog->getNumLiveUniformBlocks(); ++i) {
        printf("%s\n", prog->getUniformBlockName(i));
        printf("  hasLocation: %d = %d\n", GetUniformHasLocation(prog, i), GetUniformLocation(prog, i));
        printf("  getUniformHasBinding: %d = %d\n", GetUniformHasBinding(prog, i), GetUniformBinding(prog, i));
        printf("  getUniformHasSet: %d = %d\n", GetUniformHasSet(prog, i), GetUniformSet(prog, i));
        printf("  block size: %d\n", prog->getUniformBlockSize(i));
    }

    printf("\nGL_ACTIVE_UNIFORMS: %d\n", prog->getNumLiveUniformVariables());
    for(i = 0; i < prog->getNumLiveUniformVariables(); ++i) {
        printf("%s (0x%x)\n", prog->getUniformName(i), prog->getUniformType(i));
        printf("  hasLocation: %d = %d\n", GetUniformHasLocation(prog, i), GetUniformLocation(prog, i));
        printf("  getUniformHasBinding: %d = %d\n", GetUniformHasBinding(prog, i), GetUniformBinding(prog, i));
        printf("  getUniformHasSet: %d = %d\n", GetUniformHasSet(prog, i), GetUniformSet(prog, i));
        printf("  Uniform Block index: %d Uniform Block offset : %d Arraysize: %d\n", prog->getUniformBlockIndex(i),
                                                                                      prog->getUniformBufferOffset(i),
                                                                                      prog->getUniformArraySize(i));
    }

    printf("\nGL_ACTIVE_ATTRIBUTES varyings in: %d\n", ioMapResolver->GetNumLiveVaryingInVariables());
    for(uint32_t i = 0; i < ioMapResolver->GetNumLiveVaryingInVariables(); ++i) {
        printf("%s (%s)\n", ioMapResolver->GetVaryingInName(i), ioMapResolver->GetVaryingInType(i));
        printf("  hasLocation: %d = %d\n", ioMapResolver->GetVaryingInHasLocation(i), ioMapResolver->GetVaryingInLocation(i));
    }

    printf("\nACTIVE VARYINGS varyings out: %d\n", ioMapResolver->GetNumLiveVaryingOutVariables());
    for(uint32_t i = 0; i < ioMapResolver->GetNumLiveVaryingOutVariables(); ++i) {
        printf("%s (%s)\n", ioMapResolver->GetVaryingOutName(i), ioMapResolver->GetVaryingOutType(i));
        printf("  hasLocation: %d = %d\n", ioMapResolver->GetVaryingInHasLocation(i), ioMapResolver->GetVaryingInLocation(i));
    }

    printf("\nGL_ACTIVE_ATTRIBUTES: %d\n", prog->getNumLiveAttributes());
    for(i = 0; i < prog->getNumLiveAttributes(); ++i) {
        printf("%s (0x%x)\n", prog->getAttributeName(i), prog->getAttributeType(i));
        printf("  hasLocation: %d = %d\n", GetAttributeHasLocation(prog, i), GetAttributeLocation(prog, i));
    }

    printf("**********\n\n");
}

void
GlslangShaderCompiler::SaveShaderSourceToFile(ShaderProgram* program, bool processed, const char* source, shader_type_t shaderType) const
{
    FUN_ENTRY(GL_LOG_DEBUG);

    string filename = static_cast<ostringstream *>(&(ostringstream().flush() << (processed ? "shader_processed" : "shader_") << hex << (uintptr_t)program))->str();
    filename = filename + string(shaderType == SHADER_TYPE_VERTEX ? ".vert" : ".frag");

    FILE *fp = fopen(filename.c_str(), "w");
    assert(fp);

    if(fp) {
        size_t ASSERT_ONLY bytes;
        bytes = fwrite(source, 1, strlen(source), fp);

        assert(bytes == strlen(source));

        fclose(fp);
    }
}

uint32_t
GlslangShaderCompiler::SerializeReflection(void* binary)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return mShaderReflection->SerializeReflection(binary);
}

uint32_t
GlslangShaderCompiler::DeserializeReflection(const void* binary)
{
    FUN_ENTRY(GL_LOG_DEBUG);

    return mShaderReflection->DeserializeReflection(binary);
}
