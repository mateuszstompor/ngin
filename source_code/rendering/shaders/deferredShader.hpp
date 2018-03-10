//
//  deferredShader.h
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferred_shader_h
#define deferred_shader_h

#include "shader.hpp"

namespace ms {
	
	class DeferredShader : public virtual Shader {
		
	public:
		
		virtual void	set_projection_matrix		(const math::mat4 & proj) 		= 0;
		virtual void	set_camera_transformation	(const math::mat4 & transf) 	= 0;
		virtual void	set_model_transformation	(const math::mat4 & transf) 	= 0;
		
		virtual void	set_has_material			(bool doesItHave)	 			= 0;
		virtual void	set_material_ambient_color	(const math::vec3 & ambient)	= 0;
		virtual void	set_material_diffuse_color	(const math::vec3 & diffuse)	= 0;
		virtual void	set_material_specular_color	(const math::vec3 & specular)	= 0;
		virtual void	set_material_opacity		(float opacity)					= 0;
		virtual void	set_material_shininess		(float shininess)				= 0;
		
		virtual			~DeferredShader()									= default;
		
	};
	
}

#endif /* deferred_shader_h */
