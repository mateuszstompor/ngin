//
//  forwardShaderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "forwardShaderOGL.hpp"

ms::ForwardShaderOGL::ForwardShaderOGL(str_ptr vSS, str_ptr fSS) : ms::ShaderOGL(vSS, nullptr, nullptr, nullptr, fSS) { }

void ms::ForwardShaderOGL::set_projection_matrix (math::mat4 proj) {
	GLint persp = mglGetUniformLocation(program, "perspectiveProjection");
	mglUniformMatrix4fv(persp, 1, GL_FALSE, proj.c_array());
}

void ms::ForwardShaderOGL::set_camera_transformation (math::mat4 transf) {
	GLint cam = mglGetUniformLocation(program, "cameraTransformation");
	mglUniformMatrix4fv(cam, 1, GL_FALSE, transf.c_array());
}

void ms::ForwardShaderOGL::set_model_transformation (math::mat4 modelTransf) {
	GLint model = mglGetUniformLocation(program, "modelTransformation");
	mglUniformMatrix4fv(model, 1, GL_FALSE, modelTransf.c_array());
}

void ms::ForwardShaderOGL::set_has_directional_light (bool doesItHave) {
	GLint hasLight = mglGetUniformLocation(program, "hasDirLight");
	mglUniform1i(hasLight, doesItHave == true ? 1 : 0);
}

void ms::ForwardShaderOGL::set_directional_light_dir (math::vec3 dir) {
	GLint direction = mglGetUniformLocation(program, "dirLight.direction");
	mglUniform3fv(direction, 1, dir.c_array());
}

void ms::ForwardShaderOGL::set_directional_light_color (math::vec4 color) {
	GLint colorLocation = mglGetUniformLocation(program, "dirLight.color");
	mglUniform4fv(colorLocation, 1, color.c_array());
}

void ms::ForwardShaderOGL::set_has_material (bool doesItHave) {
	GLint hasMaterialLocation = mglGetUniformLocation(program, "material.hasMaterial");
	mglUniform1i(hasMaterialLocation, doesItHave == true ? 1 : 0);
}

void ms::ForwardShaderOGL::set_material_ambient_color (math::vec3 ambient) {
	GLint ambientLocation = mglGetUniformLocation(program, "material.ambient");
	mglUniform3fv(ambientLocation, 1, ambient.c_array());
}

void ms::ForwardShaderOGL::set_material_diffuse_color (math::vec3 diffuse) {
	GLint diffuseLocation = mglGetUniformLocation(program, "material.diffuse");
	mglUniform3fv(diffuseLocation, 1, diffuse.c_array());
}

void ms::ForwardShaderOGL::set_material_specular_color (math::vec3 specular) {
	GLint specularLocation = mglGetUniformLocation(program, "material.specular");
	mglUniform3fv(specularLocation, 1, specular.c_array());
}

void ms::ForwardShaderOGL::set_material_opacity (float opacity) {
	GLint opacityLocation = mglGetUniformLocation(program, "material.opacity");
	mglUniform1f(opacityLocation, opacity);
}

void ms::ForwardShaderOGL::set_material_shininess (float shininess) {
	GLint shininessLocation = mglGetUniformLocation(program, "material.shininess");
	mglUniform1f(shininessLocation, shininess);
}
