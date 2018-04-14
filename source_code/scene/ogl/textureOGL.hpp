//
//  textureOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 10/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef textureOGL_hpp
#define textureOGL_hpp

#include <cassert>
#include <iostream>

#include "../../utils/ogl/proxyOGL.hpp"
#include "../texture.hpp"

namespace ms {
	
	class TextureOGL : public Texture {
		
	public:
		
								TextureOGL			(Texture::Type 	type,
													 std::string		name,
												 	 Format			format,
													 AssociatedType	associatedType,
												 	 MinFilter 		minFilter,
												 	 MagFilter 		magFilter,
												 	 Wrapping 		sWrapping,
												 	 Wrapping 		tWrapping,
												 	 unsigned int 	mipMapLevel,
												 	 unsigned int 	width,
												 	 unsigned int 	height);
		
								TextureOGL			(Texture::Type 	type,
													 std::string	name,
													 Format			format,
													 AssociatedType	associatedType,
													 unsigned int 	width,
													 unsigned int 	height);
		
								TextureOGL			(Texture::Type 	type,
													 Format			format,
													 AssociatedType	associatedType,
													 unsigned int 	width,
													 unsigned int 	height);
		
								TextureOGL			(const TextureOGL & texture) = delete;
				TextureOGL &	operator =			(const TextureOGL & texture) = delete;
		
		const 	GLuint			get_underlying_id	() const;
		
		virtual void    		_load  				() override;
		virtual void 			_unload 			() override;
		virtual std::string		get_class			() const override;

		virtual void 			use					() override;
								~TextureOGL			() = default;
	protected:
		
		static 	GLenum			to_ogl				(MinFilter 			minFilter);
		static 	GLenum			to_ogl				(MagFilter 			magFilter);
		static 	GLenum			to_ogl				(Wrapping 			wrapping);
		static	GLenum			to_ogl				(Format 			format);
		static 	GLenum			to_ogl				(AssociatedType 	type);
		static 	GLenum			to_ogl				(Texture::Type 		type);
				GLenum			underlying_type		() const;
		
	protected:
		
		GLint					internalFormat;
		GLenum					colorFormat;
		GLenum					type;
		GLenum					target;
		GLuint					texture;
		
	};
	
}

#endif /* textureOGL_hpp */
