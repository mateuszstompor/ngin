//
//  deferredShaderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredShaderOGL.hpp"


ms::DeferredShaderOGL::DeferredShaderOGL(std::string vSS, std::string fSS) : ms::ShaderOGL(vSS, "", "", "", fSS) { }

void  ms::DeferredShaderOGL::_load () {
	ShaderOGL::_load();

	mglUseProgram(program);
	
	GLint diffuseTextureLocation = mglGetUniformLocation(program, "diffuseTexture");
	mglUniform1i(diffuseTextureLocation, 0);
	
	GLint specularTextureLocation = mglGetUniformLocation(program, "specularTexture");
	mglUniform1i(specularTextureLocation, 1);
	
	projectionMatrixLocation 		= mglGetUniformLocation(program, "perspectiveProjection");
	cameraTransformationLocation 	= mglGetUniformLocation(program, "cameraTransformation");
	modelTransformationLocation 	= mglGetUniformLocation(program, "modelTransformation");
	
	hasMaterialLocation 			= mglGetUniformLocation(program, "hasMaterial");
	hasDiffuseTextureLocation	 	= mglGetUniformLocation(program, "hasDiffuseTexture");
	hasSpecularTextureLocation 		= mglGetUniformLocation(program, "hasSpecularTexture");
	materialBlockLocation			= mglGetUniformBlockIndex(program, "MaterialBlock");

	mglUniformBlockBinding(program, materialBlockLocation, 0);
	
	mglUseProgram(0);
	
}

void ms::DeferredShaderOGL::set_has_diffuse_texture (bool doesItHave) {
	mglUniform1i(hasDiffuseTextureLocation, doesItHave == true ? 1 : 0);
}

void ms::DeferredShaderOGL::set_has_specular_texture (bool doesItHave) {
	mglUniform1i(hasSpecularTextureLocation, doesItHave == true ? 1 : 0);
}

void ms::DeferredShaderOGL::set_projection_matrix (const math::mat4 & proj) {
	mglUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, proj.c_array());
}

void ms::DeferredShaderOGL::set_camera_transformation (const math::mat4 & transf) {
	mglUniformMatrix4fv(cameraTransformationLocation, 1, GL_FALSE, transf.c_array());
}

void ms::DeferredShaderOGL::set_model_transformation (const math::mat4 & modelTransf) {
	mglUniformMatrix4fv(modelTransformationLocation, 1, GL_FALSE, modelTransf.c_array());
}

void ms::DeferredShaderOGL::set_has_material (bool doesItHave) {
	mglUniform1i(hasMaterialLocation, doesItHave == true ? 1 : 0);
}

void ms::DeferredShaderOGL::bind_diffuse_texture (Texture & texture) {
	mglActiveTexture(GL_TEXTURE0);
	texture.use();
}

void ms::DeferredShaderOGL::bind_specular_texture (Texture & texture) {
	mglActiveTexture(GL_TEXTURE1);
	texture.use();
}
