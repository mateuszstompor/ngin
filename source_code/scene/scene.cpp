//
//  scene.cpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "scene.hpp"

ms::Scene::Scene (float nearPlan, float farPlan, float fovDegress, float cameraAspect) : directionalLight(nullptr), nodes(std::vector<std::shared_ptr<SceneNode>>()) {
	cam = std::unique_ptr<Camera>( new PerspectiveCamera(nearPlan, farPlan, fovDegress, cameraAspect) );
}

std::unique_ptr<ms::Camera> const & ms::Scene::get_camera() {
	return this->cam;
}

std::unique_ptr<ms::DirectionalLight> const & ms::Scene::get_directional_light() {
	return this->directionalLight;
}

void ms::Scene::set_directional_light(float power, math::vec4 color, math::vec3 direction) {
	directionalLight = std::unique_ptr<ms::DirectionalLight>(new DirectionalLight(power, color, direction));
}
