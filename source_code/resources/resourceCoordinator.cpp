//
//  resourceCoordinator.c
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "resourceCoordinator.hpp"
#include <iostream>

std::shared_ptr<ms::ResourceCoordinator> ms::ResourceCoordinator::sharedInstance = nullptr;

void ms::ResourceCoordinator::register_load (Resource* resource) {
	loadedResources.push_back(resource);
}

void ms::ResourceCoordinator::register_unload (Resource* resource) {
	std::remove(loadedResources.begin(), loadedResources.end(), resource);
}

void ms::ResourceCoordinator::register_allocation (Resource* resource) {
	allocatedResources.push_back(resource);
}

void ms::ResourceCoordinator::register_deallocation (Resource* resource) {
	std::remove(allocatedResources.begin(), allocatedResources.end(), resource);
}

void ms::ResourceCoordinator::destroy_shared_instance () {
	ResourceCoordinator::sharedInstance = nullptr;
}

void ms::ResourceCoordinator::unload_all_resources () {
	for(Resource* res : loadedResources) {
		res->unload();
	}
}


ms::ResourceCoordinator::ResourceCoordinator () :
	loadedResources(std::vector<Resource*>()),
	allocatedResources(std::vector<Resource*>()) {
	
}

std::shared_ptr<ms::ResourceCoordinator> ms::ResourceCoordinator::get_instance () {
	if (ResourceCoordinator::sharedInstance == nullptr) {
		sharedInstance = std::shared_ptr<ResourceCoordinator>(new ResourceCoordinator);
	}
	return sharedInstance;
}
