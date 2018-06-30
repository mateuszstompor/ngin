//
//  PostprocessRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "postprocessRender.hpp"

ms::PostprocessRender::PostprocessRender(std::vector<std::weak_ptr<Texture2D>>      input,
                                         std::unique_ptr<Framebuffer> &&            framebuffer,
                                         std::unique_ptr<Shader> &&                 shaderProgram) :    Render(std::move(framebuffer),
                                                                                                        std::move(shaderProgram)),
                                                                                                        inputTextures(input),
                                                                                                        quad(Drawable::get_quad()),
                                                                                                        isOn{true} {}


void ms::PostprocessRender::_load () {
    Render::_load();
    quad->load();
}

void ms::PostprocessRender::_unload () {
    Render::_unload();
    quad->unload();
    quad->get_geometry()->unload();
}

void ms::PostprocessRender::set_enabled (bool state) {
    isOn = state;
}

void ms::PostprocessRender::draw() const {
	
	{
		for(int i = 0; i < inputTextures.size(); ++i) {
            shader->set_uniform("isOn", isOn ? 1 : 0);
			shader->set_uniform("in" + std::to_string(i), i);
            shader->bind_texture(i, *(inputTextures[i].lock()));
		}
	}
	
	quad->draw();
	
}

std::string ms::PostprocessRender::get_class () const {
	return "ms::PostprocessRender";
}
