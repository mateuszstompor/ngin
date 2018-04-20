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
        
		virtual                 ~DirectionalLight   () = default;
            
	};
	
}

ms::DirectionalLight::DirectionalLight(math::vec3 const & col,
                                       float              pow,
                                       math::vec3 const & dir,
                                       math::mat4 const & proj) : ms::Light(col, pow, proj, math::transform::directional_look(dir)) { }

#endif /* directional_light_hpp */
