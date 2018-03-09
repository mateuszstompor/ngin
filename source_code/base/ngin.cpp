//
//  ngin.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "ngin.hpp"

ms::NGin::NGin(unsigned int scW, unsigned int scH, float camNear, float camFar, float fovDegrees, float aspect) :
	scene(new Scene(camNear, camFar, fovDegrees, aspect)),
	screenWidth(scW),
	screenHeight(scH),
	forwardRenderer(nullptr),
	deferredRenderer(nullptr) { }

void ms::NGin::unload() {
	ResourceCoordinator::sharedInstance->unload_all_resources();
}
