
//  resourceCoordinator.c
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "resourceCoordinator.hpp"

std::shared_ptr<ms::ResourceCoordinator> ms::ResourceCoordinator::sharedInstance = nullptr;

ms::ResourceCoordinator::ResourceCoordinator () : loadedResources(std::set<std::string>()), allocatedResources(std::set<std::string>()) { }

void ms::ResourceCoordinator::register_load (Resource* resource) {
	
	#ifdef DEBUG
		assert(std::find(loadedResources.begin(), loadedResources.end(), utils::ptr_to_string(resource)) == loadedResources.end());
	#endif
	
	loadedResources.insert(utils::ptr_to_string(resource));
	
	#ifdef DEBUG
		std::cout << "#ResourceCoordinator::load " << loadedResources.size() << " " << resource << std::endl;
	#endif
}

void ms::ResourceCoordinator::register_unload (Resource* resource) {

	#ifdef DEBUG
		assert(std::find(loadedResources.begin(), loadedResources.end(), utils::ptr_to_string(resource)) != loadedResources.end());
	#endif
	
	loadedResources.erase(utils::ptr_to_string(resource));
	
	#ifdef DEBUG
		std::cout << "#ResourceCoordinator::unload " << loadedResources.size() << " " << resource << std::endl;
	#endif
}

void ms::ResourceCoordinator::register_allocation (Resource* resource) {
	allocatedResources.insert(utils::ptr_to_string(resource));
}

void ms::ResourceCoordinator::register_deallocation (Resource* resource) {
	loadedResources.erase(utils::ptr_to_string(resource));
	allocatedResources.erase(utils::ptr_to_string(resource));
}

void ms::ResourceCoordinator::destroy_shared_instance () {
	ResourceCoordinator::sharedInstance = nullptr;
}

void ms::ResourceCoordinator::unload_all_resources () {
	std::for_each(loadedResources.begin(), loadedResources.end(), [] (std::string res) { utils::ptr_from_string<Resource>(res)->unload(); });
}

std::shared_ptr<ms::ResourceCoordinator> ms::ResourceCoordinator::get_instance () {
	if (ResourceCoordinator::sharedInstance == nullptr) {
		sharedInstance = std::shared_ptr<ResourceCoordinator>(new ResourceCoordinator);
	}
	return sharedInstance;
}
