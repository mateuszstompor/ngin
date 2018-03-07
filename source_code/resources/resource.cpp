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

bool ms::Resource::is_loaded () {
	return isLoaded;
}

void ms::Resource::load () {
	
	#ifdef DEBUG
		std::cout << "#Resource::load " << std::endl;
	#endif
	
	isLoaded = true;
	ResourceCoordinator::get_instance()->register_load(this);
}

void ms::Resource::unload () {
	
	#ifdef DEBUG
		std::cout << "#Resource::unload " << std::endl;
	#endif
	
	isLoaded = false;
	ResourceCoordinator::get_instance()->register_unload(this);
}

ms::Resource::Resource () : isLoaded(false) {
	
	#ifdef DEBUG
		std::cout << "#Resource::Resource " << std::endl;
	#endif
	
	ResourceCoordinator::get_instance()->register_allocation(this);
}

ms::Resource::~Resource () {
	
	#ifdef DEBUG
		std::cout << "#Resource::~Resource " << std::endl;
	#endif
	
	ResourceCoordinator::get_instance()->register_deallocation(this);
}

