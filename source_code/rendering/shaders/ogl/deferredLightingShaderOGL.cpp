//
//  deferredLightingShaderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredLightingShaderOGL.hpp"

ms::DeferredLightingShaderOGL::DeferredLightingShaderOGL(unsigned int maximalAmountOfLights,
														 std::string vertexShaderSource, 
														 std::string fragmentShaderSource) :
														 DeferredLightingShader(maximalAmountOfLights),
														 ms::ShaderOGL(vertexShaderSource, "", "", "", fragmentShaderSource),
														 pointLightsLocations(nullptr), spotLightsLocations(nullptr) {
															 
	 spotLightsLocations 	= new GLint[AMOUNT_SPOT_LIGHT_PROPERTIES * maximalAmountOfLights];
	 pointLightsLocations 	= new GLint[AMOUNT_POINT_LIGHT_PROPERTIES * maximalAmountOfLights];

}

void  ms::DeferredLightingShaderOGL::load () {
	
	if(!is_loaded()) {
	
		ShaderOGL::load();
		
		mglUseProgram(program);
		
		GLint gPosition = mglGetUniformLocation(program, "gPosition");
		mglUniform1i(gPosition, 0);
		
		GLint gNormal = mglGetUniformLocation(program, "gNormal");
		mglUniform1i(gNormal, 1);
		
		GLint gAlbedo = mglGetUniformLocation(program, "gAlbedo");
		mglUniform1i(gAlbedo, 2);
		
		directionalLightColorLocation = mglGetUniformLocation(program, "dirLight.color");
		directionalLightDirectionLocation = mglGetUniformLocation(program, "dirLight.direction");
		hasDirectionalLightLocation = mglGetUniformLocation(program, "hasDirLight");
		
		cameraTransformationLocation = mglGetUniformLocation(program, "cameraTransformation");
		renderModeLocation = mglGetUniformLocation(program, "renderMode");
		
		spotLightsAmount = mglGetUniformLocation(program, "spotLightsAmount");
		
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
		
		mglUseProgram(0);
		
	}
	
}

void ms::DeferredLightingShaderOGL::set_amount_of_point_lights (int amount) {
	mglUniform1i(pointLightsAmount, amount);
}

void ms::DeferredLightingShaderOGL::set_point_light_power (unsigned int index, float power) {
	GLint powerLocation = pointLightsLocations[(index * AMOUNT_POINT_LIGHT_PROPERTIES) + PL_POWER];
	mglUniform1f(powerLocation, power);
}

void ms::DeferredLightingShaderOGL::set_point_light_color (unsigned int index, const math::vec4 & color) {
	GLint colorLocation = pointLightsLocations[(index * AMOUNT_POINT_LIGHT_PROPERTIES) + PL_COLOR];
	mglUniform4fv(colorLocation, 1, color.c_array());
}

void ms::DeferredLightingShaderOGL::set_point_light_position (unsigned int index, const math::vec3 & position) {
	GLint positionLocation = pointLightsLocations[(index * AMOUNT_POINT_LIGHT_PROPERTIES) + PL_POSITION];
	mglUniform3fv(positionLocation, 1, position.c_array());
}

void ms::DeferredLightingShaderOGL::set_amount_of_spot_lights (int amount) {
	mglUniform1i(spotLightsAmount, amount);
}

void ms::DeferredLightingShaderOGL::set_has_directional_light (bool doesItHave) {
	mglUniform1i(hasDirectionalLightLocation, doesItHave == true ? 1 : 0);
}

void ms::DeferredLightingShaderOGL::set_directional_light_dir (const math::vec3 & dir) {
	mglUniform3fv(directionalLightDirectionLocation, 1, dir.c_array());
}

void ms::DeferredLightingShaderOGL::set_directional_light_color (const math::vec4 & color) {
	mglUniform4fv(directionalLightColorLocation, 1, color.c_array());
}

void ms::DeferredLightingShaderOGL::set_camera_transformation (const math::mat4 & transf) {
	mglUniformMatrix4fv(cameraTransformationLocation, 1, GL_FALSE, transf.c_array());
}

void ms::DeferredLightingShaderOGL::set_spot_light_power (unsigned int index, float power) {
	GLint powerLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_POWER];
	mglUniform1f(powerLocation, power);
}

void ms::DeferredLightingShaderOGL::set_spot_light_color (unsigned int index, const math::vec4 & color) {
	GLint colorLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_COLOR];
	mglUniform4fv(colorLocation, 1, color.c_array());
}

void ms::DeferredLightingShaderOGL::set_spot_light_position (unsigned int index, const math::vec3 & position) {
	GLint positionLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_POSITION];
	glUniform3fv(positionLocation, 1, position.c_array());
}

void ms::DeferredLightingShaderOGL::set_spot_light_angle (unsigned int index, float angle) {
	GLint angleLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_ANGLE_DEGREES];
	mglUniform1f(angleLocation, angle);
}

void ms::DeferredLightingShaderOGL::set_spot_light_direction (unsigned int index, const math::vec3 direction) {
	GLint directionLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_DIRECTION];
	mglUniform3fv(directionLocation, 1, direction.c_array());
}

void ms::DeferredLightingShaderOGL::set_rendering_mode (unsigned int settings) {
	mglUniform1ui(renderModeLocation, settings);
}

ms::DeferredLightingShaderOGL::~DeferredLightingShaderOGL () {
	delete [] spotLightsLocations;
	delete [] pointLightsLocations;
}
