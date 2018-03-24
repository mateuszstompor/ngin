//
//  render.cpp
//  ngin
//
//  Created by Mateusz Stompór on 23/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "render.hpp"

ms::Render::Render(std::shared_ptr<Framebuffer> framebuffer, std::unique_ptr<Shader> shader) : framebuffer(framebuffer), shader(std::move(shader)) { }

std::string ms::Render::get_class () {
	return "ms::Render";
}

void ms::Render::clear_frame () {
	framebuffer->use();
	framebuffer->clear_frame();
}

void ms::Render::load () {
	if(!is_loaded()) {
		this->shader->load();
		Resource::load();
	}
}

void ms::Render::use () {
	if(!shader->is_loaded()) {
		shader->load();
	}
	
	shader->use();
	framebuffer->use();
}

void ms::Render::unload ()  {
	if(is_loaded()) {
		this->shader->unload();
		Resource::unload();
	}
}
