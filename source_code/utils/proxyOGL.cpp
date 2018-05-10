//
//  proxyOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 09/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "proxyOGL.hpp"

static int glGenBuffersCallsAmount 			= 0;
static int glDeleteBuffersCallsAmount 		= 0;

static int glGenVertexArraysCallsAmount 	= 0;
static int glDeleteVertexArraysCallsAmount 	= 0;

static int glGenRenderbuffersCallsAmount 	= 0;
static int glDeleteRenderbuffersCallsAmount = 0;

static int glGenFramebuffersCallsAmount 	= 0;
static int glDeleteFramebuffersCallsAmount 	= 0;

static int glCreateShaderCallsAmount		= 0;
static int glDeleteShaderCallsAmount		= 0;

static int glCreateProgramCallsAmount		= 0;
static int glDeleteProgramCallsAmount		= 0;

static int glGenTexturesCallsAmount			= 0;
static int glDeleteTexturesCallsAmount		= 0;

void ms::utils::_check_gl_error(const char *file, int line) {
    GLenum err {glGetError()};
    while(err!=GL_NO_ERROR) {
        
        std::string error;
        
        switch(err) {
            case GL_INVALID_OPERATION:              error = "INVALID_OPERATION";                  break;
            case GL_INVALID_ENUM:                   error = "INVALID_ENUM";                       break;
            case GL_INVALID_VALUE:                  error = "INVALID_VALUE";                      break;
            case GL_OUT_OF_MEMORY:                  error = "OUT_OF_MEMORY";                      break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";      break;
        }
        
        std::cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<'\n';
        assert(false);
        
    }
}

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

GLuint _mglGetUniformBlockIndex (GLuint program, const GLchar *uniformBlockName) {
	GLuint location = glGetUniformBlockIndex(program, uniformBlockName);
	if(location == -1) {
		#ifdef POGL_UNEXISING_BLOCK_UNIFORMS
			std::cerr<< "UNIFORM_BLOCK_DOESNT_EXIST " << uniformBlockName <<  '\n';
		#endif
	}
	ms::utils::check_gl_error();
	return location;
}

void _mglDeleteVertexArrays (GLsizei n, const GLuint *arrays) {
	glDeleteVertexArrays(n, arrays);
	glDeleteVertexArraysCallsAmount += 1;
	ms::utils::check_gl_error();
}

void _mglGenRenderbuffers (GLsizei n, GLuint* renderbuffers) {
	glGenRenderbuffers(n, renderbuffers);
	glGenRenderbuffersCallsAmount += 1;
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
	glGenBuffersCallsAmount += 1;
	ms::utils::check_gl_error();
}

void _mglGenVertexArrays (GLsizei n, GLuint *arrays) {
	glGenVertexArrays(n, arrays);
	glGenVertexArraysCallsAmount += 1;
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

void _mglBlendFunc (GLenum sfactor, GLenum dfactor) {
    glBlendFunc(sfactor, dfactor);
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
			std::cerr<< "UNIFORM_DOESNT_EXIST " << name << '\n';
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
	glGenTexturesCallsAmount += 1;
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
	glGenFramebuffersCallsAmount += 1;
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

void _mglDisable (GLenum cap) {
	glDisable(cap);
	ms::utils::check_gl_error();
}

void _mglFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
	glFramebufferTexture2D(target, attachment, textarget, texture, level);
	ms::utils::check_gl_error();
}

void _mglDeleteBuffers (GLsizei n, const GLuint* buffers) {
	glDeleteBuffers(n, buffers);
	glDeleteBuffersCallsAmount += 1;
	ms::utils::check_gl_error();
}

void _mglDeleteFramebuffers (GLsizei n, const GLuint* framebuffers) {
	glDeleteFramebuffers(n, framebuffers);
	glDeleteFramebuffersCallsAmount += 1;
	ms::utils::check_gl_error();
}

void _mglDeleteProgram (GLuint program) {
	glDeleteProgram(program);
	glDeleteProgramCallsAmount += 1;
	ms::utils::check_gl_error();
}

void _mglDeleteRenderbuffers (GLsizei n, const GLuint* renderbuffers) {
	glDeleteRenderbuffers(n, renderbuffers);
	glDeleteRenderbuffersCallsAmount += 1;
	ms::utils::check_gl_error();
}

void  _mglReadBuffer (GLenum mode) {
    glReadBuffer(mode);
    ms::utils::check_gl_error();
}

void _mglDeleteShader (GLuint shader) {
	glDeleteShader(shader);
	glDeleteShaderCallsAmount += 1;
	ms::utils::check_gl_error();
}

void _mglDeleteTextures (GLsizei n, const GLuint* textures) {
	glDeleteTextures(n, textures);
	glDeleteTexturesCallsAmount += 1;
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
	glCreateShaderCallsAmount += 1;
	ms::utils::check_gl_error();
	return tmp;
}

GLuint _mglCreateProgram (void) {
	GLuint tmp = glCreateProgram();
	glCreateProgramCallsAmount += 1;
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

void _mglUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix3fv(location, count, transpose, value);
    ms::utils::check_gl_error();
}

void _mglUniformBlockBinding (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) {
	glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
	ms::utils::check_gl_error();
}

void _mglBindBufferBase (GLenum target, GLuint index, GLuint buffer) {
	glBindBufferBase(target, index, buffer);
	ms::utils::check_gl_error();
}

std::string get_allocation_statistics() {
	std::string output = "";
	output += "glGenBuffers calls: " 			+ std::to_string(glGenBuffersCallsAmount) 			+ "\n";
	output += "glDeleteBuffers calls: " 		+ std::to_string(glDeleteBuffersCallsAmount) 		+ "\n";
	output += "glGenRenderbuffers calls: " 		+ std::to_string(glGenRenderbuffersCallsAmount) 	+ "\n";
	output += "glDeleteRenderbuffers calls: " 	+ std::to_string(glDeleteRenderbuffersCallsAmount) 	+ "\n";
	output += "glGenVertexArrays calls: " 		+ std::to_string(glGenVertexArraysCallsAmount) 		+ "\n";
	output += "glDeleteVertexArrays calls: " 	+ std::to_string(glDeleteVertexArraysCallsAmount) 	+ "\n";
	output += "glGenFramebuffer calls: " 		+ std::to_string(glGenFramebuffersCallsAmount) 		+ "\n";
	output += "glDeleteFramebuffer calls: " 	+ std::to_string(glDeleteFramebuffersCallsAmount) 	+ "\n";
	output += "glCreateShader calls: " 			+ std::to_string(glCreateShaderCallsAmount) 		+ "\n";
	output += "glDeleteShader calls: " 			+ std::to_string(glDeleteShaderCallsAmount) 		+ "\n";
	output += "glCreateProgram calls: " 		+ std::to_string(glCreateProgramCallsAmount) 		+ "\n";
	output += "glDeleteProgram calls: " 		+ std::to_string(glDeleteProgramCallsAmount) 		+ "\n";
	output += "glGenTextures calls: " 			+ std::to_string(glGenTexturesCallsAmount) 			+ "\n";
	output += "glDeleteTextures calls: " 		+ std::to_string(glDeleteTexturesCallsAmount) 		+ "\n";
	return output;
}
