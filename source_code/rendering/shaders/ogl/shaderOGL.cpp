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
	glUseProgram(program);
}

void ms::ShaderOGL::load() {
	if (!is_loaded()) {
		program = glCreateProgram();
		compile_program();
		Resource::load();
	}
}

void ms::ShaderOGL::unload() {
	if(is_loaded()) {
		glDeleteProgram(program);
		program = 0;
		Resource::unload();
	}
}

void ms::ShaderOGL::compile_program() {
	GLuint vshader 		= glCreateShader(GL_VERTEX_SHADER);
	
	#ifdef mac_build
	
	GLuint cshader		= glCreateShader(GL_TESS_CONTROL_SHADER);
	GLuint evalshader 	= glCreateShader(GL_TESS_EVALUATION_SHADER);
	GLuint gshader 		= glCreateShader(GL_GEOMETRY_SHADER);
	
	#endif
	
	GLuint fshader 		= glCreateShader(GL_FRAGMENT_SHADER);
	
	if (vertexSource) {
		compile_shader(program, vshader, GL_VERTEX_SHADER, vertexSource);
	}
	
	#ifdef mac_build

	if (tesselationControlSource) {
		compile_shader(program, cshader, GL_TESS_CONTROL_SHADER, tesselationControlSource);
	}
	
	if (tesselationEvalutationSource) {
		compile_shader(program, evalshader, GL_TESS_EVALUATION_SHADER, tesselationEvalutationSource);
	}

	if (geometrySource) {
		compile_shader(program, gshader, GL_GEOMETRY_SHADER, geometrySource);
	}
	
	#endif
	
	if (fragmentSource) {
		compile_shader(program, fshader, GL_FRAGMENT_SHADER, fragmentSource);
	}
	
	glLinkProgram(program);

	if (vertexSource)
		glDeleteShader(vshader);
	
	#ifdef mac_build
	
		if (tesselationControlSource)
			glDeleteShader(cshader);

		if (tesselationEvalutationSource)
			glDeleteShader(evalshader);

		if (geometrySource)
			glDeleteShader(gshader);
	
	#endif

	if (fragmentSource)
		glDeleteShader(fshader);

	#ifdef DEBUG

		std::cout << COMPILATION_COMPLETED << std::endl;

	#endif
}

void ms::ShaderOGL::compile_shader(GLuint program, GLuint shader, GLenum shaderType, str_ptr source) {
	const char * sourcePtr = source->c_str();
	glShaderSource(shader, 1, &sourcePtr, nullptr);
	glCompileShader(shader);
	get_shader_status(shader, GL_COMPILE_STATUS);
	glAttachShader(program, shader);
}

int ms::ShaderOGL::get_shader_status(GLuint shader, GLenum statusType) {
	int code; char infoLog[INFO_LOG_SIZE];
	glGetShaderiv(shader, statusType, &code);
	if (!code) {
		glGetShaderInfoLog(shader, INFO_LOG_SIZE, nullptr, infoLog);
		std::cerr << SHADER_ERROR << std::endl<< infoLog << std::endl;
	}
	return code;
}
