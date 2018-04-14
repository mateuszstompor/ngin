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

ms::DeferredRender::DeferredRender(unsigned int maxAOL,
								   std::shared_ptr<Framebuffer> framebuffer,
								   std::string gVS,
								   std::string gFS,
								   std::string lVS,
								   std::string lFS,
                                   std::string smVS,
                                   std::string smFS
								   ) : 	Render(framebuffer),
gBufferVertexShaderSource(gVS),
gBufferFragmentShaderSource(gFS),
lightingVertexShaderSource(lVS),
lightingFragmentShaderSource(lFS),
shadowMappingVertexShaderSource(smVS),
shadowMappingFragmentShaderSource(smFS),
maximalAmountOfLights(maxAOL),
gShader(nullptr),
lightingShader(nullptr),
shadowShader(nullptr),
gNormal(nullptr),
gPosition(nullptr),
gAlbedo(nullptr),
quad(nullptr),
renderMode(0),
debugMode(false),
debugType(DeferredRender::DebugType::position) { }

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

void ms::DeferredRender::draw (Drawable * node, const Scene * scene) {
        gShader->set_model_transformation(node->modelTransformation.get_transformation());
        DeferredRender::setup_material_uniforms(scene, node);
        node->draw();
}

void ms::DeferredRender::use () {
	if(!gShader->is_loaded() || !lightingShader->is_loaded() || !shadowShader->is_loaded()) {
		gShader->load();
		lightingShader->load();
        shadowShader->load();
	}
	
	gShader->use();
	gFramebuffer->use();
}

void ms::DeferredRender::clear_frame () {
	gFramebuffer->use();
	gFramebuffer->clear_frame();
}

void ms::DeferredRender::setup_material_uniforms(const Scene * scene, const Drawable * node) {
	
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
	gShader->set_camera_transformation(scene->get_camera().get_transformation());
	gShader->set_projection_matrix(scene->get_camera().get_projection_matrix());
}

