//
//  deferredRenderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredRenderOGL.hpp"

namespace ms {
	
	typedef unsigned int ui;
	
	#define G_BUF_ALBEDO 			"G_BUF_ALBEDO"
	#define G_BUF_POSITIONS 		"G_BUF_POSITIONS"
	#define G_BUF_NORMALS 			"G_BUF_NORMALS"
    #define SHADOW_MAP              "SHADOW_MAP"
	
}

ms::DeferredRenderOGL::DeferredRenderOGL (unsigned int 	                maxAOLights,
										  std::string 	                gVS,
										  std::string 	                gFS,
										  std::string 	                lVS,
										  std::string 	                lFS,
                                          std::string                   smVS,
                                          std::string                   smFS,
										  std::shared_ptr<Framebuffer>  framebuffer) :

ms::DeferredRender(maxAOLights, framebuffer, gVS, gFS, lVS, lFS, smVS, smFS) {
	gShader = std::make_unique<DeferredShaderOGL>(gVS, gFS);
	lightingShader = std::make_unique<DeferredLightingShaderOGL>(maxAOLights, lVS, lFS);
    shadowShader = std::make_unique<ShaderOGL>(smVS, "", "", "", smFS);
	
	gPosition = std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                             G_BUF_POSITIONS,
                                             Texture::Format::rgb_16_16_16,
                                             Texture::AssociatedType::FLOAT,
                                             Texture::MinFilter::linear,
                                             Texture::MagFilter::linear,
                                             Texture::Wrapping::clamp_to_edge,
                                             Texture::Wrapping::clamp_to_edge,
                                             0, framebuffer->get_width(), framebuffer->get_height());

	gNormal = std::make_shared<TextureOGL>( Texture::Type::tex_2d,
                                            G_BUF_NORMALS,
                                            Texture::Format::rgb_16_16_16,
                                            Texture::AssociatedType::FLOAT,
                                            Texture::MinFilter::linear,
                                            Texture::MagFilter::linear,
                                            Texture::Wrapping::clamp_to_edge,
                                            Texture::Wrapping::clamp_to_edge,
                                            0, framebuffer->get_width(), framebuffer->get_height());
	
	gAlbedo = std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                           G_BUF_ALBEDO,
                                           Texture::Format::rgba_8_8_8_8,
                                           Texture::AssociatedType::UNSIGNED_BYTE,
                                           Texture::MinFilter::linear,
                                           Texture::MagFilter::linear,
                                           Texture::Wrapping::clamp_to_edge,
                                           Texture::Wrapping::clamp_to_edge,
                                           0, framebuffer->get_width(), framebuffer->get_height());

	gFramebuffer = std::make_unique<FramebufferOGL>(3,
                                                    1,
                                                    framebuffer->get_width(),
                                                    framebuffer->get_height());
	
	depthRenderbuffer = std::make_shared<RenderbufferOGL>(Texture::Format::depth_24,
                                                          Texture::AssociatedType::UNSIGNED_BYTE,
                                                          0,
                                                          framebuffer->get_width(),
                                                          framebuffer->get_height());
	
    unsigned int resoultion = 1024*4;
    
    
    for (int i = 0; i < 5; ++i) {
        
        auto fb = std::make_unique<FramebufferOGL>(1,
                                                   0,
                                                   resoultion,
                                                   resoultion);
        
        auto tex = std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                                SHADOW_MAP,
                                                Texture::Format::depth_32,
                                                Texture::AssociatedType::FLOAT,
                                                Texture::MinFilter::nearest,
                                                Texture::MagFilter::nearest,
                                                Texture::Wrapping::repeat,
                                                Texture::Wrapping::repeat,
                                                0,
                                                resoultion,
                                                resoultion);
        
        fb->bind_depth_buffer(tex);
        
        fb->configure();

        auto tuple = std::make_tuple(std::move(fb), tex);
        spotLightsShadowComponents.push_back(std::move(tuple));
        
    }
    
    shadowTexture = std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                                 SHADOW_MAP,
                                                 Texture::Format::depth_32,
                                                 Texture::AssociatedType::FLOAT,
                                                 Texture::MinFilter::nearest,
                                                 Texture::MagFilter::nearest,
                                                 Texture::Wrapping::repeat,
                                                 Texture::Wrapping::repeat,
                                                 0,
                                                 resoultion,
                                                 resoultion);
    
    shadowFramebuffer = std::make_unique<FramebufferOGL>(1,
                                                         0,
                                                         resoultion,
                                                         resoultion);
    
    shadowFramebuffer->bind_depth_buffer(shadowTexture);
    
    shadowFramebuffer->configure();

	quad = DrawableOGL::get_quad();
	
}

