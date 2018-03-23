//
//  proxyOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 09/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "proxyOGL.hpp"

void _mglDrawBuffers (GLsizei n, const GLenum* bufs) {
	glDrawBuffers(n, bufs);
	ms::utils::check_gl_error();
}

GLvoid* _mglMapBufferRange (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) {
	GLvoid* tmp = glMapBufferRange(target, offset, length, access);
	ms::utils::check_gl_error();
	return tmp;
}

GLboolean _mglUnmapBuffer (GLenum target) {
	GLboolean tmp = glUnmapBuffer(target);
	ms::utils::check_gl_error();
	return tmp;
}

void _mglDeleteVertexArrays (GLsizei n, const GLuint *arrays) {
	glDeleteVertexArrays(n, arrays);
	ms::utils::check_gl_error();
}

void _mglGenRenderbuffers (GLsizei n, GLuint* renderbuffers) {
	glGenRenderbuffers(n, renderbuffers);
	ms::utils::check_gl_error();
}

void _mglFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
	glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
	ms::utils::check_gl_error();
}

void _mglRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
	glRenderbufferStorage(target, internalformat, width, height);
	ms::utils::check_gl_error();
}

void _mglBindRenderbuffer(GLenum target, GLuint renderbuffer) {
	glBindRenderbuffer(target, renderbuffer);
	ms::utils::check_gl_error();
}

void _mglBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) {
	glBufferData(target, size, data, usage);
	ms::utils::check_gl_error();
}

void _mglBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data) {
	glBufferSubData(target, offset, size, data);
	ms::utils::check_gl_error();
}

void _mglViewport (GLint x, GLint y, GLsizei width, GLsizei height) {
	glViewport(x, y, width, height);
	ms::utils::check_gl_error();
}

void _mglGenBuffers (GLsizei n, GLuint *buffers) {
	glGenBuffers(n, buffers);
	ms::utils::check_gl_error();
}

void _mglGenVertexArrays (GLsizei n, GLuint *arrays) {
	glGenVertexArrays(n, arrays);
	ms::utils::check_gl_error();
}

void _mglDepthFunc (GLenum func) {
	glDepthFunc(func);
	ms::utils::check_gl_error();
}

void _mglVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr) {
	glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
	ms::utils::check_gl_error();
}

void _mglEnableVertexAttribArray (GLuint index) {
	glEnableVertexAttribArray(index);
	ms::utils::check_gl_error();
}

GLint _mglGetUniformLocation (GLuint program, const GLchar* name) {
	GLint tmp = glGetUniformLocation(program, name);
	ms::utils::check_gl_error();
	if(tmp == -1) {
		#ifdef POGL_UNEXISING_UNIFORMS
			std::cerr<< "UNIFORM_DOESNT_EXIST" << " " << name <<  std::endl;
		#endif
	}
 	return tmp;
}

void _mglUniform4fv (GLint location, GLsizei count, const GLfloat* v) {
	glUniform4fv(location, count, v);
	ms::utils::check_gl_error();
}

void _mglUniform3fv (GLint location, GLsizei count, const GLfloat* v) {
	glUniform3fv(location, count, v);
	ms::utils::check_gl_error();
}

void _mglUniform1i (GLint location, GLint x) {
	glUniform1i(location, x);
	ms::utils::check_gl_error();
}

void _mglUniform1f (GLint location, GLfloat x) {
	glUniform1f(location, x);
	ms::utils::check_gl_error();
}

void _mglBindBuffer (GLenum target, GLuint buffer) {
	glBindBuffer(target, buffer);
	ms::utils::check_gl_error();
}

void _mglBindVertexArray (GLuint array) {
	glBindVertexArray(array);
	ms::utils::check_gl_error();
}

void _mglUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
	glUniformMatrix4fv(location, count, transpose, value);
	ms::utils::check_gl_error();
}

void _mglGenTextures (GLsizei n, GLuint* textures) {
	glGenTextures(n, textures);
	ms::utils::check_gl_error();
}

void _mglBindFramebuffer (GLenum target, GLuint framebuffer) {
	glBindFramebuffer(target, framebuffer);
	ms::utils::check_gl_error();
}

