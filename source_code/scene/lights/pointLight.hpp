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
	
	class PointLight : public Light, public Drawable {
	
	public:
		
		inline 	PointLight (math::vec3 const &  color,
                            float 		        power,
							math::vec3 const &  position);
		
        math::vec3                              get_position    () const { return math::get_position(positionedObject.get_transformation()); }
	};
	
}

ms::PointLight::PointLight (math::vec3 const &  col,
                            float               pow,
                            math::vec3 const &  pos) : Light(col, pow, math::projection4f::perspective(0.01f, 100.0f, 90.0f, 1.0f), math::transform4f::translate(pos)) { }

#endif /* point_light_hpp */
