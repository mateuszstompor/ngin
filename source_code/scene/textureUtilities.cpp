//
//  textureUtilities.cpp
//  ngin
//
//  Created by Mateusz Stompór on 03/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "textureUtilities.hpp"

namespace ms {
    
    GLenum underlying_type (texture::AssociatedType associatedType, texture::Format format) {
        if (associatedType == texture::AssociatedType::FLOAT) {
            if (format == texture::Format::rgb_16_16_16) { return GL_RGB16F; }
            else if (format == texture::Format::rgb_32_32_32) { return GL_RGB32F; }
            else if (format == texture::Format::depth_32) { return GL_DEPTH_COMPONENT32F; }
        } else if (associatedType == texture::AssociatedType::UNSIGNED_BYTE) {
            if (format == texture::Format::rgba_8_8_8_8) { return GL_RGBA8; }
            else if (format == texture::Format::rgb_8_8_8) { return GL_RGB8; }
            else if (format == texture::Format::r_8) { return GL_R8; }
        } else if (associatedType == texture::AssociatedType::UNSIGNED_INT) {
            if (format == texture::Format::depth_16) { return GL_DEPTH_COMPONENT16; }
            else if (format == texture::Format::depth_24) { return GL_DEPTH_COMPONENT24; }
        } else if (associatedType == texture::AssociatedType::UNSIGNED_SHORT) {
            if (format == texture::Format::depth_16) { return GL_DEPTH_COMPONENT16; }
        }
        std::cerr << "Format not supported\n";
        assert(false);
    }
    
    GLenum to_ogl (texture::MinFilter          minFilter) {
        switch (minFilter) {
            case texture::MinFilter::nearest:
                return GL_NEAREST;
            case texture::MinFilter::linear:
                return GL_LINEAR;
            case texture::MinFilter::nearest_mipmap_nearest:
                return GL_NEAREST_MIPMAP_NEAREST;
            case texture::MinFilter::linear_mipmap_nearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            case texture::MinFilter::nearest_mipmap_linear:
                return GL_NEAREST_MIPMAP_LINEAR;
            case texture::MinFilter::linear_mipmap_linear:
                return GL_LINEAR_MIPMAP_LINEAR;
            default:
                //critical error
                std::cerr << "Filter type not supported\n";
                assert(false);
        }
    }
    
    GLenum to_ogl (texture::MagFilter          magFilter) {
        switch (magFilter) {
            case texture::MagFilter::linear:
                return GL_LINEAR;
            case texture::MagFilter::nearest:
                return GL_NEAREST;
            default:
                std::cerr << "Filter type not supported\n";
                assert(false);
        }
    }
    
    GLenum to_ogl (texture::Wrapping           wrapping) {
        switch (wrapping) {
            case texture::Wrapping::mirrored_repeat:
                return GL_MIRRORED_REPEAT;
            case texture::Wrapping::repeat:
                return GL_REPEAT;
            case texture::Wrapping::clamp_to_edge:
                return GL_CLAMP_TO_EDGE;
                //DOESNT RUN ON IOS
    #ifndef ios_build
            case texture::Wrapping::clamp_to_border:
                return GL_CLAMP_TO_BORDER;
    #endif
            default:
                std::cerr << "Wrapping type not supported\n";
                assert(false);
        }
        
    }
    
    GLenum to_ogl (texture::Format             format) {
        switch (format) {
            case texture::Format::rgb_8_8_8:
                return GL_RGB;
            case texture::Format::rgb_16_16_16:
                return GL_RGB;
            case texture::Format::rgba_8_8_8_8:
                return GL_RGBA;
            case texture::Format::r_8:
                return GL_RED;
            case texture::Format::depth_16:
            case texture::Format::depth_24:
            case texture::Format::depth_32:
                return GL_DEPTH_COMPONENT;
            default:
                std::cerr << "format not supported\n";
                assert(false);
        }
    }
    
    GLenum to_ogl (texture::AssociatedType type) {
        switch (type) {
                
            case texture::AssociatedType::FLOAT:
                return GL_FLOAT;
            case texture::AssociatedType::UNSIGNED_BYTE:
                return GL_UNSIGNED_BYTE;
            case texture::AssociatedType::UNSIGNED_INT:
                return GL_UNSIGNED_INT;
            case texture::AssociatedType::UNSIGNED_SHORT:
                return GL_UNSIGNED_SHORT;
            default:
                std::cerr << "type not supported\n";
                assert(false);
        }
    }
    
    GLenum to_ogl (texture::Type type) {
        switch (type) {
            case texture::Type::tex_2d:
                return GL_TEXTURE_2D;
            default:
                std::cerr << "Texture type not supported\n";
                assert(false);
        }
    }
    
    int channels_amount (texture::Format f) {
        switch (f) {
            case texture::Format::depth_16:
            case texture::Format::depth_24:
            case texture::Format::depth_32:
            case texture::Format::r_8:
                return 1;
            case texture::Format::rgba_8_8_8_8:
                return 4;
            case texture::Format::rgb_8_8_8:
            case texture::Format::rgb_16_16_16:
            case texture::Format::rgb_32_32_32:
                return 3;
            default:
                std::cerr << "critical error, format not handled\n";
                assert(false);
        }
    }
    
}
