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
	deferredRenderer(nullptr) {
		
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
		std::shared_ptr<ms::SceneNode> node = get_node();
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
