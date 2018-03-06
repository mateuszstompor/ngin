//
//  material.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef material_hpp
#define material_hpp

#include "../../libs/source/glMath.h"

namespace ms {
	
	struct Material {
		
		inline Material(math::vec3 ambient, math::vec3 diffuse, math::vec3 specular, float shininess, float opacity);
		
		math::vec3 	diffuseColor;
		math::vec3 	ambientColor;
		math::vec3 	specularColor;
		float 		shininess;
		float		opacity;
		
	};
	
}

ms::Material::Material(math::vec3 ambient, math::vec3 diffuse, math::vec3 specular, float shin, float opac) :
	ambientColor(ambient),
	diffuseColor(diffuse),
	specularColor(specular),
	shininess(shin),
	opacity(opac) { }

#endif /* material_hpp */
