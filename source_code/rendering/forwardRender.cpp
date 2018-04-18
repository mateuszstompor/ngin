//
//  forwardRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 23/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "forwardRender.hpp"

ms::ForwardRender::ForwardRender(unsigned int maxAOL,
								 std::unique_ptr<Framebuffer> && framebuffer,
                                 std::unique_ptr<ForwardShader> && shader) : Render(std::move(framebuffer), std::move(shader)) {}



void ms::ForwardRender::setup_uniforms (const Scene * scene) {
	
	ForwardShader* shad = dynamic_cast<ForwardShader*>(shader.get());
	
	shad->set_camera_transformation(scene->get_camera().get_transformation());
	shad->set_projection_matrix(scene->get_camera().get_projection_matrix());
	
	if(auto dirLight = scene->get_directional_light()) {
		shad->set_has_directional_light(true);
		shad->set_directional_light_dir(dirLight->direction);
		shad->set_directional_light_color(dirLight->get_color());
	} else {
		shad->set_has_directional_light(false);
	}
	
	{
		const std::vector<std::shared_ptr<SpotLight>> & spotLights = scene->get_spot_lights();
		shad->set_amount_of_spot_lights(static_cast<int>(spotLights.size()));
		for(unsigned int index = 0; index < spotLights.size(); ++index) {
			shad->set_spot_light_power(index, spotLights[index]->power);
			shad->set_spot_light_color(index, spotLights[index]->get_color());
			shad->set_spot_light_position(index, spotLights[index]->position);
			shad->set_spot_light_angle(index, spotLights[index]->lightingAngleDegrees);
			shad->set_spot_light_direction(index, spotLights[index]->direction);
		}
	}
	
	{
		const std::vector<std::shared_ptr<PointLight>> & pointLights = scene->get_point_lights();
		shad->set_amount_of_point_lights(static_cast<int>(pointLights.size()));
		for(unsigned int index = 0; index < pointLights.size(); ++index) {
			shad->set_point_light_color(index, pointLights[index]->get_color());
			shad->set_point_light_power(index, pointLights[index]->power);
			shad->set_point_light_position(index, pointLights[index]->position);
		}
	}
	
}

void ms::ForwardRender::setup_material_uniforms_for(const Scene * scene, const Drawable * node) {

	ForwardShader* shad = dynamic_cast<ForwardShader*>(shader.get());

	if (auto mat = node->boundedMaterial.lock()) {
		shad->set_has_material(true);
		mat->use();
		
		if(auto diff = mat->boundedDiffuseTexture.lock()) {
            shad->bind_texture(0, *diff);
			shad->set_has_diffuse_texture(true);
		} else {
			shad->set_has_diffuse_texture(false);
		}
		
		if(auto spec = mat->boundedSpecularTexture.lock()) {
            shad->bind_texture(1, *spec);
			shad->set_has_specular_texture(true);
		} else {
			shad->set_has_specular_texture(false);
		}
        
        if(auto normal = mat->boundedHeightTexture.lock()) {
            //index of diffuse texture is 1
            shad->bind_texture(2, *normal);
            shad->set_has_normal_texture(true);
        } else {
            shad->set_has_normal_texture(false);
        }
		
	} else {
		shad->set_has_material(false);
	}

}

void ms::ForwardRender::draw (Drawable * node, const Scene * scene) {
	ForwardShader* shad = dynamic_cast<ForwardShader*>(shader.get());
	shad->set_model_transformation(node->modelTransformation.get_transformation());
	ForwardRender::setup_material_uniforms_for(scene, node);
	node->draw();
	
}

std::string ms::ForwardRender::get_class () const {
	return "ms::ForwardRender";
}

