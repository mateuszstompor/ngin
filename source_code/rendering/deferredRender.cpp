//
//  deferredRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 11/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredRender.hpp"

//	//	//	//	//	//	//	//	//	//	//	//
//		SETTINGS UNSIGNED INTEGER LAYOUT	//
//	//	//	//	//	//	//	//	//	//	//	//

// def. Najbardziej znaczący bit (ang. most significant bit, MSB), zwany też najstarszym bitem
// def. Najmniej znaczący bit (ang. least significant bit, LSB), zwany też najmłodszym bitem


namespace ms {

	#define RENDER_MODE_STANDARD	0
	#define RENDER_MODE_POSITION 	1
	#define RENDER_MODE_ALBEDO 		2
	#define RENDER_MODE_NORMALS 	3
	#define RENDER_MODE_SPECULAR 	4
	
}

ms::DeferredRender::DeferredRender(unsigned int                     maxPointLightsAmount,
                                   unsigned int                     maxSpotLightsAmount,
								   std::unique_ptr<Framebuffer> &&  framebuffer,
                                   std::unique_ptr<Shader> &&       gShader,
                                   std::unique_ptr<Shader> &&       lightingShader) :
Render{std::move(framebuffer), std::move(gShader)},
maxPointLightsAmount{maxPointLightsAmount},
maxSpotLightsAmount{maxSpotLightsAmount},
lightingShader{std::move(lightingShader)},
quad{Drawable::get_quad()},
renderMode{0},
debugMode{false},
debugType{DeferredRender::DebugType::position} {
    
    auto gPosition = std::make_unique<Texture>(Texture::Type::tex_2d,
                                               Texture::Format::rgb_16_16_16,
                                               Texture::AssociatedType::FLOAT,
                                               this->framebuffer->get_width(),
                                               this->framebuffer->get_height());

    auto gNormal = std::make_unique<Texture>(Texture::Type::tex_2d,
                                             Texture::Format::rgb_16_16_16,
                                             Texture::AssociatedType::FLOAT,
                                             this->framebuffer->get_width(), this->framebuffer->get_height());

    auto gAlbedo = std::make_unique<Texture>(Texture::Type::tex_2d,
                                             Texture::Format::rgba_8_8_8_8,
                                             Texture::AssociatedType::UNSIGNED_BYTE,
                                             this->framebuffer->get_width(),
                                             this->framebuffer->get_height());

    gFramebuffer = std::make_unique<Framebuffer>(3,
                                                 1,
                                                 this->framebuffer->get_width(),
                                                 this->framebuffer->get_height());

    auto depthRenderbuffer = std::make_unique<Renderbuffer>(Texture::Format::depth_32,
                                                            Texture::AssociatedType::FLOAT,
                                                            0,
                                                            this->framebuffer->get_width(),
                                                            this->framebuffer->get_height());

    gFramebuffer->bind_color_buffer(0, std::move(gPosition));
    gFramebuffer->bind_color_buffer(1, std::move(gNormal));
    gFramebuffer->bind_color_buffer(2, std::move(gAlbedo));
    gFramebuffer->bind_depth_buffer(std::move(depthRenderbuffer));

    gFramebuffer->configure();

}

void ms::DeferredRender::set_render_type (DebugType type) {
	switch (type) {
		case DeferredRender::DebugType::standard:
			renderMode = RENDER_MODE_STANDARD;
			break;
		case DeferredRender::DebugType::position:
			renderMode = RENDER_MODE_POSITION;
			break;
		case DeferredRender::DebugType::albedo:
			renderMode = RENDER_MODE_ALBEDO;
			break;
		case DeferredRender::DebugType::normals:
			renderMode = RENDER_MODE_NORMALS;
			break;
		case DeferredRender::DebugType::specular:
			renderMode = RENDER_MODE_SPECULAR;
			break;
		default:
			assert(false);
			//fatal error
			break;
	}
}

