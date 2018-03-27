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
#include "../../scene/texture.hpp"

namespace ms {
	
	class DeferredShader : public virtual Shader {
		
	public:
		
		virtual void	set_projection_matrix		(const math::mat4 & proj) 		= 0;
		virtual void	set_camera_transformation	(const math::mat4 & transf) 	= 0;
		virtual void	set_model_transformation	(const math::mat4 & transf) 	= 0;
		
		virtual void	set_has_material			(bool doesItHave)	 			= 0;
		virtual void	bind_diffuse_texture		(Texture & texture)				= 0;
		virtual void	bind_specular_texture		(Texture & texture)				= 0;
		virtual void	set_has_diffuse_texture		(bool doesItHave)	 			= 0;
		virtual void	set_has_specular_texture	(bool doesItHave)	 			= 0;
		
		virtual			~DeferredShader()											= default;
		
	};
	
}

#endif /* deferred_shader_h */
