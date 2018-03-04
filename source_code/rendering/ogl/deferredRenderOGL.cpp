//
//  deferredRenderOGL.mm
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredRenderOGL.hpp"

ms::DeferredRenderOGL::DeferredRenderOGL (std::shared_ptr<Shader> shader) : gbufferShader(shader) {
	
}

void ms::DeferredRenderOGL::use () {
	
}

void ms::DeferredRenderOGL::clear_frame () {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMask(0.0f, 0.0f, 0.0f, 1.0f);
}

void ms::DeferredRenderOGL::draw_scene (const std::shared_ptr<Scene> &scene) {
	
	std::shared_ptr<SceneNode> n = *scene->nodes.begin();
	if(!n->is_loaded()) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glViewport(0, 0, 1200, 800);
		n->load();
	}
	n->use();
	if(this->gbufferShader) {
		if(!this->gbufferShader->is_loaded()) {
			this->gbufferShader->load();
		}
		
		this->gbufferShader->use();
		n->use();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
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
