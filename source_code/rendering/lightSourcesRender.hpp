//
//  lightSourcesRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 19/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "render.hpp"

namespace ms {
	
	class LightSourcesRender : public Render {
		
	public:
		
		 		LightSourcesRender		(std::unique_ptr<Framebuffer> &&    framebuffer,
										 std::unique_ptr<Shader> &&         shader);
		
		virtual ~LightSourcesRender		() = default;
		void	draw  					(Drawable & node, const Scene & scene)			override;
		
	};
	
}
