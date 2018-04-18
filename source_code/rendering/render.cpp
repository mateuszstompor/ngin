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

void ms::Render::clear_frame () {
	framebuffer->use();
	framebuffer->clear_frame();
}

void ms::Render::_load () {
    shader->load();
    framebuffer->load();
}

void ms::Render::use () {
	if(!shader->is_loaded()) {
		shader->load();
	}
	
	shader->use();
	framebuffer->use();
}

void ms::Render::use (std::unique_ptr<Framebuffer> & fb) {
    if(!shader->is_loaded()) {
        shader->load();
    }
    
    shader->use();
    fb->use();
}

void ms::Render::_unload ()  {
	shader->unload();
    framebuffer->unload();
}
