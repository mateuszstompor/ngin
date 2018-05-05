//
//  deferredRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cassert>
#include <vector>
#include <tuple>

#include "modelRender.hpp"
#include "../scene/texture.hpp"
#include "../utils/proxyOGL.hpp"
#include "reusableGeometry.hpp"
#include "../utils/utils.hpp"
#include "../scene/drawable.hpp"

namespace ms {
	
	class DeferredRender : public ModelRender {
		
	public:
		
		enum class DebugType;
		
										DeferredRender				(unsigned int 						maxPointLightsAmount,
																	 unsigned int 						maxSpotLightsAmount,
																	 std::unique_ptr<Framebuffer> && 	framebuffer,
																	 std::unique_ptr<Shader> && 		gShader,
																	 std::unique_ptr<Shader> &&			lightingShader);
		
		void							use			     			() override;
		void 							perform_light_pass			(const Scene & scene);		
		void    						draw                    	(Drawable & node) override;
		void    						set_material            	(Material * material) override;
		void    						set_spot_lights         	(std::vector<SpotLight> const & spotLights) override;
		void    						set_point_lights        	(std::vector<PointLight> const & pointLights) override;
		void    						set_directionallight    	(DirectionalLight const * directionalLight) override;
		void    						set_camera              	(Camera const & camera) override;
		
		void							_load						() override;
		void 							_unload						() override;

//	private:
		
		std::unique_ptr<Shader>			lightingShader;
		
		unsigned int 					maxPointLightsAmount;
		unsigned int 					maxSpotLightsAmount;
				
		std::unique_ptr<Framebuffer>	gFramebuffer;
		std::shared_ptr<Drawable> 		quad;
		
	};
	
}
