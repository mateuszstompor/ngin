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
#include "../shaders/ogl/shaderOGL.hpp"

namespace ms {
	
	class PostprocessDrawerOGL : public PostprocessDrawer {
	
	public:
		
		PostprocessDrawerOGL(std::shared_ptr<Texture> input, std::shared_ptr<Framebuffer> framebuffer);

		virtual void 	draw_quad() override;
		virtual void	use		     	() 												override;
		virtual void    load  		() override;
		virtual void 	unload 		() override;
		virtual void 	clear_frame		()												override;
		virtual void	draw  			(Drawable * node, const Scene * scene)			override;
		
		GLuint						quadVAO;
		GLuint						quadVBO;
	};
	
}

#endif /* postprocessDrawerOGL_hpp */
