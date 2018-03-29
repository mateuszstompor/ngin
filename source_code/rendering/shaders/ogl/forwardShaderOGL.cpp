//
//  forwardShaderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "forwardShaderOGL.hpp"

ms::ForwardShaderOGL::ForwardShaderOGL(unsigned int maximalAmountOfLights,
									   std::string vSS,
									   std::string fSS) : ms::ForwardShader(maximalAmountOfLights), ms::ShaderOGL(vSS, "", "", "", fSS) {
	
	spotLightsLocations 	= new GLint[AMOUNT_SPOT_LIGHT_PROPERTIES * maximalAmountOfLights];
	pointLightsLocations 	= new GLint[AMOUNT_POINT_LIGHT_PROPERTIES * maximalAmountOfLights];
	
}

void  ms::ForwardShaderOGL::_load () {
	ShaderOGL::_load();
	
	mglUseProgram(program);
	
	mglUniform1i(mglGetUniformLocation(program, "diffuseTexture"), 0);
	mglUniform1i(mglGetUniformLocation(program, "specularTexture"), 1);
	mglUniform1i(mglGetUniformLocation(program, "normalTexture"), 2);
	
	projectionMatrixLocation 			= mglGetUniformLocation(program, "perspectiveProjection");
	cameraTransformationLocation 		= mglGetUniformLocation(program, "cameraTransformation");
	modelTransformationLocation 		= mglGetUniformLocation(program, "modelTransformation");
	
	hasMaterialLocation 				= mglGetUniformLocation(program, "hasMaterial");
	hasDiffuseTextureLocation	 		= mglGetUniformLocation(program, "hasDiffuseTexture");
	hasSpecularTextureLocation 			= mglGetUniformLocation(program, "hasSpecularTexture");
	hasNormalTextureLocation			= mglGetUniformLocation(program, "hasNormalTexture");
	directionalLightColorLocation 		= mglGetUniformLocation(program, "dirLight.color");
	directionalLightDirectionLocation	= mglGetUniformLocation(program, "dirLight.direction");
	hasDirectionalLightLocation	 		= mglGetUniformLocation(program, "hasDirLight");
	
	renderModeLocation 					= mglGetUniformLocation(program, "renderMode");
	
	spotLightsAmount 					= mglGetUniformLocation(program, "spotLightsAmount");
	
	
	for(unsigned int i = 0; i < maximalAmountOfLights; ++i) {
		
		{
			GLint colorLocation = mglGetUniformLocation(program, ("spotLights[" + std::to_string(i) + "].color").c_str());
			spotLightsLocations[(i * AMOUNT_SPOT_LIGHT_PROPERTIES) + (SL_COLOR) ] = colorLocation;
		}
		
		{
			GLint positionLocation = mglGetUniformLocation(program, ("spotLights[" + std::to_string(i) + "].position").c_str());
			spotLightsLocations[(i * AMOUNT_SPOT_LIGHT_PROPERTIES) + (SL_POSITION) ] = positionLocation;
		}
		
		{
			GLint powerLocation = mglGetUniformLocation(program, ("spotLights[" + std::to_string(i) + "].power").c_str());
			spotLightsLocations[(i * AMOUNT_SPOT_LIGHT_PROPERTIES) + (SL_POWER) ] = powerLocation;
		}
		
		{
			GLint angleLocation = mglGetUniformLocation(program, ("spotLights[" + std::to_string(i) + "].angleDegrees").c_str());
			spotLightsLocations[(i * AMOUNT_SPOT_LIGHT_PROPERTIES) + (SL_ANGLE_DEGREES) ] = angleLocation;
		}
		
		{
			GLint directionLocation = mglGetUniformLocation(program, ("spotLights[" + std::to_string(i) + "].direction").c_str());
			spotLightsLocations[(i * AMOUNT_SPOT_LIGHT_PROPERTIES) + (SL_DIRECTION) ] = directionLocation;
		}
		
	}
	
	pointLightsAmount = mglGetUniformLocation(program, "pointLightsAmount");
	
	for(unsigned int i = 0; i < maximalAmountOfLights; ++i) {
		
		{
			GLint colorLocation = mglGetUniformLocation(program, ("pointLights[" + std::to_string(i) + "].color").c_str());
			pointLightsLocations[(i * AMOUNT_POINT_LIGHT_PROPERTIES) + (PL_COLOR) ] = colorLocation;
		}
		
		{
			GLint positionLocation = mglGetUniformLocation(program, ("pointLights[" + std::to_string(i) + "].position").c_str());
			pointLightsLocations[(i * AMOUNT_POINT_LIGHT_PROPERTIES) + (PL_POSITION) ] = positionLocation;
		}
		
		{
			GLint powerLocation = mglGetUniformLocation(program, ("pointLights[" + std::to_string(i) + "].power").c_str());
			pointLightsLocations[(i * AMOUNT_POINT_LIGHT_PROPERTIES) + (PL_POWER) ] = powerLocation;
		}
		
	}
	
	materialBlockLocation = mglGetUniformBlockIndex(program, "MaterialBlock");
	mglUniformBlockBinding(program, materialBlockLocation, 0);
	
	mglUseProgram(0);
	
}

