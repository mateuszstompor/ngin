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
		
                                        PerspectiveCamera   (float nearPlane,
                                                             float farPlane,
                                                             float fovDegrees,
                                                             float aspectRatio);
        
    protected:
        
        float nearPlane;
        float farPlane;
        float fovDegrees;
        float aspectRatio;
                
	};
	
}

#endif /* perspective_camera_hpp */
