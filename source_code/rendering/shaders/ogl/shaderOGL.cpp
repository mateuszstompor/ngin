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
	program = glCreateProgram();
	compile();

	use();
	GLint cam = glGetUniformLocation(program, "cameraTranslation");
	GLint persp = glGetUniformLocation(program, "perspectiveProjectionMatrix");
	math::mat4 cameraPosition = math::mat4::identity();
	cameraPosition = cameraPosition * math::transform::translate<float, 4>({0.0f, 0.0f, -4.0}) ;

	glUniformMatrix4fv(cam, 1, GL_FALSE, cameraPosition.c_array());

	math::mat4 perspective =  math::projection::perspective<float>(0.01f, 100.0f, 90.0f, 1200.0f/800.0f);
	glUniformMatrix4fv(persp, 1, GL_FALSE, perspective.c_array());

	
	isLoaded = true;
	Resource::load();
}

void ms::ShaderOGL::unload() {
	glDeleteProgram(program);
	program = 0;
	isLoaded = false;
	Resource::unload();
}

void ms::ShaderOGL::compile() {
	GLuint vshader 	= 0,
	cshader 		= 0,
	evalshader 		= 0,
	gshader 		= 0,
	fshader 		= 0;
	
	if (vertexSource) {
		vshader = glCreateShader(GL_VERTEX_SHADER);
		const char * vertexShaderPtr = vertexSource->c_str();
		glShaderSource(vshader, 1, &vertexShaderPtr, nullptr);
		glCompileShader(vshader);
		get_shader_status(vshader, GL_COMPILE_STATUS);
		glAttachShader(program, vshader);
	}
	
	#ifdef mac_build

	if (tesselationControlSource) {
		cshader = glCreateShader(GL_TESS_CONTROL_SHADER);
		const char * tessConShaderPtr = tesselationControlSource->c_str();
		glShaderSource(cshader, 1, &tessConShaderPtr, nullptr);
		glCompileShader(cshader);
		get_shader_status(cshader, GL_COMPILE_STATUS);
		glAttachShader(program, cshader);

	}
	
	if (tesselationEvalutationSource) {
		evalshader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		const char * tessEvalShaderPtr = tesselationEvalutationSource->c_str();
		glShaderSource(evalshader, 1, &tessEvalShaderPtr, nullptr);
		glCompileShader(evalshader);
		get_shader_status(evalshader, GL_COMPILE_STATUS);
		glAttachShader(program, evalshader);
	}

	if (geometrySource) {
		gshader = glCreateShader(GL_GEOMETRY_SHADER);
		const char * geoShaderPtr = geometrySource->c_str();
		glShaderSource(gshader, 1, &geoShaderPtr, nullptr);
		glCompileShader(gshader);
		get_shader_status(gshader, GL_COMPILE_STATUS);
		glAttachShader(program, gshader);

	}
	
	if (fragmentSource) {
		fshader = glCreateShader(GL_FRAGMENT_SHADER);
		const char * fragmentShaderPtr = fragmentSource->c_str();
		glShaderSource(fshader, 1, &fragmentShaderPtr, nullptr);
		glCompileShader(fshader);
		get_shader_status(fshader, GL_COMPILE_STATUS);
		glAttachShader(program, fshader);
	}
	
	#endif
	
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

int ms::ShaderOGL::get_shader_status(GLuint shader, GLenum statusType) {
	int code; char infoLog[INFO_LOG_SIZE];
	glGetShaderiv(shader, statusType, &code);
	if (!code) {
		glGetShaderInfoLog(shader, INFO_LOG_SIZE, nullptr, infoLog);
		std::cerr << SHADER_ERROR << std::endl<< infoLog << std::endl;
	}
	return code;
}
