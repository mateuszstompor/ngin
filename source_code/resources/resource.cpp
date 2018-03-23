//
//  resource.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "resource.hpp"
#include "resourceCoordinator.hpp"

bool ms::Resource::is_loaded () {
	return isLoaded;
}

void ms::Resource::load () {
	
	#ifdef R_LOADS
		std::cout << "#Resource::load " << this << std::endl;
	#endif
	
	isLoaded = true;
	ResourceCoordinator::get_instance()->register_load(this);
}

void ms::Resource::unload () {
	
	#ifdef R_UNLOADS
		std::cout << "#Resource::unload " << this << std::endl;
	#endif
	
	isLoaded = false;
	ResourceCoordinator::get_instance()->register_unload(this);
}

ms::Resource::Resource () : isLoaded(false) {
	
	#ifdef R_ALLOCATIONS
		std::cout << "#Resource::Resource " << this << std::endl;
	#endif
	
	ResourceCoordinator::get_instance()->register_allocation(this);
}

std::string ms::Resource::get_class () {
	return "ms::Resource";
}

ms::Resource::~Resource () {
	
	#ifdef R_DEALLOCATIONS
		std::cout << "#Resource::~Resource " << this << std::endl;
	#endif
	
	ResourceCoordinator::get_instance()->register_deallocation(this);
}

