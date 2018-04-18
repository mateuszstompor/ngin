//
//  lightSourceDrawerShader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 18/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef light_source_drawer_shader_hpp
#define light_source_drawer_shader_hpp

#include "shader.hpp"

namespace ms {
	
	class LightSourceDrawerShader : public Shader  {
		
	public:
                LightSourceDrawerShader	    (std::string vertexShaderSource,
                                             std::string fragmentShaderSource);
		
		void	set_projection_matrix		(const math::mat4 & proj);
		void	set_camera_transformation	(const math::mat4 & transf);
		void	set_model_transformation	(const math::mat4 & transf);
        void 	set_light_source_color 		(const math::vec3 & color);
		
	};
	
}

#endif /* light_source_drawer_shader_hpp */
