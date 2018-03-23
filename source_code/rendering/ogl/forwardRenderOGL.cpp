//
//  forwardRenderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "forwardRenderOGL.hpp"

ms::ForwardRenderOGL::ForwardRenderOGL (unsigned int maxAOL, std::string vSS, std::string fSS, std::shared_ptr<Framebuffer> framebuffer) :
	ms::ForwardRender(maxAOL, framebuffer, vSS, fSS) {
		shader = std::unique_ptr<ForwardShader>(new ForwardShaderOGL(maxAOL, vSS, fSS));
}

void ms::ForwardRenderOGL::use () {
	if(!shader->is_loaded()) {
		shader->load();
	}
	
	shader->use();
	
	mglEnable(GL_DEPTH_TEST);
	mglDepthFunc(GL_LEQUAL);
}

void ms::ForwardRenderOGL::clear_frame () {
	framebuffer->use();
	framebuffer->clear_frame();
}

void ms::ForwardRenderOGL::draw (Drawable * node, const Scene * scene) {
	ForwardRender::draw(node, scene);
	mglDrawElements(GL_TRIANGLES, node->geometry->amount_of_indices(), GL_UNSIGNED_INT, nullptr);
}

void ms::ForwardRenderOGL::load () {
	this->shader->load();
	Resource::load();
}

bool ms::ForwardRenderOGL::is_loaded () {
	return this->shader->is_loaded();
}

void ms::ForwardRenderOGL::unload ()  {
	this->shader->unload();
	Resource::unload();
}
