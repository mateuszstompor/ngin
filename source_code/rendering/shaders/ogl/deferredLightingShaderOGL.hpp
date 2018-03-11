//
//  deferredLightingShaderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferred_lightingShader_ogl_hpp
#define deferred_lightingShader_ogl_hpp

#include <string>

#include "../deferredLightingShader.hpp"
#include "shaderOGL.hpp"

namespace ms {
	
	class DeferredLightingShaderOGL : public ShaderOGL, public DeferredLightingShader {
	
	public:
		
						DeferredLightingShaderOGL			(str_ptr vertexShaderSource, str_ptr fragmentShaderSource);
		
		virtual void 	set_amount_of_point_lights			(unsigned int amount) 									override;
		virtual void 	set_point_light_power				(unsigned int index, float power) 						override;
		virtual void 	set_point_light_color				(unsigned int index, const math::vec4 & color) 			override;
		virtual void 	set_point_light_transformation		(unsigned int index, const math::mat4 & transformation) override;
		
		virtual void	set_has_directional_light			(bool doesItHave)										override;
		virtual void	set_directional_light_dir			(const math::vec3 & dir)								override;
		virtual void	set_directional_light_color			(const math::vec4 & color)								override;
		
		virtual void 	set_amount_of_spot_lights			(unsigned int amount)									override;
		virtual void 	set_spot_light_power				(unsigned int index, float power) 						override;
		virtual void 	set_spot_light_color				(unsigned int index, const math::vec4 & color)			override;
		virtual void 	set_spot_light_transformation		(unsigned int index, const math::mat4 & transformation) override;
		virtual void 	set_spot_light_angle				(unsigned int index, float angle) 						override;
		virtual void 	set_spot_light_direction			(unsigned int index, const math::vec3 direction) 		override;
		
		virtual void 	set_rendering_mode					(unsigned int settings) 								override;

		virtual void	load								()														override;
	};
	
}

#endif /* deferred_lightingShader_ogl_hpp */
