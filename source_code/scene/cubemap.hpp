//
//  cubemap.hpp
//  ngin
//
//  Created by Mateusz Stompór on 05/06/2018.
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
     * W chwili obecnej służy do reprezentowania map cieni dla świateł punktowych.
     */
    class CubeMap : public Resource {
        
        friend class Framebuffer;
        friend class Shader;
        friend class NGin;
        
    public:

        /**
         * Specyfikuje ścianę cubemapy.
         */
        enum Face {
            right_positive_x = 0,
            left_negative_x = 1,
            top_positive_y = 2,
            bottom_negative_y = 3,
            back_positive_z = 4,
            front_negative_z = 5
        };

        std::string                 get_class           () const override;
        
    private:

                                    CubeMap             (texture::Format             format,
                                                         texture::AssociatedType     associatedType,
                                                         Size<unsigned int>          positiveX,
                                                         Size<unsigned int>          positiveY,
                                                         Size<unsigned int>          positiveZ,
                                                         Size<unsigned int>          negativeX,
                                                         Size<unsigned int>          negativeY,
                                                         Size<unsigned int>          negativeZ,
                                                         std::string                 name = "",
                                                         texture::MinFilter          minFilter = texture::MinFilter::nearest,
                                                         texture::MagFilter          magFilter = texture::MagFilter::nearest,
                                                         texture::Wrapping           sWrapping = texture::Wrapping::repeat,
                                                         texture::Wrapping           tWrapping = texture::Wrapping::repeat,
                                                         texture::Wrapping           rWrapping = texture::Wrapping::repeat,
                                                         unsigned int                mipMapLevel = 0);

                                    CubeMap             (texture::Format             format,
                                                         texture::AssociatedType     associatedType,
                                                         Size<unsigned int>          size,
                                                         std::string                 name = "",
                                                         texture::MinFilter          minFilter = texture::MinFilter::nearest,
                                                         texture::MagFilter          magFilter = texture::MagFilter::nearest,
                                                         texture::Wrapping           sWrapping = texture::Wrapping::repeat,
                                                         texture::Wrapping           tWrapping = texture::Wrapping::repeat,
                                                         texture::Wrapping           rWrapping = texture::Wrapping::repeat,
                                                         unsigned int                mipMapLevel = 0);

                                    CubeMap             (const CubeMap & texture) = delete;
        CubeMap &                   operator =          (const CubeMap & texture) = delete;
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
        Size<unsigned int>          positiveX;
        Size<unsigned int>          positiveY;
        Size<unsigned int>          positiveZ;
        Size<unsigned int>          negativeX;
        Size<unsigned int>          negativeY;
        Size<unsigned int>          negativeZ;
        GLenum                      glInternalFormat;
        GLenum                      glColorFormat;
        GLenum                      glType;
        GLenum                      glTarget;
        GLuint                      glTexture;
        
    };
    
}
