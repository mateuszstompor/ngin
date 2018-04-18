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
	
	internalFormat = TextureOGL::underlying_type(associatedType,format);

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
	mglRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
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

GLuint ms::RenderbufferOGL::get_underlying_id () {
	return renderBuffer;
}
