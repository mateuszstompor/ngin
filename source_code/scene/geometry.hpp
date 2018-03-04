//
//  geometry.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef geometry_hpp
#define geometry_hpp

#include <vector>

#include "../resources/resource.hpp"
//#include "../../libs/source/glMath.h"

namespace ms {
	
	class Geometry : public Resource {
	
	public:
		
		virtual void 	use_normals 		() = 0;
		virtual void 	use_vertices 		() = 0;
						Geometry			() = default;
		virtual			~Geometry			() = default;
		
	public:
		
		std::vector<float> vertices;
		std::vector<float> normals;
		
	};
	
}

#endif /* geometry_hpp */
