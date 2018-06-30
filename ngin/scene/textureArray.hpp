//
//  textureArray.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "textureUtilities.hpp"
#include "../resources/resource.hpp"
#include "../utils/proxyOGL.hpp"

namespace ms {

    /**
     * Wewnętrzna klasa biblioteki, niedostępna dla użytkownika.
     * Służy do reprezentowania tablic tekstur w programach cieniąjących, które ze względu na specyfikę API wymagając odrębnej implementacji.
     */
    class Texture2DArray : public Resource {
    
        friend class Framebuffer;
        friend class Shader;
        
    public:
                                    Texture2DArray           (std::size_t               amount,
                                                              texture::Format           format,
                                                              texture::AssociatedType   associatedType,
                                                              unsigned int              width,
                                                              unsigned int              height,
                                                              std::string               name = "",
                                                              texture::MinFilter        minFilter = texture::MinFilter::nearest,
                                                              texture::MagFilter        magFilter = texture::MagFilter::nearest,
                                                              texture::Wrapping         sWrapping = texture::Wrapping::repeat,
                                                              texture::Wrapping         tWrapping = texture::Wrapping::repeat,
                                                              unsigned int              mipMapLevel = 0);
        
        std::string                 get_class               () const override;
                                    ~Texture2DArray         () = default;
    private:
                                    Texture2DArray          (const Texture2DArray & texture) = delete;
        Texture2DArray &            operator =              (const Texture2DArray & texture) = delete;
        constexpr   GLuint          get_underlying_id       () const { return glTexture; }
        void                        _load                   () override;
        void                        _unload                 () override;
        void                        use                     ();
        
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
        std::size_t                 layerAmount;
        GLenum                      glInternalFormat;
        GLenum                      glColorFormat;
        GLenum                      glType;
        GLenum                      glTarget;
        GLuint                      glTexture;
        
    };
    
}
