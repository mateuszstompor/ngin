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
#include "shaders/shader.hpp"

namespace ms {
	
    class Render : public Resource {
		
    public:
								Render			(std::unique_ptr<Framebuffer> && 	framebuffer,
												 std::unique_ptr<Shader> && 		shader);
		
		virtual void			use		     	();
		virtual void			use		     	(Framebuffer & framebuffer);
		virtual	void			_load			() override;
		virtual	void 			_unload			() override;
		virtual std::string 	get_class		() const override;
		virtual void			draw  			(Drawable & node, const Scene & scene) = 0;
		Framebuffer &			get_framebuffer () { return *framebuffer; }
		Shader &				get_shader		() { return *shader; }
		virtual					~Render 		() = default;
		
	protected:
		
		std::unique_ptr<Framebuffer> 	framebuffer;
		std::unique_ptr<Shader>			shader;

    };
    
}



#endif /* render_h */
