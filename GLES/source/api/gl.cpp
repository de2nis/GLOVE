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
 *  @file       gl.cpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Entry points for the OpenGL ES API calls
 *
 */

#include "context/context.h"

#define CONTEXT_EXEC(func)          FUN_ENTRY(GL_LOG_INFO);                      \
                                    Context * context = GetCurrentContext();     \
                                    if (context) {                               \
                                        context->func;                           \
                                    }

#define CONTEXT_EXEC_RETURN(func)   FUN_ENTRY(GL_LOG_INFO);                      \
                                    Context * context = GetCurrentContext();     \
                                    return context ? context->func : 0;

GL_APICALL void GL_APIENTRY
glActiveTexture(GLenum texture)
{
    CONTEXT_EXEC(ActiveTexture(texture));
}

GL_APICALL void GL_APIENTRY
glAttachShader(GLuint program, GLuint shader)
{
    CONTEXT_EXEC(AttachShader(program, shader));
}

GL_APICALL void GL_APIENTRY
glBindAttribLocation(GLuint program, GLuint index, const char* name)
{
    CONTEXT_EXEC(BindAttribLocation(program, index, name));
}

GL_APICALL void GL_APIENTRY
glBindBuffer(GLenum target, GLuint buffer)
{
    CONTEXT_EXEC(BindBuffer(target, buffer));
}

GL_APICALL void GL_APIENTRY
glBindFramebuffer(GLenum target, GLuint framebuffer)
{
    CONTEXT_EXEC(BindFramebuffer(target, framebuffer));
}

GL_APICALL void GL_APIENTRY
glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
    CONTEXT_EXEC(BindRenderbuffer(target, renderbuffer));
}

GL_APICALL void GL_APIENTRY
glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    CONTEXT_EXEC(BlendColor(red, green, blue, alpha));
}

GL_APICALL void GL_APIENTRY
glBlendEquation(GLenum mode)
{
    CONTEXT_EXEC(BlendEquation(mode));
}

GL_APICALL void GL_APIENTRY
glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    CONTEXT_EXEC(BlendEquationSeparate(modeRGB, modeAlpha));
}

GL_APICALL void GL_APIENTRY
glBlendFunc(GLenum sfactor, GLenum dfactor)
{
    CONTEXT_EXEC(BlendFunc(sfactor, dfactor));
}

GL_APICALL void GL_APIENTRY
glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    CONTEXT_EXEC(BlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha));
}

GL_APICALL void GL_APIENTRY
glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    CONTEXT_EXEC(BufferData(target, size, data, usage));
}

GL_APICALL void GL_APIENTRY
glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
    CONTEXT_EXEC(BufferSubData(target, offset, size, data));
}

GL_APICALL GLenum GL_APIENTRY
glCheckFramebufferStatus(GLenum target)
{
    CONTEXT_EXEC_RETURN(CheckFramebufferStatus(target));
}

GL_APICALL void GL_APIENTRY
glClear(GLbitfield mask)
{
    CONTEXT_EXEC(Clear(mask));
}

GL_APICALL void GL_APIENTRY
glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    CONTEXT_EXEC(ClearColor(red, green, blue, alpha));
}

GL_APICALL void GL_APIENTRY
glClearDepthf(GLclampf depth)
{
    CONTEXT_EXEC(ClearDepthf(depth));
}

GL_APICALL void GL_APIENTRY
glClearStencil(GLint s)
{
    CONTEXT_EXEC(ClearStencil(s));
}

GL_APICALL void GL_APIENTRY
glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    CONTEXT_EXEC(ColorMask(red, green, blue, alpha));
}

GL_APICALL void GL_APIENTRY
glCompileShader(GLuint shader)
{
    CONTEXT_EXEC(CompileShader(shader));
}

GL_APICALL void GL_APIENTRY
glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
    CONTEXT_EXEC(CompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data));
}

GL_APICALL void GL_APIENTRY
glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data)
{
    CONTEXT_EXEC(CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data));
}

GL_APICALL void GL_APIENTRY
glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    CONTEXT_EXEC(CopyTexImage2D(target, level, internalformat, x, y, width, height, border));
}

