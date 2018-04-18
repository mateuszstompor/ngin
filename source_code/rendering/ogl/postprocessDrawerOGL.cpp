//
//  postprocessDrawerOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "postprocessDrawerOGL.hpp"

ms::PostprocessDrawerOGL::PostprocessDrawerOGL(std::vector<std::weak_ptr<Texture>> 	input,
											   std::unique_ptr<Framebuffer> && framebuffer,
											   std::unique_ptr<Shader> && shaderProgram) : PostprocessDrawer(input,
                                                                                                             std::move(framebuffer),
                                                                                                             std::move(shaderProgram)) {
	quad = DrawableOGL::get_quad();
}

void ms::PostprocessDrawerOGL::_load () {
    PostprocessDrawer::_load();
    quad->load();
}

void ms::PostprocessDrawerOGL::_unload () {
    PostprocessDrawer::_unload();
    quad->unload();
}

void ms::PostprocessDrawerOGL::draw_quad() const {
	framebuffer->use();
	shader->use();
	
	{
		ShaderOGL* shad = dynamic_cast<ShaderOGL*>(shader.get());
		for(int i = 0; i < inputTextures.size(); ++i) {
			shad->set_uniform("in" + std::to_string(i), i);
            shader->bind_texture(i, *(inputTextures[i].lock()));
		}
	}
	
	quad->draw();
	
}

std::string ms::PostprocessDrawerOGL::get_class () const {
	return "ms::PostprocessDrawerOGL";
}
