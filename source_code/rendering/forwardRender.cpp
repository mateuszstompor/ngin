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
                                 std::unique_ptr<Shader> && shader) : Render(std::move(framebuffer), std::move(shader)), maximalAmountOfLights{maxAOL} {}

void ms::ForwardRender::_load () {
    Render::_load();
    shader->use();
    shader->set_uniform("diffuseTexture", 0);
    shader->set_uniform("specularTexture", 1);
    shader->set_uniform("normalTexture", 2);
    shader->set_uniform("dirLightShadowMap", 3);
    
    for (int i = 0; i < maximalAmountOfLights; ++i) {
        shader->set_uniform("spotLightsShadowMaps[" + std::to_string(i) + "]", 4 + i);
    }
    
}

void ms::ForwardRender::setup_uniforms (const Scene * scene) {
    
    shader->set_uniform("cameraTransformation", scene->get_camera().get_transformation());
    shader->set_uniform("perspectiveProjection", scene->get_camera().get_projection_matrix());

    if(auto dirLight = scene->get_directional_light()) {
        shader->set_uniform("hasDirLight", 1);
        shader->set_uniform("dirLight.color", dirLight->get_color());
        shader->set_uniform("dirLight.direction", math::back(dirLight->get_transformation()));
        shader->set_uniform("dirLightProjection", scene->get_directional_light()->get_projection());
        shader->set_uniform("dirLightTransformation", scene->get_directional_light()->get_transformation());
        shader->set_uniform("hasDirLightShadowMap", 1);
    } else {
        shader->set_uniform("hasDirLightShadowMap", 0);
        shader->set_uniform("hasDirLight", 0);
    }

    {
        const auto & spotLights = scene->get_spot_lights();
        shader->set_uniform("spotLightsAmount", static_cast<int>(spotLights.size()));
        for(unsigned int i = 0; i < spotLights.size(); ++i) {
            shader->set_uniform("spotLights[" + std::to_string(i) + "].power", spotLights[i].get_power());
            shader->set_uniform("spotLights[" + std::to_string(i) + "].color", spotLights[i].get_color());
            shader->set_uniform("spotLights[" + std::to_string(i) + "].angleDegrees", spotLights[i].get_angle_degrees());
            shader->set_uniform("spotLights[" + std::to_string(i) + "].position", math::get_position(spotLights[i].get_transformation()));
            shader->set_uniform("spotLights[" + std::to_string(i) + "].direction", math::back(spotLights[i].get_transformation()));
            shader->set_uniform("spotLightsProjections[" + std::to_string(i) + "]", spotLights[i].get_projection());
            shader->set_uniform("spotLightsToLightTransformations[" + std::to_string(i) + "]", spotLights[i].get_transformation());
        }
    }

    {
        const auto & pointLights = scene->get_point_lights();
        shader->set_uniform("pointLightsAmount", static_cast<int>(pointLights.size()));
        for(unsigned int i = 0; i < pointLights.size(); ++i) {
            shader->set_uniform("pointLights[" + std::to_string(i) + "].color", pointLights[i].get_color());
            shader->set_uniform("pointLights[" + std::to_string(i) + "].position", math::get_position(pointLights[i].get_transformation()));
            shader->set_uniform("pointLights[" + std::to_string(i) + "].power", pointLights[i].get_power());
        }
    }
    
}

void ms::ForwardRender::setup_material_uniforms_for(const Scene * scene, const Drawable * node) {

    auto & shader = *this->shader;
    
	if (auto mat = node->boundedMaterial.lock()) {
        shader.set_uniform("hasMaterial", 1);
        auto & material = *mat;
		material.use();
		
		if(auto diff = material.boundedDiffuseTexture.lock()) {
            shader.bind_texture(0, *diff);
            shader.set_uniform("hasDiffuseTexture", 1);
		} else {
            shader.set_uniform("hasDiffuseTexture", 0);
		}
		
		if(auto spec = material.boundedSpecularTexture.lock()) {
            shader.bind_texture(1, *spec);
            shader.set_uniform("hasSpecularTexture", 1);
		} else {
            shader.set_uniform("hasSpecularTexture", 0);
		}
        
        if(auto normal = material.boundedHeightTexture.lock()) {
            shader.bind_texture(2, *normal);
            shader.set_uniform("hasNormalTexture", 1);
        } else {
            shader.set_uniform("hasNormalTexture", 0);
        }
		
	} else {
		shader.set_uniform("hasMaterial", 0);
	}

}

void ms::ForwardRender::draw (Drawable & node, const Scene & scene) {
    shader->set_uniform("modelTransformation", node.transformation);
	ForwardRender::setup_material_uniforms_for(&scene, &node);
	node.draw();
}

std::string ms::ForwardRender::get_class () const {
	return "ms::ForwardRender";
}

