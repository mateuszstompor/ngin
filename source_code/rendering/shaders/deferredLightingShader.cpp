//
//  deferredLightingShader.cpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredLightingShader.hpp"

ms::DeferredLightingShader::DeferredLightingShader(unsigned int maxAmountOfPointLights,
                                                         unsigned int maxAmountOfSpotLights,
														 std::string vertexShaderSource, 
														 std::string fragmentShaderSource) :
														 maxAmountOfPointLights(maxAmountOfPointLights), maxAmountOfSpotLights(maxAmountOfSpotLights),
														 ms::Shader(vertexShaderSource, "", "", "", fragmentShaderSource),
														 pointLightsLocations(nullptr), spotLightsLocations(nullptr) {
															 
     spotLightsLocations    = new GLint[AMOUNT_SPOT_LIGHT_PROPERTIES * maxAmountOfSpotLights];
     pointLightsLocations   = new GLint[AMOUNT_POINT_LIGHT_PROPERTIES * maxAmountOfPointLights];

}

void  ms::DeferredLightingShader::_load () {
	
	Shader::_load();
	
	mglUseProgram(program);
	
	GLint gPosition = mglGetUniformLocation(program, "gPosition");
	mglUniform1i(gPosition, 0);
	
	GLint gNormal = mglGetUniformLocation(program, "gNormal");
	mglUniform1i(gNormal, 1);
	
	GLint gAlbedo = mglGetUniformLocation(program, "gAlbedo");
	mglUniform1i(gAlbedo, 2);
    
    GLint shadowMap = mglGetUniformLocation(program, "shadowMap");
    mglUniform1i(shadowMap, 3);
    
	directionalLightColorLocation = mglGetUniformLocation(program, "dirLight.color");
	directionalLightDirectionLocation = mglGetUniformLocation(program, "dirLight.direction");
	hasDirectionalLightLocation = mglGetUniformLocation(program, "hasDirLight");
	
	cameraTransformationLocation = mglGetUniformLocation(program, "cameraTransformation");
	renderModeLocation = mglGetUniformLocation(program, "renderMode");
	
	spotLightsAmount = mglGetUniformLocation(program, "spotLightsAmount");
	
	for(unsigned int i = 0; i < maxAmountOfSpotLights; ++i) {
		
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
        
        GLint spotLightShadowMap = mglGetUniformLocation(program, ("spotLightsShadowMaps[" + std::to_string(i) + "]").c_str());
        mglUniform1i(spotLightShadowMap, 4 + i);
	}
	
	pointLightsAmount = mglGetUniformLocation(program, "pointLightsAmount");
	
	for(unsigned int i = 0; i < maxAmountOfPointLights; ++i) {
		
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

void ms::DeferredLightingShader::set_amount_of_point_lights (int amount) {
	mglUniform1i(pointLightsAmount, amount);
}

void ms::DeferredLightingShader::set_point_light_power (unsigned int index, float power) {
	GLint powerLocation = pointLightsLocations[(index * AMOUNT_POINT_LIGHT_PROPERTIES) + PL_POWER];
	mglUniform1f(powerLocation, power);
}

void ms::DeferredLightingShader::set_point_light_color (unsigned int index, const math::vec3 & color) {
	GLint colorLocation = pointLightsLocations[(index * AMOUNT_POINT_LIGHT_PROPERTIES) + PL_COLOR];
	mglUniform3fv(colorLocation, 1, color.c_array());
}

void ms::DeferredLightingShader::set_point_light_position (unsigned int index, const math::vec3 & position) {
	GLint positionLocation = pointLightsLocations[(index * AMOUNT_POINT_LIGHT_PROPERTIES) + PL_POSITION];
	mglUniform3fv(positionLocation, 1, position.c_array());
}

void ms::DeferredLightingShader::set_amount_of_spot_lights (int amount) {
	mglUniform1i(spotLightsAmount, amount);
}

void ms::DeferredLightingShader::set_has_directional_light (bool doesItHave) {
	mglUniform1i(hasDirectionalLightLocation, doesItHave == true ? 1 : 0);
}

void ms::DeferredLightingShader::set_directional_light_dir (const math::vec3 & dir) {
	mglUniform3fv(directionalLightDirectionLocation, 1, dir.c_array());
}

void ms::DeferredLightingShader::set_directional_light_color (const math::vec3 & color) {
	mglUniform3fv(directionalLightColorLocation, 1, color.c_array());
}

void ms::DeferredLightingShader::set_camera_transformation (const math::mat4 & transf) {
	mglUniformMatrix4fv(cameraTransformationLocation, 1, GL_FALSE, transf.c_array());
}

void ms::DeferredLightingShader::set_spot_light_power (unsigned int index, float power) {
	GLint powerLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_POWER];
	mglUniform1f(powerLocation, power);
}

void ms::DeferredLightingShader::set_spot_light_color (unsigned int index, const math::vec3 & color) {
	GLint colorLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_COLOR];
	mglUniform3fv(colorLocation, 1, color.c_array());
}

void ms::DeferredLightingShader::set_spot_light_position (unsigned int index, const math::vec3 & position) {
	GLint positionLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_POSITION];
	glUniform3fv(positionLocation, 1, position.c_array());
}

void ms::DeferredLightingShader::set_spot_light_angle (unsigned int index, float angle) {
	GLint angleLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_ANGLE_DEGREES];
	mglUniform1f(angleLocation, angle);
}

void ms::DeferredLightingShader::set_spot_light_direction (unsigned int index, const math::vec3 direction) {
	GLint directionLocation = spotLightsLocations[(index * AMOUNT_SPOT_LIGHT_PROPERTIES) + SL_DIRECTION];
	mglUniform3fv(directionLocation, 1, direction.c_array());
}

void ms::DeferredLightingShader::set_rendering_mode (unsigned int settings) {
	mglUniform1ui(renderModeLocation, settings);
}

void ms::DeferredLightingShader::bind_g_buf_albedo (Texture & albedoTexture) {
	mglActiveTexture(GL_TEXTURE2);
	albedoTexture.use();
}

void ms::DeferredLightingShader::bind_shadow_map (Texture & shadowTexture) {
    mglActiveTexture(GL_TEXTURE3);
    shadowTexture.use();
}

void ms::DeferredLightingShader::set_directional_sm_transform (const math::mat4 & color) {
    
}

void ms::DeferredLightingShader::set_directional_sm_projection (const math::mat4 & color) {

}

void ms::DeferredLightingShader::bind_g_buf_normals (Texture & normalsTexture) {
	mglActiveTexture(GL_TEXTURE1);
	normalsTexture.use();
}

void ms::DeferredLightingShader::bind_g_buf_posiitons (Texture & positionsTexture) {
	mglActiveTexture(GL_TEXTURE0);
	positionsTexture.use();
}

ms::DeferredLightingShader::~DeferredLightingShader () {
	delete [] spotLightsLocations;
	delete [] pointLightsLocations;
}