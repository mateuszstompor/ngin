//
//  texture_utilities.h
//  ngin
//
//  Created by Mateusz Stompór on 03/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "../utils/proxyOGL.hpp"

namespace ms {

    namespace texture {
        
        enum class MinFilter {
            nearest,
            linear,
            nearest_mipmap_nearest,
            linear_mipmap_nearest,
            nearest_mipmap_linear,
            linear_mipmap_linear
        };
        
        enum class MagFilter {
            nearest,
            linear
        };
        
        enum class Wrapping {
            repeat,
            mirrored_repeat,
            clamp_to_edge,
            clamp_to_border
        };
        
        enum class Type {
            tex_2d,
        };
        
        enum class Format {
            rgb_32_32_32,
            rgb_16_16_16,
            rgb_8_8_8,
            rgba_8_8_8_8,
            r_8,
            depth_16,
            depth_24,
            depth_32
        };
        
        enum class AssociatedType {
            FLOAT,
            UNSIGNED_BYTE,
            UNSIGNED_INT,
            UNSIGNED_SHORT
        };
        
    }
    
    GLenum  to_ogl           (texture::MinFilter          minFilter);
    GLenum  to_ogl           (texture::MagFilter          magFilter);
    GLenum  to_ogl           (texture::Wrapping           wrapping);
    GLenum  to_ogl           (texture::Format             format);
    GLenum  to_ogl           (texture::AssociatedType     type);
    GLenum  to_ogl           (texture::Type               type);
    GLenum  underlying_type  (texture::AssociatedType     associatedType,
                              texture::Format             format);
    int     channels_amount  (texture::Format format);

    
}

