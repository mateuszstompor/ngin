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
	ms::ForwardRender(sW, sH, fbW, fbH, vSS, fSS) {
		shader = std::shared_ptr<ForwardShader>(new ForwardShaderOGL(vSS, fSS));
}

void ms::ForwardRenderOGL::use () {
	if(!shader->is_loaded()) {
		shader->load();
	}
	
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void ms::ForwardRenderOGL::clear_frame () {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

void ms::ForwardRenderOGL::draw_scene (const std::shared_ptr<Scene> & scene) {
	shader->use();
	
	shader->set_camera_transformation(scene->get_camera()->get_transformation());
	shader->set_projection_matrix(scene->get_camera()->get_projection_matrix());
	
	if(auto & dirLight = scene->get_directional_light()) {
		shader->set_has_directional_light(true);
		shader->set_directional_light_dir(dirLight->direction);
		shader->set_directional_light_color(dirLight->color);
	} else {
		shader->set_has_directional_light(false);
	}
	
	for (const std::shared_ptr<SceneNode> & node : scene->nodes) {
		node->use();
		
		shader->set_model_transformation(node->modelTransformation.get_transformation());
		
		if (auto material = node->material) {
			shader->set_has_material(true);
			shader->set_material_ambient_color(material->ambientColor);
			shader->set_material_diffuse_color(material->diffuseColor);
			shader->set_material_specular_color(material->specularColor);
			shader->set_material_opacity(material->opacity);
			shader->set_material_shininess(material->shininess);
		} else {
			shader->set_has_material(false);
		}
		
		this->shader->use();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
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
