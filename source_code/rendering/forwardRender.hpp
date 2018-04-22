//
//  forwardRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "render.hpp"

namespace ms {
	
	class ForwardRender : public Render {
	
	public:
		
						ForwardRender					(unsigned int 						maximalAmountOfLights,
														 std::unique_ptr<Framebuffer> && 	framebuffer,
														 std::unique_ptr<Shader> && 		shader);
		
		std::string 	get_class						() const override;
		void 			draw  							(Drawable & node, const Scene & scene) override;
		void 			setup_uniforms					(const Scene * scene);
		void 			setup_material_uniforms_for		(const Scene * scene, const Drawable * node);
		
	private:
		
		void			_load							() override;
		unsigned int 	maximalAmountOfLights;
				
	};
	
}
