//
//  scene.cpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "scene.hpp"

ms::Scene::Scene (float nearPlan, float farPlan, float fovDegress, float cameraAspect) : Scene() {
																								
	cam = std::make_unique<PerspectiveCamera>(nearPlan, farPlan, fovDegress, cameraAspect);
																								
}

ms::Scene::Scene () :   directionalLight(nullptr),
                        nodes(std::vector<std::shared_ptr<Drawable>>()),
                        materials(std::map<std::string, std::shared_ptr<Material>>()) { }

ms::Scene::Scene (std::unique_ptr<Camera> && c) : Scene() {
    this->cam = std::move(c);
}

void ms::Scene::set_directional_light(float power, math::vec3 color, math::vec3 direction) {
	directionalLight = std::make_unique<DirectionalLight>(color, direction);
}

ms::Camera const & ms::Scene::get_camera() const {
    return *cam;
}

ms::Camera & ms::Scene::get_camera() {
    return *cam;
}

ms::DirectionalLight * ms::Scene::get_directional_light() {
    return directionalLight.get();
}

ms::DirectionalLight const * ms::Scene::get_directional_light() const {
    return directionalLight.get();
}
