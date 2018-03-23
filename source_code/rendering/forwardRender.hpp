//
//  forwardRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef forward_render_hpp
#define forward_render_hpp

#include <memory>

#include "render.hpp"
#include "shaders/forwardShader.hpp"
#include "shaders/lightSourceDrawerShader.hpp"

namespace ms {
	
	class ForwardRender : public Render {
	
	public:
		
		inline 						ForwardRender					(unsigned int 					maximalAmountOfLights,
																	std::shared_ptr<Framebuffer> 	framebuffer,
																	std::string 					vertexShSource,
																	std::string 					fragmentShSource);
		inline virtual std::string 	get_class						() = 0;
		inline virtual void 		draw  							(Drawable * node, const Scene * scene) 	override;
		inline virtual void 		setup_uniforms					(const Scene * scene) 					;
		inline virtual void 		setup_material_uniforms_for		(const Scene * scene, const Drawable * node);
		
		virtual 					~ForwardRender() = default;
	
	protected:
		
		std::string 							vertexShaderSource;
		std::string 							fragmentShaderSource;
		unsigned int 							maximalAmountOfLights;
		
		std::unique_ptr<ForwardShader>			shader;
		
	};
	
}

ms::ForwardRender::ForwardRender(unsigned int maxAOL,
								 std::shared_ptr<Framebuffer> framebuffer,
								 std::string vertexShSource,
								 std::string fragmentShSource
								 ) : 	Render(framebuffer),
										vertexShaderSource(vertexShSource),
										fragmentShaderSource(fragmentShSource),
										maximalAmountOfLights(maxAOL),
										shader(nullptr) { }


void ms::ForwardRender::setup_uniforms (const Scene * scene) {
	shader->set_camera_transformation(scene->get_camera().get_transformation());
	shader->set_projection_matrix(scene->get_camera().get_projection_matrix());
	
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
	
}

void ms::ForwardRender::setup_material_uniforms_for(const Scene * scene, const Drawable * node) {
	auto material = scene->materials.find(node->geometry->get_material_name());
	
	if (material != scene->materials.end()) {
		shader->set_has_material(true);
		
		if(material->second->diffuseTexturesNames.size() > 0) {
			
			auto textureIt = scene->textures.find(material->second->diffuseTexturesNames[0]);
			
			if ( textureIt != scene->textures.end()) {
				shader->bind_diffuse_texture(*textureIt->second);
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
				shader->bind_specular_texture(*textureIt->second);
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
}

void ms::ForwardRender::draw (Drawable * node, const Scene * scene) {
	
	shader->set_model_transformation(node->modelTransformation.get_transformation());
	node->use();
	ForwardRender::setup_material_uniforms_for(scene, node);
	node->geometry->use_indicies();
	
}

std::string ms::ForwardRender::get_class () {
	return "ms::ForwardRender";
}


#endif /* forward_render_hpp */
