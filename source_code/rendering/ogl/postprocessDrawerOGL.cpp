//
//  postprocessDrawerOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "postprocessDrawerOGL.hpp"

ms::PostprocessDrawerOGL::PostprocessDrawerOGL(std::vector<std::shared_ptr<Texture>> 	input,
											   std::shared_ptr<Framebuffer> 			framebuffer,
											   std::unique_ptr<Shader>					shaderProgram) : PostprocessDrawer(input,
																														   framebuffer,
																														   std::move(shaderProgram)) {
	quad = DrawableOGL::get_quad();
}

void ms::PostprocessDrawerOGL::draw_quad() {
	framebuffer->use();
	shader->use();
	
	
	
//	mglBindVertexArray(quadVAO);

	
	{
		ShaderOGL* shad = dynamic_cast<ShaderOGL*>(shader.get());
		for(int i = 0; i<inputTextures.size(); ++i) {
			shad->set_uniform("in" + std::to_string(i), i);
			shader->bind_texture(i, *(inputTextures[i]));
		}
	}

	quad->draw();
//	mglDrawArrays(GL_TRIANGLES, 0, 6);
}

void ms::PostprocessDrawerOGL::draw (Drawable * node, const Scene * scene) {
	assert(false);
}

void ms::PostprocessDrawerOGL::load () {
	if(!is_loaded()) {
		
		shader->load();
		shader->use();
		GLuint a = glGetUniformLocation(dynamic_cast<ShaderOGL*>(shader.get())->get_gl_id(), "passedtexture");
		glUniform1i(a, 0);
		
		
//		mglGenVertexArrays(1, &quadVAO);
//		mglBindVertexArray(quadVAO);

//		mglGenBuffers(1, &quadVBO);
//		mglBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//		mglBufferData(GL_ARRAY_BUFFER, sizeof(quad::vertices) + sizeof(quad::textureCoordinates), nullptr, GL_STATIC_DRAW);
//		mglBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad::vertices), quad::vertices);
//		mglBufferSubData(GL_ARRAY_BUFFER, sizeof(quad::vertices), sizeof(quad::textureCoordinates), quad::textureCoordinates);

		//vertices
//		mglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
//		mglEnableVertexAttribArray(0);

		//texturecoordinates
//		mglVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(18 * sizeof(GL_FLOAT)));
//		mglEnableVertexAttribArray(1);
//
//		mglBindBuffer(GL_ARRAY_BUFFER, 0);
//		mglBindVertexArray(0);
		
//		quad->load();
		
		Resource::load();
	}
}

std::string ms::PostprocessDrawerOGL::get_class () {
	return "ms::PostprocessDrawerOGL";
}

void ms::PostprocessDrawerOGL::unload () {
	if(is_loaded()) {
		
//		mglDeleteVertexArrays(1, &quadVAO);
//		mglDeleteBuffers(1, &quadVBO);
		
		Resource::unload();
	}
}
