//
//  nginOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef nginOGL_hpp
#define nginOGL_hpp

#include "../ngin.hpp"
#include "../../rendering/ogl/deferredRenderOGL.hpp"
#include "../../rendering/ogl/postprocessDrawerOGL.hpp"
#include "../../rendering/shaders/ogl/forwardShaderOGL.hpp"
#include "../../rendering/shaders/ogl/deferredShaderOGL.hpp"
#include "../../rendering/shaders/ogl/lightSourceDrawerShaderOGL.hpp"

#include "../../utils/loader.hpp"
#include "../../scene/ogl/drawableOGL.hpp"
#include "../../scene/lights/ogl/pointLightOGL.hpp"
#include "../../scene/lights/ogl/spotLightOGL.hpp"
#include "../../shaders/shaderHeaders.hpp"

namespace ms {
    
    class NGinOGL : public NGin {
        
    public:
                                        NGinOGL             (unsigned int 	screenWidth,
                                                             unsigned int 	screenHeight,
                                                             unsigned int   frameBufferWidth,
                                                             unsigned int   frameBufferHeight,
                                                             float          camNear,
                                                             float          camFar,
                                                             float          fovDegrees,
                                                             float          aspect);
        
                                        NGinOGL             (unsigned int                   screenWidth,
                                                             unsigned int                   screenHeight,
                                                             unsigned int                   frameBufferWidth,
                                                             unsigned int                   frameBufferHeight,
                                                             std::unique_ptr<Camera> &&     cam,
                                                             std::unique_ptr<Framebuffer> && defaultFramebuffer);
        
                                        NGinOGL             (unsigned int 	screenWidth,
                                                             unsigned int 	screenHeight,
                                                             unsigned int frameBufferWidth,
                                                             unsigned int frameBufferHeight,
                                                             float camNear,
                                                             float camFar,
                                                             float fovDegrees,
                                                             float aspect,
                                                             std::unique_ptr<Framebuffer> && defaultFramebuffer);
		
		void 							unload				() 							override;
		void 							load				() 							override;
		std::shared_ptr<Drawable>		get_drawable		() 							override;
		std::shared_ptr<PointLight>		get_point_light		(float 			power,
															 math::vec3 	color,
															 math::vec3 	position) 	override;
		
		std::shared_ptr<SpotLight>		get_spot_light		(float 			power,
															 math::vec3 	color,
															 math::vec3 	position,
															 float 			lightingAngleDegrees,
															 math::vec3 	direction) 	override;
		
    };
    
}

#endif /* nginOGL_hpp */
