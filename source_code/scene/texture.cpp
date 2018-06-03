//
//  texture.cpp
//  ngin
//
//  Created by Mateusz Stompór on 10/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "texture.hpp"

ms::Texture2D::Texture2D 	(texture::Format            internalFormat,
							 texture::AssociatedType    associatedType,
							 unsigned int               width,
							 unsigned int               height,
							 std::string                name,
							 texture::MinFilter         minFilter,
							 texture::MagFilter         magFilter,
							 texture::Wrapping          sWrapping,
							 texture::Wrapping          tWrapping,
							 unsigned int               mipMapLevel) :
                                                            name{name},
                                                            format(internalFormat),
                                                            associatedType{associatedType},
                                                            minFilter{minFilter},
                                                            magFilter{magFilter},
                                                            sWrapping{sWrapping},
                                                            tWrapping{tWrapping},
                                                            mipMapLevel{mipMapLevel},
                                                            width{width},
                                                            height{height},
                                                            rawData{nullptr},
                                                            glTexture{0},
                                                            glTarget{GL_TEXTURE_2D},
                                                            glType{to_ogl(associatedType)},
                                                            glColorFormat{to_ogl(internalFormat)},
                                                            glInternalFormat{underlying_type(associatedType, format)} {

}

void ms::Texture2D::_load () {
		mglGenTextures(1, &glTexture);
		mglBindTexture(glTarget, glTexture);
		mglTexImage2D(glTarget, mipMapLevel, glInternalFormat, width, height, 0, glColorFormat, glType, rawData);
        if (rawData) {
            delete [] rawData;
            rawData = nullptr;
        }
		mglTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, to_ogl(minFilter));
		mglTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, to_ogl(magFilter));
		mglTexParameteri(glTarget, GL_TEXTURE_WRAP_S, to_ogl(sWrapping));
		mglTexParameteri(glTarget, GL_TEXTURE_WRAP_T, to_ogl(tWrapping));
		mglBindTexture(glTarget, 0);
}

void ms::Texture2D::use () {
    load();
	mglBindTexture(glTarget, glTexture);
}

void ms::Texture2D::_unload () {
	mglDeleteTextures(1, &glTexture);
	glTexture = 0;
}

void ms::Texture2D::copy_data (byte const * data, size_t size) {
    if (rawData) { delete [] rawData; }
    rawData = new byte [size];
    std::memcpy(rawData, data, size);
}

std::string ms::Texture2D::get_class () const {
    return "ms::Texture";
}

ms::Texture2D::~Texture2D() {
    if (rawData) { delete [] rawData; }
}
