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

#include "../../utils/proxyOGL.hpp"
#include "../../scene/texture.hpp"

namespace ms {
	
	class Renderbuffer : public Resource {
		
        friend class Framebuffer;
        friend class DeferredRender;
        friend class NGin;
        
    public:
                                    Renderbuffer        (texture::Format            format,
                                                         texture::AssociatedType    associatedType,
                                                         unsigned int               mipMapLevel,
                                                         unsigned int               width,
                                                         unsigned int               height);
        
                                    Renderbuffer        (texture::Format            format,
                                                         texture::AssociatedType    associatedType,
                                                         unsigned int               width,
                                                         unsigned int               height);
                                    ~Renderbuffer       () = default;
        std::string                 get_class           () const override;
        
                                    Renderbuffer        (const Renderbuffer &) = delete;
        Renderbuffer &              operator =          (const Texture2D &) = delete;
        
 	protected:
        
		virtual void 		        use					();
        constexpr GLuint		    get_underlying_id	() { return renderBuffer; }
		
	private:
        
        void                        _load               () override;
        void                        _unload             () override;
        
        texture::AssociatedType     associatedType;
        texture::Format             format;
        unsigned int                mipMapLevel;
        unsigned int                width;
        unsigned int                height;
		GLenum				        internalFormat;
		GLuint				        renderBuffer;
		
	};
	
}
