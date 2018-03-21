//
//  pointLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef point_light_hpp
#define point_light_hpp

#include "general/light.hpp"
#include "../positionedObject.hpp"
#include "../drawable.hpp"

namespace ms {
	
	class PointLight : public virtual Light, public virtual Drawable {
	
	public:
		
		inline 	PointLight (float 		power,
							math::vec3 	color,
							math::vec3 	position);
		
		virtual ~PointLight() = default;
		
		float			power;
		math::vec3 		position;
		
	};
	
}

ms::PointLight::PointLight (float p, math::vec3 c, math::vec3 pos) : Light(c), power(p), position(pos) { }

#endif /* point_light_hpp */
