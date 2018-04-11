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
		
        enum class FrustumPlane;
        
		inline 						            Camera                  ();
		virtual 					            ~Camera                 () = default;
        inline virtual bool                     is_in_camera_sight      (math::mat4 const & boundingBoxTransformation,
                                                                         math::BoundingBox<float> const & boundingBox) final;
        virtual math::Plane<float>              get_camera_plane        (FrustumPlane plane) = 0;
		inline virtual math::mat4 	            get_projection_matrix   ();
	
	protected:
        
        math::mat4 					            projectionMatrix {};
        std::unique_ptr<math::Plane<float>>     front {};
        std::unique_ptr<math::Plane<float>>     back {};
        
        std::unique_ptr<math::Plane<float>>     top {};
        std::unique_ptr<math::Plane<float>>     bottom {};
        
        std::unique_ptr<math::Plane<float>>     left {};
        std::unique_ptr<math::Plane<float>>     right {};
        
        math::vec3 a, b, c, d, e, f, g, h;
        math::vec3 nearPlaneOrigin;
        math::vec3 farPlaneOrigin;
        math::vec3 leftPlaneOrigin;
        math::vec3 rightPlaneOrigin;
        math::vec3 topPlaneOrigin;
        math::vec3 bottomPlaneOrigin;
        
	};
	
}

ms::Camera::Camera() : PositionedObject() {}

ms::math::mat4 ms::Camera::get_projection_matrix() {
	return projectionMatrix;
}


//
//               e-------f
//              /|      /|
//             / |     / |
//            a--|----b  |
//            |  g----|--h
//            | /     | /
//            c-------d
//
//                 ^
//                 |       direction of sight
//               camera
//
//
// points {c, d, g, h} - plane bottom
// points {a, b, e, f} - plane top
// points {c, g, a, e} - plane left
// points {d, b, h, f} - plane right
// points {a, b, c, d} - plane front
// points {g, e, f, h} - plane back


enum class ms::Camera::FrustumPlane {
    front, back,
    left, right,
    top, bottom
};

bool ms::Camera::is_in_camera_sight(math::mat4 const & boundingBoxTransformation, math::BoundingBox<float> const & boundingBox) {
    auto finalTransformation = this->transformation * boundingBoxTransformation;
    
    if(back->get_position(finalTransformation, boundingBox) == math::Plane<float>::RelativePosition::in_front) {
        return false;
    }
    
    if(left->get_position(finalTransformation, boundingBox) == math::Plane<float>::RelativePosition::in_front) {
        return false;
    }
    
    if(right->get_position(finalTransformation, boundingBox) == math::Plane<float>::RelativePosition::in_front) {
        return false;
    }
    
    if(front->get_position(finalTransformation, boundingBox) == math::Plane<float>::RelativePosition::in_front) {
        return false;
    }
    
    if(top->get_position(finalTransformation, boundingBox) == math::Plane<float>::RelativePosition::in_front) {
        return false;
    }
    
    return true;
    
}


#endif /* camera_hpp */
