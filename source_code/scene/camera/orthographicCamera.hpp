//
//  orthographicCamera.hpp
//  ngin
//
//  Created by Mateusz Stompór on 12/04/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef orthographic_camera_hpp
#define orthographic_camera_hpp

#include "camera.hpp"

namespace ms {
    
    class OrthographicCamera : public Camera {
        
    public:
        
                                        OrthographicCamera      (float far, float near,
                                                                 float top, float bottom,
                                                                 float left, float right);
        
        virtual                         ~OrthographicCamera     () = default;
        
    protected:
        
        float _far;
        float _near;
        float _top;
        float _bottom;
        float _left;
        float _right;
        
    };
    
}

#endif /* orthographic_camera_hpp */