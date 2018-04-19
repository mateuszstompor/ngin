//
//  framebuffer.cpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "framebuffer.hpp"

bool ms::Framebuffer::is_complete () const {
	if (mglCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "FRAMEBUFFER IS NOT COMPLETE" << std::endl;
		return false;
	}
	return true;
}

void ms::Framebuffer::_load () {
	if(!is_default_framebuffer) {
		mglGenFramebuffers(1, &this->framebuffer);
	}
}

void ms::Framebuffer::clear_frame () {
	glClearColor(clearingColor.x(), clearingColor.y(), clearingColor.z(), clearingColor.w());
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	
}

void ms::Framebuffer::copy_depth_from (Framebuffer & frame, Texture::MagFilter filter) {
	frame.use_for_read();
	(*this).use_for_write();
	mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, this->get_width(), this->get_height(), GL_DEPTH_BUFFER_BIT, Texture::to_ogl(filter));
}

void ms::Framebuffer::copy_color_from (Framebuffer & frame, Texture::MagFilter filter) {
    frame.use_for_read();
    (*this).use_for_write();
    mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, this->get_width(), this->get_height(), GL_COLOR_BUFFER_BIT, Texture::to_ogl(filter));
}

void ms::Framebuffer::clear_color () {
	glClear(GL_COLOR_BUFFER_BIT);
}

void ms::Framebuffer::clear_depth () {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ms::Framebuffer::use () {
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

void ms::Framebuffer::use_for_write () {
	if(!is_loaded()) {
		load();
	}

	mglBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebuffer);
}

void ms::Framebuffer::use_for_read () {
	if(!is_loaded()) {
		load();
	}
	
	mglBindFramebuffer(GL_READ_FRAMEBUFFER, this->framebuffer);
}

void ms::Framebuffer::_unload () {
	if(!is_default_framebuffer) {
		mglDeleteFramebuffers(1, &this->framebuffer);
        std::for_each(colorTextureAttachments.begin(), colorTextureAttachments.end(), [&] (auto & color) { if (color) { color->unload(); } } );
        std::for_each(colorRenderbufferAttachments.begin(), colorRenderbufferAttachments.end(), [&] (auto & color) { if (color) { color->unload(); } } );
        if(depthTexture) { depthTexture->unload(); }
        if(depthRenderbuffer) { depthRenderbuffer->unload(); }
		this->framebuffer = 0;
	}
}

void ms::Framebuffer::bind_color_buffer (int index, std::unique_ptr<Texture> && texture) {
	this->use();
	texture->use();
	mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, texture->get_underlying_id(), 0);
    colorTextureAttachments[index] = std::move(texture);
}

void ms::Framebuffer::bind_color_buffer (int index, std::unique_ptr<Renderbuffer> && renderbuffer) {
	this->use();
	renderbuffer->use();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, renderbuffer->get_underlying_id());
    colorRenderbufferAttachments[index] = std::move(renderbuffer);
    // should work, but check it
	assert(false);
}

void ms::Framebuffer::copy_framebuffer (Framebuffer & frame, Texture::MagFilter filter) {
    frame.use_for_read();
    (*this).use_for_write();
    mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, this->get_width(), this->get_height(), GL_COLOR_BUFFER_BIT, Texture::to_ogl(filter));
    mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, this->get_width(), this->get_height(), GL_DEPTH_BUFFER_BIT, Texture::to_ogl(filter));
}

void ms::Framebuffer::bind_depth_buffer	(std::unique_ptr<Renderbuffer> && renderbuffer) {
	this->use();
	renderbuffer->use();
	mglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->get_underlying_id());
    depthRenderbuffer = std::move(renderbuffer);
}

void ms::Framebuffer::configure () {
	
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

    isConfigured = true;
}

ms::Framebuffer::Framebuffer(int colorAttachmentsAmount,
                             int renderbufferAttachmentsAmount,
                             int width,
                             int height) :
width{width},
height{height},
colorTextureAttachmentsAmount{colorAttachmentsAmount},
colorRenderbufferAttachmentsAmount{renderbufferAttachmentsAmount},
isDepthTestEnabled{true},
framebuffer{0},
is_default_framebuffer{false} {
    
    colorTextureAttachments.resize(colorAttachmentsAmount);
    colorRenderbufferAttachments.resize(renderbufferAttachmentsAmount);
}

void ms::Framebuffer::set_clear_color (math::vec4 const & color) {
    this->clearingColor = color;
}

ms::Framebuffer::weak_depth_texbuffer ms::Framebuffer::get_depth_texture() {
    return std::weak_ptr<Texture>(depthTexture);
}

ms::Framebuffer::weak_color_texbuffers ms::Framebuffer::get_colors() {
    weak_color_texbuffers weak_colors;
    for(const auto & tex : this->colorTextureAttachments) {
        weak_colors.push_back(std::weak_ptr<Texture>(tex));
    }
    return weak_colors;
}

void ms::Framebuffer::set_depth_test (bool enabled) {
    isDepthTestEnabled = enabled;
}

std::string ms::Framebuffer::get_class () const {
	return "ms::Framebuffer";
}

void ms::Framebuffer::set_underlying_id (GLuint framebufferID) {
	if(!is_default_framebuffer) {
		std::cerr << "ONLY DEFAULT FRAMEBUFFER'S ID CAN BE CHANGED" << std::endl;
		assert(false);
	}
	framebuffer = framebufferID;
}

void ms::Framebuffer::bind_depth_buffer (std::unique_ptr<Texture> && texture) {
    this->use();
    texture->use();
    mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->get_underlying_id(), 0);
    depthTexture = std::move(texture);
}

std::unique_ptr<ms::Framebuffer> ms::Framebuffer::window_framebuffer	(int width, int height) {
	auto framebuffer = std::make_unique<ms::Framebuffer>(0, 0, width, height);
	framebuffer->is_default_framebuffer = true;
	framebuffer->framebuffer = 0;
	return framebuffer;
}



