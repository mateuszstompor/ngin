//
//  forwardRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef forward_render_hpp
#define forward_render_hpp

#include <memory>

#include "render.hpp"
#include "shaders/forwardShader.hpp"

namespace ms {
	
	class ForwardRender : public Render {
	
	public:
		
		inline 	ForwardRender(unsigned int screenWidth,
							  unsigned int screenHeight,
							  unsigned int frameBufferWidth,
							  unsigned int frameBufferHeight,
							  std::shared_ptr<std::string> vertexShSource,
							  std::shared_ptr<std::string> fragmentShSource
							  );
		
		virtual ~ForwardRender() = default;
	
	protected:
		
		std::shared_ptr<std::string> 			vertexShaderSource;
		std::shared_ptr<std::string> 			fragmentShaderSource;
		std::shared_ptr<ForwardShader>			shader;
		
	};
	
}

ms::ForwardRender::ForwardRender(unsigned int sW,
								 unsigned int sH,
								 unsigned int fbW,
								 unsigned int fbH,
								 std::shared_ptr<std::string> vertexShSource,
								 std::shared_ptr<std::string> fragmentShSource
								 ) : Render(sW, sH, fbW, fbH), vertexShaderSource(vertexShSource), fragmentShaderSource(fragmentShSource), shader(nullptr) { }

#endif /* forward_render_hpp */