GL_APICALL void GL_APIENTRY
glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    CONTEXT_EXEC(CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height));
}

GL_APICALL GLuint GL_APIENTRY
glCreateProgram(void)
{
    CONTEXT_EXEC_RETURN(CreateProgram());
}

GL_APICALL GLuint GL_APIENTRY
glCreateShader(GLenum type)
{
    CONTEXT_EXEC_RETURN(CreateShader(type));
}

GL_APICALL void GL_APIENTRY
glCullFace(GLenum mode)
{
    CONTEXT_EXEC(CullFace(mode));
}

GL_APICALL void GL_APIENTRY
glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
    CONTEXT_EXEC(DeleteBuffers(n, buffers));
}

GL_APICALL void GL_APIENTRY
glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
    CONTEXT_EXEC(DeleteFramebuffers(n, framebuffers));
}

GL_APICALL void GL_APIENTRY
glDeleteProgram(GLuint program)
{
    CONTEXT_EXEC(DeleteProgram(program));
}

GL_APICALL void GL_APIENTRY
glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
    CONTEXT_EXEC(DeleteRenderbuffers(n, renderbuffers));
}

GL_APICALL void GL_APIENTRY
glDeleteShader(GLuint shader)
{
    CONTEXT_EXEC(DeleteShader(shader));
}

GL_APICALL void GL_APIENTRY
glDeleteTextures(GLsizei n, const GLuint* textures)
{
    CONTEXT_EXEC(DeleteTextures(n, textures));
}

GL_APICALL void GL_APIENTRY
glDepthFunc(GLenum func)
{
    CONTEXT_EXEC(DepthFunc(func));
}

GL_APICALL void GL_APIENTRY
glDepthMask(GLboolean flag)
{
    CONTEXT_EXEC(DepthMask(flag));
}

GL_APICALL void GL_APIENTRY
glDepthRangef(GLclampf zNear, GLclampf zFar)
{
    CONTEXT_EXEC(DepthRangef(zNear, zFar));
}

GL_APICALL void GL_APIENTRY
glDetachShader(GLuint program, GLuint shader)
{
    CONTEXT_EXEC(DetachShader(program, shader));
}

GL_APICALL void GL_APIENTRY
glDisable(GLenum cap)
{
    CONTEXT_EXEC(Disable(cap));
}

GL_APICALL void GL_APIENTRY
glDisableVertexAttribArray(GLuint index)
{
    CONTEXT_EXEC(DisableVertexAttribArray(index));
}

GL_APICALL void GL_APIENTRY
glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    CONTEXT_EXEC(DrawArrays(mode, first, count));
}

GL_APICALL void GL_APIENTRY
glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
    CONTEXT_EXEC(DrawElements(mode, count, type, indices));
}

GL_APICALL void GL_APIENTRY
glEnable(GLenum cap)
{
    CONTEXT_EXEC(Enable(cap));
}

GL_APICALL void GL_APIENTRY
glEnableVertexAttribArray(GLuint index)
{
    CONTEXT_EXEC(EnableVertexAttribArray(index));
}

GL_APICALL void GL_APIENTRY
glFinish(void)
{
    CONTEXT_EXEC(Finish());
}

GL_APICALL void GL_APIENTRY
glFlush(void)
{
    CONTEXT_EXEC(Flush());
}

GL_APICALL void GL_APIENTRY
glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    CONTEXT_EXEC(FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer));
}

GL_APICALL void GL_APIENTRY
glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    CONTEXT_EXEC(FramebufferTexture2D(target, attachment, textarget, texture, level));
}

GL_APICALL void GL_APIENTRY
glFrontFace(GLenum mode)
{
    CONTEXT_EXEC(FrontFace(mode));
}

GL_APICALL void GL_APIENTRY
glGenBuffers(GLsizei n, GLuint* buffers)
{
    CONTEXT_EXEC(GenBuffers(n, buffers));
}

GL_APICALL void GL_APIENTRY
glGenerateMipmap(GLenum target)
{
    CONTEXT_EXEC(GenerateMipmap(target));
}

