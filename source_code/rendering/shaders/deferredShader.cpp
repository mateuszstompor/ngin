//
//  deferredShader.cpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredShader.hpp"


ms::DeferredShader::DeferredShader(std::string vSS, std::string fSS) : ms::Shader(vSS, "", "", "", fSS) { }

void  ms::DeferredShader::_load () {
	Shader::_load();

	mglUseProgram(program);
	
	mglUniform1i(mglGetUniformLocation(program, "diffuseTexture"), 0);
	mglUniform1i(mglGetUniformLocation(program, "specularTexture"), 1);
	mglUniform1i(mglGetUniformLocation(program, "normalTexture"), 2);
	
	projectionMatrixLocation 		= mglGetUniformLocation(program, "perspectiveProjection");
	cameraTransformationLocation 	= mglGetUniformLocation(program, "cameraTransformation");
	modelTransformationLocation 	= mglGetUniformLocation(program, "modelTransformation");
	
	hasMaterialLocation 			= mglGetUniformLocation(program, "hasMaterial");
	hasDiffuseTextureLocation	 	= mglGetUniformLocation(program, "hasDiffuseTexture");
	hasSpecularTextureLocation 		= mglGetUniformLocation(program, "hasSpecularTexture");
	hasNormalTextureLocation 		= mglGetUniformBlockIndex(program, "hasNormalTexture");
	materialBlockLocation			= mglGetUniformBlockIndex(program, "MaterialBlock");
	
	
	mglUniformBlockBinding(program, materialBlockLocation, 0);
	
	mglUseProgram(0);
	
}

void ms::DeferredShader::set_has_diffuse_texture (bool doesItHave) {
	mglUniform1i(hasDiffuseTextureLocation, doesItHave == true ? 1 : 0);
}

void ms::DeferredShader::set_has_specular_texture (bool doesItHave) {
	mglUniform1i(hasSpecularTextureLocation, doesItHave == true ? 1 : 0);
}

void ms::DeferredShader::set_projection_matrix (const math::mat4 & proj) {
	mglUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, proj.c_array());
}

void ms::DeferredShader::set_camera_transformation (const math::mat4 & transf) {
	mglUniformMatrix4fv(cameraTransformationLocation, 1, GL_FALSE, transf.c_array());
}

void ms::DeferredShader::set_model_transformation (const math::mat4 & modelTransf) {
	mglUniformMatrix4fv(modelTransformationLocation, 1, GL_FALSE, modelTransf.c_array());
}

void ms::DeferredShader::set_has_material (bool doesItHave) {
	mglUniform1i(hasMaterialLocation, doesItHave == true ? 1 : 0);
}

void ms::DeferredShader::set_has_normal_texture (bool doesItHave) {
	mglUniform1i(hasNormalTextureLocation, doesItHave == true ? 1 : 0);
}
