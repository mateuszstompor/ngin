//
//  deferredRenderOGL.mm
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredRenderOGL.hpp"

void ms::DeferredRenderOGL::use () {
	
}

void ms::DeferredRenderOGL::clear_frame () {
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(0.0f, 0.0f, 0.0f, 1.0f);
}

void ms::DeferredRenderOGL::draw_scene (const std::unique_ptr<Scene> &scene) {
	
}

void ms::DeferredRenderOGL::load () {
	
}

bool ms::DeferredRenderOGL::is_loaded () {
	return true;
}

void ms::DeferredRenderOGL::unload ()  {
	
}

ms::DeferredRenderOGL::~DeferredRenderOGL () {
	
}
