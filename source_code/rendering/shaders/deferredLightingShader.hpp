//
//  deferredLightingShader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferred_lightingShader_hpp
#define deferred_lightingShader_hpp

#include <string>

#include "./deferredLightingShader.hpp"
#include "shader.hpp"
#include "../../scene/texture.hpp"

namespace ms {
	
	class DeferredLightingShader : public Shader {
	
	public:
		
                DeferredLightingShader			    (unsigned int maxAmountOfPointLights,
                                                     unsigned int maxAmountOfSpotLights,
                                                     std::string vertexShaderSource,
                                                     std::string fragmentShaderSource);

		void 	set_camera_transformation 			(const math::mat4 & transf) ;
		
        void 	set_amount_of_point_lights			(int amount) ;
		void 	set_point_light_power				(unsigned int index, float power);
		void 	set_point_light_color				(unsigned int index, const math::vec3 & color);
		void 	set_point_light_position			(unsigned int index, const math::vec3 & position);
		
		void	set_has_directional_light			(bool doesItHave);
		void	set_directional_light_dir			(const math::vec3 & dir);
		void	set_directional_light_color			(const math::vec3 & color);
        void    set_directional_sm_transform        (const math::mat4 & color);
        void    set_directional_sm_projection       (const math::mat4 & color);
		
		void 	set_amount_of_spot_lights			(int amount);
		void 	set_spot_light_power				(unsigned int index, float power);
		void 	set_spot_light_color				(unsigned int index, const math::vec3 & color);
		void 	set_spot_light_position				(unsigned int index, const math::vec3 & position);
		void 	set_spot_light_angle				(unsigned int index, float angle);
		void 	set_spot_light_direction			(unsigned int index, const math::vec3 direction);
        
		void	bind_g_buf_albedo					(Texture & albedoTexture);
		void	bind_g_buf_normals					(Texture & normalsTexture);
		void	bind_g_buf_posiitons				(Texture & positionsTexture);
        void    bind_shadow_map                     (Texture & shadowTexture);
		
        void 	set_rendering_mode					(unsigned int settings);

        void	_load								();
		
				~DeferredLightingShader			    ();
		
	private:
		
        unsigned int    maxAmountOfPointLights;
        unsigned int    maxAmountOfSpotLights;
        
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

#endif /* deferred_lightingShader_hpp */
