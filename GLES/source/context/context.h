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
 *  @file       context.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      OpenGL ES API and Context Declarations
 *
 */

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "utils/glUtils.h"
#include "utils/glLogger.h"
#include "glslang/glslangShaderCompiler.h"
#include "state/stateManager.h"
#include "resources/resourceManager.h"
#include "vulkan/pipeline.h"
#include "vulkan/clearPass.h"
#include "vulkan/context.h"
#include "rendering_api_interface.h"

typedef enum {
    GLOVE_HOST_X86_BINARY = 1,
    GLOVE_HOST_ARM_BINARY,
    GLOVE_DEV_BINARY,
    GLOVE_MAX_BINARY_FORMATS
} glove_program_binary_formats_e;

Context    *GetCurrentContext(void);
void        SetCurrentContext(Context *ctx);

class Context {

private:
// ------------
    const vkContext_t *                         mVkContext;
// ------------
    StateManager                                mStateManager;
    ResourceManager                             mResourceManager;
    ShaderCompiler *                            mShaderCompiler;
    vulkanAPI::Pipeline *                       mPipeline;
    vulkanAPI::ClearPass *                      mClearPass;

// ------------
    void        *                               mWriteSurface;
    void        *                               mReadSurface;
    BufferObject*                               mTempIbo;
    Framebuffer *                               mWriteFBO;

    Framebuffer *                               mSystemFBO;
    vector<Texture *>                           mSystemTextures;
// ------------

    Shader        *GetShaderPtr(GLuint shader);
    ShaderProgram *GetProgramPtr(GLuint program);

    Framebuffer   *CreateFBOFromEGLSurface(EGLSurfaceInterface *eglSurfaceInterface);
    Framebuffer   *InitializeFrameBuffer(EGLSurfaceInterface *eglSurfaceInterface);
    Framebuffer   *AllocatePBufferTexture(EGLSurfaceInterface *eglSurfaceInterface);

    Texture       *CreateDepthStencil(EGLSurfaceInterface *eglSurfaceInterface);

    void BeginRendering(void);
    void PushGeometry(uint32_t vertCount, uint32_t firstVertex, bool indexed, GLenum type, const void *indices);
    void UpdateVertexAttributes(uint32_t vertCount, uint32_t firstVertex);
    void BindUniformDescriptors(VkCommandBuffer *CmdBuffer);
    void BindVertexBuffers(VkCommandBuffer *CmdBuffer, const void *indices, GLenum type, bool indexed, uint32_t vertCount);
    void DrawGeometry(VkCommandBuffer *CmdBuffer, bool indexed, uint32_t firstVertex, uint32_t vertCount);
    void SetCapability(GLenum cap, GLboolean enable);

    bool AllocateTempIndexBuffer(const void *srcData, size_t size, BufferObject** ibo);
    bool ConvertIndexBufferToUint16(const void* srcData, size_t elementCount, BufferObject** ibo);

    void InitializeDefaultTextures(void);

    void SetClearRect(void);
    void SetClearAttachments(bool clearColor, bool clearDepth, bool clearStencil);
    bool SetPipelineProgramShaderStages(ShaderProgram *progPtr);
    void SetSystemFramebuffer(Framebuffer *FBO);

// Get Functions
           uint32_t         GetProgramId(const ShaderProgram *progPtr)           { FUN_ENTRY(GL_LOG_TRACE); return (progPtr)   ? mResourceManager.FindShaderProgramID(progPtr) : 0; }
           uint32_t         GetShaderId(const Shader *shaderPtr)                 { FUN_ENTRY(GL_LOG_TRACE); return (shaderPtr) ? mResourceManager.FindShaderID(shaderPtr)      : 0; }

// Is/Has Functions
    inline bool             HasShaderCompiler(void)                              { FUN_ENTRY(GL_LOG_TRACE); GLboolean compilerSupport;
                                                                                                        GetBooleanv(GL_SHADER_COMPILER, &compilerSupport);
                                                                                                        if(!compilerSupport) { RecordError(GL_INVALID_OPERATION); return false; }
                                                                                                        return true; }
    inline bool             IsDrawModeTriangle(GLenum mode)                const { FUN_ENTRY(GL_LOG_TRACE); return (mode == GL_TRIANGLE_STRIP || mode  == GL_TRIANGLE_FAN || mode == GL_TRIANGLES); }
// Other Functions
    inline void             RecordError(GLenum error)                            { FUN_ENTRY(GL_LOG_TRACE); if (mStateManager.GetError() == GL_NO_ERROR) { mStateManager.SetError(error); } }

public:
    Context();
    ~Context();

