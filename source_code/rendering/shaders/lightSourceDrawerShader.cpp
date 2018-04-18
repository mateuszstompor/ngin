//
//  LightSourceDrawerShader.cpp
//  ngin
//
//  Created by Mateusz Stompór on 18/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "lightSourceDrawerShader.hpp"


ms::LightSourceDrawerShader::LightSourceDrawerShader (std::string vSS,
															std::string fSS) : ms::Shader(vSS, "", "", "", fSS) { }

void ms::LightSourceDrawerShader::set_projection_matrix (const math::mat4 & proj) {
	GLint persp = mglGetUniformLocation(program, "perspectiveProjection");
	mglUniformMatrix4fv(persp, 1, GL_FALSE, proj.c_array());
}

void ms::LightSourceDrawerShader::set_camera_transformation (const math::mat4 & transf){
	GLint cam = mglGetUniformLocation(program, "cameraTransformation");
	mglUniformMatrix4fv(cam, 1, GL_FALSE, transf.c_array());
}

void ms::LightSourceDrawerShader::set_model_transformation (const math::mat4 & transf) {
	GLint model = mglGetUniformLocation(program, "modelTransformation");
	mglUniformMatrix4fv(model, 1, GL_FALSE, transf.c_array());
}

void ms::LightSourceDrawerShader::set_light_source_color (const math::vec3 & color) {
	GLint ambientLocation = mglGetUniformLocation(program, "lightSourceColor");
	mglUniform3fv(ambientLocation, 1, color.c_array());
}
