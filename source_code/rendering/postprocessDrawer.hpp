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
#include <vector>

#include "render.hpp"
#include "shaders/shader.hpp"
#include "framebuffer.hpp"

namespace ms {
	
	class PostprocessDrawer : public Render {
		
	public:
		inline							PostprocessDrawer	(std::vector<std::shared_ptr<Texture>> 	input,
															 std::shared_ptr<Framebuffer> 			framebuffer,
															 std::unique_ptr<Shader>				shaderProgram);
		
				virtual void 			draw_quad			() = 0;
		inline 	virtual std::string   	get_class			() = 0;
		virtual							~PostprocessDrawer	() = default;

	protected:
	
		std::unique_ptr<Shader> 				shaderProgram;
		std::vector<std::shared_ptr<Texture>> 	inputTextures;
		
	};
	
}

ms::PostprocessDrawer::PostprocessDrawer(std::vector<std::shared_ptr<Texture>> 	input,
										 std::shared_ptr<Framebuffer> 			framebuffer,
										 std::unique_ptr<Shader> 				shaderProgram) : 	Render(framebuffer),
																									inputTextures(input),
																									shaderProgram(std::move(shaderProgram)) {}

std::string ms::PostprocessDrawer::get_class () {
	return "ms::PostprocessDrawer";
}

#endif /* postprocess_drawer_hpp */
