//
//  positionedObject.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef positioned_object_hpp
#define positioned_object_hpp

#include "../../libs/source/glMath.h"

namespace ms {

	class PositionedObject {

	public:
		
		virtual math::mat4 get_transformation() = 0;
		
		math::mat4 transformation;

	};

}

#endif /* positioned_object_hpp */
