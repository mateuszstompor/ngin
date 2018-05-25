//
//  directionalLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "light.hpp"

namespace ms {
	
	class DirectionalLight : public Light {
        
	public:
        
		inline                          DirectionalLight    (math::vec3 const & color,
                                                             float              power,
                                                             math::vec3 const & direction,
                                                             bool               castsShadow,
                                                             math::mat4 const & projection = math::projection4f::orthogonal_cube(44.0f));
        
        constexpr math::mat4 const &    get_projection      () const { return projection; }
		virtual                         ~DirectionalLight   () = default;
        
    private:
        
        math::mat4 projection;
        
	};
	
}

ms::DirectionalLight::DirectionalLight(math::vec3 const & col,
                                       float              pow,
                                       math::vec3 const & dir,
                                       bool               castsShadow,
                                       math::mat4 const & projection) :
Light{col, pow, math::mat4::identity(), castsShadow},
projection{projection} { }
