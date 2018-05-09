//
//  forwardRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "modelRender.hpp"

namespace ms {
	
	class ForwardRender : public ModelRender {
	
		friend class NGin;
		
	public:
		
						ForwardRender					(unsigned int 						maximalAmountOfLights,
														 std::unique_ptr<Framebuffer> && 	framebuffer,
														 std::unique_ptr<Shader> && 		shader);
						~ForwardRender					() = default;
		std::string 	get_class						() const override;
	
	private:
	
		void 			draw  							(Drawable & node) override;
		void    		draw                    		(Drawable & node, math::mat4 const & transformation);
		void    		set_material            		(Material * material) override;
		void    		set_spot_lights         		(std::vector<SpotLight> const & spotLights) override;
		void    		set_point_lights        		(std::vector<PointLight> const & pointLights) override;
		void    		set_directionallight    		(DirectionalLight const * directionalLight) override;
		void    		set_camera              		(Camera const & camera) override;
		void			_load							() override;
		unsigned int 	maximalAmountOfLights;
				
	};
	
}
