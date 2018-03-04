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
	ResourceCoordinator::get_instance()->register_load(this);
}

void ms::Resource::unload () {
	ResourceCoordinator::get_instance()->register_unload(this);
}

ms::Resource::Resource () : isLoaded(false) {
	ResourceCoordinator::get_instance()->register_allocation(this);
}

ms::Resource::~Resource () {
	ResourceCoordinator::get_instance()->register_deallocation(this);
}

