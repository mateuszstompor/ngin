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
#include "../../utils/ogl/loaderOGL.hpp"
#include "../../scene/ogl/sceneNodeOGL.hpp"

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
							 float aspect,
							 GLuint defaultFBO
							 );
		
        void    						draw_scene  () override;
		void 							load		() override;
		std::unique_ptr<Loader>			get_loader	() override;
		std::shared_ptr<SceneNode>		get_node	() override;
		
    };
    
}

#endif /* nginOGL_hpp */
