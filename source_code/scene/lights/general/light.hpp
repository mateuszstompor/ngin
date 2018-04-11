//
//  light.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include "../../../../glMath/source/glMath.h"

namespace ms {
	
	class Light {
	
	public:
		
		inline 			Light	(math::vec3 color);
		virtual		 	~Light	() = default;
		
		math::vec3 		color;
		
	};
	
}

ms::Light::Light (math::vec3 c) : color(c) { }

#endif /* light_hpp */
