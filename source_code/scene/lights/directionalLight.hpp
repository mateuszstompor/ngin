//
//  directionalLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef directional_light_hpp
#define directional_light_hpp
#define HALF_SIDE 44.0f
#include "general/light.hpp"

namespace ms {
	
	class DirectionalLight : public virtual Light {
	
	public:
		
		inline 		                            DirectionalLight(math::vec3 const & color,
                                                                 math::vec3 const & direction);
        
        inline                                  DirectionalLight(math::vec3 && color,
                                                                 math::vec3 && direction);
        
		virtual		                            ~DirectionalLight() = default;

		math::vec3 	direction;
		
	};
	
}

ms::DirectionalLight::DirectionalLight(math::vec3 const & c, math::vec3 const & d) : ms::Light(c, math::projection::orthogonal(HALF_SIDE, HALF_SIDE, HALF_SIDE, HALF_SIDE, HALF_SIDE, HALF_SIDE)), direction(d) { }

ms::DirectionalLight::DirectionalLight(math::vec3 && c, math::vec3 && d) : ms::Light(std::move(c),
                                                                           math::projection::orthogonal(HALF_SIDE, HALF_SIDE, HALF_SIDE, HALF_SIDE, HALF_SIDE, HALF_SIDE)),
                                                                           direction(std::move(d)) { }

#endif /* directional_light_hpp */
