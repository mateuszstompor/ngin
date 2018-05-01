//
//  resource.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "resource.hpp"
#include "resourceCoordinator.hpp"

bool ms::Resource::is_loaded () const {
	return isLoaded;
}

void ms::Resource::load () {
	if(!isLoaded) {
		isLoaded = true;
		ResourceCoordinator::get_instance()->register_load(this);
		#ifdef R_LOADS
			std::cout << "#Resource::load " << this << '\n';
		#endif
		_load();
	}
}

void ms::Resource::unload () {
	if(isLoaded) {
		isLoaded = false;
		ResourceCoordinator::get_instance()->register_unload(this);
		#ifdef R_UNLOADS
			std::cout << "#Resource::unload " << this << '\n';
		#endif
		_unload();
	}
}

ms::Resource::Resource () : isLoaded(false) {
	
	#ifdef R_ALLOCATIONS
		std::cout << "#Resource::Resource " << this << '\n';
	#endif
	
	ResourceCoordinator::get_instance()->register_allocation(this);
}

ms::Resource::~Resource () {
	
	#ifdef R_DEALLOCATIONS
		std::cout << "#Resource::~Resource " << this << '\n';
	#endif
	
	ResourceCoordinator::get_instance()->register_deallocation(this);
}

