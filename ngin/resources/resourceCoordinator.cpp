
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
	
	#ifdef RC_ASSERTIONS
		assert(std::find(loadedResources.begin(), loadedResources.end(), utils::ptr_to_string(resource)) == loadedResources.end());
	#endif
	
	loadedResources.insert(utils::ptr_to_string(resource));
	
	#ifdef RC_ALLOCATIONS
		std::cout << "#ResourceCoordinator::load " << loadedResources.size() << " " << resource << '\n';
	#endif
}

void ms::ResourceCoordinator::register_unload (Resource* resource) {

	#ifdef RC_ASSERTIONS
		assert(std::find(loadedResources.begin(), loadedResources.end(), utils::ptr_to_string(resource)) != loadedResources.end());
		assert(std::find(allocatedResources.begin(), allocatedResources.end(), utils::ptr_to_string(resource)) != allocatedResources.end());
	#endif
	
	loadedResources.erase(utils::ptr_to_string(resource));
	
	#ifdef RC_UNLOADS
		std::cout << "#ResourceCoordinator::unload " << loadedResources.size() << " " << resource << '\n';
	#endif
}

void ms::ResourceCoordinator::register_allocation (Resource* resource) {
	#ifdef RC_ALLOCATIONS
		std::cout << "#ResourceCoordinator::register_allocation " << allocatedResources.size() << " " << resource << '\n';
	#endif
	allocatedResources.insert(utils::ptr_to_string(resource));
}

void ms::ResourceCoordinator::register_deallocation (Resource* resource) {
	#ifdef RC_DEALLOCATIONS
		std::cout << "#ResourceCoordinator::register_deallocation " << allocatedResources.size() << " " << resource << '\n';
		assert(std::find(allocatedResources.begin(), allocatedResources.end(), utils::ptr_to_string(resource)) != allocatedResources.end());
	#endif
	
	#ifdef DEBUG
	if(std::find(loadedResources.begin(), loadedResources.end(), utils::ptr_to_string(resource)) != loadedResources.end()) {
		std::cerr << "#ResourceCoordinator::OBJECT DEALLOCATED BUT NOT UNLOADED, LEAK!\n";
	}
	#endif
	

	loadedResources.erase(utils::ptr_to_string(resource));
	allocatedResources.erase(utils::ptr_to_string(resource));
}

void ms::ResourceCoordinator::destroy_shared_instance () {
	ResourceCoordinator::sharedInstance = nullptr;
}

void ms::ResourceCoordinator::unload_all_resources () {
	std::set<std::string> tmp(loadedResources.begin(), loadedResources.end());
	std::for_each(tmp.begin(), tmp.end(), [] (std::string res) { utils::ptr_from_string<Resource>(res)->unload(); });
}

std::shared_ptr<ms::ResourceCoordinator> ms::ResourceCoordinator::get_instance () {
	if (ResourceCoordinator::sharedInstance == nullptr)
		sharedInstance = std::shared_ptr<ResourceCoordinator>(new ResourceCoordinator);
	return sharedInstance;
}
