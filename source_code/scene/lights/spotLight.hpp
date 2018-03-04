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
		
		inline 		SpotLight 	(float power, math::vec4 color, math::mat4 transformation, float lightingAngleDegrees);
		
		virtual		~SpotLight 	() = default;
		
		float 		lightingAngleDegrees;
		
	};
	
}

#endif /* spot_light_hpp */
