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

namespace ms {
	
	class PointLight : public Light, public PositionedObject {
		
		inline PointLight (float power, math::vec4 color, math::mat4 transformation);
		
	};
	
}

ms::PointLight::PointLight (float p, math::vec4 c, math::mat4 t) : Light(p, c), ms::PositionedObject(t) { }

#endif /* point_light_hpp */
