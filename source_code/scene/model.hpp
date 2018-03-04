//
//  model.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef model_hpp
#define model_hpp

#include <vector>

#include "../resources/resource.hpp"
#include "../../libs/source/glMath.h"

namespace ms {
	
	class Model : public Resource {
	
	protected:
		
		Model					() = default;
		
	public:
		
		std::vector<math::vec3> vertices;
		std::vector<math::vec3> normals;
		
	};
	
}

#endif /* model_hpp */
