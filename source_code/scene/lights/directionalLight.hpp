//
//  directionalLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef directional_light_hpp
#define directional_light_hpp

#define PROJECTION_CUBE_SIDE 38.0f

#include "general/light.hpp"

namespace ms {
	
	class DirectionalLight : public virtual Light {
        
	public:
        
		inline 		                            DirectionalLight(math::vec3 const & color,
                                                                 math::vec3 const & direction,
                                                                 math::mat4 const & projection = math::projection::orthogonal_cube<float>(PROJECTION_CUBE_SIDE));
        
        inline                                  DirectionalLight(math::vec3 && color,
                                                                 math::vec3 && direction,
                                                                 math::mat4 && projection = math::projection::orthogonal_cube<float>(PROJECTION_CUBE_SIDE));
        
		virtual		                            ~DirectionalLight() = default;
        
		math::vec3 	direction;
		
	};
	
}

ms::DirectionalLight::DirectionalLight(math::vec3 const & c, math::vec3 const & d, math::mat4 const & p) : ms::Light(c, p), direction(d) { }
ms::DirectionalLight::DirectionalLight(math::vec3 && c, math::vec3 && d, math::mat4 && p) : ms::Light(std::move(c), std::move(p)), direction(std::move(d)) { }

#endif /* directional_light_hpp */