void ms::DeferredRenderOGL::_load () {
	this->gShader->load();
	this->lightingShader->load();
	
	gFramebuffer->use();
	
	gFramebuffer->bind_color_buffer(0, gPosition);
	gFramebuffer->bind_color_buffer(1, gNormal);
	gFramebuffer->bind_color_buffer(2, gAlbedo);
	gFramebuffer->bind_depth_buffer(depthRenderbuffer);
	
	gFramebuffer->configure();
	framebuffer->use();
	
	quad->load();
}

void ms::DeferredRenderOGL::_unload () {
	this->gAlbedo->unload();
	this->gNormal->unload();
	this->gPosition->unload();
	this->gShader->unload();
	this->lightingShader->unload();
}

void ms::DeferredRenderOGL::perform_light_pass (const Scene * scene) {
    
    shadowFramebuffer->use();
    shadowFramebuffer->clear_frame();
    
    auto shadow = dynamic_cast<ShaderOGL*>(shadowShader.get());
    shadowShader->use();
    assert(shadow->set_uniform("projection", scene->get_directional_light()->get_projection()) >= 0);
    assert(shadow->set_uniform("toLight", scene->get_directional_light()->get_transformation()) >= 0);
    for(auto n : scene->get_nodes()) {
        assert( shadow->set_uniform("toWorld", n->modelTransformation.get_transformation()) >= 0);
        n->draw();
    }
    
    int i = 0;
    
    for(auto spotLight : scene->get_spot_lights()) {
        std::get<0>(spotLightsShadowComponents[i])->use();
        std::get<0>(spotLightsShadowComponents[i])->clear_frame();
        assert(shadow->set_uniform("projection", spotLight->get_projection()) >= 0);
        assert(shadow->set_uniform("toLight", spotLight->get_transformation()) >= 0);
        for(auto n : scene->get_nodes()) {
            assert( shadow->set_uniform("toWorld", n->modelTransformation.get_transformation()) >= 0);
            n->draw();
        }
        i+=0;
    }
    
    
	framebuffer->use();
	framebuffer->clear_frame();
	
    
    auto lightingSh = dynamic_cast<ShaderOGL*>(lightingShader.get());
    
	lightingShader->use();
	
	lightingShader->set_rendering_mode(this->renderMode);
	
    i = 0;
    
    for(auto spotLight : scene->get_spot_lights()) {
        assert(lightingSh->set_uniform("spot_sm_projection[" + std::to_string(i) + "]", spotLight->get_projection()) >= 0);
        assert(lightingSh->set_uniform("spot_sm_cameraTransformation[" + std::to_string(i) + "]", spotLight->get_transformation()) >= 0);
        i+=0;
    }
    
    assert(lightingSh->set_uniform("sm_projection", scene->get_directional_light()->get_projection()) >= 0);
    assert(lightingSh->set_uniform("sm_cameraTransformation", scene->get_directional_light()->get_transformation()) >= 0);
    
	lightingShader->set_camera_transformation(scene->get_camera().get_transformation().c_array());
	
	DeferredRender::setup_lightpass_uniforms(scene);
	
	lightingShader->bind_g_buf_posiitons(*gPosition);
	lightingShader->bind_g_buf_albedo(*gAlbedo);
    lightingShader->bind_shadow_map(*shadowTexture);
	lightingShader->bind_g_buf_normals(*gNormal);
	
	quad->draw();
	
	framebuffer->copy_depth_from(*gFramebuffer);
	
}

