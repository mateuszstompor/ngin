//
//  render.h
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef render_h
#define render_h

#include <memory>

#include "../resources/resource.hpp"
#include "../scene/scene.hpp"

namespace ms {
	
    class Render : public Resource {
		
    public:
		
		inline			Render(unsigned int screenWidth, unsigned int screenHeight,
							   unsigned int frameBufferWidth, unsigned int frameBufferHeight);
		
        virtual void	use     	() 										= 0	;
		virtual void 	clear_frame	()										= 0 ;
		virtual void	draw_scene  (const Scene * scene) 					= 0	;
		virtual			~Render 	() = default;
		
	protected:
		
		unsigned int screenWidth;
		unsigned int screenHeight;
		unsigned int frameBufferWidth;
		unsigned int frameBufferHeight;
		
    };
    
}

ms::Render::Render(unsigned int sW, unsigned int sH, unsigned int fbW, unsigned int fbH) :
			screenHeight(sH), screenWidth(sW), frameBufferWidth(fbW), frameBufferHeight(fbH) { }

#endif /* render_h */
