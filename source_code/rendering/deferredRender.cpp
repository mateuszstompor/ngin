//
//  deferredRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 11/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredRender.hpp"

ms::DeferredRender::DeferredRender(unsigned int                     maxPointLightsAmount,
                                   unsigned int                     maxSpotLightsAmount,
								   std::unique_ptr<Framebuffer> &&  framebuffer,
                                   std::unique_ptr<Shader> &&       gShader,
                                   std::unique_ptr<Shader> &&       lightingShader) :
ModelRender{std::move(framebuffer), std::move(gShader)},
maxPointLightsAmount{maxPointLightsAmount},
maxSpotLightsAmount{maxSpotLightsAmount},
lightingShader{std::move(lightingShader)},
quad{Drawable::get_quad()} { }

void ms::DeferredRender::draw (Drawable & node, math::mat4 const & transformation) {
    auto material = node.get_material();
    if (material) {
        if(auto diff = material->get_diffuse_texture()) {
            shader->bind_texture(0, *diff);
            shader->set_uniform("hasDiffuseTexture", 1);
        } else {
            shader->set_uniform("hasDiffuseTexture", 0);
        }
        
        if(auto spec = material->get_specular_texture()) {
            shader->bind_texture(1, *spec);
            shader->set_uniform("hasSpecularTexture", 1);
        } else {
            shader->set_uniform("hasSpecularTexture", 0);
        }
        
        if(auto normal = material->get_height_texture()) {
            shader->bind_texture(2, *normal);
            shader->set_uniform("hasNormalTexture", 1);
        } else {
            shader->set_uniform("hasNormalTexture", 0);
        }
    }
    set_material(material);
    shader->set_uniform("modelTransformation", transformation);
    node.draw();
}

void ms::DeferredRender::draw (Drawable & node) {
    draw(node, node.get_transformation());
}

void ms::DeferredRender::set_material (Material * material) {
    if (material) {
        shader->set_uniform("hasMaterial", 1);
        material->use();
    } else {
        shader->set_uniform("hasMaterial", 0);
    }
}

void ms::DeferredRender::set_spot_lights (std::vector<SpotLight> const & spotLights) {
    lightingShader->set_uniform("spotLightsAmount", static_cast<int>(spotLights.size()));
    for(auto i {0}; i < spotLights.size(); ++i) {
        lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].power", spotLights[i].get_power());
        lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].color", spotLights[i].get_color());
        lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].angleDegrees", spotLights[i].get_angle_degrees());
        lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].transformation", spotLights[i].get_transformation());
        lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].projection", spotLights[i].get_frustum().get_projection_matrix());
        lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].castsShadow", spotLights[i].casts_shadow() == true ? int{1} : int{0});
    }
}

void ms::DeferredRender::set_point_lights (std::vector<PointLight> const & pointLights) {
    lightingShader->set_uniform("pointLightsAmount", static_cast<int>(pointLights.size()));
    for(auto i {0}; i < pointLights.size(); ++i) {
        lightingShader->set_uniform("pointLights[" + std::to_string(i) + "].color", pointLights[i].get_color());
        lightingShader->set_uniform("pointLights[" + std::to_string(i) + "].power", pointLights[i].get_power());
        lightingShader->set_uniform("pointLights[" + std::to_string(i) + "].castsShadow", pointLights[i].casts_shadow() == true ? int{1} : int{0});
        lightingShader->set_uniform("pointLights[" + std::to_string(i) + "].projection", pointLights[i].get_projection());
        lightingShader->set_uniform("pointLights[" + std::to_string(i) + "].transformation", pointLights[i].get_transformation());
    }
}

