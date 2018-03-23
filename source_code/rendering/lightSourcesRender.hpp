//
//  lightSourcesRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 19/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef lightSourcesRender_hpp
#define lightSourcesRender_hpp

#include "render.hpp"
#include "shaders/lightSourceDrawerShader.hpp"

namespace ms {
	
	class LightSourcesRender : public Render {
		
	public:
		
		inline 	LightSourcesRender(std::shared_ptr<Framebuffer> framebuffer,
								   std::string 	vertexShaderSource,
								   std::string 	fragmentShaderSource
								   );
		
		virtual ~LightSourcesRender() = default;
		
	protected:
		
		std::string 								vertexShaderSource;
		std::string 								fragmentShaderSource;
		std::unique_ptr<LightSourceDrawerShader>	shader;
		
	};
	
}

ms::LightSourcesRender::LightSourcesRender(
								 std::shared_ptr<Framebuffer> framebuffer,
								 std::string vertexShSource,
								 std::string fragmentShSource
								 ) : 	Render(framebuffer),
										vertexShaderSource(vertexShSource),
										fragmentShaderSource(fragmentShSource),
										shader(nullptr) { }

#endif /* lightSourcesRender_hpp */
