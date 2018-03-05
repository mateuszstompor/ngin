//
//  light.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include "../../../../libs/source/glMath.h"

namespace ms {
	
	class Light {
	
	public:
		
		inline 			Light	(float power, math::vec4 color);
		virtual		 	~Light	() = default;
		
		float			power;
		math::vec4 		color;
		
	};
	
}

ms::Light::Light (float p, math::vec4 c) : power(p), color(c) { }

#endif /* light_hpp */
