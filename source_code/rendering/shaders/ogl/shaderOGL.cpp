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

ms::ShaderOGL::ShaderOGL(std::string vS,
						 std::string tcS,
						 std::string teS,
						 std::string gS,
						 std::string fS) :

											vertexSource(vS),
											tesselationControlSource(tcS),
											tesselationEvalutationSource(teS),
											geometrySource(gS),
											fragmentSource(fS),
											program(0) {
			
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
	
	if (!vertexSource.empty()) {
		compile_shader(program, vshader, GL_VERTEX_SHADER, vertexSource);
	}
	
	#ifndef ios_build

	if (!tesselationControlSource.empty()) {
		compile_shader(program, cshader, GL_TESS_CONTROL_SHADER, tesselationControlSource);
	}
	
	if (!tesselationEvalutationSource.empty()) {
		compile_shader(program, evalshader, GL_TESS_EVALUATION_SHADER, tesselationEvalutationSource);
	}

	if (!geometrySource.empty()) {
		compile_shader(program, gshader, GL_GEOMETRY_SHADER, geometrySource);
	}
	
	#endif
	
	if (!fragmentSource.empty()) {
		compile_shader(program, fshader, GL_FRAGMENT_SHADER, fragmentSource);
	}
	
	mglLinkProgram(program);

	if (!vertexSource.empty())
		mglDeleteShader(vshader);
	
	#ifndef ios_build
	
		if (!tesselationControlSource.empty())
			mglDeleteShader(cshader);

		if (!tesselationEvalutationSource.empty())
			mglDeleteShader(evalshader);

		if (!geometrySource.empty())
			mglDeleteShader(gshader);
	
	#endif

	if (!fragmentSource.empty())
		mglDeleteShader(fshader);

	#ifdef DEBUG

		std::cout << COMPILATION_COMPLETED << std::endl;

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
