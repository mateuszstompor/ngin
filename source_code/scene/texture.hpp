//
//  texture.hpp
//  ngin
//
//  Created by Mateusz Stompór on 10/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include <iostream>
#include <cstdint>


#include "textureUtilities.hpp"
#include "../resources/resource.hpp"
#include "../utils/proxyOGL.hpp"

namespace ms {
	
	class Texture2D : public Resource {
		
        friend class Renderbuffer;
        friend class Framebuffer;
        friend class Shader;
        
        using byte = std::uint8_t;
        
	public:
        
                                    Texture2D           (texture::Format             format,
                                                         texture::AssociatedType     associatedType,
                                                         unsigned int       width,
                                                         unsigned int       height,
                                                         std::string        name = "",
                                                         texture::MinFilter          minFilter = texture::MinFilter::nearest,
                                                         texture::MagFilter          magFilter = texture::MagFilter::nearest,
                                                         texture::Wrapping           sWrapping = texture::Wrapping::repeat,
                                                         texture::Wrapping           tWrapping = texture::Wrapping::repeat,
                                                         unsigned int       mipMapLevel = 0);

                                    Texture2D           (const Texture2D & texture) = delete;
                    Texture2D &	    operator =			(const Texture2D & texture) = delete;
        
                    std::string		get_class			() const override;
        
                                    ~Texture2D			();
                    void            copy_data           (byte const * data, size_t size);
        
	private:
        
        constexpr   GLuint          get_underlying_id   () const { return glTexture; }
                    void            _load               () override;
                    void            _unload             () override;
                    void             use                ();
    
        std::string                 name;
        texture::Format             format;
        texture::AssociatedType     associatedType;
        texture::MinFilter          minFilter;
        texture::MagFilter          magFilter;
        texture::Wrapping           sWrapping;
        texture::Wrapping           tWrapping;
        unsigned int                mipMapLevel;
        unsigned int                width;
        unsigned int                height;
        byte *                      rawData;
        
        GLenum                      glInternalFormat;
        GLenum                      glColorFormat;
        GLenum                      glType;
        GLenum                      glTarget;
        GLuint                      glTexture;
		
	};
	
}
