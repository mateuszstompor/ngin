//
//  forwardRenderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef forward_render_ogl_hpp
#define forward_render_ogl_hpp

#include <memory>

#include "../../utils/ogl/proxyOGL.hpp"
#include "../render.hpp"
#include "../shaders/forwardShader.hpp"
#include "../forwardRender.hpp"
#include "../shaders/ogl/forwardShaderOGL.hpp"
#include "../shaders/ogl/lightSourceDrawerShaderOGL.hpp"

namespace ms {
    
    class ForwardRenderOGL : public ForwardRender {
		
	public:
		
						ForwardRenderOGL	(unsigned int 	maximalAmountOfLights,
											std::string vertexShaderSource,
											std::string fragmentShaderSource,
											std::shared_ptr<Framebuffer> framebuffer);
		
		void			use     			() 										override;
		void 			clear_frame			()										override;
		std::string 	get_class			() 										override;
		void			draw  				(Drawable * node, const Scene * scene)	override;
		void			load				() 										override;
		bool			is_loaded			() 										override;
		void 			unload				() 										override;
		virtual			~ForwardRenderOGL 	() = default;
        
    };
    
}

#endif /* forward_render_ogl_hpp */
