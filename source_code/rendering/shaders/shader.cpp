//
//  Shader.cpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "shader.hpp"

namespace ms {
	#define SHADER_ERROR 			"Shader error"
	#define INFO_LOG_SIZE 			512
}

ms::Shader::Shader(std::string vS,
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

void ms::Shader::use() {
	mglUseProgram(program);
}

GLint  ms::Shader::set_uniform (std::string const & name, math::mat4 const & m) {
	GLint location = mglGetUniformLocation(program, name.c_str());
	mglUniformMatrix4fv(location, 1, GL_FALSE, m.c_array());
	return location;
}

GLint  ms::Shader::set_uniform (std::string const & name, math::mat3 const & m) {
	GLint location = mglGetUniformLocation(program, name.c_str());
	mglUniformMatrix3fv(location, 1, GL_FALSE, m.c_array());
	return location;
}

GLint ms::Shader::set_uniform (std::string const & name, int value) {
	GLint location = mglGetUniformLocation(program, name.c_str());
	mglUniform1i(location, value);
	return location;
}

void ms::Shader::_load() {
	program = mglCreateProgram();
	compile_program();
}

void ms::Shader::_unload() {
	mglDeleteProgram(program);
	program = 0;
}

void ms::Shader::compile_program() {
	
	GLuint vshader 		= 0;
	
	#ifndef ios_build
	
	GLuint cshader 		= 0;
	GLuint evalshader 	= 0;
	GLuint gshader 		= 0;
	
	#endif
	
	GLuint fshader 		= 0;
	
	if (!vertexSource.empty()) {
		vshader = mglCreateShader(GL_VERTEX_SHADER);
		compile_shader(program, vshader, GL_VERTEX_SHADER, vertexSource);
	}
	
	#ifndef ios_build

	if (!tesselationControlSource.empty()) {
		cshader = mglCreateShader(GL_TESS_CONTROL_SHADER);
		compile_shader(program, cshader, GL_TESS_CONTROL_SHADER, tesselationControlSource);
	}
	
	if (!tesselationEvalutationSource.empty()) {
		evalshader = mglCreateShader(GL_TESS_EVALUATION_SHADER);
		compile_shader(program, evalshader, GL_TESS_EVALUATION_SHADER, tesselationEvalutationSource);
	}

	if (!geometrySource.empty()) {
		gshader = mglCreateShader(GL_GEOMETRY_SHADER);
		compile_shader(program, gshader, GL_GEOMETRY_SHADER, geometrySource);
	}
	
	#endif
	
	if (!fragmentSource.empty()) {
		fshader = mglCreateShader(GL_FRAGMENT_SHADER);
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

}

void ms::Shader::bind_texture(unsigned int index, Texture & texture) {
	this->use();
	mglActiveTexture(GL_TEXTURE0 + index);
	texture.use();
}

ms::Shader::program_ptr ms::Shader::vf_program 	(std::string const & vertexSource, std::string const & fragmentSource) {
	return std::make_unique<Shader>(vertexSource, "", "", "", fragmentSource);
}

std::string ms::Shader::get_class () const {
	return "ms::Shader";
}

void ms::Shader::compile_shader(GLuint program, GLuint shader, GLenum shaderType, std::string source) {
	const char * sourcePtr = source.c_str();
	mglShaderSource(shader, 1, &sourcePtr, nullptr);
	mglCompileShader(shader);
	get_shader_status(shader, GL_COMPILE_STATUS);
	mglAttachShader(program, shader);
}

int ms::Shader::get_shader_status(GLuint shader, GLenum statusType) {
	int code; char infoLog[INFO_LOG_SIZE];
	mglGetShaderiv(shader, statusType, &code);
	if (!code) {
		mglGetShaderInfoLog(shader, INFO_LOG_SIZE, nullptr, infoLog);
		std::cerr << SHADER_ERROR << std::endl<< infoLog << std::endl;
		assert(false);
	}
	return code;
}

GLuint ms::Shader::get_gl_id () const {
	return program;
}