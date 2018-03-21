//
//  pointLightOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 19/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef pointLightOGL_hpp
#define pointLightOGL_hpp

#include "../../ogl/drawableOGL.hpp"
#include "../pointLight.hpp"

namespace ms {
	
	class PointLightOGL : public PointLight, public DrawableOGL {
	
	public:
		
		inline PointLightOGL (float 		power,
							  math::vec3 	color,
							  math::vec3 	position);
		
	};
	
}

ms::PointLightOGL::PointLightOGL (float power,
								  math::vec3 color,
								  math::vec3 position) : PointLight(power, color, position), ms::Light(color) { }

#endif /* pointLightOGL_hpp */
