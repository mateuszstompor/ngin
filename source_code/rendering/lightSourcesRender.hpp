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
		
		 		LightSourcesRender		(std::shared_ptr<Framebuffer> 				framebuffer,
										 std::unique_ptr<LightSourceDrawerShader>  	shader);
		
		virtual ~LightSourcesRender		() = default;
		void	draw  					(Drawable * node, const Scene * scene)			override;
		
	protected:
		
		std::string 								vertexShaderSource;
		std::string 								fragmentShaderSource;

	};
	
}

#endif /* lightSourcesRender_hpp */
