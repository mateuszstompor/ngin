//
//  renderbuffer.cpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "renderbuffer.hpp"

ms::Renderbuffer::Renderbuffer (Texture::Format             format,
                                Texture::AssociatedType     associatedType,
                                unsigned int 				mipMapLevel,
                                unsigned int 				width,
                                unsigned int 				height ) :
                                                                        format{format},
                                                                        associatedType{associatedType},
                                                                        mipMapLevel{mipMapLevel},
                                                                        width{width},
                                                                        height{height},
                                                                        internalFormat{TextureOGL::underlying_type(associatedType,format)},
                                                                        renderBuffer{0} {
	
}

ms::Renderbuffer::Renderbuffer (Texture::Format			    format,
                                Texture::AssociatedType	    associatedType,
                                unsigned int 				width,
                                unsigned int 				height ) : Renderbuffer(format, associatedType, 0, width, height) {}

std::string ms::Renderbuffer::get_class () {
	return "ms::RenderbufferOGL";
}

void ms::Renderbuffer::_load () {
	mglGenRenderbuffers(1, &this->renderBuffer);
	mglBindRenderbuffer(GL_RENDERBUFFER, this->renderBuffer);
	mglRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}

void ms::Renderbuffer::_unload () {
	mglDeleteRenderbuffers(1, &this->renderBuffer);
	this->renderBuffer = 0;
}

void ms::Renderbuffer::use () {
	if(!is_loaded()) {
		load();
	}
	
	mglBindRenderbuffer(GL_RENDERBUFFER, this->renderBuffer);
}

GLuint ms::Renderbuffer::get_underlying_id () {
	return renderBuffer;
}
