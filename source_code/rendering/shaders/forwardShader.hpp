//
//  forwardShader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef forwardShader_hpp
#define forwardShader_hpp

#include "shader.hpp"

namespace ms {
	
	class ForwardShader : public Shader {
		
	public:
		
						ForwardShader				(unsigned int maximalAmountOfLights,
													 std::string vertexShaderSource,
													 std::string fragmentShaderSource);
		
		virtual void	_load						() override;
		
		virtual void 	set_projection_matrix		(const math::mat4 & proj);
		virtual void 	set_camera_transformation	(const math::mat4 & transf);
		virtual void	set_model_transformation	(const math::mat4 & modelTransf);
		
		virtual void 	set_amount_of_point_lights	(int amount);
		virtual void 	set_point_light_power		(unsigned int index, float power);
		virtual void 	set_point_light_color		(unsigned int index, const math::vec3 & color);
		virtual void 	set_point_light_position	(unsigned int index, const math::vec3 & position);
		
		virtual void 	set_amount_of_spot_lights	(int amount);
		virtual void 	set_spot_light_power		(unsigned int index, float power);
		virtual void 	set_spot_light_color		(unsigned int index, const math::vec3 & color);
		virtual void 	set_spot_light_position		(unsigned int index, const math::vec3 & position);
		virtual void 	set_spot_light_angle		(unsigned int index, float angle);
		virtual void 	set_spot_light_direction	(unsigned int index, const math::vec3 direction);
		
		virtual void	set_has_directional_light	(bool doesItHave);
		virtual void	set_directional_light_dir	(const math::vec3 & dir);
		virtual void	set_directional_light_color	(const math::vec3 & );
		
		virtual void	set_has_material			(bool doesItHave);

		virtual void	set_has_diffuse_texture		(bool doesItHave);
		virtual void	set_has_specular_texture	(bool doesItHave);
		virtual void	set_has_normal_texture		(bool doesItHave);

		
						~ForwardShader();
		
	private:
		
		unsigned int 	maximalAmountOfLights;
		
		GLint			projectionMatrixLocation;
		GLint			cameraTransformationLocation;
		GLint			modelTransformationLocation;
		
		GLint			hasMaterialLocation;
		
		GLint			hasDiffuseTextureLocation;
		GLint			hasSpecularTextureLocation;
		GLint			hasNormalTextureLocation;
		
		GLint			directionalLightColorLocation;
		GLint			directionalLightDirectionLocation;
		GLint			hasDirectionalLightLocation;
		
		GLint			renderModeLocation;
		
		GLint			spotLightsAmount;
		GLint*			spotLightsLocations;
		
		
		GLint			pointLightsAmount;
		GLint*			pointLightsLocations;
		
		GLint			materialBlockLocation;
		
	};
	
}

#endif /* forwardShader_hpp */
