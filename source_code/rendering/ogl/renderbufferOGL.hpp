//
//  renderbufferOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef renderbufferOGL_hpp
#define renderbufferOGL_hpp

#include <iostream>
#include <cassert>

#include "../../utils/ogl/proxyOGL.hpp"
#include "../renderbuffer.hpp"
#include "../../scene/ogl/textureOGL.hpp"

namespace ms {
	
	class RenderbufferOGL : public Renderbuffer {
		
	public:
                                RenderbufferOGL     (const RenderbufferOGL &) = delete;
        
                                RenderbufferOGL		(Texture::Format			format,
                                                     Texture::AssociatedType	associatedType,
                                                     unsigned int 				mipMapLevel,
                                                     unsigned int 				width,
                                                     unsigned int 				height);
		
                                RenderbufferOGL		(Texture::Format			format,
                                                     Texture::AssociatedType	associatedType,
                                                     unsigned int 				width,
                                                     unsigned int 				height);
		
        RenderbufferOGL &       operator =          (const Texture &) = delete;
		virtual std::string	    get_class			() override;
		virtual void 		    use					() override;
		virtual void    	    _load  				() override;
		virtual void 		    _unload 			() override;
				GLuint		    get_underlying_id	();
		
	protected:
		
		GLenum				    internalFormat;
		GLuint				    renderBuffer;
		
	};
	
}

#endif /* renderbufferOGL_hpp */
