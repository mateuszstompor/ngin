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

ms::ForwardRenderOGL::ForwardRenderOGL (std::string vSS, std::string fSS, ui sW, ui sH, ui fbW, ui fbH) :
	ms::ForwardRender(sW, sH, fbW, fbH, vSS, fSS) {
		shader = std::unique_ptr<ForwardShader>(new ForwardShaderOGL(vSS, fSS));
}

void ms::ForwardRenderOGL::use () {
	if(!shader->is_loaded()) {
		shader->load();
	}
	
	shader->use();
	
	mglViewport(0, 0, screenWidth, screenHeight);
	mglEnable(GL_DEPTH_TEST);
	mglDepthFunc(GL_LEQUAL);
}

void ms::ForwardRenderOGL::clear_frame () {
	mglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mglClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

void ms::ForwardRenderOGL::draw (Drawable * node, const Scene * scene) {
	
	shader->set_camera_transformation(scene->get_camera().get_transformation());
	shader->set_projection_matrix(scene->get_camera().get_projection_matrix());
	shader->set_model_transformation(node->modelTransformation.get_transformation());

	node->use();
	
	if(auto dirLight = scene->get_directional_light()) {
		shader->set_has_directional_light(true);
		shader->set_directional_light_dir(dirLight->direction);
		shader->set_directional_light_color(dirLight->color);
	} else {
		shader->set_has_directional_light(false);
	}
	
	{
		const std::vector<std::shared_ptr<SpotLight>> & spotLights = scene->get_spot_lights();
		shader->set_amount_of_spot_lights(static_cast<int>(spotLights.size()));
		for(unsigned int index = 0; index < spotLights.size(); ++index) {
			shader->set_spot_light_power(index, spotLights[index]->power);
			shader->set_spot_light_color(index, spotLights[index]->color);
			shader->set_spot_light_position(index, spotLights[index]->position);
			shader->set_spot_light_angle(index, spotLights[index]->lightingAngleDegrees);
			shader->set_spot_light_direction(index, spotLights[index]->direction);
		}
	}
	
	{
		const std::vector<std::shared_ptr<PointLight>> & pointLights = scene->get_point_lights();
		shader->set_amount_of_point_lights(static_cast<int>(pointLights.size()));
		for(unsigned int index = 0; index < pointLights.size(); ++index) {
			shader->set_point_light_color(index, pointLights[index]->color);
			shader->set_point_light_power(index, pointLights[index]->power);
			shader->set_point_light_position(index, pointLights[index]->position);
		}
	}
	
	auto material = scene->materials.find(node->geometry->get_material_name());
	
	if (material != scene->materials.end()) {
		shader->set_has_material(true);
		
		if(material->second->diffuseTexturesNames.size() > 0) {

			auto textureIt = scene->textures.find(material->second->diffuseTexturesNames[0]);

			if ( textureIt != scene->textures.end()) {
				mglActiveTexture(GL_TEXTURE0);
				textureIt->second->use();
				shader->set_has_diffuse_texture(true);
			} else {
				shader->set_has_diffuse_texture(false);
			}

		} else {
			shader->set_has_diffuse_texture(false);
		}

		if(material->second->specularTexturesNames.size() > 0) {
			auto textureIt = scene->textures.find(material->second->specularTexturesNames[0]);
			if ( textureIt != scene->textures.end()) {
				mglActiveTexture(GL_TEXTURE1);
				textureIt->second->use();
				shader->set_has_specular_texture(true);

			} else {
				shader->set_has_specular_texture(false);
			}

		} else {
			shader->set_has_specular_texture(false);
		}
		
		
		shader->set_material_ambient_color(material->second->ambientColor);
		shader->set_material_diffuse_color(material->second->diffuseColor);
		shader->set_material_specular_color(material->second->specularColor);
		shader->set_material_opacity(material->second->opacity);
		shader->set_material_shininess(material->second->shininess);
	} else {
		shader->set_has_material(false);
	}
	
	node->geometry->use_indicies();

	mglDrawElements(GL_TRIANGLES, node->geometry->amount_of_indices(), GL_UNSIGNED_INT, nullptr);
	
}

void  ms::ForwardRenderOGL::setup_uniforms (const Scene * scene) {
	
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
