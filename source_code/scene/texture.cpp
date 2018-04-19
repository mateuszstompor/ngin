//
//  texture.cpp
//  ngin
//
//  Created by Mateusz Stompór on 10/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "texture.hpp"

ms::Texture::Texture	(Texture::Type      textureType,
                         Format             internalFormat,
                         AssociatedType     associatedType,
                         unsigned int       width,
                         unsigned int       height,
                         std::string        name,
                         MinFilter          minFilter,
                         MagFilter          magFilter,
                         Wrapping           sWrapping,
                         Wrapping           tWrapping,
                         unsigned int       mipMapLevel) :
                                                            type{textureType},
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
                                                            glTarget{to_ogl(textureType)},
                                                            glType{Texture::to_ogl(associatedType)},
                                                            glColorFormat{Texture::to_ogl(internalFormat)},
                                                            glInternalFormat{Texture::underlying_type(this->associatedType, this->format)} {

}

void ms::Texture::_load () {
		mglGenTextures(1, &this->glTexture);
		
		mglBindTexture(this->glTarget, this->glTexture);

		mglTexImage2D(this->glTarget, this->mipMapLevel, this->glInternalFormat, width, height, 0, this->glColorFormat, this->glType, rawData);
    
        if (rawData) {
            delete [] this->rawData;
            rawData = nullptr;
        }
		
		mglTexParameteri(this->glTarget, GL_TEXTURE_MIN_FILTER, to_ogl(this->minFilter));
		mglTexParameteri(this->glTarget, GL_TEXTURE_MAG_FILTER, to_ogl(this->magFilter));
		mglTexParameteri(this->glTarget, GL_TEXTURE_WRAP_S, to_ogl(this->sWrapping));
		mglTexParameteri(this->glTarget, GL_TEXTURE_WRAP_T, to_ogl(this->tWrapping));
		
		mglBindTexture(this->glTarget, 0);
}

void ms::Texture::use () {
	if(!is_loaded())
		load();
	
	mglBindTexture(this->glTarget, this->glTexture);
}

void ms::Texture::_unload () {
	mglDeleteTextures(1, &this->glTexture);
	this->glTexture = 0;
}

GLenum ms::Texture::to_ogl (MinFilter minFilter) {
	switch (minFilter) {
		case Texture::MinFilter::nearest:
			return GL_NEAREST;
		case Texture::MinFilter::linear:
			return GL_LINEAR;
		case Texture::MinFilter::nearest_mipmap_nearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		case Texture::MinFilter::linear_mipmap_nearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case Texture::MinFilter::nearest_mipmap_linear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case Texture::MinFilter::linear_mipmap_linear:
			return GL_LINEAR_MIPMAP_LINEAR;
		default:
			//critical error
            std::cerr << "Filter type not supported" << std::endl;
			assert(false);
	}
}

GLenum ms::Texture::to_ogl (MagFilter magFilter) {
	switch (magFilter) {
		case Texture::MagFilter::linear:
			return GL_LINEAR;
		case Texture::MagFilter::nearest:
			return GL_NEAREST;
		default:
			std::cerr << "Filter type not supported" << std::endl;
			assert(false);
	}
}

GLenum ms::Texture::to_ogl (Wrapping wrapping) {
	switch (wrapping) {
		case Texture::Wrapping::mirrored_repeat:
			return GL_MIRRORED_REPEAT;
		case Texture::Wrapping::repeat:
			return GL_REPEAT;
		case Texture::Wrapping::clamp_to_edge:
			return GL_CLAMP_TO_EDGE;
	//DOESNT RUN ON IOS
	#ifndef ios_build
		case Texture::Wrapping::clamp_to_border:
			return GL_CLAMP_TO_BORDER;
	#endif
		default:
			std::cerr << "Wrapping type not supported" << std::endl;
			assert(false);
	}
    
}

GLenum	ms::Texture::to_ogl (Texture::Type type) {
	switch (type) {
		case ms::Texture::Type::tex_2d:
			return GL_TEXTURE_2D;
		case ms::Texture::Type::tex_cube_map:
			return GL_TEXTURE_CUBE_MAP;
		default:
            std::cerr << "Texture type not supported" << std::endl;
            assert(false);
	}
}

GLenum ms::Texture::to_ogl (Format format) {
	switch (format) {
		case Format::rgb_8_8_8:
			return GL_RGB;
		case Format::rgb_16_16_16:
			return GL_RGB;
		case Format::rgba_8_8_8_8:
			return GL_RGBA;
		case Format::r_8:
			return GL_RED;
        case Format::depth_16:
        case Format::depth_24:
        case Format::depth_32:
            return GL_DEPTH_COMPONENT;
		default:
			std::cerr << "format not supported" << std::endl;
			assert(false);
	}
}

GLenum ms::Texture::to_ogl (AssociatedType type) {
	switch (type) {
        
		case Texture::AssociatedType::FLOAT:
			return GL_FLOAT;
		case Texture::AssociatedType::UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
        case Texture::AssociatedType::UNSIGNED_INT:
            return GL_UNSIGNED_INT;
        case Texture::AssociatedType::UNSIGNED_SHORT:
            return GL_UNSIGNED_SHORT;
		default:
			std::cerr << "type not supported" << std::endl;
			assert(false);
	}
}

void ms::Texture::copy_data (byte* data, size_t size) {
    if (rawData) { delete [] rawData; }
    rawData = new byte [size];
    std::memcpy(rawData, data, size);
}

std::string ms::Texture::get_class () const {
    return "ms::Texture";
}

int ms::Texture::channels_amount () const {
    switch (this->format) {
        case Format::depth_16:
        case Format::depth_24:
        case Format::depth_32:
        case Format::r_8:
            return 1;
        case Format::rgba_8_8_8_8:
            return 4;
        case Format::rgb_8_8_8:
        case Format::rgb_16_16_16:
            return 3;
        default:
            std::cerr << "critical error, format not handled" << std::endl;
            assert(false);
    }
}

ms::Texture::~Texture() {
    if (rawData) { delete [] rawData; }
}


GLenum ms::Texture::underlying_type (AssociatedType associatedType, Format format) {
	if (associatedType == AssociatedType::FLOAT) {
		if (format == Texture::Format::rgb_16_16_16) {
			return GL_RGB16F;
        } else if (format == Texture::Format::depth_32) {
            return GL_DEPTH_COMPONENT32F;
        }
	} else if (associatedType == AssociatedType::UNSIGNED_BYTE) {
		if (format == Texture::Format::rgba_8_8_8_8) {
			return GL_RGBA8;
		} else if (format == Texture::Format::rgb_8_8_8) {
			return GL_RGB8;
		} else if (format == Texture::Format::r_8) {
			return GL_R8;
        }
    } else if (associatedType == AssociatedType::UNSIGNED_INT) {
        if (format == Texture::Format::depth_16) {
            return GL_DEPTH_COMPONENT16;
        } else if (format == Texture::Format::depth_24) {
            return GL_DEPTH_COMPONENT24;
        }
    } else if (associatedType == AssociatedType::UNSIGNED_SHORT) {
        if (format == Texture::Format::depth_16) {
            return GL_DEPTH_COMPONENT16;
        }
    }
		
	std::cerr << "Format not supported" << std::endl;
	assert(false);
}

