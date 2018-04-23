//
//  proxyOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 09/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cassert>
#include <string>
#include <iostream>

#include "../../config.hpp"
#include "utilsOGL.hpp"

#ifdef __WIN32__

	#include <glad/glad.h>

#endif

#if __APPLE__

	#ifdef ios_build

		#import <OpenGLES/ES3/gl.h>
		#import <OpenGLES/ES3/glext.h>

	#elif mac_build

		#include <OpenGL/gl3.h>

	#endif

#endif

#if DEBUG

void 		_mglDrawBuffers (GLsizei n, const GLenum* bufs);
GLint 		_mglGetUniformLocation (GLuint program, const GLchar* name);
void 		_mglUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void        _mglUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void 		_mglUniform4fv (GLint location, GLsizei count, const GLfloat* v);
void 		_mglUniform3fv (GLint location, GLsizei count, const GLfloat* v);
void 		_mglUniform1i (GLint location, GLint x);
void 		_mglUniform1f (GLint location, GLfloat x);
void 		_mglDepthFunc (GLenum func);
void 		_mglVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr);
void 		_mglDisable (GLenum cap);
void 		_mglDisableVertexAttribArray (GLuint index);
void 		_mglActiveTexture (GLenum texture);
void 		_mglViewport (GLint x, GLint y, GLsizei width, GLsizei height);
void 		_mglEnableVertexAttribArray (GLuint index);
void 		_mglEnable (GLenum cap);
void 		_mglClear (GLbitfield mask);
void 		_mglClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void 		_mglBindBuffer (GLenum target, GLuint buffer);
void 		_mglBindFramebuffer (GLenum target, GLuint framebuffer);
void 		_mglBindVertexArray (GLuint array);
void 		_mglBindTexture (GLenum target, GLuint texture);
void 		_mglBindFramebuffer (GLenum target, GLuint framebuffer);
void 		_mglGenFramebuffers (GLsizei n, GLuint* framebuffers);
void 		_mglGenRenderbuffers (GLsizei n, GLuint* renderbuffers);
void 		_mglGenTextures (GLsizei n, GLuint* textures);
void 		_mglGenBuffers (GLsizei n, GLuint *buffers);
void 		_mglGenVertexArrays (GLsizei n, GLuint *arrays);
void 		_mglBufferData (GLenum target, GLsizeiptr size, const GLvoid 		*data, GLenum usage);
void 		_mglFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void 		_mglBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid 		*data);
void 		_mglDrawArrays (GLenum mode, GLint first, GLsizei count);
void 		_mglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
void 		_mglTexParameteri (GLenum target, GLenum pname, GLint param);
void 		_mglFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void 		_mglDeleteBuffers (GLsizei n, const GLuint* buffers);
void 		_mglDeleteFramebuffers (GLsizei n, const GLuint* framebuffers);
void 		_mglDeleteProgram (GLuint program);
void 		_mglDeleteRenderbuffers (GLsizei n, const GLuint* renderbuffers);
void 		_mglDeleteShader (GLuint shader);
void 		_mglDeleteTextures (GLsizei n, const GLuint* textures);
void 		_mglRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void 		_mglBindRenderbuffer(GLenum target, GLuint renderbuffer);
GLuint 		_mglCreateShader (GLenum type);
void 		_mglLinkProgram (GLuint program);
void 		_mglAttachShader (GLuint program, GLuint shader);
void 		_mglUseProgram (GLuint program);
GLuint 		_mglCreateProgram (void);
void 		_mglShaderSource (GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length);
void 		_mglCompileShader (GLuint shader);
void 		_mglGetShaderiv (GLuint shader, GLenum pname, GLint *params);
void 		_mglGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLvoid* 	_mglMapBufferRange (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLboolean	_mglUnmapBuffer (GLenum target);
void 		_mglDeleteVertexArrays (GLsizei n, const GLuint *arrays);
void 		_mglUniform1ui (GLint location, GLuint v0);
void 		_mglDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void		_mglBlitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLenum 		_mglCheckFramebufferStatus (GLenum target);
GLuint 		_mglGetUniformBlockIndex (GLuint program, const GLchar *uniformBlockName);
void		_mglUniformBlockBinding (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
void	 	_mglBindBufferBase (GLenum target, GLuint index, GLuint buffer);
void        _mglReadBuffer (GLenum mode);

#define mglDrawElements				_mglDrawElements
#define mglFramebufferRenderbuffer	_mglFramebufferRenderbuffer
#define mglViewport					_mglViewport
#define mglDepthFunc				_mglDepthFunc
#define mglClear					_mglClear
#define mglClearColor				_mglClearColor
#define mglDrawBuffers 				_mglDrawBuffers
#define mglGetUniformLocation 		_mglGetUniformLocation
#define mglUniformMatrix4fv			_mglUniformMatrix4fv
#define mglUniformMatrix3fv         _mglUniformMatrix3fv
#define mglUniform4fv				_mglUniform4fv
#define mglUniform3fv				_mglUniform3fv
#define mglUniform1i				_mglUniform1i
#define mglUniform1f				_mglUniform1f
#define mglVertexAttribPointer		_mglVertexAttribPointer
#define mglDisableVertexAttribArray	_mglDisableVertexAttribArray
#define mglDisable					_mglDisable
#define mglDrawArrays				_mglDrawArrays
#define	mglActiveTexture			_mglActiveTexture
#define mglEnableVertexAttribArray	_mglEnableVertexAttribArray
#define mglEnable					_mglEnable
#define mglBindBuffer				_mglBindBuffer
#define mglBindFramebuffer			_mglBindFramebuffer
#define mglBindVertexArray			_mglBindVertexArray
#define mglGenTextures				_mglGenTextures
#define mglGenRenderbuffers			_mglGenRenderbuffers
#define mglGenFramebuffers			_mglGenFramebuffers
#define mglGenVertexArrays			_mglGenVertexArrays
#define mglGenBuffers				_mglGenBuffers
#define mglBindTexture				_mglBindTexture
#define mglTexImage2D				_mglTexImage2D
#define mglTexParameteri			_mglTexParameteri
#define mglFramebufferTexture2D		_mglFramebufferTexture2D
#define mglDeleteBuffers			_mglDeleteBuffers
#define mglDeleteFramebuffers		_mglDeleteFramebuffers
#define mglDeleteProgram			_mglDeleteProgram
#define mglDeleteRenderbuffers		_mglDeleteRenderbuffers
#define mglDeleteBuffers			_mglDeleteBuffers
#define mglDeleteTextures			_mglDeleteTextures
#define mglBufferData				_mglBufferData
#define mglBufferSubData			_mglBufferSubData
#define mglRenderbufferStorage		_mglRenderbufferStorage
#define mglBindRenderbuffer			_mglBindRenderbuffer
#define mglCreateShader				_mglCreateShader
#define mglLinkProgram				_mglLinkProgram
#define mglAttachShader				_mglAttachShader
#define mglUseProgram				_mglUseProgram
#define mglDeleteShader				_mglDeleteShader
#define mglCreateProgram			_mglCreateProgram
#define mglShaderSource				_mglShaderSource
#define mglCompileShader			_mglCompileShader
#define mglGetShaderiv				_mglGetShaderiv
#define mglGetShaderInfoLog			_mglGetShaderInfoLog
#define mglMapBufferRange			_mglMapBufferRange
#define mglUnmapBuffer				_mglUnmapBuffer
#define mglDeleteVertexArrays		_mglDeleteVertexArrays
#define mglUniform1ui				_mglUniform1ui
#define mglBlitFramebuffer			_mglBlitFramebuffer
#define mglCheckFramebufferStatus	_mglCheckFramebufferStatus
#define mglGetUniformBlockIndex		_mglGetUniformBlockIndex
#define mglBindBufferBase			_mglBindBufferBase
#define mglUniformBlockBinding		_mglUniformBlockBinding
#define mglReadBuffer               _mglReadBuffer

#else

#define mglDrawElements				glDrawElements
#define mglFramebufferRenderbuffer	glFramebufferRenderbuffer
#define mglViewport					glViewport
#define mglDepthFunc				glDepthFunc
#define mglClear					glClear
#define mglClearColor				glClearColor
#define mglDrawBuffers 				glDrawBuffers
#define mglGetUniformLocation 		glGetUniformLocation
#define mglUniformMatrix4fv			glUniformMatrix4fv
#define mglUniformMatrix3fv         glUniformMatrix3fv
#define mglUniform4fv				glUniform4fv
#define mglUniform3fv				glUniform3fv
#define mglUniform1i				glUniform1i
#define mglUniform1f				glUniform1f
#define mglVertexAttribPointer		glVertexAttribPointer
#define mglDisableVertexAttribArray	glDisableVertexAttribArray
#define mglDisable					glDisable
#define mglDrawArrays				glDrawArrays
#define	mglActiveTexture			glActiveTexture
#define mglEnableVertexAttribArray	glEnableVertexAttribArray
#define mglEnable					glEnable
#define mglBindBuffer				glBindBuffer
#define mglBindFramebuffer			glBindFramebuffer
#define mglBindVertexArray			glBindVertexArray
#define mglGenTextures				glGenTextures
#define mglGenRenderbuffers			glGenRenderbuffers
#define mglGenFramebuffers			glGenFramebuffers
#define mglGenVertexArrays			glGenVertexArrays
#define mglGenBuffers				glGenBuffers
#define mglBindTexture				glBindTexture
#define mglTexImage2D				glTexImage2D
#define mglTexParameteri			glTexParameteri
#define mglFramebufferTexture2D		glFramebufferTexture2D
#define mglDeleteBuffers			glDeleteBuffers
#define mglDeleteFramebuffers		glDeleteFramebuffers
#define mglDeleteProgram			glDeleteProgram
#define mglDeleteRenderbuffers		glDeleteRenderbuffers
#define mglDeleteBuffers			glDeleteBuffers
#define mglDeleteTextures			glDeleteTextures
#define mglBufferData				glBufferData
#define mglBufferSubData			glBufferSubData
#define mglRenderbufferStorage		glRenderbufferStorage
#define mglBindRenderbuffer			glBindRenderbuffer
#define mglCreateShader				glCreateShader
#define mglLinkProgram				glLinkProgram
#define mglAttachShader				glAttachShader
#define mglUseProgram				glUseProgram
#define mglDeleteShader				glDeleteShader
#define mglCreateProgram			glCreateProgram
#define mglShaderSource				glShaderSource
#define mglCompileShader			glCompileShader
#define mglGetShaderiv				glGetShaderiv
#define mglGetShaderInfoLog			glGetShaderInfoLog
#define mglMapBufferRange			glMapBufferRange
#define mglUnmapBuffer				glUnmapBuffer
#define mglDeleteVertexArrays		glDeleteVertexArrays
#define mglUniform1ui				glUniform1ui
#define mglBlitFramebuffer			glBlitFramebuffer
#define mglCheckFramebufferStatus	glCheckFramebufferStatus
#define mglGetUniformBlockIndex		glGetUniformBlockIndex
#define mglBindBufferBase			glBindBufferBase
#define mglUniformBlockBinding		glUniformBlockBinding
#define mglReadBuffer               glReadBuffer

#endif

std::string get_allocation_statistics();
