//
//  camera.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <memory>

#include "../../../glMath/source/glMath.h"
#include "../positionedObject.hpp"

namespace ms {
	
	class Camera : public PositionedObject {
	
	public:
		
		inline 						            Camera                  ();
		virtual 					            ~Camera                 () = default;
        virtual math::mat4 const &              get_projection_matrix   () const = 0;
        virtual bool                            is_in_camera_sight      (math::mat4 const & boundingBoxTransformation,
                                                                         math::BoundingBox<float> const & boundingBox) const = 0;
    
	};
	
}

ms::Camera::Camera() : PositionedObject() {}

#endif /* camera_hpp */
