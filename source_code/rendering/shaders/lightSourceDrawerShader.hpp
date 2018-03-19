//
//  lightSourceDrawerShader.h
//  ngin
//
//  Created by Mateusz Stompór on 18/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef lightSourceDrawerShader_hpp
#define lightSourceDrawerShader_hpp

#include "shader.hpp"

namespace ms {
	
	class LightSourceDrawerShader : public virtual Shader {
		
	public:
		virtual void	set_projection_matrix		(const math::mat4 & proj) 							= 0;
		virtual void	set_camera_transformation	(const math::mat4 & transf) 						= 0;
		virtual void	set_model_transformation	(const math::mat4 & transf) 						= 0;
		virtual void	set_light_source_color		(const math::vec3 & color) 	= 0;
		
		virtual 		~LightSourceDrawerShader	()							= default;
		
	};
	
}

#endif /* lightSourceDrawerShader_hpp */
