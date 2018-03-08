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
	gShader = std::shared_ptr<DeferredShader>(new DeferredShaderOGL(gVS, gFS));
	lightingShader = std::shared_ptr<DeferredLightingShader>(new DeferredLightingShaderOGL(lVS, lFS));
}

void ms::DeferredRenderOGL::use () {
	if(!gShader->is_loaded() || !lightingShader->is_loaded()) {
		gShader->load();
		lightingShader->load();
	}
	
	//setup quad
	
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad::vertices) + sizeof(quad::textureCoordinates), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad::vertices), quad::vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(quad::vertices), sizeof(quad::textureCoordinates), quad::textureCoordinates);
	
	//vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	
	//texturecoordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(18 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//end
	
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void ms::DeferredRenderOGL::clear_frame () {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

void ms::DeferredRenderOGL::draw_scene (const std::shared_ptr<Scene> &scene) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	
	glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
	
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
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	lightingShader->use();
	glBindVertexArray(quadVAO);
//	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	
}

void ms::DeferredRenderOGL::load () {
	if (!is_loaded()) {
		this->gShader->load();
		this->lightingShader->is_loaded();
		
		glGenFramebuffers(1, &gFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
		
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, frameBufferWidth, frameBufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
		
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, frameBufferWidth, frameBufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
		
		glGenTextures(1, &gAlbedo);
		glBindTexture(GL_TEXTURE_2D, gAlbedo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameBufferWidth, frameBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
		
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
		
		glGenRenderbuffers(1, &gRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, gRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frameBufferWidth, frameBufferHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gRenderBuffer);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Resource::load();
	}
}

bool ms::DeferredRenderOGL::is_loaded () {
	return this->gShader->is_loaded();
}

void ms::DeferredRenderOGL::unload () {
	if (is_loaded()) {
		
		glDeleteFramebuffers(1, &gFrameBuffer);
		glDeleteRenderbuffers(1, &gRenderBuffer);
		
		glDeleteTextures(1, &gAlbedo);
		glDeleteTextures(1, &gNormal);
		glDeleteTextures(1, &gPosition);
		
		this->gShader->unload();
		this->lightingShader->unload();
		Resource::unload();
	}
}
