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
#include "framebuffer.hpp"

namespace ms {
	
    class Render : public Resource {
		
    public:
		
		inline			Render			(std::shared_ptr<Framebuffer> framebuffer);
        virtual void	use		     	() 												= 0;
		virtual void 	clear_frame		()												= 0;
		virtual void	draw  			(Drawable * node, const Scene * scene)			= 0;
		virtual			~Render 		() = default;
		
	protected:
		
		std::shared_ptr<Framebuffer> framebuffer;
		
    };
    
}

ms::Render::Render(std::shared_ptr<Framebuffer> framebuffer) : framebuffer(framebuffer) { }

#endif /* render_h */
