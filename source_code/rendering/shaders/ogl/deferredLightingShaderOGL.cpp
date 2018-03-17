//
//  deferredLightingShaderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredLightingShaderOGL.hpp"

ms::DeferredLightingShaderOGL::DeferredLightingShaderOGL(str_ptr vertexShaderSource, str_ptr fragmentShaderSource) :
ms::ShaderOGL(vertexShaderSource, nullptr, nullptr, nullptr, fragmentShaderSource) { }

void  ms::DeferredLightingShaderOGL::load () {
    ShaderOGL::load();

    mglUseProgram(program);

    GLint gPosition = mglGetUniformLocation(program, "gPosition");
    mglUniform1i(gPosition, 0);

    GLint gNormal = mglGetUniformLocation(program, "gNormal");
    mglUniform1i(gNormal, 1);

    GLint gAlbedo = mglGetUniformLocation(program, "gAlbedo");
    mglUniform1i(gAlbedo, 2);

    mglUseProgram(0);

}

void ms::DeferredLightingShaderOGL::set_amount_of_point_lights (int amount) {
	GLint amountLocation = mglGetUniformLocation(program, "pointLightsAmount");
	mglUniform1i(amountLocation, amount);
}

void ms::DeferredLightingShaderOGL::set_point_light_power (unsigned int index, float power) {
	std::string name = "pointLights[" + std::to_string(index) + "].power";
	GLint powerLocation = mglGetUniformLocation(program, name.c_str());
	mglUniform1f(powerLocation, power);
}

void ms::DeferredLightingShaderOGL::set_point_light_color (unsigned int index, const math::vec4 & color) {
	std::string name = "pointLights[" + std::to_string(index) + "].color";
	GLint colorLocation = mglGetUniformLocation(program, name.c_str());
	mglUniform4fv(colorLocation, 1, color.c_array());
}

void ms::DeferredLightingShaderOGL::set_point_light_position (unsigned int index, const math::vec3 & position) {
	std::string name = "pointLights[" + std::to_string(index) + "].position";
	GLint positionLocation = mglGetUniformLocation(program, name.c_str());
	mglUniform3fv(positionLocation, 1, position.c_array());
}

void ms::DeferredLightingShaderOGL::set_amount_of_spot_lights (int amount) {
	GLint amountLocation = mglGetUniformLocation(program, "spotLightsAmount");
	mglUniform1i(amountLocation, amount);
}

void ms::DeferredLightingShaderOGL::set_spot_light_power (unsigned int index, float power) {
	std::string name = "spotLights[" + std::to_string(index) + "].power";
	GLint powerLocation = mglGetUniformLocation(program, name.c_str());
	mglUniform1f(powerLocation, power);
}

void ms::DeferredLightingShaderOGL::set_has_directional_light (bool doesItHave) {
	GLint hasLight = mglGetUniformLocation(program, "hasDirLight");
	mglUniform1i(hasLight, doesItHave == true ? 1 : 0);
}

void ms::DeferredLightingShaderOGL::set_directional_light_dir (const math::vec3 & dir) {
	GLint direction = mglGetUniformLocation(program, "dirLight.direction");
	mglUniform3fv(direction, 1, dir.c_array());
}

void ms::DeferredLightingShaderOGL::set_directional_light_color (const math::vec4 & color) {
	GLint colorLocation = mglGetUniformLocation(program, "dirLight.color");
	mglUniform4fv(colorLocation, 1, color.c_array());
}

void ms::DeferredLightingShaderOGL::set_spot_light_color (unsigned int index, const math::vec4 & color) {
	std::string name = "spotLights[" + std::to_string(index) + "].color";
	GLint colorLocation = mglGetUniformLocation(program, name.c_str());
	mglUniform4fv(colorLocation, 1, color.c_array());
}

void ms::DeferredLightingShaderOGL::set_spot_light_position (unsigned int index, const math::vec3 & position) {
	std::string name = "spotLights[" + std::to_string(index) + "].position";
	GLint positionLocation = mglGetUniformLocation(program, name.c_str());
	glUniform3fv(positionLocation, 1, position.c_array());
}

void ms::DeferredLightingShaderOGL::set_spot_light_angle (unsigned int index, float angle) {
	std::string name = "spotLights[" + std::to_string(index) + "].angleDegrees";
	GLint angleLocation = mglGetUniformLocation(program, name.c_str());
	mglUniform1f(angleLocation, angle);
}

void ms::DeferredLightingShaderOGL::set_spot_light_direction (unsigned int index, const math::vec3 direction) {
	std::string name = "spotLights[" + std::to_string(index) + "].direction";
	GLint directionLocation = mglGetUniformLocation(program, name.c_str());
	mglUniform3fv(directionLocation, 1, direction.c_array());
}

void ms::DeferredLightingShaderOGL::set_rendering_mode (unsigned int settings) {
	GLint settingsLocation = mglGetUniformLocation(program, "renderMode");
	mglUniform1ui(settingsLocation, settings);
}
