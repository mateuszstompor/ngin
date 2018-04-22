//
//  spotLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "pointLight.hpp"
#include "directionalLight.hpp"

namespace ms {
	
	class SpotLight : public Light {
		
	public:
		
		inline 		                    		SpotLight 	        (math::vec3 const & color,
																	 float 			    power,
																	 math::vec3 const & position,
																	 float 			    lightingAngleDegrees,
																	 math::vec3 const & direction,
																	 bool               castsShadow,
																	 float              nearPlane = 0.0001f,
																	 float              farPlane = 100.0f,
																	 float              aspectRatio = 1.0f);

        constexpr float						get_angle_degrees   () const { return lightingAngleDegrees; }
		constexpr math::frustum4f const &		get_frustum		   	() const { return frustum; }
        
    private:
        
        math::frustum4f    						frustum;
		float 		                    		lightingAngleDegrees;
		
	};
	
}

ms::SpotLight::SpotLight (math::vec3 const &    color,
                          float                 power,
                          math::vec3 const &    position,
                          float                 lAD,
                          math::vec3 const &    direction,
                          bool                  castsShadow,
                          float                 nearPlane,
                          float                 farPlane,
                          float                 aspectRatio) :
Light{color, power, math::mat4::identity(), castsShadow},
lightingAngleDegrees{lAD},
frustum {nearPlane, farPlane, lAD, aspectRatio} { }
