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
#include "../../scene/drawable.hpp"
#include "../shaders/shader.hpp"

namespace ms {
	
	class PostprocessDrawerOGL : public PostprocessDrawer {
	
	public:
		
									PostprocessDrawerOGL	(std::vector<std::weak_ptr<Texture>> 	input,
															 std::unique_ptr<Framebuffer> &&        framebuffer,
															 std::unique_ptr<Shader> &&             shaderProgram);
        virtual void                _load                   ()                                      override;
        virtual void                _unload                 ()                                      override;
		virtual void 				draw_quad				() const                                override;
		virtual std::string			get_class				() const                                override;
	
	};
	
}

#endif /* postprocessDrawerOGL_hpp */
