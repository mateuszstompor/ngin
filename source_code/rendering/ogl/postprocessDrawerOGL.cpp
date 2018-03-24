//
//  postprocessDrawerOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "postprocessDrawerOGL.hpp"

ms::PostprocessDrawerOGL::PostprocessDrawerOGL(std::vector<std::shared_ptr<Texture>> 	input,
											   std::shared_ptr<Framebuffer> 			framebuffer,
											   std::unique_ptr<Shader>					shaderProgram) : PostprocessDrawer(input,
																														   framebuffer,
																														   std::move(shaderProgram)) {
	quad = DrawableOGL::get_quad();
}

void ms::PostprocessDrawerOGL::draw_quad() {
	framebuffer->use();
	shader->use();
	
	{
		ShaderOGL* shad = dynamic_cast<ShaderOGL*>(shader.get());
		for(int i = 0; i < inputTextures.size(); ++i) {
			shad->set_uniform("in" + std::to_string(i), i);
			shader->bind_texture(i, *(inputTextures[i]));
		}
	}
	
	quad->draw();
	
}

void ms::PostprocessDrawerOGL::draw (Drawable * node, const Scene * scene) {
	assert(false);
}

std::string ms::PostprocessDrawerOGL::get_class () {
	return "ms::PostprocessDrawerOGL";
}
