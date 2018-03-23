//
//  lightSourceRenderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 19/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "lightSourceRenderOGL.hpp"


namespace ms {
	typedef unsigned int ui;
}

ms::LightSourceRenderOGL::LightSourceRenderOGL (std::string vSS, std::string fSS, std::shared_ptr<Framebuffer> framebuffer) :
ms::LightSourcesRender(framebuffer, vSS, fSS) {
	shader = std::unique_ptr<LightSourceDrawerShader>(new LightSourceDrawerShaderOGL(vSS, fSS));
}

void ms::LightSourceRenderOGL::use () {
	if(!shader->is_loaded()) {
		shader->load();
	}
	
	shader->use();
	
	framebuffer->use();
	mglEnable(GL_DEPTH_TEST);
	mglDepthFunc(GL_LEQUAL);
}

void ms::LightSourceRenderOGL::clear_frame () {
	framebuffer->use();
	framebuffer->clear_frame();
}

void ms::LightSourceRenderOGL::draw (Drawable * node, const Scene * scene) {
	
	shader->set_camera_transformation(scene->get_camera().get_transformation());
	shader->set_projection_matrix(scene->get_camera().get_projection_matrix());
	shader->set_model_transformation(node->modelTransformation.get_transformation());
	auto pointLight = dynamic_cast<PointLight*>(node);
	
	if(pointLight) {
		shader->set_light_source_color(pointLight->color.c_array());
	} else {
		shader->set_light_source_color(ms::math::vec3{0.0f, 1.0f, 0.0f}.c_array());
	}
	
	node->use();
	
	node->geometry->use_indicies();
	mglDrawElements(GL_TRIANGLES, node->geometry->amount_of_indices(), GL_UNSIGNED_INT, nullptr);
	
}

void  ms::LightSourceRenderOGL::setup_uniforms (const Scene * scene) {
	
}

void ms::LightSourceRenderOGL::load () {
	this->shader->load();
	Resource::load();
}

bool ms::LightSourceRenderOGL::is_loaded () {
	return this->shader->is_loaded();
}

void ms::LightSourceRenderOGL::unload ()  {
	this->shader->unload();
	Resource::unload();
}
