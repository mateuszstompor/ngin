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

ms::DeferredRenderOGL::DeferredRenderOGL (unsigned int                      maxPointLightsAmount,
                                          unsigned int                      maxSpotLightsAmount,
										  std::unique_ptr<Framebuffer> &&   framebuffer,
                                          g_pass_shader &&                  gShader,
                                          lighting_shader &&                lightingShader) :
//TODO add this to deferred lighting hsader
ms::DeferredRender(maxPointLightsAmount, maxSpotLightsAmount, std::move(framebuffer), std::move(gShader), std::move(lightingShader)) {
	
    auto gPosition = std::make_unique<TextureOGL>(Texture::Type::tex_2d,
                                                  G_BUF_POSITIONS,
                                                  Texture::Format::rgb_16_16_16,
                                                  Texture::AssociatedType::FLOAT,
                                                  Texture::MinFilter::linear,
                                                  Texture::MagFilter::linear,
                                                  Texture::Wrapping::clamp_to_edge,
                                                  Texture::Wrapping::clamp_to_edge,
                                                  0, this->framebuffer->get_width(), this->framebuffer->get_height());

    auto gNormal = std::make_unique<TextureOGL>(Texture::Type::tex_2d,
                                                G_BUF_NORMALS,
                                                Texture::Format::rgb_16_16_16,
                                                Texture::AssociatedType::FLOAT,
                                                Texture::MinFilter::linear,
                                                Texture::MagFilter::linear,
                                                Texture::Wrapping::clamp_to_edge,
                                                Texture::Wrapping::clamp_to_edge,
                                                0, this->framebuffer->get_width(), this->framebuffer->get_height());
    
    auto gAlbedo = std::make_unique<TextureOGL>(Texture::Type::tex_2d,
                                                G_BUF_ALBEDO,
                                                Texture::Format::rgba_8_8_8_8,
                                                Texture::AssociatedType::UNSIGNED_BYTE,
                                                Texture::MinFilter::linear,
                                                Texture::MagFilter::linear,
                                                Texture::Wrapping::clamp_to_edge,
                                                Texture::Wrapping::clamp_to_edge,
                                                0, this->framebuffer->get_width(), this->framebuffer->get_height());

    gFramebuffer = std::make_unique<FramebufferOGL>(3,
                                                    1,
                                                    this->framebuffer->get_width(),
                                                    this->framebuffer->get_height());

    auto depthRenderbuffer = std::make_unique<RenderbufferOGL>(Texture::Format::depth_32,
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

    quad = DrawableOGL::get_quad();
	
}

void ms::DeferredRenderOGL::_load () {
    gFramebuffer->load();
    shader->load();
    lightingShader->load();
    framebuffer->use();
    quad->load();
}

void ms::DeferredRenderOGL::_unload () {
    gFramebuffer->unload();
    shader->unload();
    lightingShader->unload();
    framebuffer->unload();
    quad->unload();
//    shadowFramebuffer->unload();
}

void ms::DeferredRenderOGL::perform_light_pass (const Scene * scene) {
    
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

    auto lightingSh = dynamic_cast<ShaderOGL*>(lightingShader.get());

    lightingShader->use();

    lightingShader->set_rendering_mode(this->renderMode);

//    for(int i = 0; i < scene->get_spot_lights().size(); ++i) {
//        const auto & spotLight = scene->get_spot_lights()[i];
//        assert(lightingSh->set_uniform("spotLightsProjections[" + std::to_string(i) + "]", spotLight->Light::get_projection()) >= 0);
//        assert(lightingSh->set_uniform("spotLightsToLightTransformations[" + std::to_string(i) + "]", spotLight->Light::get_transformation()) >= 0);
//        lightingSh->bind_texture(4 + i, *spotLightsShadowComponents[i]->get_depth_texture().lock());
//    }

    if(scene->get_directional_light()) {
        assert(lightingSh->set_uniform("sm_projection", scene->get_directional_light()->get_projection()) >= 0);
        assert(lightingSh->set_uniform("sm_cameraTransformation", scene->get_directional_light()->get_transformation()) >= 0);
    }

    lightingShader->set_camera_transformation(scene->get_camera().get_transformation().c_array());

    DeferredRender::setup_lightpass_uniforms(scene);

    lightingShader->bind_g_buf_posiitons(*gFramebuffer->get_colors()[0].lock());
    lightingShader->bind_g_buf_albedo(*gFramebuffer->get_colors()[2].lock());
//    lightingShader->bind_shadow_map(*shadowFramebuffer->get_depth_texture().lock());
    lightingShader->bind_g_buf_normals(*gFramebuffer->get_colors()[1].lock());

    quad->draw();

    framebuffer->copy_depth_from(*gFramebuffer);
	
}

