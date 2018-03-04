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
	
}

void ms::ResourceCoordinator::register_unload (Resource* resource) {
	
}

void ms::ResourceCoordinator::register_allocation (Resource* resource) {
	
}

void ms::ResourceCoordinator::register_deallocation (Resource* resource) {
	
}

void ms::ResourceCoordinator::destroy_shared_instance () {
	ResourceCoordinator::sharedInstance = nullptr;
}

ms::ResourceCoordinator::ResourceCoordinator () :
	loadedResources(std::vector<std::weak_ptr<Resource>>()),
	allocatedResources(std::vector<std::weak_ptr<Resource>>()) {
	
}

std::shared_ptr<ms::ResourceCoordinator> ms::ResourceCoordinator::get_instance () {
	if (ResourceCoordinator::sharedInstance == nullptr) {
		sharedInstance = std::shared_ptr<ResourceCoordinator>(new ResourceCoordinator);
	}
	return sharedInstance;
}
