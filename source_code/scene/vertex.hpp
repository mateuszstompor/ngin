//
//  vertex.hpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef vertex_h
#define vertex_h

#include "../../glMath/source/glMath.h"

namespace ms {
	
	struct Vertex {
		math::vec3 position;
		math::vec3 normal;
        math::vec3 tangent;
        math::vec3 bitangent;
		math::vec2 textureCoordinates;
	};

}

#endif /* vertex_h */
