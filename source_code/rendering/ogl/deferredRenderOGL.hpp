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
#include "../../scene/texture.hpp"
#include "framebufferOGL.hpp"
#include "../renderbuffer.hpp"
#include "../../scene/ogl/drawableOGL.hpp"

namespace ms {
	
	class DeferredRenderOGL : public DeferredRender {
		
		public:
		
                                    DeferredRenderOGL   (unsigned int                       maxPointLightsAmount,
                                                         unsigned int                       maxSpotLightsAmount,
												 		 std::unique_ptr<Framebuffer> &&    framebuffer,
                                                         g_pass_shader &&                   gShader,
                                                         lighting_shader &&                 lightingShader);
		
									DeferredRenderOGL	(const DeferredRenderOGL &) = delete;
			DeferredRenderOGL & 	operator = 			(const DeferredRenderOGL &) = delete;
			void					_load				() 												override;
			void 					_unload				() 												override;
			void 					perform_light_pass	(const Scene * scene) 							override;
		
	};
	
}

#endif /* deferred_render_ogl_hpp */
