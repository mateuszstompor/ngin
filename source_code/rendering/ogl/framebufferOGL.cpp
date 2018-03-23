//
//  framebufferOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "framebufferOGL.hpp"


ms::FramebufferOGL::FramebufferOGL(int colorAttachmentsAmount,
								   int renderbufferAttachmentsAmount,
								   int width,
								   int height) : ms::Framebuffer(colorAttachmentsAmount, renderbufferAttachmentsAmount, width, height), framebuffer(0), is_default_framebuffer(false) {
	
}

bool ms::FramebufferOGL::is_complete () const {
	if (mglCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "FRAMEBUFFER IS NOT COMPLETE" << std::endl;
		return false;
	}
	return true;
}

void ms::FramebufferOGL::load () {
	if(!is_loaded()) {
		if(!is_default_framebuffer) {
			mglGenFramebuffers(1, &this->framebuffer);
		}
		Resource::load();
	}
}

void ms::FramebufferOGL::clear_frame () {
	glClearColor(clearingColor.x(), clearingColor.y(), clearingColor.z(), clearingColor.w());
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	
}

void ms::FramebufferOGL::copy_depth_from (Framebuffer & frame) {
	frame.use_for_read();
	(*this).use_for_write();
	mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, this->get_width(), this->get_height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void ms::FramebufferOGL::clear_color () {
	glClear(GL_COLOR_BUFFER_BIT);
}

void ms::FramebufferOGL::clear_depth () {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ms::FramebufferOGL::use () {
	if(!is_loaded()) {
		load();
	}
	mglViewport(0, 0, width, height);
	mglBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
}

void ms::FramebufferOGL::use_for_write () {
	if(!is_loaded()) {
		load();
	}

	mglBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebuffer);
}

void ms::FramebufferOGL::use_for_read () {
	if(!is_loaded()) {
		load();
	}
	
	mglBindFramebuffer(GL_READ_FRAMEBUFFER, this->framebuffer);
}

void ms::FramebufferOGL::unload () {
	if(is_loaded()) {
		if(!is_default_framebuffer) {
			mglDeleteFramebuffers(1, &this->framebuffer);
			this->framebuffer = 0;
		}
		Resource::unload();
	}
}

void ms::FramebufferOGL::bind_color_buffer	(int index, std::shared_ptr<Texture> texture) {
	this->use();
	texture->use();
	GLuint textureID = (dynamic_cast<TextureOGL*>(texture.get()))->get_underlying_id();
	mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, textureID, 0);
	colorAttachments[index] = texture;
}

void ms::FramebufferOGL::bind_color_buffer	(int index, std::shared_ptr<Renderbuffer> renderbuffer) {
	//TODO not implemented yet
	std::cerr << "NOT IMPLEMENTED YET" << std::endl;
	assert(false);
}

void ms::FramebufferOGL::bind_depth_buffer	(std::shared_ptr<Renderbuffer> renderbuffer) {
	this->use();
	renderbuffer->use();
	GLuint renderBufferId = dynamic_cast<RenderbufferOGL*>(renderbuffer.get())->get_underlying_id();
	mglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);
}

void ms::FramebufferOGL::configure () {
	
	this->use();
	GLuint* attachments = new GLuint[colorAttachmentsAmount];
	for(int i = 0; i < colorAttachmentsAmount; ++i) {
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	mglDrawBuffers(colorAttachmentsAmount, attachments);

	if(!is_complete()) {
		std::cerr << "FATAL ERROR" << std::endl;
		assert(false);
	} else {
		#ifdef DEBUG
			std::cout << "FRAMEBUFFER COMPLETE" << std::endl;
		#endif
	}

	delete [] attachments;

	Framebuffer::configure();
}

GLuint ms::FramebufferOGL::get_underlying_id () {
	return framebuffer;
}

std::shared_ptr<ms::FramebufferOGL> ms::FramebufferOGL::window_framebuffer	(int width, int height) {
	auto framebuffer = std::shared_ptr<ms::FramebufferOGL>(new FramebufferOGL(0, 0, width, height));
	framebuffer->is_default_framebuffer = true;
	framebuffer->framebuffer = 0;
	return framebuffer;
}



