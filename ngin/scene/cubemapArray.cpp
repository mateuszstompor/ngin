//
//  cubemapArray.cpp
//  ngin
//
//  Created by Mateusz Stompór on 13/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "cubemapArray.hpp"

std::string ms::CubemapArray::get_class () const {
    return "ms::CubemapArray";
}

ms::CubemapArray::CubemapArray    (texture::Format             format,
                                   texture::AssociatedType     associatedType,
                                   math::Size<unsigned int>          size,
                                   unsigned int                layerAmount,
                                   std::string                 name,
                                   texture::MinFilter          minFilter,
                                   texture::MagFilter          magFilter,
                                   texture::Wrapping           sWrapping,
                                   texture::Wrapping           tWrapping,
                                   texture::Wrapping           rWrapping,
                                   unsigned int                mipMapLevel) :
name{name},
format(format),
associatedType{associatedType},
minFilter{minFilter},
magFilter{magFilter},
sWrapping{sWrapping},
tWrapping{tWrapping},
rWrapping{rWrapping},
mipMapLevel{mipMapLevel},
size{size},
glTexture{0},
#ifndef ios_build
glTarget{GL_TEXTURE_CUBE_MAP_ARRAY},
#endif
glType{to_ogl(associatedType)},
glColorFormat{to_ogl(format)},
glInternalFormat{underlying_type(this->associatedType, this->format)},
amountOfLayers {layerAmount} { }

void ms::CubemapArray::_load () {
    mglGenTextures(1, &glTexture);
    mglBindTexture(glTarget, glTexture);
    mglTexImage3D(glTarget, mipMapLevel, glInternalFormat, size.width, size.height, 6 * static_cast<int>(amountOfLayers), 0, glColorFormat, glType, nullptr);
    mglTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, to_ogl(minFilter));
    mglTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, to_ogl(magFilter));
    mglTexParameteri(glTarget, GL_TEXTURE_WRAP_S, to_ogl(sWrapping));
    mglTexParameteri(glTarget, GL_TEXTURE_WRAP_T, to_ogl(tWrapping));
    mglTexParameteri(glTarget, GL_TEXTURE_WRAP_R, to_ogl(rWrapping));
}

void ms::CubemapArray::_unload () {
    mglDeleteTextures(1, &glTexture);
    glTexture = 0;
}

void ms::CubemapArray::use () {
    load();
    mglBindTexture(glTarget, glTexture);
}
