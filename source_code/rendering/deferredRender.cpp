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
                                   g_pass_shader &&                 gShader,
                                   lighting_shader &&               lightingShader) : 	Render(std::move(framebuffer), std::move(gShader)),
maxPointLightsAmount(maxPointLightsAmount),
maxSpotLightsAmount(maxSpotLightsAmount),
lightingShader(std::move(lightingShader)),
quad(nullptr),
renderMode(0),
debugMode(false),
debugType(DeferredRender::DebugType::position) {
    
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

    //    shadowFramebuffer->bind_depth_buffer(std::move(shadowTexture));

    //    shadowFramebuffer->configure();

    quad = Drawable::get_quad();
    
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
	lightingShader->set_camera_transformation(scene->get_camera().get_transformation());
	
	if(auto dirLight = scene->get_directional_light()) {
		lightingShader->set_has_directional_light(true);
		lightingShader->set_directional_light_dir(dirLight->direction);
		lightingShader->set_directional_light_color(dirLight->get_color());
	} else {
		lightingShader->set_has_directional_light(false);
	}
	
	{
		const auto & spotLights = scene->get_spot_lights();
		lightingShader->set_amount_of_spot_lights(static_cast<int>(spotLights.size()));
		for(unsigned int index = 0; index < spotLights.size(); ++index) {
            auto & sl = spotLights[index];
			lightingShader->set_spot_light_power(index, sl->power);
			lightingShader->set_spot_light_color(index, sl->get_color());
			lightingShader->set_spot_light_position(index, sl->position);
			lightingShader->set_spot_light_angle(index, sl->lightingAngleDegrees);
			lightingShader->set_spot_light_direction(index, sl->direction);
		}
	}
	
	{
		const auto & pointLights = scene->get_point_lights();
		lightingShader->set_amount_of_point_lights(static_cast<int>(pointLights.size()));
		for(unsigned int index = 0; index < pointLights.size(); ++index) {
            auto & pl = pointLights[index];
			lightingShader->set_point_light_color(index, pl->get_color());
			lightingShader->set_point_light_power(index, pl->power);
			lightingShader->set_point_light_position(index, pl->position);
		}
	}
	
}

void ms::DeferredRender::draw (Drawable & node, const Scene & scene) {
    auto gShader = dynamic_cast<DeferredShader*>(shader.get());
    gShader->set_model_transformation(node.modelTransformation.get_transformation());
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
	
    auto gShader = dynamic_cast<DeferredShader*>(shader.get());
	if (auto mat = node->boundedMaterial.lock()) {
		mat->use();
		gShader->set_has_material(true);
		
		if(auto diff = mat->boundedDiffuseTexture.lock()) {
            //index of diffuse texture is 0
            gShader->bind_texture(0, *diff);
			gShader->set_has_diffuse_texture(true);
		} else {
			gShader->set_has_diffuse_texture(false);
		}
		
		if(auto spec = mat->boundedSpecularTexture.lock()) {
            //index of diffuse texture is 1
            gShader->bind_texture(1, *spec);
			gShader->set_has_specular_texture(true);
		} else {
			gShader->set_has_specular_texture(false);
		}
        
        // Assimp bug "Normals maps actually laoded as height maps #430"
        // I need to treat height map as normal map
        if(auto normal = mat->boundedHeightTexture.lock()) {
            //index of diffuse texture is 1
            gShader->bind_texture(2, *normal);
            gShader->set_has_normal_texture(true);
        } else {
            gShader->set_has_normal_texture(false);
        }
        
	} else {
		gShader->set_has_material(false);
	}
	
}

void ms::DeferredRender::setup_g_buffer_uniforms (const Scene * scene) {
    auto gShader = dynamic_cast<DeferredShader*>(shader.get());
	gShader->set_camera_transformation(scene->get_camera().get_transformation());
	gShader->set_projection_matrix(scene->get_camera().get_projection_matrix());
}

void ms::DeferredRender::_load () {
    gFramebuffer->load();
    shader->load();
    lightingShader->load();
    framebuffer->use();
    quad->load();
}

void ms::DeferredRender::_unload () {
    gFramebuffer->unload();
    shader->unload();
    lightingShader->unload();
    framebuffer->unload();
    quad->unload();
    //    shadowFramebuffer->unload();
}

void ms::DeferredRender::perform_light_pass (const Scene * scene) {
    
    //    shadowFramebuffer->use();
    //    shadowFramebuffer->clear_frame();
    
    //    auto shadow = dynamic_cast<ShaderOGL*>(shadowShader.get());
    //    shadowShader->use();
    //    if(scene->get_directional_light()) {
    //        assert(shadow->set_uniform("projection", scene->get_directional_light()->get_projection()) >= 0);
    //        assert(shadow->set_uniform("toLight", scene->get_directional_light()->get_transformation()) >= 0);
    //        for(auto n : scene->get_nodes()) {
    //            assert( shadow->set_uniform("toWorld", n->modelTransformation.get_transformation()) >= 0);
    //            n->draw();
    //        }
    //    }
    
    //    for(int i = 0; i < scene->get_spot_lights().size(); ++i) {
    //        const auto & spotLight = scene->get_spot_lights()[i];
    //        spotLightsShadowComponents[i]->use();
    //        spotLightsShadowComponents[i]->clear_frame();
    //
    //        assert(shadow->set_uniform("projection", spotLight->Light::get_projection()) >= 0);
    //        assert(shadow->set_uniform("toLight", spotLight->Light::get_transformation()) >= 0);
    //        for(auto n : scene->get_nodes()) {
    //            if(spotLight->frustum.is_in_camera_sight(spotLight->get_transformation(), *n->geometry->get_bounding_box())) {
    //                assert( shadow->set_uniform("toWorld", n->modelTransformation.get_transformation()) >= 0);
    //                n->draw();
    //            }
    //        }
    //    }
    
    framebuffer->use();
    framebuffer->clear_frame();
    
    lightingShader->use();
    
    lightingShader->set_rendering_mode(this->renderMode);
    
    //    for(int i = 0; i < scene->get_spot_lights().size(); ++i) {
    //        const auto & spotLight = scene->get_spot_lights()[i];
    //        assert(lightingSh->set_uniform("spotLightsProjections[" + std::to_string(i) + "]", spotLight->Light::get_projection()) >= 0);
    //        assert(lightingSh->set_uniform("spotLightsToLightTransformations[" + std::to_string(i) + "]", spotLight->Light::get_transformation()) >= 0);
    //        lightingSh->bind_texture(4 + i, *spotLightsShadowComponents[i]->get_depth_texture().lock());
    //    }
    
    //    if(scene->get_directional_light()) {
    //        assert(lightingSh->set_uniform("sm_projection", scene->get_directional_light()->get_projection()) >= 0);
    //        assert(lightingSh->set_uniform("sm_cameraTransformation", scene->get_directional_light()->get_transformation()) >= 0);
    //    }
    
    lightingShader->set_camera_transformation(scene->get_camera().get_transformation().c_array());
    
    DeferredRender::setup_lightpass_uniforms(scene);
    
    lightingShader->bind_g_buf_posiitons(*gFramebuffer->get_colors()[0].lock());
    lightingShader->bind_g_buf_albedo(*gFramebuffer->get_colors()[2].lock());
    //    lightingShader->bind_shadow_map(*shadowFramebuffer->get_depth_texture().lock());
    lightingShader->bind_g_buf_normals(*gFramebuffer->get_colors()[1].lock());
    
    quad->draw();
    
    framebuffer->copy_depth_from(*gFramebuffer);
    
}



