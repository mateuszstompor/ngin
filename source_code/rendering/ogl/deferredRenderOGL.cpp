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
	
}

ms::DeferredRenderOGL::DeferredRenderOGL (unsigned int 	maxAOLights,
										  std::string 	gVS,
										  std::string 	gFS,
										  std::string 	lVS,
										  std::string 	lFS,
										  std::shared_ptr<Framebuffer> framebuffer) :

ms::DeferredRender(maxAOLights, framebuffer, gVS, gFS, lVS, lFS) {
	gShader = std::unique_ptr<DeferredShader>(new DeferredShaderOGL(gVS, gFS));
	lightingShader = std::unique_ptr<DeferredLightingShader>(new DeferredLightingShaderOGL(maxAOLights, lVS, lFS));
	
	
	gPosition = std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
														G_BUF_POSITIONS,
														Texture::Format::rgb_16_16_16,
														Texture::AssociatedType::FLOAT,
														Texture::MinFilter::linear,
														Texture::MagFilter::linear,
														Texture::Wrapping::clamp_to_edge,
														Texture::Wrapping::clamp_to_edge,
														0, framebuffer->get_width(), framebuffer->get_height()));

	gNormal = std::shared_ptr<Texture>(new TextureOGL(	Texture::Type::tex_2d,
														G_BUF_NORMALS,
														Texture::Format::rgb_16_16_16,
														Texture::AssociatedType::FLOAT,
														Texture::MinFilter::linear,
														Texture::MagFilter::linear,
														Texture::Wrapping::clamp_to_edge,
														Texture::Wrapping::clamp_to_edge,
														0, framebuffer->get_width(), framebuffer->get_height()));
	
	gAlbedo = std::shared_ptr<Texture>(new TextureOGL(	Texture::Type::tex_2d,
														G_BUF_ALBEDO,
													  	Texture::Format::rgba_8_8_8_8,
														Texture::AssociatedType::UNSIGNED_BYTE,
														Texture::MinFilter::linear,
														Texture::MagFilter::linear,
														Texture::Wrapping::clamp_to_edge,
														Texture::Wrapping::clamp_to_edge,
														0, framebuffer->get_width(), framebuffer->get_height()));

	gFramebuffer = std::unique_ptr<Framebuffer>(new FramebufferOGL(3,
																   1,
																   framebuffer->get_width(),
																   framebuffer->get_height()));
	
	depthRenderbuffer = std::shared_ptr<Renderbuffer>(new RenderbufferOGL(Texture::Format::depth_24,
																		  Texture::AssociatedType::UNSIGNED_BYTE,
																		  0,
																		  framebuffer->get_width(),
																		  framebuffer->get_height()));
	
	

	quad = DrawableOGL::get_quad();
	
}

void ms::DeferredRenderOGL::load () {
	
	if (!is_loaded()) {
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
		
		Resource::load();
	}
}

bool ms::DeferredRenderOGL::is_loaded () {
	return this->gShader->is_loaded();
}

void ms::DeferredRenderOGL::unload () {
	if (is_loaded()) {
		this->gAlbedo->unload();
		this->gNormal->unload();
		this->gPosition->unload();
		this->gShader->unload();
		this->lightingShader->unload();
		Resource::unload();
	}
}

void ms::DeferredRenderOGL::perform_light_pass (const Scene * scene) {
	framebuffer->use();
	framebuffer->clear_frame();
	
	lightingShader->use();
	
	lightingShader->set_rendering_mode(this->renderMode);
	
	lightingShader->set_camera_transformation(scene->cam->get_transformation().c_array());
	
	DeferredRender::setup_lightpass_uniforms(scene);
	
	lightingShader->bind_g_buf_posiitons(*gPosition);
	lightingShader->bind_g_buf_albedo(*gAlbedo);
	lightingShader->bind_g_buf_normals(*gNormal);
	
	quad->draw();
	
	framebuffer->copy_depth_from(*gFramebuffer);
	
}