void ms::ForwardShaderOGL::set_projection_matrix (const math::mat4 & proj) {
	mglUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, proj.c_array());
}

void ms::ForwardShaderOGL::set_camera_transformation (const math::mat4 & transf) {
	mglUniformMatrix4fv(cameraTransformationLocation, 1, GL_FALSE, transf.c_array());
}

void ms::ForwardShaderOGL::set_model_transformation (const math::mat4 & modelTransf) {
	mglUniformMatrix4fv(modelTransformationLocation, 1, GL_FALSE, modelTransf.c_array());
}

void ms::ForwardShaderOGL::set_has_directional_light (bool doesItHave) {
	mglUniform1i(hasDirectionalLightLocation, doesItHave == true ? 1 : 0);
}

void ms::ForwardShaderOGL::set_amount_of_point_lights (int amount) {
	mglUniform1i(pointLightsAmount, amount);
}

void ms::ForwardShaderOGL::set_point_light_power (unsigned int index, float power) {
	GLint powerLocation = pointLightsLocations[(index * AMOUNT_POINT_LIGHT_PROPERTIES) + PL_POWER];
	mglUniform1f(powerLocation, power);
}

void ms::ForwardShaderOGL::set_point_light_color (unsigned int index, const math::vec3 & color) {
	GLint colorLocation = pointLightsLocations[(index * AMOUNT_POINT_LIGHT_PROPERTIES) + PL_COLOR];
	mglUniform3fv(colorLocation, 1, color.c_array());
}

void ms::ForwardShaderOGL::set_point_light_position (unsigned int index, const math::vec3 & position) {
	GLint positionLocation = pointLightsLocations[(index * AMOUNT_POINT_LIGHT_PROPERTIES) + PL_POSITION];
	mglUniform3fv(positionLocation, 1, position.c_array());
}

void ms::ForwardShaderOGL::set_has_normal_texture(bool doesItHave) {
	mglUniform1i(hasNormalTextureLocation, doesItHave == true ? 1 : 0);
}

void ms::ForwardShaderOGL::set_amount_of_spot_lights (int amount) {
	mglUniform1i(spotLightsAmount, amount);
}

void ms::ForwardShaderOGL::set_spot_light_power (unsigned int index, float power) {
	GLint powerLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_POWER];
	mglUniform1f(powerLocation, power);
}

void ms::ForwardShaderOGL::set_spot_light_color (unsigned int index, const math::vec3 & color) {
	GLint colorLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_COLOR];
	mglUniform3fv(colorLocation, 1, color.c_array());
}

void ms::ForwardShaderOGL::set_spot_light_position (unsigned int index, const math::vec3 & position) {
	GLint positionLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_POSITION];
	glUniform3fv(positionLocation, 1, position.c_array());
}

void ms::ForwardShaderOGL::set_spot_light_angle (unsigned int index, float angle) {
	GLint angleLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_ANGLE_DEGREES];
	mglUniform1f(angleLocation, angle);
}

void ms::ForwardShaderOGL::set_spot_light_direction (unsigned int index, const math::vec3 direction) {
	GLint directionLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_DIRECTION];
	mglUniform3fv(directionLocation, 1, direction.c_array());
}

void ms::ForwardShaderOGL::set_directional_light_dir (const math::vec3 & dir) {
	mglUniform3fv(directionalLightDirectionLocation, 1, dir.c_array());
}

void ms::ForwardShaderOGL::set_directional_light_color (const math::vec3 & color) {
	mglUniform3fv(directionalLightColorLocation, 1, color.c_array());
}

void ms::ForwardShaderOGL::set_has_material (bool doesItHave) {
	mglUniform1i(hasMaterialLocation, doesItHave == true ? 1 : 0);
}

void ms::ForwardShaderOGL::set_has_diffuse_texture (bool doesItHave) {
	mglUniform1i(hasDiffuseTextureLocation, doesItHave == true ? 1 : 0);
}

void ms::ForwardShaderOGL::set_has_specular_texture (bool doesItHave) {
	mglUniform1i(hasSpecularTextureLocation, doesItHave == true ? 1 : 0);
}

ms::ForwardShaderOGL::~ForwardShaderOGL() {
	delete [] spotLightsLocations;
	delete [] pointLightsLocations;
}
