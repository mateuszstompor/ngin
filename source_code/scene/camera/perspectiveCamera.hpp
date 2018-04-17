//
//  perspectiveCamera.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef perspective_camera_hpp
#define perspective_camera_hpp

#include "camera.hpp"

namespace ms {
	
	class PerspectiveCamera : public Camera {

	public:
		
                                        PerspectiveCamera       (float nearPlane,
                                                                 float farPlane,
                                                                 float fovDegrees,
                                                                 float aspectRatio);
        
        bool                            is_in_camera_sight      (math::mat4 const & boundingBoxTransformation,
                                                                 math::BoundingBox<float> const & boundingBox) const override;
        
        math::mat4 const &              get_projection_matrix   () const override;
        
    private:
        
        math::FrustumViewport<float>    frustum;
                
	};
	
}

#endif /* perspective_camera_hpp */
