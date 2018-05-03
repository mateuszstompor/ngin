//
//  render.cpp
//  ngin
//
//  Created by Mateusz Stompór on 23/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "render.hpp"

ms::Render::Render(std::unique_ptr<Framebuffer> && framebuffer, std::unique_ptr<Shader> && shader) : framebuffer(std::move(framebuffer)), shader(std::move(shader)) { }

std::string ms::Render::get_class () const {
	return "ms::Render";
}

void ms::Render::_load () {
    shader->load();
    if (framebuffer) { framebuffer->load(); }
}

void ms::Render::use () {
    shader->load();
	
	shader->use();
	framebuffer->use();
}

void ms::Render::use (Framebuffer & fb) {
    shader->load();
    shader->use();
    fb.use();
}

void ms::Render::_unload ()  {
	shader->unload();
    if ( framebuffer ) { framebuffer->unload(); }
}