GL_APICALL void GL_APIENTRY
glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
    CONTEXT_EXEC(GenFramebuffers(n, framebuffers));
}

GL_APICALL void GL_APIENTRY
glGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
    CONTEXT_EXEC(GenRenderbuffers(n, renderbuffers));
}

GL_APICALL void GL_APIENTRY
glBindTexture(GLenum target, GLuint texture)
{
    CONTEXT_EXEC(BindTexture(target, texture));
}

GL_APICALL void GL_APIENTRY
glGenTextures(GLsizei n, GLuint* textures)
{
    CONTEXT_EXEC(GenTextures(n, textures));
}

GL_APICALL void GL_APIENTRY
glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
    CONTEXT_EXEC(GetActiveAttrib(program, index, bufsize, length, size, type, name));
}

GL_APICALL void GL_APIENTRY
glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
    CONTEXT_EXEC(GetActiveUniform(program, index, bufsize, length, size, type, name));
}

GL_APICALL void GL_APIENTRY
glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
    CONTEXT_EXEC(GetAttachedShaders(program, maxcount, count, shaders));
}

GL_APICALL int  GL_APIENTRY
glGetAttribLocation(GLuint program, const char* name)
{
    CONTEXT_EXEC_RETURN(GetAttribLocation(program, name));
}

GL_APICALL void GL_APIENTRY
glGetBooleanv(GLenum pname, GLboolean* params)
{
    CONTEXT_EXEC(GetBooleanv(pname, params));
}

GL_APICALL void GL_APIENTRY
glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
    CONTEXT_EXEC(GetBufferParameteriv(target, pname, params));
}

GL_APICALL GLenum GL_APIENTRY
glGetError(void)
{
    CONTEXT_EXEC_RETURN(GetError());
}

GL_APICALL void GL_APIENTRY
glGetFloatv(GLenum pname, GLfloat* params)
{
    CONTEXT_EXEC(GetFloatv(pname, params));
}

GL_APICALL void GL_APIENTRY
glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
    CONTEXT_EXEC(GetFramebufferAttachmentParameteriv(target, attachment, pname, params));
}

GL_APICALL void GL_APIENTRY
glGetIntegerv(GLenum pname, GLint* params)
{
    CONTEXT_EXEC(GetIntegerv(pname, params));
}

GL_APICALL void GL_APIENTRY
glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
    CONTEXT_EXEC(GetProgramiv(program, pname, params));
}

GL_APICALL void GL_APIENTRY
glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
{
    CONTEXT_EXEC(GetProgramInfoLog(program, bufsize, length, infolog));
}

GL_APICALL void GL_APIENTRY
glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
    CONTEXT_EXEC(GetRenderbufferParameteriv(target, pname, params));
}

GL_APICALL void GL_APIENTRY
glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
    CONTEXT_EXEC(GetShaderiv(shader, pname, params));
}

GL_APICALL void GL_APIENTRY
glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
{
    CONTEXT_EXEC(GetShaderInfoLog(shader, bufsize, length, infolog));
}

GL_APICALL void GL_APIENTRY
glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
    CONTEXT_EXEC(GetShaderPrecisionFormat(shadertype, precisiontype, range, precision));
}

GL_APICALL void GL_APIENTRY
glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source)
{
    CONTEXT_EXEC(GetShaderSource(shader, bufsize, length, source));
}

GL_APICALL const GLubyte* GL_APIENTRY
glGetString(GLenum name)
{
    CONTEXT_EXEC_RETURN(GetString(name));
}

GL_APICALL void GL_APIENTRY
glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
    CONTEXT_EXEC(GetTexParameterfv(target, pname, params));
}

GL_APICALL void GL_APIENTRY
glGetTexParameteriv(GLenum target, GLenum pname, GLint* params)
{
    CONTEXT_EXEC(GetTexParameteriv(target, pname, params));
}

GL_APICALL void GL_APIENTRY
glGetUniformfv(GLuint program, GLint location, GLfloat* params)
{
    CONTEXT_EXEC(GetUniformfv(program, location, params));
}

GL_APICALL void GL_APIENTRY
glGetUniformiv(GLuint program, GLint location, GLint* params)
{
    CONTEXT_EXEC(GetUniformiv(program, location, params));
}

