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
								   int height) : ms::Framebuffer(colorAttachmentsAmount,
																 renderbufferAttachmentsAmount,
																 width,
																 height), framebuffer(0), is_default_framebuffer(false) {
	
}

bool ms::FramebufferOGL::is_complete () const {
	if (mglCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "FRAMEBUFFER IS NOT COMPLETE" << std::endl;
		return false;
	}
	return true;
}

void ms::FramebufferOGL::_load () {
	if(!is_default_framebuffer) {
		mglGenFramebuffers(1, &this->framebuffer);
	}
}

void ms::FramebufferOGL::clear_frame () {
	glClearColor(clearingColor.x(), clearingColor.y(), clearingColor.z(), clearingColor.w());
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	
}

void ms::FramebufferOGL::copy_depth_from (Framebuffer & frame, Texture::MagFilter filter) {
	frame.use_for_read();
	(*this).use_for_write();
	mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, this->get_width(), this->get_height(), GL_DEPTH_BUFFER_BIT, TextureOGL::to_ogl(filter));
}

void ms::FramebufferOGL::copy_color_from (Framebuffer & frame, Texture::MagFilter filter) {
    frame.use_for_read();
    (*this).use_for_write();
    mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, this->get_width(), this->get_height(), GL_COLOR_BUFFER_BIT, TextureOGL::to_ogl(filter));
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
	
	if(isDepthTestEnabled) {
		mglEnable(GL_DEPTH_TEST);
		mglDepthFunc(GL_LEQUAL);
	} else {
		mglDisable(GL_DEPTH_TEST);
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

void ms::FramebufferOGL::_unload () {
	if(!is_default_framebuffer) {
		mglDeleteFramebuffers(1, &this->framebuffer);
        std::for_each(colorTextureAttachments.begin(), colorTextureAttachments.end(), [&] (auto & color) { if (color) { color->unload(); } } );
        std::for_each(colorRenderbufferAttachments.begin(), colorRenderbufferAttachments.end(), [&] (auto & color) { if (color) { color->unload(); } } );
        if(depthTexture) { depthTexture->unload(); }
        if(depthRenderbuffer) { depthRenderbuffer->unload(); }
		this->framebuffer = 0;
	}
}

void ms::FramebufferOGL::bind_color_buffer (int index, std::unique_ptr<Texture> && texture) {
	this->use();
	texture->use();
	GLuint textureID = (dynamic_cast<TextureOGL*>(texture.get()))->get_underlying_id();
	mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, textureID, 0);
    colorTextureAttachments[index] = std::move(texture);
}

void ms::FramebufferOGL::bind_color_buffer (int index, std::unique_ptr<Renderbuffer> && renderbuffer) {
	this->use();
	renderbuffer->use();
	GLuint renderbufferID = (dynamic_cast<Renderbuffer*>(renderbuffer.get()))->get_underlying_id();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, renderbufferID);
    colorRenderbufferAttachments[index] = std::move(renderbuffer);
    // should work, but check it
	assert(false);
}

void ms::FramebufferOGL::copy_framebuffer (Framebuffer & frame, Texture::MagFilter filter) {
    frame.use_for_read();
    (*this).use_for_write();
    mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, this->get_width(), this->get_height(), GL_COLOR_BUFFER_BIT, TextureOGL::to_ogl(filter));
    mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, this->get_width(), this->get_height(), GL_DEPTH_BUFFER_BIT, TextureOGL::to_ogl(filter));
}

void ms::FramebufferOGL::bind_depth_buffer	(std::unique_ptr<Renderbuffer> && renderbuffer) {
	this->use();
	renderbuffer->use();
	GLuint renderBufferId = dynamic_cast<Renderbuffer*>(renderbuffer.get())->get_underlying_id();
	mglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);
    depthRenderbuffer = std::move(renderbuffer);
}

void ms::FramebufferOGL::configure () {
	
	this->use();
    if (colorTextureAttachmentsAmount > 0) {
        GLuint* attachments = new GLuint[colorTextureAttachmentsAmount];
        for(int i = 0; i < colorTextureAttachmentsAmount; ++i) {
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        mglDrawBuffers(colorTextureAttachmentsAmount, attachments);
        delete [] attachments;
    } else {
        GLenum drawBuffers[] = { GL_NONE };
        mglDrawBuffers(1, drawBuffers);
        mglReadBuffer(GL_NONE);
    }
    
	if(!is_complete()) {
		std::cerr << "FATAL ERROR" << std::endl;
		assert(false);
	}

	Framebuffer::configure();
}

std::string ms::FramebufferOGL::get_class () {
	return "ms::FramebufferOGL";
}

GLuint ms::FramebufferOGL::get_underlying_id () const {
	return framebuffer;
}

void ms::FramebufferOGL::set_underlying_id (GLuint framebufferID) {
	if(!is_default_framebuffer) {
		std::cerr << "ONLY DEFAULT FRAMEBUFFER'S ID CAN BE CHANGED" << std::endl;
		assert(false);
	}
	framebuffer = framebufferID;
}

void ms::FramebufferOGL::bind_depth_buffer (std::unique_ptr<Texture> && texture) {
    this->use();
    texture->use();
    GLuint textureID = (dynamic_cast<TextureOGL*>(texture.get()))->get_underlying_id();
    mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
    depthTexture = std::move(texture);
}

std::unique_ptr<ms::FramebufferOGL> ms::FramebufferOGL::window_framebuffer	(int width, int height) {
	auto framebuffer = std::make_unique<ms::FramebufferOGL>(0, 0, width, height);
	framebuffer->is_default_framebuffer = true;
	framebuffer->framebuffer = 0;
	return framebuffer;
}



