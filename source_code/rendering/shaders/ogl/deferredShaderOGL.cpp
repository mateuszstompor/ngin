//
//  deferredShaderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredShaderOGL.hpp"


ms::DeferredShaderOGL::DeferredShaderOGL(str_ptr vSS, str_ptr fSS) : ms::ShaderOGL(vSS, nullptr, nullptr, nullptr, fSS) { }

void ms::DeferredShaderOGL::set_projection_matrix (const math::mat4 & proj) {
	GLint persp = mglGetUniformLocation(program, "perspectiveProjection");
	mglUniformMatrix4fv(persp, 1, GL_FALSE, proj.c_array());
}

void ms::DeferredShaderOGL::set_camera_transformation (const math::mat4 & transf) {
	GLint cam = mglGetUniformLocation(program, "cameraTransformation");
	mglUniformMatrix4fv(cam, 1, GL_FALSE, transf.c_array());
}

void ms::DeferredShaderOGL::set_model_transformation (const math::mat4 & modelTransf) {
	GLint model = mglGetUniformLocation(program, "modelTransformation");
	mglUniformMatrix4fv(model, 1, GL_FALSE, modelTransf.c_array());
}

void ms::DeferredShaderOGL::set_has_material (bool doesItHave) {
	GLint hasMaterialLocation = mglGetUniformLocation(program, "material.hasMaterial");
	mglUniform1i(hasMaterialLocation, doesItHave == true ? 1 : 0);
}

void ms::DeferredShaderOGL::set_material_ambient_color (const math::vec3 & ambient) {
	GLint ambientLocation = mglGetUniformLocation(program, "material.ambient");
	mglUniform3fv(ambientLocation, 1, ambient.c_array());
}

void ms::DeferredShaderOGL::set_material_diffuse_color (const math::vec3 & diffuse) {
	GLint diffuseLocation = mglGetUniformLocation(program, "material.diffuse");
	mglUniform3fv(diffuseLocation, 1, diffuse.c_array());
}

void ms::DeferredShaderOGL::set_material_specular_color (const math::vec3 & specular) {
	GLint specularLocation = mglGetUniformLocation(program, "material.specular");
	mglUniform3fv(specularLocation, 1, specular.c_array());
}

void ms::DeferredShaderOGL::set_material_opacity (float opacity) {
	GLint opacityLocation = mglGetUniformLocation(program, "material.opacity");
	mglUniform1f(opacityLocation, opacity);
}

void ms::DeferredShaderOGL::set_material_shininess (float shininess) {
	GLint shininessLocation = mglGetUniformLocation(program, "material.shininess");
	mglUniform1f(shininessLocation, shininess);
}
