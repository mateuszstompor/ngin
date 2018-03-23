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

ms::DeferredRender(maxAOLights, framebuffer, gVS, gFS, lVS, lFS), quadVAO(0), quadVBO(0) {
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
	
	

	
}

void ms::DeferredRenderOGL::use () {
	if(!gShader->is_loaded() || !lightingShader->is_loaded()) {
		gShader->load();
		lightingShader->load();
	}

	gShader->use();
	mglEnable(GL_DEPTH_TEST);
	mglDepthFunc(GL_LEQUAL);
}

void ms::DeferredRenderOGL::clear_frame () {
	gFramebuffer->use();
	gFramebuffer->clear_frame();
}

void ms::DeferredRenderOGL::draw (Drawable * node, const Scene * scene) {
	gShader->set_model_transformation(node->modelTransformation.get_transformation());
	
	node->use();
	
	DeferredRender::setup_material_uniforms(scene, node);
	
	node->geometry->use_indicies();
	
	mglDrawElements(GL_TRIANGLES, node->geometry->amount_of_indices(), GL_UNSIGNED_INT, nullptr);

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
		
		//setup quad
		mglGenVertexArrays(1, &quadVAO);
		mglBindVertexArray(quadVAO);

		mglGenBuffers(1, &quadVBO);
		mglBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		mglBufferData(GL_ARRAY_BUFFER, sizeof(quad::vertices) + sizeof(quad::textureCoordinates), nullptr, GL_STATIC_DRAW);
		mglBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad::vertices), quad::vertices);
		mglBufferSubData(GL_ARRAY_BUFFER, sizeof(quad::vertices), sizeof(quad::textureCoordinates), quad::textureCoordinates);

		//vertices
		mglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		mglEnableVertexAttribArray(0);

		//texturecoordinates
		mglVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(18 * sizeof(GL_FLOAT)));
		mglEnableVertexAttribArray(1);

		mglBindBuffer(GL_ARRAY_BUFFER, 0);
		mglBindVertexArray(0);

		//end

		mglEnable(GL_DEPTH_TEST);
		mglDepthFunc(GL_LEQUAL);

		Resource::load();
	}
}

bool ms::DeferredRenderOGL::is_loaded () {
	return this->gShader->is_loaded();
}

void ms::DeferredRenderOGL::unload () {
	if (is_loaded()) {
		
		mglDeleteVertexArrays(1, &quadVAO);
		mglDeleteBuffers(1, &quadVBO);
		
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
	
	mglBindVertexArray(quadVAO);
	
	lightingShader->bind_g_buf_posiitons(*gPosition);
	lightingShader->bind_g_buf_albedo(*gAlbedo);
	lightingShader->bind_g_buf_normals(*gNormal);
	
	mglDrawArrays(GL_TRIANGLES, 0, 6);
	
	framebuffer->copy_depth_from(*gFramebuffer);
	
}

