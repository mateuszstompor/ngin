//
//  shaderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "shaderOGL.hpp"

namespace ms {
	#define SHADER_ERROR 			"Shader error"
	#define INFO_LOG_SIZE 			512
	#define COMPILATION_COMPLETED 	"Compilation completed"
}

ms::ShaderOGL::ShaderOGL(str_ptr vS, str_ptr tcS, str_ptr teS, str_ptr gS, str_ptr fS) :
		vertexSource(vS), tesselationControlSource(tcS), tesselationEvalutationSource(teS),
		geometrySource(gS), fragmentSource(fS), program(0) {
			
}

void ms::ShaderOGL::use() {
	mglUseProgram(program);
}

void ms::ShaderOGL::load() {
	if (!is_loaded()) {
		program = mglCreateProgram();
		compile_program();
		Resource::load();
	}
}

void ms::ShaderOGL::unload() {
	if(is_loaded()) {
		mglDeleteProgram(program);
		program = 0;
		Resource::unload();
	}
}

void ms::ShaderOGL::compile_program() {
	
	GLuint vshader 		= mglCreateShader(GL_VERTEX_SHADER);
	
	#ifndef ios_build
	
	GLuint cshader		= mglCreateShader(GL_TESS_CONTROL_SHADER);
	GLuint evalshader 	= mglCreateShader(GL_TESS_EVALUATION_SHADER);
	GLuint gshader 		= mglCreateShader(GL_GEOMETRY_SHADER);
	
	#endif
	
	GLuint fshader 		= mglCreateShader(GL_FRAGMENT_SHADER);
	
	if (vertexSource) {
		compile_shader(program, vshader, GL_VERTEX_SHADER, add_header_to_source(*vertexSource));
	}
	
	#ifndef ios_build

	if (tesselationControlSource) {
		compile_shader(program, cshader, GL_TESS_CONTROL_SHADER, add_header_to_source(*tesselationControlSource));
	}
	
	if (tesselationEvalutationSource) {
		compile_shader(program, evalshader, GL_TESS_EVALUATION_SHADER, add_header_to_source(*tesselationEvalutationSource));
	}

	if (geometrySource) {
		compile_shader(program, gshader, GL_GEOMETRY_SHADER, add_header_to_source(*geometrySource));
	}
	
	#endif
	
	if (fragmentSource) {
		compile_shader(program, fshader, GL_FRAGMENT_SHADER, add_header_to_source(*fragmentSource));
	}
	
	mglLinkProgram(program);

	if (vertexSource)
		mglDeleteShader(vshader);
	
	#ifndef ios_build
	
		if (tesselationControlSource)
			mglDeleteShader(cshader);

		if (tesselationEvalutationSource)
			mglDeleteShader(evalshader);

		if (geometrySource)
			mglDeleteShader(gshader);
	
	#endif

	if (fragmentSource)
		mglDeleteShader(fshader);

	#ifdef DEBUG

		std::cout << COMPILATION_COMPLETED << std::endl;

	#endif
}

std::string ms::ShaderOGL::add_header_to_source(std::string source) {
	
	#ifdef ios_build
	
		return ms::shader::ios_header + source;
	
	#else
	
		return ms::shader::mac_os_header + source;
	
	#endif
	
}

void ms::ShaderOGL::compile_shader(GLuint program, GLuint shader, GLenum shaderType, std::string source) {
	const char * sourcePtr = source.c_str();
	mglShaderSource(shader, 1, &sourcePtr, nullptr);
	mglCompileShader(shader);
	get_shader_status(shader, GL_COMPILE_STATUS);
	mglAttachShader(program, shader);
}

int ms::ShaderOGL::get_shader_status(GLuint shader, GLenum statusType) {
	int code; char infoLog[INFO_LOG_SIZE];
	mglGetShaderiv(shader, statusType, &code);
	if (!code) {
		mglGetShaderInfoLog(shader, INFO_LOG_SIZE, nullptr, infoLog);
		std::cerr << SHADER_ERROR << std::endl<< infoLog << std::endl;
		assert(false);
	}
	return code;
}
