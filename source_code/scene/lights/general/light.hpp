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
		
		inline 			Light	(math::vec4 color);
		virtual		 	~Light	() = default;
		
		math::vec4 		color;
		
	};
	
}

ms::Light::Light (math::vec4 c) : color(c) { }

#endif /* light_hpp */
