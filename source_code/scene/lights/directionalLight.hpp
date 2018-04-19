//
//  directionalLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef directional_light_hpp
#define directional_light_hpp

#include "general/light.hpp"

namespace ms {
	
	class DirectionalLight : public Light {
        
	public:
        
		inline                  DirectionalLight    (math::vec3 const & color,
                                                     float              power,
                                                     math::vec3 const & direction,
                                                     math::mat4 const & projection);
        
        inline                  DirectionalLight    (math::vec3 && color,
                                                     float         power,
                                                     math::vec3 && direction,
                                                     math::mat4 && projection);

        inline      math::vec3  get_direction       () const;
		virtual                 ~DirectionalLight   () = default;
            
	};
	
}

ms::DirectionalLight::DirectionalLight(math::vec3 const & col,
                                       float              pow,
                                       math::vec3 const & dir,
                                       math::mat4 const & proj) : ms::Light(col, pow, proj, math::mat4::identity()) { }

ms::DirectionalLight::DirectionalLight(math::vec3 && col,
                                       float         pow,
                                       math::vec3 && dir,
                                       math::mat4 && proj) : ms::Light(std::move(col), pow, std::move(proj), math::mat4::identity()) { }

ms::math::vec3 ms::DirectionalLight::get_direction () const {
    return math::back(positionedObject.get_transformation());
}

#endif /* directional_light_hpp */
