//
//  pointLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "light.hpp"
#include "../drawable.hpp"

namespace ms {
	
	class PointLight : public Light {
	
	public:
		
		inline 	                            PointLight              (math::vec3 const &  color,
                                                                     float 		        power,
                                                                     math::vec3 const &  position,
                                                                     bool                castsShadow);
        constexpr math::mat4 const &        get_projection          () const { return projection; }

    private:
        
        math::mat4 projection;
	};
	
}
ms::PointLight::PointLight (math::vec3 const &  col,
                            float               pow,
                            math::vec3 const &  pos,
                            bool                castsShadow) :
Light(col, pow,  math::transform4f::translate(pos), castsShadow),
projection{math::projection4f::perspective(0.001f, 100.0f, 90.0f, 1.0f)}{ }
