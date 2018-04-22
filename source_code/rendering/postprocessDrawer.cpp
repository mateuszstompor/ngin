//
//  postprocessDrawer.cpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "postprocessDrawer.hpp"

ms::PostprocessDrawer::PostprocessDrawer(std::vector<std::weak_ptr<Texture2D>>     input,
                                         std::unique_ptr<Framebuffer> && framebuffer,
                                         std::unique_ptr<Shader> && shaderProgram) :    Render(std::move(framebuffer),
                                                                                                std::move(shaderProgram)),
                                                                                        inputTextures(input),
                                                                                        quad(Drawable::get_quad()) {}


void ms::PostprocessDrawer::_load () {
    Render::_load();
    quad->load();
}

void ms::PostprocessDrawer::_unload () {
    Render::_unload();
    quad->unload();
}

void ms::PostprocessDrawer::draw_quad() const {
	
	{
		for(int i = 0; i < inputTextures.size(); ++i) {
			shader->set_uniform("in" + std::to_string(i), i);
            shader->bind_texture(i, *(inputTextures[i].lock()));
		}
	}
	
	quad->draw();
	
}

std::string ms::PostprocessDrawer::get_class () const {
	return "ms::PostprocessDrawer";
}
