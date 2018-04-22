//
//  renderbuffer.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <iostream>
#include <cassert>

#include "../utils/ogl/proxyOGL.hpp"
#include "../scene/texture.hpp"

namespace ms {
	
	class Renderbuffer : public Resource {
		
	public:
                                Renderbuffer        (const Renderbuffer &) = delete;
        
                                Renderbuffer		(Texture2D::Format			format,
                                                     Texture2D::AssociatedType	associatedType,
                                                     unsigned int 				mipMapLevel,
                                                     unsigned int 				width,
                                                     unsigned int 				height);
		
                                Renderbuffer		(Texture2D::Format			format,
                                                     Texture2D::AssociatedType	associatedType,
                                                     unsigned int 				width,
                                                     unsigned int 				height);
		
        Renderbuffer &          operator =          (const Texture2D &) = delete;
		virtual std::string	    get_class			() const override;
		virtual void 		    use					();
		virtual void    	    _load  				() override;
		virtual void 		    _unload 			() override;
        constexpr GLuint		get_underlying_id	() { return renderBuffer; }
		
	protected:
        
        Texture2D::AssociatedType associatedType;
        Texture2D::Format         format;
        unsigned int            mipMapLevel;
        unsigned int            width;
        unsigned int            height;

		GLenum				    internalFormat;
		GLuint				    renderBuffer;
		
	};
	
}
