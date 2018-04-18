//
//  texture.hpp
//  ngin
//
//  Created by Mateusz Stompór on 10/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#include <memory>
#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include <iostream>

#include "../resources/resource.hpp"
#include "../utils/ogl/proxyOGL.hpp"

namespace ms {
	
	class Texture : public Resource {
		
        using byte = u_int8_t;
        
	public:
        
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
            tex_cube_map
        };
        
        enum class Format {
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
        
                                    Texture			    (Texture::Type      type,
                                                         Format             format,
                                                         AssociatedType     associatedType,
                                                         unsigned int       width,
                                                         unsigned int       height,
                                                         std::string        name = "",
                                                         MinFilter          minFilter = MinFilter::nearest,
                                                         MagFilter          magFilter = MagFilter::nearest,
                                                         Wrapping           sWrapping = Wrapping::repeat,
                                                         Wrapping           tWrapping = Wrapping::repeat,
                                                         unsigned int       mipMapLevel = 0);

                                    Texture			    (const Texture & texture) = delete;
                    Texture &	    operator =			(const Texture & texture) = delete;
		const       GLuint          get_underlying_id	() const;
                    void            _load  				() override;
		            void            _unload 			() override;
                    std::string		get_class			() const override;
                    void 			use					() ;
                                    ~Texture			();
                    void            copy_data           (byte* data, size_t size);
                    int             channels_amount     () const;

        static      GLenum          to_ogl              (MinFilter          minFilter);
        static      GLenum          to_ogl              (MagFilter          magFilter);
        static      GLenum          to_ogl              (Wrapping           wrapping);
        static      GLenum          to_ogl              (Format             format);
        static      GLenum          to_ogl              (AssociatedType     type);
        static      GLenum          to_ogl              (Texture::Type      type);
        static      GLenum          underlying_type     (AssociatedType     associatedType,
                                                         Format             format);
		
	private:
		
        std::string                 name;
        Type                        type;
        Format                      format;
        AssociatedType              associatedType;
        MinFilter                   minFilter;
        MagFilter                   magFilter;
        Wrapping                    sWrapping;
        Wrapping                    tWrapping;
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

#endif /* texture_hpp */
