//
//  deferredShaderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferredShaderOGL_hpp
#define deferredShaderOGL_hpp

#include "shaderOGL.hpp"
#include "../deferredShader.hpp"

namespace ms {
	
	class DeferredShaderOGL : public virtual ShaderOGL, public virtual DeferredShader {
	
	public:
		
		DeferredShaderOGL(str_ptr vertexShaderSource, str_ptr fragmentShaderSource);
		
		virtual void 	set_projection_matrix(math::mat4 proj) 					override;
		virtual void 	set_camera_transformation(math::mat4 transf) 			override;
		virtual void	set_model_transformation	(math::mat4 modelTransf) 	override;
		virtual void	set_has_directional_light	(bool doesItHave)	 		override;
		virtual void	set_directional_light_dir	(math::vec3 dir)	 		override;
		virtual void	set_directional_light_color	(math::vec4 color)	 		override;
		virtual void	set_has_material			(bool doesItHave)	 		override;
		virtual void	set_material_ambient_color	(math::vec3 ambient)		override;
		virtual void	set_material_diffuse_color	(math::vec3 diffuse)		override;
		virtual void	set_material_specular_color	(math::vec3 specular)		override;
		virtual void	set_material_opacity		(float opacity)				override;
		virtual void	set_material_shininess		(float shininess)			override;
		
		
	};
	
}

#endif /* deferredShaderOGL_hpp */
