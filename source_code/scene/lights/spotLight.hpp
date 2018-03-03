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
		
	};
	
}

#endif /* spot_light_hpp */
