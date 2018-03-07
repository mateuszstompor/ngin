
//  resourceCoordinator.c
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "resourceCoordinator.hpp"
#include <cstdlib>
#include <cstdio>

using namespace std;

string ToString(ms::Resource* sz) {
	
	const void * address = static_cast<const void*>(sz);
	std::stringstream ss;
	ss << address;
	return ss.str();
}

ms::Resource* fromString (std::string s){
	size_t index;
	sscanf(s.c_str(), "%zu", &index);
	return (ms::Resource*)index;
}


std::shared_ptr<ms::ResourceCoordinator> ms::ResourceCoordinator::sharedInstance = nullptr;

ms::ResourceCoordinator::ResourceCoordinator () : loadedResources(std::list<std::string>()), allocatedResources(std::list<std::string>()) { }

void ms::ResourceCoordinator::register_load (Resource* resource) {
	loadedResources.push_back(ToString(resource));
	#ifdef DEBUG
		std::cout << "#ResourceCoordinator::load " << loadedResources.size() << " " << resource << std::endl;
	#endif
}

void ms::ResourceCoordinator::register_unload (Resource* resource) {
	
	loadedResources.erase(std::remove(loadedResources.begin(), loadedResources.end(), ToString(resource)));
	
	#ifdef DEBUG
		std::cout << "#ResourceCoordinator::unload " << loadedResources.size() << " " << resource << std::endl;
	#endif
}

void ms::ResourceCoordinator::register_allocation (Resource* resource) {
	allocatedResources.push_back(ToString(resource));
}

void ms::ResourceCoordinator::register_deallocation (Resource* resource) {
//	loadedResources.erase(std::remove(loadedResources.begin(), loadedResources.end(), ToString(resource)));
//	allocatedResources.erase(std::remove(allocatedResources.begin(), allocatedResources.end(), ToString(resource)));
	std::cout<< "dealloc " << resource << std::endl;
}

void ms::ResourceCoordinator::destroy_shared_instance () {
//	ResourceCoordinator::sharedInstance = nullptr;
}

void ms::ResourceCoordinator::unload_all_resources () {
//	std::for_each(loadedResources.begin(), loadedResources.end(), [] (std::string r) { (fromString(r))->unload(); });
//	loadedResources.erase(loadedResources.begin(), loadedResources.end());
}

std::shared_ptr<ms::ResourceCoordinator> ms::ResourceCoordinator::get_instance () {
	if (ResourceCoordinator::sharedInstance == nullptr) {
		sharedInstance = std::shared_ptr<ResourceCoordinator>(new ResourceCoordinator);
	}
	return sharedInstance;
}
