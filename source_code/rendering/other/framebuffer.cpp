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
		std::cerr << "FRAMEBUFFER IS NOT COMPLETE\n";
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
	mglClearColor(clearingColor.x(), clearingColor.y(), clearingColor.z(), clearingColor.w());
	mglClear(GL_COLOR_BUFFER_BIT);
	mglClear(GL_DEPTH_BUFFER_BIT);
	
}

void ms::Framebuffer::copy_depth_from (Framebuffer & frame, texture::MagFilter filter) {
	frame.use_for_read();
	use_for_write();
	mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, width, height, GL_DEPTH_BUFFER_BIT, to_ogl(filter));
}

void ms::Framebuffer::copy_color_from (Framebuffer & frame, texture::MagFilter filter) {
    frame.use_for_read();
    use_for_write();
    mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, width, height, GL_COLOR_BUFFER_BIT, to_ogl(filter));
}

void ms::Framebuffer::clear_color () {
	mglClear(GL_COLOR_BUFFER_BIT);
}

void ms::Framebuffer::clear_depth () {
	mglClear(GL_DEPTH_BUFFER_BIT);
}

void ms::Framebuffer::use () {
	load();
	
	if(isDepthTestEnabled) {
		mglEnable(GL_DEPTH_TEST);
		mglDepthFunc(GL_LEQUAL);
	} else {
		mglDisable(GL_DEPTH_TEST);
	}
	
    if(isBlendingEnabled) {
        mglEnable(GL_BLEND);
        mglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        mglDisable(GL_BLEND);
    }
    
	mglViewport(0, 0, width, height);
	mglBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
}

void ms::Framebuffer::use_for_write () {
	load();
	mglBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebuffer);
}

void ms::Framebuffer::use_for_read () {
	load();
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

void ms::Framebuffer::bind_color_buffer (int index, std::unique_ptr<Texture2D> && texture) {
	use();
	texture->use();
	mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(index), GL_TEXTURE_2D, texture->get_underlying_id(), 0);
    colorTextureAttachments[index] = std::move(texture);
}

void ms::Framebuffer::bind_color_buffer (int index, std::unique_ptr<Renderbuffer> && renderbuffer) {
	use();
	renderbuffer->use();
	mglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(index), GL_RENDERBUFFER, renderbuffer->get_underlying_id());
    colorRenderbufferAttachments[index] = std::move(renderbuffer);
    // should work, but check it
	assert(false);
}

void ms::Framebuffer::copy_framebuffer (Framebuffer & frame, texture::MagFilter filter) {
    frame.use_for_read();
    use_for_write();
    mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, width, height, GL_COLOR_BUFFER_BIT, to_ogl(filter));
    mglBlitFramebuffer(0, 0, frame.get_width(), frame.get_height(), 0, 0, width, height, GL_DEPTH_BUFFER_BIT, to_ogl(filter));
}

void ms::Framebuffer::bind_depth_buffer	(std::unique_ptr<Renderbuffer> && renderbuffer) {
	this->use();
	renderbuffer->use();
	mglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->get_underlying_id());
    depthRenderbuffer = std::move(renderbuffer);
}

void ms::Framebuffer::configure () {
	use();
    if (colorTextureAttachmentsAmount > 0) {
        GLenum* attachments;
		attachments = new GLenum[colorTextureAttachmentsAmount];
        for(int i = 0; i < colorTextureAttachmentsAmount; ++i) {
            attachments[i] = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i);
        }
        mglDrawBuffers(colorTextureAttachmentsAmount, attachments);
		delete attachments;
    } else {
		GLenum attachments = { GL_NONE };
        mglDrawBuffers(1, &attachments);
        mglReadBuffer(GL_NONE);
    }
    
    if(!is_complete()){
        std::cerr << "FATAL ERROR\n";
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
is_default_framebuffer{false},
isBlendingEnabled{false} {
    
    colorTextureAttachments.resize(static_cast<size_t>(colorAttachmentsAmount));
    colorRenderbufferAttachments.resize(static_cast<size_t>(renderbufferAttachmentsAmount));
}

void ms::Framebuffer::set_clear_color (math::vec4 const & color) {
    clearingColor = color;
}

ms::Framebuffer::weak_depth_texbuffer ms::Framebuffer::get_depth_texture() {
    return std::weak_ptr<Texture2D>(depthTexture);
}

ms::Framebuffer::weak_color_texbuffers ms::Framebuffer::get_colors() {
    weak_color_texbuffers weak_colors;
    for(const auto & tex : this->colorTextureAttachments) {
        weak_colors.push_back(std::weak_ptr<Texture2D>(tex));
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
		std::cerr << "ONLY DEFAULT FRAMEBUFFER'S ID CAN BE CHANGED\n";
		assert(false);
	}
	framebuffer = framebufferID;
}

void ms::Framebuffer::set_blending (bool enabled) {
    isBlendingEnabled = enabled;
}

void ms::Framebuffer::bind_depth_buffer (std::unique_ptr<Texture2D> && texture) {
    use();
    texture->use();
    mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->get_underlying_id(), 0);
    depthTexture = std::move(texture);
}

void ms::Framebuffer::bind_depth_buffer (std::unique_ptr<Texture2DArray> & texture, std::size_t layer) {
    use();
    texture->use();
    mglFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->get_underlying_id(), 0, static_cast<int>(layer));
}

void ms::Framebuffer::bind_depth_buffer (std::unique_ptr<CubeMap> & texture, /*CubeMap::Face*/ int face) {
    use();
    texture->use();
    mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, texture->get_underlying_id(), 0);
}

std::unique_ptr<ms::Framebuffer> ms::Framebuffer::window_framebuffer	(int width, int height) {
    auto framebuffer = std::unique_ptr<Framebuffer>(new Framebuffer{0, 0, width, height});
	framebuffer->is_default_framebuffer = true;
	framebuffer->framebuffer = 0;
	return framebuffer;
}



