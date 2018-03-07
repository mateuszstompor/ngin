//
//  directionalLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef directional_light_hpp
#define directional_light_hpp

#include "general/light.hpp"

namespace ms {
	
	class DirectionalLight : public virtual Light {
	
	public:
		
		inline 		DirectionalLight(math::vec4 color, math::vec3 direction);
		
		virtual		~DirectionalLight() = default;
		
		math::vec3 	direction;
		
	};
	
}

ms::DirectionalLight::DirectionalLight(math::vec4 c, math::vec3 d) : ms::Light(c), direction(d) { }

#endif /* directional_light_hpp */
