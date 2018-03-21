//
//  spotLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef spot_light_hpp
#define spot_light_hpp

#include "pointLight.hpp"
#include "directionalLight.hpp"

namespace ms {
	
	class SpotLight : public PointLight, public DirectionalLight {
		
	public:
		
		inline 		SpotLight 	(float 			power,
								 math::vec3 	color,
								 math::vec3 	position,
								 float 			lightingAngleDegrees,
								 math::vec3 	direction);
		
		virtual		~SpotLight 	() = default;
		
		float 		lightingAngleDegrees;
		
	};
	
}

ms::SpotLight::SpotLight (float 		power,
						  math::vec3 	color,
						  math::vec3 	position,
						  float			lightingAngle,
						  math::vec3 	direction) :

ms::Light(color),
ms::PointLight(power, color, position),
ms::DirectionalLight(color, direction),
lightingAngleDegrees(lightingAngle) {}

#endif /* spot_light_hpp */
