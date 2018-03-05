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

namespace ms {
    
    class NGinOGL : public NGin {
        
    public:
				NGinOGL     (
							 std::shared_ptr<std::string> vertexShaderSource,
							 std::shared_ptr<std::string> fragmentShaderSource,
							 unsigned int screenWidth,
							 unsigned int screenHeight,
							 unsigned int frameBufferWidth,
							 unsigned int frameBufferHeight
							 );
		
        void    draw_scene  () override;
		void 	unload		() override;
		
    };
    
}

#endif /* nginOGL_hpp */
