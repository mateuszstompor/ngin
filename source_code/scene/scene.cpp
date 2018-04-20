//
//  scene.cpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "scene.hpp"

ms::Scene::Scene (std::unique_ptr<Camera> && c) :
directionalLight(nullptr),
cam{std::move(c)} { }

ms::Camera const & ms::Scene:: get_camera() const {
	return *cam;
}

ms::Camera & ms::Scene::  get_camera() {
	return *cam;
}

ms::DirectionalLight const * ms::Scene::get_directional_light() const {
	return directionalLight.get();
}

ms::DirectionalLight  * ms::Scene::  get_directional_light()  {
	return directionalLight.get();
}
