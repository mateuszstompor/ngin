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
										  unsigned int 	sW,
										  unsigned int 	sH,
										  unsigned int 	fbW,
										  unsigned int 	fbH) :

ms::DeferredRender(maxAOLights, sW, sH, fbW, fbH, gVS, gFS, lVS, lFS), gFrameBuffer(0), gRenderBuffer(0), quadVAO(0), quadVBO(0), defaultFBO(0) {
	gShader = std::unique_ptr<DeferredShader>(new DeferredShaderOGL(gVS, gFS));
	lightingShader = std::unique_ptr<DeferredLightingShader>(new DeferredLightingShaderOGL(maxAOLights, lVS, lFS));
	
	
	gPosition = std::unique_ptr<Texture>(new TextureOGL(GL_TEXTURE_2D,
														G_BUF_POSITIONS,
														Texture::Format::rgb_16_16_16,
														Texture::AssociatedType::FLOAT,
														Texture::MinFilter::linear,
														Texture::MagFilter::linear,
														Texture::Wrapping::clamp_to_edge,
														Texture::Wrapping::clamp_to_edge,
														0, frameBufferWidth, frameBufferHeight));
	
	gNormal = std::unique_ptr<Texture>(new TextureOGL(	GL_TEXTURE_2D,
														G_BUF_NORMALS,
														Texture::Format::rgb_16_16_16,
														Texture::AssociatedType::FLOAT,
														Texture::MinFilter::linear,
														Texture::MagFilter::linear,
														Texture::Wrapping::clamp_to_edge,
														Texture::Wrapping::clamp_to_edge,
														0, frameBufferWidth, frameBufferHeight));
	
	gAlbedo = std::unique_ptr<Texture>(new TextureOGL(	GL_TEXTURE_2D,
														G_BUF_ALBEDO,
													  	Texture::Format::rgba_8_8_8_8,
														Texture::AssociatedType::UNSIGNED_BYTE,
														Texture::MinFilter::linear,
														Texture::MagFilter::linear,
														Texture::Wrapping::clamp_to_edge,
														Texture::Wrapping::clamp_to_edge,
														0, frameBufferWidth, frameBufferHeight));
	
}

void ms::DeferredRenderOGL::use () {
	if(!gShader->is_loaded() || !lightingShader->is_loaded()) {
		gShader->load();
		lightingShader->load();
	}

	gShader->use();
	mglViewport(0, 0, frameBufferWidth, frameBufferHeight);
	mglEnable(GL_DEPTH_TEST);
	mglDepthFunc(GL_LEQUAL);
}

void ms::DeferredRenderOGL::clear_frame () {
	mglBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
	mglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ms::DeferredRenderOGL::draw (Drawable * node, const Scene * scene) {
	mglViewport(0, 0, frameBufferWidth, frameBufferHeight);

	gShader->set_model_transformation(node->modelTransformation.get_transformation());
	
	node->use();
	
	DeferredRender::setup_material_uniforms(scene, node);
	
	node->geometry->use_indicies();
	
	mglDrawElements(GL_TRIANGLES, node->geometry->amount_of_indices(), GL_UNSIGNED_INT, nullptr);

}

void ms::DeferredRenderOGL::load () {
	
	if (!is_loaded()) {
		this->gShader->load();
		this->lightingShader->is_loaded();
		
		mglGenFramebuffers(1, &gFrameBuffer);
		mglBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
		
		gPosition->load();
		gNormal->load();
		gAlbedo->load();
		
		gPosition->use();
		mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (dynamic_cast<TextureOGL*>(gPosition.get()))->get_underlying_id(), 0);
		
		gNormal->use();
		mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, (dynamic_cast<TextureOGL*>(gNormal.get()))->get_underlying_id(), 0);

		gAlbedo->use();
		mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, (dynamic_cast<TextureOGL*>(gAlbedo.get()))->get_underlying_id(), 0);

		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		
		mglDrawBuffers(3, attachments);

		mglGenRenderbuffers(1, &gRenderBuffer);
		mglBindRenderbuffer(GL_RENDERBUFFER, gRenderBuffer);

		//TODO Set depth component to 16 in order to see image distortion
		#ifdef ios_build
		mglRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, frameBufferWidth, frameBufferHeight);
		#else
		mglRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, frameBufferWidth, frameBufferHeight);
		#endif

		mglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gRenderBuffer);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer not complete!" << std::endl;
		}
		

		mglBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);

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
		mglViewport(0, 0, frameBufferWidth, frameBufferHeight);
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
		
		mglDeleteFramebuffers(1, &gFrameBuffer);
		mglDeleteRenderbuffers(1, &gRenderBuffer);
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

void ms::DeferredRenderOGL::set_default_FBO (GLuint defFBO) {
	defaultFBO = defFBO;
}

void ms::DeferredRenderOGL::perform_light_pass (const Scene * scene) {
	mglBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
	
	mglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mglViewport(0, 0, screenWidth, screenHeight);

	lightingShader->use();
	
	lightingShader->set_rendering_mode(this->renderMode);
	
	lightingShader->set_camera_transformation(scene->cam->get_transformation().c_array());
	
	DeferredRender::setup_lightpass_uniforms(scene);
	
	mglBindVertexArray(quadVAO);
	
	lightingShader->bind_g_buf_posiitons(*gPosition);
	lightingShader->bind_g_buf_albedo(*gAlbedo);
	lightingShader->bind_g_buf_normals(*gNormal);
	
	mglDrawArrays(GL_TRIANGLES, 0, 6);
	
	mglBindFramebuffer(GL_READ_FRAMEBUFFER, gFrameBuffer);
	mglBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFBO);
	mglBlitFramebuffer(0, 0, frameBufferWidth, frameBufferHeight, 0, 0, screenWidth, screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	mglBindFramebuffer(GL_FRAMEBUFFER, 0);
}