    void                    DeleteShader(Shader *shaderPtr);

// Get Functions
    inline  StateManager    *GetStateManager(void)                                { FUN_ENTRY(GL_LOG_TRACE); return &mStateManager; }

// Set Functions
            void             SetWriteSurface(EGLSurfaceInterface *eglSurfaceInterface);
            void             SetReadSurface(EGLSurfaceInterface *eglSurfaceInterface);
    inline  void             SetNextImageIndex(uint32_t imageIndex)               { FUN_ENTRY(GL_LOG_TRACE); if(mStateManager.GetActiveObjectsState()->IsDefaultFramebufferObjectActive()) mWriteFBO->SetWriteBufferIndex(imageIndex); }

// GL API core functions
    void            ActiveTexture(GLenum texture);
    void            AttachShader(GLuint program, GLuint shader);
    void            BindAttribLocation(GLuint program, GLuint index, const char* name);
    void            BindBuffer(GLenum target, GLuint buffer);
    void            BindFramebuffer(GLenum target, GLuint framebuffer);
    void            BindRenderbuffer(GLenum target, GLuint renderbuffer);
    void            BindTexture(GLenum target, GLuint texture);
    void            BlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void            BlendEquation(GLenum mode);
    void            BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
    void            BlendFunc(GLenum sfactor, GLenum dfactor);
    void            BlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
    void            BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
    void            BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
    GLenum          CheckFramebufferStatus(GLenum target);
    void            Clear(GLbitfield mask);
    void            ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void            ClearDepthf(GLclampf depth);
    void            ClearStencil(GLint s);
    void            ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    void            CompileShader(GLuint shader);
    void            CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
    void            CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
    void            CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
    void            CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
    GLuint          CreateProgram(void);
    GLuint          CreateShader(GLenum type);
    void            CullFace(GLenum mode);
    void            DeleteBuffers(GLsizei n, const GLuint* buffers);
    void            DeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
    void            DeleteProgram(GLuint program);
    void            DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
    void            DeleteShader(GLuint shader);
    void            DeleteTextures(GLsizei n, const GLuint* textures);
    void            DepthFunc(GLenum func);
    void            DepthMask(GLboolean flag);
    void            DepthRangef(GLclampf zNear, GLclampf zFar);
    void            DetachShader(GLuint program, GLuint shader);
    void            Disable(GLenum cap);
    void            DisableVertexAttribArray(GLuint index);
    void            DrawArrays(GLenum mode, GLint first, GLsizei count);
    void            DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
    void            Enable(GLenum cap);
    void            EnableVertexAttribArray(GLuint index);
    void            Finish(void);
    void            Flush(void);
    void            FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    void            FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    void            FrontFace(GLenum mode);
    void            GenBuffers(GLsizei n, GLuint* buffers);
    void            GenerateMipmap(GLenum target);
    void            GenFramebuffers(GLsizei n, GLuint* framebuffers);
    void            GenRenderbuffers(GLsizei n, GLuint* renderbuffers);
    void            GenTextures(GLsizei n, GLuint* textures);
    void            GetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
    void            GetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
    void            GetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
    int             GetAttribLocation(GLuint program, const char* name);
    void            GetBooleanv(GLenum pname, GLboolean* params);
    void            GetBufferParameteriv(GLenum target, GLenum pname, GLint* params);
    GLenum          GetError(void);
    void            GetFloatv(GLenum pname, GLfloat* params);
    void            GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params);
    void            GetIntegerv(GLenum pname, GLint* params);
    void            GetProgramiv(GLuint program, GLenum pname, GLint* params);
    void            GetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
    void            GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params);
    void            GetShaderiv(GLuint shader, GLenum pname, GLint* params);
    void            GetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
    void            GetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
    void            GetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
    const GLubyte*  GetString(GLenum name);
    void            GetTexParameterfv(GLenum target, GLenum pname, GLfloat* params);
    void            GetTexParameteriv(GLenum target, GLenum pname, GLint* params);
    void            GetUniformfv(GLuint program, GLint location, GLfloat* params);
    void            GetUniformiv(GLuint program, GLint location, GLint* params);
    int             GetUniformLocation(GLuint program, const char* name);
    void            GetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params);
    void            GetVertexAttribiv(GLuint index, GLenum pname, GLint* params);
    void            GetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer);
    void            Hint(GLenum target, GLenum mode);
    GLboolean       IsBuffer(GLuint buffer);
    GLboolean       IsEnabled(GLenum cap);
    GLboolean       IsFramebuffer(GLuint framebuffer);
    GLboolean       IsProgram(GLuint program);
    GLboolean       IsRenderbuffer(GLuint renderbuffer);
    GLboolean       IsShader(GLuint shader);
    GLboolean       IsTexture(GLuint texture);
    void            LineWidth(GLfloat width);
    void            LinkProgram(GLuint program);
    void            PixelStorei(GLenum pname, GLint param);
    void            PolygonOffset(GLfloat factor, GLfloat units);
    void            ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
    void            ReleaseShaderCompiler(void);
    void            RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    void            SampleCoverage(GLclampf value, GLboolean invert);
    void            Scissor(GLint x, GLint y, GLsizei width, GLsizei height);
    void            ShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length);
    void            ShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
    void            StencilFunc(GLenum func, GLint ref, GLuint mask);
    void            StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
    void            StencilMask(GLuint mask);
    void            StencilMaskSeparate(GLenum face, GLuint mask);
    void            StencilOp(GLenum fail, GLenum zfail, GLenum zpass);
    void            StencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
    void            TexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
    void            TexParameterf(GLenum target, GLenum pname, GLfloat param);
    void            TexParameterfv(GLenum target, GLenum pname, const GLfloat* params);
    void            TexParameteri(GLenum target, GLenum pname, GLint param);
    void            TexParameteriv(GLenum target, GLenum pname, const GLint* params);
    void            TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
    void            Uniform1f(GLint location, GLfloat x);
    void            Uniform1fv(GLint location, GLsizei count, const GLfloat* v);
    void            Uniform1i(GLint location, GLint x);
    void            Uniform1iv(GLint location, GLsizei count, const GLint* v);
    void            Uniform2f(GLint location, GLfloat x, GLfloat y);
    void            Uniform2fv(GLint location, GLsizei count, const GLfloat* v);
    void            Uniform2i(GLint location, GLint x, GLint y);
    void            Uniform2iv(GLint location, GLsizei count, const GLint* v);
    void            Uniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z);
    void            Uniform3fv(GLint location, GLsizei count, const GLfloat* v);
    void            Uniform3i(GLint location, GLint x, GLint y, GLint z);
    void            Uniform3iv(GLint location, GLsizei count, const GLint* v);
    void            Uniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void            Uniform4fv(GLint location, GLsizei count, const GLfloat* v);
    void            Uniform4i(GLint location, GLint x, GLint y, GLint z, GLint w);
    void            Uniform4iv(GLint location, GLsizei count, const GLint* v);
    void            UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            UseProgram(GLuint program);
    void            ValidateProgram(GLuint program);
    void            VertexAttrib1f(GLuint index, GLfloat x);
    void            VertexAttrib1fv(GLuint index, const GLfloat* values);
    void            VertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
    void            VertexAttrib2fv(GLuint index, const GLfloat* values);
    void            VertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
    void            VertexAttrib3fv(GLuint index, const GLfloat* values);
    void            VertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void            VertexAttrib4fv(GLuint index, const GLfloat* values);
    void            VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
    void            Viewport(GLint x, GLint y, GLsizei width, GLsizei height);

// GL API extension functions
    void            EGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image);
    void            EGLImageTargetRenderBufferStorageOES(GLenum target, GLeglImageOES image);
    void            InsertEventMarkerEXT(GLsizei length, const GLchar *marker);
    void            PushGroupMarkerEXT(GLsizei length, const GLchar *marker);
    void            PushGroupMarkerEXT(void);
    void            GetProgramBinaryOES(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
    void            ProgramBinaryOES(GLuint program, GLenum binaryFormat, const void *binary, GLint length);

};

#endif // __CONTEXT_H__
