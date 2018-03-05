//
//  resource.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "resource.hpp"
#include "resourceCoordinator.hpp"
#include <iostream>

#ifdef DEBUG
	int ms::Resource::allocatedInstances = 0;
	int ms::Resource::loadedInstances = 0;
#endif

bool ms::Resource::is_loaded () {
	return isLoaded;
}

void ms::Resource::load () {
	
	#ifdef DEBUG
		std::cout << "#Resource::load " << loadedInstances << std::endl;
		Resource::loadedInstances += 1;
	#endif
	
	ResourceCoordinator::get_instance()->register_load(this);
}

void ms::Resource::unload () {
	
	#ifdef DEBUG
		Resource::loadedInstances -= 1;
		std::cout << "#Resource::unload " << loadedInstances << std::endl;
	#endif
	
	ResourceCoordinator::get_instance()->register_unload(this);
}

ms::Resource::Resource () : isLoaded(false) {
	
	#ifdef DEBUG
		std::cout << "#Resource::Resource " << allocatedInstances << std::endl;
		Resource::allocatedInstances += 1;
	#endif
	
	ResourceCoordinator::get_instance()->register_allocation(this);
}

ms::Resource::~Resource () {
	
	#ifdef DEBUG
		Resource::allocatedInstances -= 1;
		std::cout << "#Resource::~Resource " << allocatedInstances << std::endl;
	#endif
	
	ResourceCoordinator::get_instance()->register_deallocation(this);
}

