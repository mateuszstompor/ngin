//
//  nginOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "nginOGL.hpp"

ms::NGinOGL::NGinOGL() : NGin() {
    
//    deferredRenderer = std::unique_ptr<Render>(new DeferredRenderOGL());
	
}

void ms::NGinOGL::draw_scene() {
//	deferredRenderer->clear_frame();
    deferredRenderer->draw_scene(scene);
}

void ms::NGinOGL::unload () {
	NGin::unload();
}
