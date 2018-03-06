//
//  forwardShader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef forward_shader_hpp
#define forward_shader_hpp

#include "shader.hpp"
#include "../../../libs/source/glMath.h"

namespace ms {
	
	class ForwardShader : public virtual Shader {
		
	public:
		
		virtual void	set_projection_matrix		(math::mat4 proj) 		= 0;
		virtual void	set_camera_transformation	(math::mat4 transf) 	= 0;
		virtual void	set_model_transformation	(math::mat4 transf) 	= 0;
		
		virtual void	set_has_directional_light	(bool doesItHave)	 	= 0;
		virtual void	set_directional_light_dir	(math::vec3 dir)	 	= 0;
		virtual void	set_directional_light_color	(math::vec4 color)	 	= 0;
		virtual void	set_directional_light_pow	(float power)		 	= 0;
		
		virtual void	set_has_material			(bool doesItHave)	 	= 0;
		virtual void	set_material_ambient_color	(math::vec3 ambient)	= 0;
		virtual void	set_material_diffuse_color	(math::vec3 diffuse)	= 0;
		virtual void	set_material_specular_color	(math::vec3 specular)	= 0;
		virtual void	set_material_opacity		(float opacity)			= 0;
		virtual void	set_material_shininess		(float shininess)		= 0;

	};
	
}

#endif /* forward_shader_hpp */
