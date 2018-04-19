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
	// TODO make it able to translate
	class SpotLight : public DirectionalLight, public Drawable {
		
	public:
		
		inline 		                    SpotLight 	(math::vec3 const & color,
                                                     float 			    power,
                                                     math::vec3 const & position,
                                                     float 			    lightingAngleDegrees,
                                                     math::vec3 const & direction);
		
        math::vec3                              get_position    () const { return math::get_position(positionedObject.get_transformation()); }
		
		float 		                    lightingAngleDegrees;
//        math::FrustumViewport<float>    frustum;
		
	};
	
}

ms::SpotLight::SpotLight (math::vec3 const &    color,
                          float                 power,
                          math::vec3 const &    position,
                          float                 lightingAngleDegrees,
                          math::vec3 const &    direction) :
DirectionalLight{color, power, direction, math::projection4f::perspective(0.001f, 100.0f, lightingAngleDegrees, 1.0f)},
lightingAngleDegrees{lightingAngleDegrees} {
//    Light::projection =    math::projection::perspective(0.001f, 100.0f, lightingAngle, 1.0f);
//    auto look = math::transform::look_at(position, position + direction, math::vec3{0.0f, 1.0f, 0.0f});
//    Light::transformation = look;

}

#endif /* spot_light_hpp */
