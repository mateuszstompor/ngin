//
//  textureArray.cpp
//  ngin
//
//  Created by Mateusz Stompór on 03/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "textureArray.hpp"

ms::Texture2DArray::Texture2DArray  ( std::size_t               amount,
                                      texture::Format           internalFormat,
                                      texture::AssociatedType   associatedType,
                                      unsigned int              width,
                                      unsigned int              height,
                                      std::string               name,
                                      texture::MinFilter        minFilter,
                                      texture::MagFilter        magFilter,
                                      texture::Wrapping         sWrapping,
                                      texture::Wrapping         tWrapping,
                                      unsigned int              mipMapLevel) :
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
glTexture{0},
glTarget{GL_TEXTURE_2D_ARRAY},
glType{to_ogl(associatedType)},
glColorFormat{to_ogl(internalFormat)},
glInternalFormat{underlying_type(this->associatedType, this->format)},
layerAmount {amount} {
    
}

std::string ms::Texture2DArray::get_class () const {
    return "Texture2DArray";
}

void ms::Texture2DArray::_load () {
    mglGenTextures(1, &glTexture);
    mglBindTexture(GL_TEXTURE_2D_ARRAY, glTexture);
    mglTexImage3D(GL_TEXTURE_2D_ARRAY, mipMapLevel, glInternalFormat, width, height, static_cast<int>(layerAmount), 0, glColorFormat, glType, nullptr);
    mglTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, to_ogl(minFilter));
    mglTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, to_ogl(magFilter));
    mglTexParameteri(glTarget, GL_TEXTURE_WRAP_S, to_ogl(sWrapping));
    mglTexParameteri(glTarget, GL_TEXTURE_WRAP_T, to_ogl(tWrapping));
}

void ms::Texture2DArray::_unload () {
    mglDeleteTextures(1, &glTexture);
    glTexture = 0;
}

void ms::Texture2DArray::use () {
    load();
    mglBindTexture(GL_TEXTURE_2D_ARRAY, glTexture);
}
