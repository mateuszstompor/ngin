//
//  renderbufferOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "renderbufferOGL.hpp"

ms::RenderbufferOGL::RenderbufferOGL (Texture::Format			format,
									  Texture::AssociatedType	associatedType,
									  unsigned int 				mipMapLevel,
									  unsigned int 				width,
									  unsigned int 				height
									  ) : Renderbuffer(format, associatedType, mipMapLevel, width, height), renderBuffer(0) {
	
	internalFormat = RenderbufferOGL::get_internal_format(format, associatedType);
	
}

ms::RenderbufferOGL::RenderbufferOGL (Texture::Format			format,
									  Texture::AssociatedType	associatedType,
									  unsigned int 				width,
									  unsigned int 				height
									  ) : RenderbufferOGL(format, associatedType, 0, width, height) {}

std::string ms::RenderbufferOGL::get_class () {
	return "ms::RenderbufferOGL";
}

void ms::RenderbufferOGL::_load () {
	mglGenRenderbuffers(1, &this->renderBuffer);
	mglBindRenderbuffer(GL_RENDERBUFFER, this->renderBuffer);
	mglRenderbufferStorage(GL_RENDERBUFFER, RenderbufferOGL::get_internal_format(format, associatedType), width, height);
}

void ms::RenderbufferOGL::_unload () {
	mglDeleteRenderbuffers(1, &this->renderBuffer);
	this->renderBuffer = 0;
}

void ms::RenderbufferOGL::use () {
	if(!is_loaded()) {
		load();
	}
	
	mglBindRenderbuffer(GL_RENDERBUFFER, this->renderBuffer);
}

GLenum ms::RenderbufferOGL::get_internal_format (Texture::Format format, Texture::AssociatedType associatedType) {
	
	if(associatedType == Texture::AssociatedType::UNSIGNED_BYTE) {
		switch (format) {
		#ifndef ios_build
			case Texture::Format::depth_32:
			return GL_DEPTH_COMPONENT32;
		#endif
			case Texture::Format::depth_16:
			return GL_DEPTH_COMPONENT16;
			case Texture::Format::depth_24:
			return GL_DEPTH_COMPONENT24;
			default:
			break;
		}
	}
	
	std::cerr << "CRITICAL ERROR, FORMAT NOT SUPPORTED" << std::endl;
	assert(false);
}

GLuint ms::RenderbufferOGL::get_underlying_id () {
	return renderBuffer;
}
