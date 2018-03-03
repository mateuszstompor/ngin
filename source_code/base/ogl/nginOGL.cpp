//
//  nginOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "nginOGL.hpp"

ms::NGinOGL::NGinOGL() : NGin() {
    
    coordinator = std::unique_ptr<ResourceCoordinator>(new ResourceCoordinatorOGL());
//    deferredRenderer = std::unique_ptr<Render>(new DeferredRenderOGL());
	
}

void ms::NGinOGL::draw_scene() {
//    deferredRenderer->draw_scene();
}

void ms::NGinOGL::initialize () {
	
}

bool ms::NGinOGL::is_loaded () {
	return true;
}

void ms::NGinOGL::destroy () {
	
}
