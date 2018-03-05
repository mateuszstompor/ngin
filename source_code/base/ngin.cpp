//
//  ngin.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "ngin.hpp"

ms::NGin::NGin(unsigned int scW, unsigned int scH) : scene(new Scene()), screenWidth(scW), screenHeight(scH) { }

void ms::NGin::unload() {
	ResourceCoordinator::sharedInstance->unload_all_resources();
	ResourceCoordinator::destroy_shared_instance();
}