void ms::DeferredRender::setup_lightpass_uniforms (const Scene * scene) {
    lightingShader->set_uniform("cameraTransformation", scene->get_camera().get_transformation());

    
    if(auto dirLight = scene->get_directional_light()) {
        lightingShader->set_uniform("hasDirLight", 1);
        lightingShader->set_uniform("dirLight.color", dirLight->get_color());
        lightingShader->set_uniform("dirLight.direction", dirLight->get_direction());
    } else {
        lightingShader->set_uniform("hasDirLight", 0);
    }
    
    {
        const std::vector<std::shared_ptr<SpotLight>> & spotLights = scene->get_spot_lights();
        lightingShader->set_uniform("spotLightsAmount", static_cast<int>(spotLights.size()));
        for(unsigned int i = 0; i < spotLights.size(); ++i) {
            
            lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].power", spotLights[i]->get_power());
            lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].color", spotLights[i]->get_color());
            lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].angleDegrees", spotLights[i]->lightingAngleDegrees);
            lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].position", spotLights[i]->get_position());
            lightingShader->set_uniform("spotLights[" + std::to_string(i) + "].direction", spotLights[i]->get_direction());
            
        }
    }
    
    {
        const std::vector<std::shared_ptr<PointLight>> & pointLights = scene->get_point_lights();
        shader->set_uniform("pointLightsAmount", static_cast<int>(pointLights.size()));
        for(unsigned int i = 0; i < pointLights.size(); ++i) {
            shader->set_uniform("pointLights[" + std::to_string(i) + "].color", pointLights[i]->get_color());
            shader->set_uniform("pointLights[" + std::to_string(i) + "].position", pointLights[i]->get_position());
            shader->set_uniform("pointLights[" + std::to_string(i) + "].power", pointLights[i]->get_power());
        }
    }
	
}

void ms::DeferredRender::draw (Drawable & node, const Scene & scene) {
    shader->set_uniform("modelTransformation", node.modelTransformation.get_transformation());
    DeferredRender::setup_material_uniforms(&scene, &node);
    node.draw();
}

void ms::DeferredRender::use () {
	if(!shader->is_loaded() || !lightingShader->is_loaded()) {
		shader->load();
		lightingShader->load();
        
	}
	
	shader->use();
	gFramebuffer->use();
}

void ms::DeferredRender::setup_material_uniforms(const Scene * scene, const Drawable * node) {
	
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

void ms::DeferredRender::setup_g_buffer_uniforms (const Scene * scene) {
    shader->set_uniform("cameraTransformation", scene->get_camera().get_transformation());
    shader->set_uniform("perspectiveProjection", scene->get_camera().get_projection_matrix());
}

void ms::DeferredRender::_load () {
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
}

void ms::DeferredRender::_unload () {
    gFramebuffer->unload();
    shader->unload();
    lightingShader->unload();
    framebuffer->unload();
    quad->unload();
}

void ms::DeferredRender::perform_light_pass (const Scene * scene) {
    
    lightingShader->use();
    lightingShader->set_uniform("renderMode", this->renderMode);
    
    if(scene->get_directional_light()) {
        lightingShader->set_uniform("dirLightProjection", scene->get_directional_light()->get_projection());
        lightingShader->set_uniform("dirLightTransformation", scene->get_directional_light()->get_positionedObject().get_transformation());
        lightingShader->set_uniform("hasDirLightShadowMap", 1);
    } else {
        lightingShader->set_uniform("hasDirLightShadowMap", 0);
    }
    
    lightingShader->set_uniform("cameraTransformation", scene->get_camera().get_transformation());
    
    DeferredRender::setup_lightpass_uniforms(scene);
    
    lightingShader->bind_texture(0, *gFramebuffer->get_colors()[0].lock());
    lightingShader->bind_texture(1, *gFramebuffer->get_colors()[1].lock());
    lightingShader->bind_texture(2, *gFramebuffer->get_colors()[2].lock());
    
    quad->draw();
    
    framebuffer->copy_depth_from(*gFramebuffer);
    
}



