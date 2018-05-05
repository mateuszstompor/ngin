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
                                 std::unique_ptr<Shader> && shader) : ModelRender(std::move(framebuffer), std::move(shader)), maximalAmountOfLights{maxAOL} {}

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

void ms::ForwardRender::draw (Drawable & node) {
    if(auto diff = node.boundedDiffuseTexture.lock()) {
        shader->bind_texture(0, *diff);
        shader->set_uniform("hasDiffuseTexture", 1);
    } else {
        shader->set_uniform("hasDiffuseTexture", 0);
    }
    
    if(auto spec = node.boundedSpecularTexture.lock()) {
        shader->bind_texture(1, *spec);
        shader->set_uniform("hasSpecularTexture", 1);
    } else {
        shader->set_uniform("hasSpecularTexture", 0);
    }
    
    if(auto normal = node.boundedHeightTexture.lock()) {
        shader->bind_texture(2, *normal);
        shader->set_uniform("hasNormalTexture", 1);
    } else {
        shader->set_uniform("hasNormalTexture", 0);
    }
    shader->set_uniform("modelTransformation", node.transformation);
    node.draw();
}

void ms::ForwardRender::set_material (Material * material) {
    if(material != nullptr) {
        shader->set_uniform("hasMaterial", 1);
        material->use();
    } else {
        shader->set_uniform("hasMaterial", 0);
    }
}

void ms::ForwardRender::set_spot_lights (std::vector<SpotLight> const & spotLights) {
    shader->set_uniform("spotLightsAmount", static_cast<int>(spotLights.size()));
    for(unsigned int i = 0; i < spotLights.size(); ++i) {
        shader->set_uniform("spotLights[" + std::to_string(i) + "].power", spotLights[i].get_power());
        shader->set_uniform("spotLights[" + std::to_string(i) + "].color", spotLights[i].get_color());
        shader->set_uniform("spotLights[" + std::to_string(i) + "].angleDegrees", spotLights[i].get_angle_degrees());
        shader->set_uniform("spotLights[" + std::to_string(i) + "].transformation", spotLights[i].get_transformation());
        shader->set_uniform("spotLights[" + std::to_string(i) + "].projection", spotLights[i].get_frustum().get_projection_matrix());
        shader->set_uniform("spotLights[" + std::to_string(i) + "].castsShadow", spotLights[i].casts_shadow() == true ? int{1} : int{0});
    }
}

void ms::ForwardRender::set_point_lights (std::vector<PointLight> const & pointLights) {
    shader->set_uniform("pointLightsAmount", static_cast<int>(pointLights.size()));
    for(unsigned int i = 0; i < pointLights.size(); ++i) {
        shader->set_uniform("pointLights[" + std::to_string(i) + "].color", pointLights[i].get_color());
        shader->set_uniform("pointLights[" + std::to_string(i) + "].position", math::get_position(pointLights[i].get_transformation()));
        shader->set_uniform("pointLights[" + std::to_string(i) + "].power", pointLights[i].get_power());
    }
}

void ms::ForwardRender::set_directionallight (DirectionalLight const * directionalLight) {
    if(directionalLight) {
        shader->set_uniform("hasDirLight", 1);
        shader->set_uniform("dirLight.color", directionalLight->get_color());
        shader->set_uniform("dirLight.direction", math::back(directionalLight->get_transformation()));
        shader->set_uniform("dirLightProjection", directionalLight->get_projection());
        shader->set_uniform("dirLightTransformation", directionalLight->get_transformation());
        shader->set_uniform("hasDirLightShadowMap", 1);
    } else {
        shader->set_uniform("hasDirLightShadowMap", 0);
        shader->set_uniform("hasDirLight", 0);
    }
}

void ms::ForwardRender::set_camera (Camera const & camera) {
    shader->set_uniform("cameraTransformation", camera.get_transformation());
    shader->set_uniform("perspectiveProjection", camera.get_projection_matrix());
}

std::string ms::ForwardRender::get_class () const {
	return "ms::ForwardRender";
}

