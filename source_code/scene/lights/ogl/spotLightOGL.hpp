//
//  spotLightOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 19/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef spotLightOGL_hpp
#define spotLightOGL_hpp

#include "../spotLight.hpp"
#include "../../ogl/drawableOGL.hpp"

namespace ms {

	class SpotLightOGL : public SpotLight, public DrawableOGL {

	public:

		inline 		SpotLightOGL	(float 			power,
									 math::vec4 	color,
									 math::vec3 	position,
									 float 			lightingAngleDegrees,
									 math::vec3 	direction);

	};

}

ms::SpotLightOGL::SpotLightOGL (float 		power,
								math::vec4 	color,
								math::vec3 	position,
								float			lightingAngle,
								math::vec3 	direction) : Light(color), ms::SpotLight(power, color, position, lightingAngle, direction) { }

#endif /* spotLightOGL_hpp */
