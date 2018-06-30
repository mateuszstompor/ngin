//
//  cubemap.cpp
//  ngin
//
//  Created by Mateusz Stompór on 05/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "cubemap.hpp"

ms::CubeMap::CubeMap    (texture::Format             internalFormat,
                         texture::AssociatedType     associatedType,
                         math::Size<unsigned int>          positiveX,
                         math::Size<unsigned int>          positiveY,
                         math::Size<unsigned int>          positiveZ,
                         math::Size<unsigned int>          negativeX,
                         math::Size<unsigned int>          negativeY,
                         math::Size<unsigned int>          negativeZ,
                         std::string                 name,
                         texture::MinFilter          minFilter,
                         texture::MagFilter          magFilter,
                         texture::Wrapping           sWrapping,
                         texture::Wrapping           tWrapping,
                         texture::Wrapping           rWrapping,
                         unsigned int                mipMapLevel) :
name{name},
format{internalFormat},
associatedType{associatedType},
minFilter{minFilter},
magFilter{magFilter},
sWrapping{sWrapping},
tWrapping{tWrapping},
rWrapping{rWrapping},
mipMapLevel{mipMapLevel},
positiveX{positiveX},
positiveY{positiveY},
positiveZ{positiveZ},
negativeX{negativeX},
negativeY{negativeY},
negativeZ{negativeZ},
glTexture{0},
glTarget{GL_TEXTURE_CUBE_MAP},
glType{to_ogl(associatedType)},
glColorFormat{to_ogl(internalFormat)},
glInternalFormat{underlying_type(associatedType, internalFormat)} {}

ms::CubeMap::CubeMap    (texture::Format             format,
                         texture::AssociatedType     associatedType,
                         math::Size<unsigned int>          size,
                         std::string                 name,
                         texture::MinFilter          minFilter,
                         texture::MagFilter          magFilter,
                         texture::Wrapping           sWrapping,
                         texture::Wrapping           tWrapping,
                         texture::Wrapping           rWrapping,
                         unsigned int                mipMapLevel) :
CubeMap {format, associatedType, size, size, size, size, size, size, name, minFilter, magFilter, sWrapping, tWrapping, rWrapping, mipMapLevel} {}

void ms::CubeMap::_load () {
    mglGenTextures(1, &glTexture);
    mglBindTexture(glTarget, glTexture);

    mglTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, mipMapLevel, glInternalFormat, positiveX.width, positiveX.height, 0, glColorFormat, glType, nullptr);
    mglTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, mipMapLevel, glInternalFormat, negativeX.width, negativeX.height, 0, glColorFormat, glType, nullptr);
    
    mglTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, mipMapLevel, glInternalFormat, positiveY.width, positiveY.height, 0, glColorFormat, glType, nullptr);
    mglTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, mipMapLevel, glInternalFormat, negativeY.width, negativeY.height, 0, glColorFormat, glType, nullptr);
    
    mglTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, mipMapLevel, glInternalFormat, positiveZ.width, positiveZ.height, 0, glColorFormat, glType, nullptr);
    mglTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, mipMapLevel, glInternalFormat, negativeZ.width, negativeZ.height, 0, glColorFormat, glType, nullptr);
    
    mglTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, to_ogl(minFilter));
    mglTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, to_ogl(magFilter));
    mglTexParameteri(glTarget, GL_TEXTURE_WRAP_S, to_ogl(sWrapping));
    mglTexParameteri(glTarget, GL_TEXTURE_WRAP_T, to_ogl(tWrapping));
    mglTexParameteri(glTarget, GL_TEXTURE_WRAP_R, to_ogl(rWrapping));
    mglBindTexture(glTarget, 0);
}

void ms::CubeMap::_unload () {
    mglDeleteTextures(1, &glTexture);
    glTexture = 0;
}

void ms::CubeMap::use () {
    load();
    mglBindTexture(glTarget, glTexture);
}

std::string ms::CubeMap::get_class () const {
    return "CubeMap";
}