GL_APICALL int  GL_APIENTRY
glGetUniformLocation(GLuint program, const char* name)
{
    CONTEXT_EXEC_RETURN(GetUniformLocation(program, name));
}

GL_APICALL void GL_APIENTRY
glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params)
{
    CONTEXT_EXEC(GetVertexAttribfv(index, pname, params));
}

GL_APICALL void GL_APIENTRY
glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params)
{
    CONTEXT_EXEC(GetVertexAttribiv(index, pname, params));
}

GL_APICALL void GL_APIENTRY
glGetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer)
{
    CONTEXT_EXEC(GetVertexAttribPointerv(index, pname, pointer));
}

GL_APICALL void GL_APIENTRY
glHint(GLenum target, GLenum mode)
{
    CONTEXT_EXEC(Hint(target, mode));
}

GL_APICALL GLboolean GL_APIENTRY
glIsBuffer(GLuint buffer)
{
    CONTEXT_EXEC_RETURN(IsBuffer(buffer));
}

GL_APICALL GLboolean GL_APIENTRY
glIsEnabled(GLenum cap)
{
    CONTEXT_EXEC_RETURN(IsEnabled(cap));
}

GL_APICALL GLboolean GL_APIENTRY
glIsFramebuffer(GLuint framebuffer)
{
    CONTEXT_EXEC_RETURN(IsFramebuffer(framebuffer));
}

GL_APICALL GLboolean GL_APIENTRY
glIsProgram(GLuint program)
{
    CONTEXT_EXEC_RETURN(IsProgram(program));
}

GL_APICALL GLboolean GL_APIENTRY
glIsRenderbuffer(GLuint renderbuffer)
{
    CONTEXT_EXEC_RETURN(IsRenderbuffer(renderbuffer));
}

GL_APICALL GLboolean GL_APIENTRY
glIsShader(GLuint shader)
{
    CONTEXT_EXEC_RETURN(IsShader(shader));
}

GL_APICALL GLboolean GL_APIENTRY
glIsTexture(GLuint texture)
{
    CONTEXT_EXEC_RETURN(IsTexture(texture));
}

GL_APICALL void GL_APIENTRY
glLineWidth(GLfloat width)
{
    CONTEXT_EXEC(LineWidth(width));
}

GL_APICALL void GL_APIENTRY
glLinkProgram(GLuint program)
{
    CONTEXT_EXEC(LinkProgram(program));
}

GL_APICALL void GL_APIENTRY
glPixelStorei(GLenum pname, GLint param)
{
    CONTEXT_EXEC(PixelStorei(pname, param));
}

GL_APICALL void GL_APIENTRY
glPolygonOffset(GLfloat factor, GLfloat units)
{
    CONTEXT_EXEC(PolygonOffset(factor, units));
}

GL_APICALL void GL_APIENTRY
glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels)
{
    CONTEXT_EXEC(ReadPixels(x, y, width, height, format, type, pixels));
}

GL_APICALL void GL_APIENTRY
glReleaseShaderCompiler(void)
{
    CONTEXT_EXEC(ReleaseShaderCompiler());
}

GL_APICALL void GL_APIENTRY
glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    CONTEXT_EXEC(RenderbufferStorage(target, internalformat, width, height));
}

GL_APICALL void GL_APIENTRY
glSampleCoverage(GLclampf value, GLboolean invert)
{
    CONTEXT_EXEC(SampleCoverage(value, invert));
}

GL_APICALL void GL_APIENTRY
glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    CONTEXT_EXEC(Scissor(x, y, width, height));
}

GL_APICALL void GL_APIENTRY
glShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length)
{
    CONTEXT_EXEC(ShaderBinary(n, shaders, binaryformat, binary, length));
}

GL_APICALL void GL_APIENTRY
glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
    CONTEXT_EXEC(ShaderSource(shader, count, string, length));
}

GL_APICALL void GL_APIENTRY
glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
    CONTEXT_EXEC(StencilFunc(func, ref, mask));
}

