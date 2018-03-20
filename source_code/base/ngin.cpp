//
//  ngin.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "ngin.hpp"

ms::NGin::NGin(unsigned int scW, unsigned int scH, float camNear, float camFar, float fovDegrees, float aspect) :
	scene(new Scene(camNear, camFar, fovDegrees, aspect)),
	screenWidth(scW),
	screenHeight(scH),
	forwardRenderer(nullptr),
	deferredRenderer(nullptr),
	lightSourceRenderer(nullptr) {
		
}

void ms::NGin::unload() {
	ResourceCoordinator::sharedInstance->unload_all_resources();
}

ms::DeferredRender & ms::NGin::get_deferred_render () const {
	return *deferredRenderer;
}

ms::ForwardRender & ms::NGin::get_forward_render () const {
	return *forwardRenderer;
}

void ms::NGin::load_model (std::string absolutePath) {
	
	if(loader == nullptr) {
		this->loader = get_loader();
	}
	
	Loader::model_data loadedData = loader->load_model(absolutePath);
	Loader::geometries_vec & geo = std::get<0>(loadedData);
	Loader::materials_map & mat = std::get<1>(loadedData);
	Loader::textures_map & tex = std::get<2>(loadedData);
	
	for (auto geometry : geo) {
		std::shared_ptr<ms::Drawable> node = get_drawable();
		node->geometry=geometry;
		scene->nodes.push_back(node);
	}
	
	for (auto material : mat) {
		scene->materials.insert(material);
	}
	
	for (auto texture : tex) {
		scene->textures.insert(texture);
	}
	
}

void ms::NGin::load_point_light	(float power,
								 math::vec4 color,
								 math::vec3 position,
								 std::string absolutePath) {
	
	if(loader == nullptr) {
		this->loader = get_loader();
	}
	
	Loader::model_data loadedData = loader->load_model(absolutePath);
	Loader::geometries_vec & geo = std::get<0>(loadedData);
	Loader::materials_map & mat = std::get<1>(loadedData);
	Loader::textures_map & tex = std::get<2>(loadedData);
	
	for (auto geometry : geo) {
		std::shared_ptr<ms::PointLight> node = get_point_light(power, color, position);
		node->geometry=geometry;
		scene->pointLights.push_back(node);
	}
	
	for (auto material : mat) {
		scene->materials.insert(material);
	}
	
	for (auto texture : tex) {
		scene->textures.insert(texture);
	}
	
}

void ms::NGin::load_spot_light (float power, math::vec4 color, math::vec3 position, float lightingAngleDegrees, math::vec3 direction, std::string absolutePath) {
	
	if(loader == nullptr) {
		this->loader = get_loader();
	}
	
	Loader::model_data loadedData = loader->load_model(absolutePath);
	Loader::geometries_vec & geo = std::get<0>(loadedData);
	Loader::materials_map & mat = std::get<1>(loadedData);
	Loader::textures_map & tex = std::get<2>(loadedData);
	
	for (auto geometry : geo) {
		std::shared_ptr<ms::SpotLight> node = get_spot_light(power, color, position, lightingAngleDegrees, direction);
		node->geometry=geometry;
		scene->spotLights.push_back(node);
	}
	
	for (auto material : mat) {
		scene->materials.insert(material);
	}
	
	for (auto texture : tex) {
		scene->textures.insert(texture);
	}
	
}

void ms::NGin::count_fps () {
	static int fps = 0;
	static auto start = std::chrono::high_resolution_clock::now();

	
	auto now = std::chrono::high_resolution_clock::now();
	auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
	
	fps += 1;
	
	if(timeElapsed > 1000.0f){
		std::cout << fps/(timeElapsed/1000.0f) << std::endl;
		fps = 0;
		start = now;
	}

}

void ms::NGin::draw_scene() {
	
	count_fps();
	
	int items = static_cast<int>(scene->nodes.size());
	
	deferredRenderer->use();
	deferredRenderer->setup_uniforms(scene.get());
	deferredRenderer->clear_frame();
	for(int i = 0; i < items; ++i) {
		deferredRenderer->draw(scene->nodes[i].get(), scene.get());
	}
	deferredRenderer->perform_light_pass(scene.get());
	
	lightSourceRenderer->use();
	for(int i = 0; i < scene->pointLights.size(); ++i) {
		lightSourceRenderer->draw(scene->pointLights[i].get(), scene.get());
	}
	for(int i = 0; i < scene->spotLights.size(); ++i) {
		lightSourceRenderer->draw(scene->spotLights[i].get(), scene.get());
	}
	
//	forwardRenderer->use();
//	for(int i = 0; i < scene->nodes.size(); ++i) {
//		forwardRenderer->draw(scene->nodes[i].get(), scene.get());
//	}

}