void ms::DeferredRender::set_directionallight (DirectionalLight const * directionalLight) {
    if(directionalLight) {
        lightingShader->set_uniform("hasDirLight", 1);
        lightingShader->set_uniform("dirLight.color", directionalLight->get_color());
        lightingShader->set_uniform("dirLight.power", directionalLight->get_power());
        lightingShader->set_uniform("dirLight.direction", math::back(directionalLight->get_transformation()));
        lightingShader->set_uniform("dirLightProjection", directionalLight->get_projection());
        lightingShader->set_uniform("dirLightTransformation", directionalLight->get_transformation());
        lightingShader->set_uniform("hasDirLightShadowMap", 1);
    } else {
        lightingShader->set_uniform("hasDirLight", 0);
        // check, it should not be necessary
        lightingShader->set_uniform("hasDirLightShadowMap", 0);
    }
}

void ms::DeferredRender::set_camera (Camera const & camera) {
    shader->set_uniform("cameraTransformation", camera.get_transformation());
    shader->set_uniform("perspectiveProjection", camera.get_projection_matrix());
}

void ms::DeferredRender::use () {
	if(!shader->is_loaded() || !lightingShader->is_loaded()) {
		shader->load();
		lightingShader->load();
        
	}
	shader->use();
	gFramebuffer->use();
}

void ms::DeferredRender::_load () {
    
    auto gPosition = std::make_unique<Texture2D>(texture::Format::rgb_16_16_16, texture::AssociatedType::FLOAT,
                                                 framebuffer->get_width(), framebuffer->get_height());
    
    auto gNormal = std::make_unique<Texture2D>(texture::Format::rgb_16_16_16, texture::AssociatedType::FLOAT,
                                               framebuffer->get_width(), framebuffer->get_height());
    
    auto gAlbedo = std::make_unique<Texture2D>(texture::Format::rgba_8_8_8_8, texture::AssociatedType::UNSIGNED_BYTE,
                                               framebuffer->get_width(), framebuffer->get_height());
    
    auto depthRenderbuffer = std::unique_ptr<Renderbuffer>(new Renderbuffer(texture::Format::depth_32, texture::AssociatedType::FLOAT, 0,
                                                                            framebuffer->get_width(), framebuffer->get_height()));
    
    gFramebuffer = std::unique_ptr<Framebuffer>(new Framebuffer{3, 1, framebuffer->get_width(), framebuffer->get_height()});
    gFramebuffer->bind_color_buffer(0, std::move(gPosition));
    gFramebuffer->bind_color_buffer(1, std::move(gNormal));
    gFramebuffer->bind_color_buffer(2, std::move(gAlbedo));
    gFramebuffer->bind_depth_buffer(std::move(depthRenderbuffer));
    gFramebuffer->configure();
    gFramebuffer->load();
    shader->load();
    lightingShader->load();
    framebuffer->use();
    quad->load();
    shader->use();
    shader->set_uniform("diffuseTexture", 0);
    shader->set_uniform("specularTexture", 1);
    shader->set_uniform("normalTexture", 2);
    lightingShader->use();
    lightingShader->set_uniform("gPosition", 0);
    lightingShader->set_uniform("gNormal", 1);
    lightingShader->set_uniform("gAlbedo", 2);
    lightingShader->set_uniform("dirLightShadowMap", 3);
    lightingShader->set_uniform("spotLightsShadowMaps", 4);
    lightingShader->set_uniform("pointLightShadow", 5);
    
}

void ms::DeferredRender::_unload () {
    gFramebuffer->unload();
    lightingShader->unload();
    quad->unload();
    quad->get_geometry()->unload();
    Render::_unload();
}

void ms::DeferredRender::perform_light_pass (const Scene & scene) {
    lightingShader->use();
    lightingShader->set_uniform("cameraTransformation", scene.get_camera().get_transformation());
    set_directionallight(scene.get_directional_light());
    set_spot_lights(scene.get_spot_lights());
    set_point_lights(scene.get_point_lights());
    lightingShader->bind_texture(0, *gFramebuffer->get_colors()[0].lock());
    lightingShader->bind_texture(1, *gFramebuffer->get_colors()[1].lock());
    lightingShader->bind_texture(2, *gFramebuffer->get_colors()[2].lock());
    quad->draw();
    framebuffer->copy_depth_from(*gFramebuffer);
}



