//
//  renderbuffer.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef renderbuffer_hpp
#define renderbuffer_hpp

#include <iostream>
#include <cassert>

#include "../utils/ogl/proxyOGL.hpp"
#include "../scene/ogl/textureOGL.hpp"

namespace ms {
	
	class Renderbuffer : public Resource {
		
	public:
                                Renderbuffer        (const Renderbuffer &) = delete;
        
                                Renderbuffer		(Texture::Format			format,
                                                     Texture::AssociatedType	associatedType,
                                                     unsigned int 				mipMapLevel,
                                                     unsigned int 				width,
                                                     unsigned int 				height);
		
                                Renderbuffer		(Texture::Format			format,
                                                     Texture::AssociatedType	associatedType,
                                                     unsigned int 				width,
                                                     unsigned int 				height);
		
        Renderbuffer &          operator =          (const Texture &) = delete;
		virtual std::string	    get_class			() ;
		virtual void 		    use					() ;
		virtual void    	    _load  				() override;
		virtual void 		    _unload 			() override;
				GLuint		    get_underlying_id	();
		
	protected:
        
        Texture::AssociatedType associatedType;
        Texture::Format         format;
        unsigned int            mipMapLevel;
        unsigned int            width;
        unsigned int            height;

		GLenum				    internalFormat;
		GLuint				    renderBuffer;
		
	};
	
}

#endif /* renderbuffer_hpp */
