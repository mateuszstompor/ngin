//
//  postprocessDrawer.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef postprocess_drawer_hpp
#define postprocess_drawer_hpp

#include <memory>

#include "render.hpp"
#include "shaders/shader.hpp"
#include "framebuffer.hpp"

namespace ms {
	
	class PostprocessDrawer : public Render {
		
	public:
						PostprocessDrawer(std::shared_ptr<Texture> input, std::shared_ptr<Framebuffer> framebuffer) : Render(framebuffer), input(input) {}
		virtual void 	draw_quad() = 0;
		virtual			~PostprocessDrawer() = default;
// TODO make it protected
//	protected:
		
		std::shared_ptr<Shader> 	shaderProgram;
		std::shared_ptr<Texture> 	input;
		
	};
	
}

#endif /* postprocess_drawer_hpp */
