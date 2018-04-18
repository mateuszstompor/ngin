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
#include "../../rendering/shaders/forwardShader.hpp"
#include "../../rendering/shaders/deferredShader.hpp"
#include "../../rendering/shaders/lightSourceDrawerShader.hpp"

#include "../../utils/loader.hpp"
#include "../../scene/drawable.hpp"
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
        
    };
    
}

#endif /* nginOGL_hpp */