void _mglDrawArrays (GLenum mode, GLint first, GLsizei count) {
	glDrawArrays(mode, first, count);
	ms::utils::check_gl_error();
}

void _mglClear (GLbitfield mask) {
	glClear(mask);
	ms::utils::check_gl_error();
}

void _mglClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	glClearColor(red, green, blue, alpha);
	ms::utils::check_gl_error();
}

void _mglActiveTexture (GLenum texture) {
	glActiveTexture(texture);
	ms::utils::check_gl_error();
}

void _mglEnable (GLenum cap) {
	glEnable(cap);
	ms::utils::check_gl_error();
}

void _mglGenFramebuffers (GLsizei n, GLuint* framebuffers) {
	glGenFramebuffers(n, framebuffers);
	ms::utils::check_gl_error();
}

void _mglBindTexture (GLenum target, GLuint texture) {
	glBindTexture(target, texture);
	ms::utils::check_gl_error();
}

void _mglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels) {
	glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
	ms::utils::check_gl_error();
}

void _mglTexParameteri (GLenum target, GLenum pname, GLint param) {
	glTexParameteri(target, pname, param);
	ms::utils::check_gl_error();
}

void _mglFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
	glFramebufferTexture2D(target, attachment, textarget, texture, level);
	ms::utils::check_gl_error();
}

void _mglDeleteBuffers (GLsizei n, const GLuint* buffers) {
	glDeleteBuffers(n, buffers);
	ms::utils::check_gl_error();
}

void _mglDeleteFramebuffers (GLsizei n, const GLuint* framebuffers) {
	glDeleteFramebuffers(n, framebuffers);
	ms::utils::check_gl_error();
}

void _mglDeleteProgram (GLuint program) {
	glDeleteProgram(program);
	ms::utils::check_gl_error();
}

void _mglDeleteRenderbuffers (GLsizei n, const GLuint* renderbuffers) {
	glDeleteRenderbuffers(n, renderbuffers);
	ms::utils::check_gl_error();
}

void _mglDeleteShader (GLuint shader) {
	glDeleteShader(shader);
	ms::utils::check_gl_error();
}

void _mglDeleteTextures (GLsizei n, const GLuint* textures) {
	glDeleteTextures(n, textures);
	ms::utils::check_gl_error();
}

void _mglLinkProgram (GLuint program) {
	glLinkProgram(program);
	ms::utils::check_gl_error();
}

void _mglAttachShader (GLuint program, GLuint shader) {
	glAttachShader(program, shader);
	ms::utils::check_gl_error();
}

void _mglUseProgram (GLuint program) {
	glUseProgram(program);
	ms::utils::check_gl_error();
}

GLuint _mglCreateShader (GLenum type) {
	GLuint tmp = glCreateShader(type);
	ms::utils::check_gl_error();
	return tmp;
}

GLuint _mglCreateProgram (void) {
	GLuint tmp = glCreateProgram();
	ms::utils::check_gl_error();
	return tmp;
}

void _mglShaderSource (GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length) {
	glShaderSource(shader, count, string, length);
	ms::utils::check_gl_error();
}

void _mglCompileShader (GLuint shader) {
	glCompileShader(shader);
	ms::utils::check_gl_error();
}

void _mglGetShaderiv (GLuint shader, GLenum pname, GLint *params) {
	glGetShaderiv(shader, pname, params);
	ms::utils::check_gl_error();
}

void _mglGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
	glGetShaderInfoLog(shader, bufSize, length, infoLog);
	ms::utils::check_gl_error();
}

void _mglUniform1ui (GLint location, GLuint v0) {
	glUniform1ui(location, v0);
	ms::utils::check_gl_error();
}

void _mglDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {
	glDrawElements(mode, count, type, indices);
	ms::utils::check_gl_error();
}

void _mglBlitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
	glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	ms::utils::check_gl_error();
}

GLenum _mglCheckFramebufferStatus (GLenum target) {
	GLenum status = glCheckFramebufferStatus(target);
	ms::utils::check_gl_error();
	return status;
}

