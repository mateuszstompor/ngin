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
		
                        DeferredLightingShaderOGL			(unsigned int maxAmountOfPointLights,
                                                             unsigned int maxAmountOfSpotLights,
															 std::string vertexShaderSource,
															 std::string fragmentShaderSource);
		
		virtual void 	set_camera_transformation 			(const math::mat4 & transf)								override;
		
		virtual void 	set_amount_of_point_lights			(int amount) 											override;
		virtual void 	set_point_light_power				(unsigned int index, float power) 						override;
		virtual void 	set_point_light_color				(unsigned int index, const math::vec3 & color) 			override;
		virtual void 	set_point_light_position			(unsigned int index, const math::vec3 & position) 		override;
		
		virtual void	set_has_directional_light			(bool doesItHave)										override;
		virtual void	set_directional_light_dir			(const math::vec3 & dir)								override;
		virtual void	set_directional_light_color			(const math::vec3 & color)								override;
        virtual void    set_directional_sm_transform        (const math::mat4 & color)                              override;
        virtual void    set_directional_sm_projection       (const math::mat4 & color)                              override;
		
		virtual void 	set_amount_of_spot_lights			(int amount)											override;
		virtual void 	set_spot_light_power				(unsigned int index, float power) 						override;
		virtual void 	set_spot_light_color				(unsigned int index, const math::vec3 & color)			override;
		virtual void 	set_spot_light_position				(unsigned int index, const math::vec3 & position) 		override;
		virtual void 	set_spot_light_angle				(unsigned int index, float angle) 						override;
		virtual void 	set_spot_light_direction			(unsigned int index, const math::vec3 direction) 		override;
        
		virtual void	bind_g_buf_albedo					(Texture & albedoTexture)								override;
		virtual void	bind_g_buf_normals					(Texture & normalsTexture)								override;
		virtual void	bind_g_buf_posiitons				(Texture & positionsTexture)							override;
        virtual void    bind_shadow_map                     (Texture & shadowTexture)                               override;
		
		virtual void 	set_rendering_mode					(unsigned int settings) 								override;

		virtual void	_load								()														override;
		
						~DeferredLightingShaderOGL			();
		
	protected:
		
		GLint			directionalLightColorLocation;
		GLint			directionalLightDirectionLocation;
		GLint			hasDirectionalLightLocation;
		
		GLint			cameraTransformationLocation;
		GLint			renderModeLocation;
		
		GLint			spotLightsAmount;
		GLint*			spotLightsLocations;
		
		GLint			pointLightsAmount;
		GLint*			pointLightsLocations;
		
	};
	
}

#endif /* deferred_lightingShader_ogl_hpp */
