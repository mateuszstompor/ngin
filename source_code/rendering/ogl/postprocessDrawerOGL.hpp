//
//  postprocessDrawerOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef postprocessDrawerOGL_hpp
#define postprocessDrawerOGL_hpp

#include "../../utils/ogl/proxyOGL.hpp"
#include "../postprocessDrawer.hpp"
#include "../reusableGeometry.hpp"
#include "../../scene/ogl/drawableOGL.hpp"
#include "../shaders/ogl/shaderOGL.hpp"

namespace ms {
	
	class PostprocessDrawerOGL : public PostprocessDrawer {
	
	public:
		
									PostprocessDrawerOGL	(std::vector<std::shared_ptr<Texture>> 	input,
															 std::shared_ptr<Framebuffer> 			framebuffer,
															 std::unique_ptr<Shader>				shaderProgram);

		virtual void 				draw_quad				() 										override;
		virtual std::string			get_class				() 										override;
		virtual void				draw  					(Drawable * node, const Scene * scene)	override;
	
	};
	
}

#endif /* postprocessDrawerOGL_hpp */
