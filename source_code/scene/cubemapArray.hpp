//
//  cubemapArray.hpp
//  ngin
//
//  Created by Mateusz Stompór on 13/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "textureUtilities.hpp"
#include "../resources/resource.hpp"
#include "../utils/proxyOGL.hpp"
#include "../other/size.hpp"

namespace ms {
    
    /**
     * Wewnętrzna klasa biblioteki, niedostępna dla użytkownika.
     * Służy do reprezentowania tablic tekstur w programach cieniąjących, które ze względu na specyfikę API wymagając odrębnej implementacji.
     */
    class CubemapArray : public Resource {
    
        friend class NGin;
        friend class Framebuffer;
        friend class Shader;
        
    public:
    
        std::string                 get_class           () const override;
        
    private:
                                    CubemapArray        (texture::Format             format,
                                                         texture::AssociatedType     associatedType,
                                                         Size<unsigned int>          size,
                                                         unsigned int                layerAmount,
                                                         std::string                 name = "",
                                                         texture::MinFilter          minFilter = texture::MinFilter::nearest,
                                                         texture::MagFilter          magFilter = texture::MagFilter::nearest,
                                                         texture::Wrapping           sWrapping = texture::Wrapping::repeat,
                                                         texture::Wrapping           tWrapping = texture::Wrapping::repeat,
                                                         texture::Wrapping           rWrapping = texture::Wrapping::repeat,
                                                         unsigned int                mipMapLevel = 0);
                                    CubemapArray        (const CubemapArray & texture) = delete;
        CubemapArray &              operator =          (const CubemapArray & texture) = delete;
        constexpr   GLuint          get_underlying_id   () const { return glTexture; }
        void                        _load               () override;
        void                        _unload             () override;
        void                        use                 ();
        
        std::string                 name;
        texture::Format             format;
        texture::AssociatedType     associatedType;
        texture::MinFilter          minFilter;
        texture::MagFilter          magFilter;
        texture::Wrapping           sWrapping;
        texture::Wrapping           tWrapping;
        texture::Wrapping           rWrapping;
        unsigned int                mipMapLevel;
        Size<unsigned int>          size;
        unsigned int                amountOfLayers;
        
        GLenum                      glInternalFormat;
        GLenum                      glColorFormat;
        GLenum                      glType;
        GLenum                      glTarget;
        GLuint                      glTexture;
        
    };
 
}