GL_APICALL void GL_APIENTRY
glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    CONTEXT_EXEC(StencilFuncSeparate(face, func, ref, mask));
}

GL_APICALL void GL_APIENTRY
glStencilMask(GLuint mask)
{
    CONTEXT_EXEC(StencilMask(mask));
}

GL_APICALL void GL_APIENTRY
glStencilMaskSeparate(GLenum face, GLuint mask)
{
    CONTEXT_EXEC(StencilMaskSeparate(face, mask));
}

GL_APICALL void GL_APIENTRY
glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    CONTEXT_EXEC(StencilOp(fail, zfail, zpass));
}

GL_APICALL void GL_APIENTRY
glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
    CONTEXT_EXEC(StencilOpSeparate(face, fail, zfail, zpass));
}

GL_APICALL void GL_APIENTRY
glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
{
    CONTEXT_EXEC(TexImage2D(target, level, internalformat, width, height, border, format, type, pixels));
}

GL_APICALL void GL_APIENTRY
glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    CONTEXT_EXEC(TexParameterf(target, pname, param));
}

GL_APICALL void GL_APIENTRY
glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params)
{
    CONTEXT_EXEC(TexParameterfv(target, pname, params));
}

GL_APICALL void GL_APIENTRY
glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    CONTEXT_EXEC(TexParameteri(target, pname, param));
}

GL_APICALL void GL_APIENTRY
glTexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
    CONTEXT_EXEC(TexParameteriv(target, pname, params));
}

GL_APICALL void GL_APIENTRY
glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
    CONTEXT_EXEC(TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels));
}

GL_APICALL void GL_APIENTRY
glUniform1f(GLint location, GLfloat x)
{
    CONTEXT_EXEC(Uniform1f(location, x));
}

GL_APICALL void GL_APIENTRY
glUniform1fv(GLint location, GLsizei count, const GLfloat* v)
{
    CONTEXT_EXEC(Uniform1fv(location, count, v));
}

GL_APICALL void GL_APIENTRY
glUniform1i(GLint location, GLint x)
{
    CONTEXT_EXEC(Uniform1i(location, x));
}

GL_APICALL void GL_APIENTRY
glUniform1iv(GLint location, GLsizei count, const GLint* v)
{
    CONTEXT_EXEC(Uniform1iv(location, count, v));
}

GL_APICALL void GL_APIENTRY
glUniform2f(GLint location, GLfloat x, GLfloat y)
{
    CONTEXT_EXEC(Uniform2f(location, x, y));
}

GL_APICALL void GL_APIENTRY
glUniform2fv(GLint location, GLsizei count, const GLfloat* v)
{
    CONTEXT_EXEC(Uniform2fv(location, count, v));
}

GL_APICALL void GL_APIENTRY
glUniform2i(GLint location, GLint x, GLint y)
{
    CONTEXT_EXEC(Uniform2i(location, x, y));
}

GL_APICALL void GL_APIENTRY
glUniform2iv(GLint location, GLsizei count, const GLint* v)
{
    CONTEXT_EXEC(Uniform2iv(location, count, v));
}

GL_APICALL void GL_APIENTRY
glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
    CONTEXT_EXEC(Uniform3f(location, x, y, z));
}

GL_APICALL void GL_APIENTRY
glUniform3fv(GLint location, GLsizei count, const GLfloat* v)
{
    CONTEXT_EXEC(Uniform3fv(location, count, v));
}

GL_APICALL void GL_APIENTRY
glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
    CONTEXT_EXEC(Uniform3i(location, x, y, z));
}

GL_APICALL void GL_APIENTRY
glUniform3iv(GLint location, GLsizei count, const GLint* v)
{
    CONTEXT_EXEC(Uniform3iv(location, count, v));
}

GL_APICALL void GL_APIENTRY
glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    CONTEXT_EXEC(Uniform4f(location, x, y, z, w));
}

GL_APICALL void GL_APIENTRY
glUniform4fv(GLint location, GLsizei count, const GLfloat* v)
{
    CONTEXT_EXEC(Uniform4fv(location, count, v));
}

GL_APICALL void GL_APIENTRY
glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
    CONTEXT_EXEC(Uniform4i(location, x, y, z, w));
}

