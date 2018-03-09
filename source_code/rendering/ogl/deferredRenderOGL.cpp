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
}

ms::DeferredRenderOGL::DeferredRenderOGL (std::shared_ptr<std::string> gVS,
										  std::shared_ptr<std::string> gFS,
										  std::shared_ptr<std::string> lVS,
										  std::shared_ptr<std::string> lFS,
										  unsigned int sW,
										  unsigned int sH,
										  unsigned int fbW,
										  unsigned int fbH) :

ms::DeferredRender(sW, sH, fbW, fbH, gVS, gFS, lVS, lFS) {
	gShader = std::unique_ptr<DeferredShader>(new DeferredShaderOGL(gVS, gFS));
	lightingShader = std::unique_ptr<DeferredLightingShader>(new DeferredLightingShaderOGL(lVS, lFS));
}

void ms::DeferredRenderOGL::use () {
	if(!gShader->is_loaded() || !lightingShader->is_loaded()) {
		gShader->load();
		lightingShader->load();
	}
	
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
	
	mglViewport(0, 0, screenWidth, screenHeight);
	mglEnable(GL_DEPTH_TEST);
	mglDepthFunc(GL_LEQUAL);
}

void ms::DeferredRenderOGL::clear_frame () {
	mglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mglClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

void ms::DeferredRenderOGL::draw_scene (const Scene * scene) {
	mglBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
	mglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gShader->use();

	gShader->set_camera_transformation(scene->get_camera().get_transformation());
	gShader->set_projection_matrix(scene->get_camera().get_projection_matrix());

	if(auto dirLight = scene->get_directional_light()) {
		gShader->set_has_directional_light(true);
		gShader->set_directional_light_dir(dirLight->direction);
		gShader->set_directional_light_color(dirLight->color);
	} else {
		gShader->set_has_directional_light(false);
	}

	for (const std::shared_ptr<SceneNode> & node : scene->nodes) {
		node->use();

		gShader->set_model_transformation(node->modelTransformation.get_transformation());
		
		if (auto material = node->material) {
			gShader->set_has_material(true);
			gShader->set_material_ambient_color(material->ambientColor);
			gShader->set_material_diffuse_color(material->diffuseColor);
			gShader->set_material_specular_color(material->specularColor);
			gShader->set_material_opacity(material->opacity);
			gShader->set_material_shininess(material->shininess);
		} else {
			gShader->set_has_material(false);
		}

		this->gShader->use();
		mglDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	mglBindFramebuffer(GL_FRAMEBUFFER, 0);

	mglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightingShader->use();
	mglBindVertexArray(quadVAO);

	mglActiveTexture(GL_TEXTURE0);
	mglBindTexture(GL_TEXTURE_2D, gPosition);
	
	mglActiveTexture(GL_TEXTURE1);
	mglBindTexture(GL_TEXTURE_2D, gNormal);
	
	mglActiveTexture(GL_TEXTURE2);
	mglBindTexture(GL_TEXTURE_2D, gAlbedo);
	
	mglDrawArrays(GL_TRIANGLES, 0, 6);

}

void ms::DeferredRenderOGL::load () {
	
	if (!is_loaded()) {
		this->gShader->load();
		this->lightingShader->is_loaded();
		
		mglGenFramebuffers(1, &gFrameBuffer);
		mglBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
		
		mglGenTextures(1, &gPosition);
		mglBindTexture(GL_TEXTURE_2D, gPosition);
		mglTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, frameBufferWidth, frameBufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
		mglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		mglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
		
		
		mglGenTextures(1, &gNormal);
		mglBindTexture(GL_TEXTURE_2D, gNormal);
		mglTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, frameBufferWidth, frameBufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
		mglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		mglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
		
		
		mglGenTextures(1, &gAlbedo);
		mglBindTexture(GL_TEXTURE_2D, gAlbedo);
		mglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameBufferWidth, frameBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		mglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		mglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
		
		{
			auto glstatus = glGetError();
			if (glstatus != GL_NO_ERROR) { std::cout << "Error in GL call:" << glstatus << std::endl; }
			}
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

		
		mglDrawBuffers(3, attachments);

		mglGenRenderbuffers(1, &gRenderBuffer);
		mglBindRenderbuffer(GL_RENDERBUFFER, gRenderBuffer);

		mglRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, frameBufferWidth, frameBufferHeight);


		mglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gRenderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		mglBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		
		mglDeleteTextures(1, &gAlbedo);
		mglDeleteTextures(1, &gNormal);
		mglDeleteTextures(1, &gPosition);
		
		this->gShader->unload();
		this->lightingShader->unload();
		Resource::unload();
	}
}
