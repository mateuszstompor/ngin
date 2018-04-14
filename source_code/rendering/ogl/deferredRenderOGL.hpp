//
//  deferredRenderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferred_render_ogl_hpp
#define deferred_render_ogl_hpp

#include "../../utils/ogl/proxyOGL.hpp"
#include "../deferredRender.hpp"
#include "../shaders/ogl/deferredShaderOGL.hpp"
#include "../shaders/ogl/deferredLightingShaderOGL.hpp"
#include "../reusableGeometry.hpp"
#include "../../utils/ogl/utilsOGL.hpp"
#include "../../scene/ogl/textureOGL.hpp"
#include "framebufferOGL.hpp"
#include "renderbufferOGL.hpp"
#include "../../scene/ogl/drawableOGL.hpp"

namespace ms {
	
	class DeferredRenderOGL : public DeferredRender {
		
		public:
		
			DeferredRenderOGL							(unsigned int maximalAmountOfLights,
												 		std::string gBufferVertexShaderSource,
												 		std::string gBufferFragmentShaderSource,
												 		std::string lightingVertexShaderSource,
												 		std::string lightingFragmentShaderSource,
                                                        std::string shadowMappingVertexShader,
                                                        std::string shadowMappingFragmentShader,
												 		std::shared_ptr<Framebuffer> framebuffer);
		
									DeferredRenderOGL	(const DeferredRenderOGL &) = delete;
			DeferredRenderOGL & 	operator = 			(const DeferredRenderOGL &) = delete;
			void					_load				() 												override;
			void 					_unload				() 												override;
			void 					perform_light_pass	(const Scene * scene) 							override;
			virtual					~DeferredRenderOGL 	() = default;
		
	};
	
}

#endif /* deferred_render_ogl_hpp */
