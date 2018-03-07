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
#include "../../rendering/ogl/forwardRenderOGL.hpp"
#include "../../rendering/ogl/deferredRenderOGL.hpp"

namespace ms {
    
    class NGinOGL : public NGin {
        
    public:
				NGinOGL     (
							 std::shared_ptr<std::string> forwardRenderVertexShaderSource,
							 std::shared_ptr<std::string> forwardRenderFragmentShaderSource,
							 std::shared_ptr<std::string> deferredRenderVertexShaderSource,
							 std::shared_ptr<std::string> deferredRenderFragmentShaderSource,
							 std::shared_ptr<std::string> deferredRenderLightingVertexShaderSource,
							 std::shared_ptr<std::string> deferredRenderLightingFragmentShaderSource,
							 unsigned int screenWidth,
							 unsigned int screenHeight,
							 unsigned int frameBufferWidth,
							 unsigned int frameBufferHeight,
							 float camNear,
							 float camFar,
							 float fovDegrees,
							 float aspect
							 );
		
        void    draw_scene  () override;
		void 	load		() override;
		void 	unload		() override;
		
    };
    
}

#endif /* nginOGL_hpp */
