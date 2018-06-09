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

	/**
	 * Jedna z pochodnych ModelRener stworzona do generowania modeli na bufor ramki z oparci o algorytm deferred renderingu.
	 * W chwili obencej niemożliwe jest jej .
	 */
	class DeferredRender : public ModelRender {
		
		friend class NGin;

	private:
		
										DeferredRender				(unsigned int 						maxPointLightsAmount,
																	 unsigned int 						maxSpotLightsAmount,
																	 std::unique_ptr<Framebuffer> && 	framebuffer,
																	 std::unique_ptr<Shader> && 		gShader,
																	 std::unique_ptr<Shader> &&			lightingShader);
		void							use			     			() override;
		void 							perform_light_pass			(const Scene & scene);		
		void    						draw                    	(Drawable & node) override;
		void    						draw                    	(Drawable & node, math::mat4 const & transformation) override;
		void    						set_material            	(Material * material) override;
		void    						set_spot_lights         	(std::vector<SpotLight> const & spotLights) override;
		void    						set_point_lights        	(std::vector<PointLight> const & pointLights) override;
		void    						set_directionallight    	(DirectionalLight const * directionalLight) override;
		void    						set_camera              	(Camera const & camera) override;
		void							_load						() override;
		void 							_unload						() override;
		unsigned int 					maxPointLightsAmount;
		unsigned int 					maxSpotLightsAmount;
		std::unique_ptr<Shader>			lightingShader;
		std::unique_ptr<Framebuffer>	gFramebuffer;
		std::shared_ptr<Drawable> 		quad;
		
	};
	
}