GL_APICALL void GL_APIENTRY
glUniform4iv(GLint location, GLsizei count, const GLint* v)
{
    CONTEXT_EXEC(Uniform4iv(location, count, v));
}

GL_APICALL void GL_APIENTRY
glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    CONTEXT_EXEC(UniformMatrix2fv(location, count, transpose, value));
}

GL_APICALL void GL_APIENTRY
glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    CONTEXT_EXEC(UniformMatrix3fv(location, count, transpose, value));
}

GL_APICALL void GL_APIENTRY
glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    CONTEXT_EXEC(UniformMatrix4fv(location, count, transpose, value));
}

GL_APICALL void GL_APIENTRY
glUseProgram(GLuint program)
{
    CONTEXT_EXEC(UseProgram(program));
}

GL_APICALL void GL_APIENTRY
glValidateProgram(GLuint program)
{
    CONTEXT_EXEC(ValidateProgram(program));
}

GL_APICALL void GL_APIENTRY
glVertexAttrib1f(GLuint indx, GLfloat x)
{
    CONTEXT_EXEC(VertexAttrib1f(indx, x));
}

GL_APICALL void GL_APIENTRY
glVertexAttrib1fv(GLuint indx, const GLfloat* values)
{
    CONTEXT_EXEC(VertexAttrib1fv(indx, values));
}

GL_APICALL void GL_APIENTRY
glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y)
{
    CONTEXT_EXEC(VertexAttrib2f(indx, x, y));
}

GL_APICALL void GL_APIENTRY
glVertexAttrib2fv(GLuint indx, const GLfloat* values)
{
    CONTEXT_EXEC(VertexAttrib2fv(indx, values));
}

GL_APICALL void GL_APIENTRY
glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
    CONTEXT_EXEC(VertexAttrib3f(indx, x, y, z));
}

GL_APICALL void GL_APIENTRY
glVertexAttrib3fv(GLuint indx, const GLfloat* values)
{
    CONTEXT_EXEC(VertexAttrib3fv(indx, values));
}

GL_APICALL void GL_APIENTRY
glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    CONTEXT_EXEC(VertexAttrib4f(indx, x, y, z, w));
}

GL_APICALL void GL_APIENTRY
glVertexAttrib4fv(GLuint indx, const GLfloat* values)
{
    CONTEXT_EXEC(VertexAttrib4fv(indx, values));
}

GL_APICALL void GL_APIENTRY
glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
    CONTEXT_EXEC(VertexAttribPointer(indx, size, type, normalized, stride, ptr));
}

GL_APICALL void GL_APIENTRY
glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    CONTEXT_EXEC(Viewport(x, y, width, height));
}

GL_APICALL void GL_APIENTRY
glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
   CONTEXT_EXEC(EGLImageTargetTexture2DOES(target, image));
}

GL_APICALL void GL_APIENTRY
glEGLImageTargetRenderBufferStorageOES(GLenum target, GLeglImageOES image)
{
    CONTEXT_EXEC(EGLImageTargetRenderBufferStorageOES(target, image));
}

GL_APICALL void GL_APIENTRY
glInsertEventMarkerEXT(GLsizei length, const GLchar *marker)
{
    CONTEXT_EXEC(InsertEventMarkerEXT(length, marker));
}

GL_APICALL void GL_APIENTRY
glPushGroupMarkerEXT(GLsizei length, const GLchar *marker)
{
    CONTEXT_EXEC(PushGroupMarkerEXT(length, marker));
}

GL_APICALL void GL_APIENTRY
glPopGroupMarkerEXT(void)
{
    CONTEXT_EXEC(PushGroupMarkerEXT());
}

GL_APICALL void GL_APIENTRY glGetProgramBinaryOES(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
    CONTEXT_EXEC(GetProgramBinaryOES(program, bufSize, length, binaryFormat, binary));
}

GL_APICALL void GL_APIENTRY glProgramBinaryOES(GLuint program, GLenum binaryFormat, const void *binary, GLint length)
{
    CONTEXT_EXEC(ProgramBinaryOES(program, binaryFormat, binary, length));
}
