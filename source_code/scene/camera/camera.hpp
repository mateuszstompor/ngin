//
//  camera.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include "../../../libs/source/glMath.h"
#include "../positionedObject.hpp"

namespace ms {
	
	class Camera : public PositionedObject {
	
	public:
		
		inline 						Camera();
		virtual 					~Camera() = default;
		
		inline virtual math::mat4 	get_projection_matrix();
	
	protected:
		
		math::mat4 					projectionMatrix;
		
	};
	
}

ms::Camera::Camera() : PositionedObject() { }

ms::math::mat4 ms::Camera::get_projection_matrix() {
	return projectionMatrix;
}

#endif /* camera_hpp */
