//
//  lightSourceDrawerShaderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 18/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef light_source_drawer_shader_ogl_hpp
#define light_source_drawer_shader_ogl_hpp

#include "../lightSourceDrawerShader.hpp"
#include "shaderOGL.hpp"

namespace ms {
	
	class LightSourceDrawerShaderOGL : public ShaderOGL, public LightSourceDrawerShader  {
		
	public:
						LightSourceDrawerShaderOGL	(std::string vertexShaderSource,
													 std::string fragmentShaderSource);
		
		virtual void	set_projection_matrix		(const math::mat4 & proj) 		override;
		virtual void	set_camera_transformation	(const math::mat4 & transf) 	override;
		virtual void	set_model_transformation	(const math::mat4 & transf) 	override;
		virtual void 	set_light_source_color 		(const math::vec3 & color) 		override;
		
	};
	
}

#endif /* light_source_drawer_shader_ogl_hpp */
