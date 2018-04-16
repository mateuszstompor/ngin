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
lightingAngleDegrees(lightingAngle) {
	Light::projection =	math::projection::perspective(0.001f, 100.0f, lightingAngle, 1.0f);
    auto look = math::transform::look_at(position, position + direction, math::vec3{0.0f, 1.0f, 0.0f});
    std::cout << look.to_string() << std::endl;
	Light::transformation = look;

}

#endif /* spot_light_hpp */
