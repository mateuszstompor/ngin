//
//  forwardRenderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "forwardRenderOGL.hpp"

namespace ms {
	typedef unsigned int ui;
}

ms::ForwardRenderOGL::ForwardRenderOGL (std::shared_ptr<std::string> vSS, std::shared_ptr<std::string> fSS, ui sW, ui sH, ui fbW, ui fbH) :
	ms::Render(sW, sH, fbW, fbH), fragmentShaderSource(fSS), vertexShaderSource(vSS) {
		shader = std::shared_ptr<Shader>(new ShaderOGL(vSS, nullptr, nullptr, nullptr, fSS));
}

void ms::ForwardRenderOGL::use () {
	if(!shader->is_loaded()) {
		shader->load();
	}
}

void ms::ForwardRenderOGL::clear_frame () {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

void ms::ForwardRenderOGL::draw_scene (const std::shared_ptr<Scene> &scene) {
	
	std::shared_ptr<SceneNode> n = *scene->nodes.begin();
	if(!n->is_loaded()) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glViewport(0, 0, screenWidth, screenHeight);
		n->load();
	}
	n->use();
	if(this->shader) {
		if(!this->shader->is_loaded()) {
			this->shader->load();
		}
		
		this->shader->use();
		n->use();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
}

void ms::ForwardRenderOGL::load () {
	
}

bool ms::ForwardRenderOGL::is_loaded () {
	return true;
}

void ms::ForwardRenderOGL::unload ()  {
	
}

ms::ForwardRenderOGL::~ForwardRenderOGL () {
	
}
