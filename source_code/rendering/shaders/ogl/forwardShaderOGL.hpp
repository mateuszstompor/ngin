//
//  forwardShaderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef forwardShaderOGL_hpp
#define forwardShaderOGL_hpp

#include "shaderOGL.hpp"
#include "../forwardShader.hpp"

namespace ms {
	
	class ForwardShaderOGL : public ShaderOGL, public ForwardShader {
		
	public:
		
						ForwardShaderOGL(std::string vertexShaderSource, std::string fragmentShaderSource);
		
		virtual void 	set_projection_matrix		(const math::mat4 & proj)							override;
		virtual void 	set_camera_transformation	(const math::mat4 & transf)							override;
		virtual void	set_model_transformation	(const math::mat4 & modelTransf) 					override;
		
		virtual void 	set_amount_of_point_lights	(int amount) 										override;
		virtual void 	set_point_light_power		(unsigned int index, float power) 					override;
		virtual void 	set_point_light_color		(unsigned int index, const math::vec4 & color) 		override;
		virtual void 	set_point_light_position	(unsigned int index, const math::vec3 & position) 	override;
		
		virtual void 	set_amount_of_spot_lights	(int amount)										override;
		virtual void 	set_spot_light_power		(unsigned int index, float power) 					override;
		virtual void 	set_spot_light_color		(unsigned int index, const math::vec4 & color) 		override;
		virtual void 	set_spot_light_position		(unsigned int index, const math::vec3 & position)	override;
		virtual void 	set_spot_light_angle		(unsigned int index, float angle) 					override;
		virtual void 	set_spot_light_direction	(unsigned int index, const math::vec3 direction) 	override;
		
		virtual void	set_has_directional_light	(bool doesItHave) 									override;
		virtual void	set_directional_light_dir	(const math::vec3 & dir) 							override;
		virtual void	set_directional_light_color	(const math::vec4 & color) 							override;
		
		virtual void	set_has_material			(bool doesItHave)	 								override;
		virtual void	set_material_ambient_color	(const math::vec3 & ambient)						override;
		virtual void	set_material_diffuse_color	(const math::vec3 & diffuse)						override;
		virtual void	set_material_specular_color	(const math::vec3 & specular)						override;
		virtual void	set_material_opacity		(float opacity)										override;
		virtual void	set_material_shininess		(float shininess)									override;
		
		virtual void	set_has_diffuse_texture		(bool doesItHave)	 								override;
		virtual void	set_has_specular_texture	(bool doesItHave)	 								override;
		
	};
	
}

#endif /* forwardShaderOGL_hpp */
