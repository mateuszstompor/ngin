//
//  deferredLightingShader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferred_lighting_shader_h
#define deferred_lighting_shader_h

#include "shader.hpp"
#include "../../scene/texture.hpp"

namespace ms {
	
	class DeferredLightingShader : public virtual Shader {
	
	public:
		
		inline			DeferredLightingShader				(unsigned int maximalAmountOfLights);
		
		virtual void	set_camera_transformation			(const math::mat4 & transformation)				 			= 0;
		
		virtual void 	set_amount_of_point_lights			(int amount) 												= 0;
		virtual void 	set_point_light_power				(unsigned int index, float power) 							= 0;
		virtual void 	set_point_light_color				(unsigned int index, const math::vec3 & color) 				= 0;
		virtual void 	set_point_light_position			(unsigned int index, const math::vec3 & position) 			= 0;
		
		virtual void 	set_amount_of_spot_lights			(int amount)												= 0;
		virtual void 	set_spot_light_power				(unsigned int index, float power) 							= 0;
		virtual void 	set_spot_light_color				(unsigned int index, const math::vec3 & color) 				= 0;
		virtual void 	set_spot_light_position				(unsigned int index, const math::vec3 & position) 			= 0;
		virtual void 	set_spot_light_angle				(unsigned int index, float angle) 							= 0;
		virtual void 	set_spot_light_direction			(unsigned int index, const math::vec3 direction) 			= 0;

		virtual void	set_has_directional_light			(bool doesItHave) 											= 0;
		virtual void	set_directional_light_dir			(const math::vec3 & dir) 									= 0;
		virtual void	set_directional_light_color			(const math::vec3 & color) 									= 0;
        virtual void    set_directional_sm_transform        (const math::mat4 & color)                                  = 0;
        virtual void    set_directional_sm_projection       (const math::mat4 & color)                                  = 0;
		
		virtual void	bind_g_buf_albedo					(Texture & albedoTexture)									= 0;
		virtual void	bind_g_buf_normals					(Texture & normalsTexture)									= 0;
		virtual void	bind_g_buf_posiitons				(Texture & positionsTexture)								= 0;
        virtual void    bind_shadow_map                     (Texture & shadowTexture)                                = 0;
		
		virtual void 	set_rendering_mode					(unsigned int settings) 									= 0;
		
		virtual 		~DeferredLightingShader				()															= default;
		
	protected:
		
		unsigned int maximalAmountOfLights;
		
	};
	
}

ms::DeferredLightingShader::DeferredLightingShader (unsigned int maximalAmountOfLights) : maximalAmountOfLights(maximalAmountOfLights) { }

#endif /* deferred_lighting_shader_h */
