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
                                                                 float right, float left);
        
                                        ~OrthographicCamera     () = default;
        bool                            is_in_camera_sight      (math::mat4 const & boundingBoxTransformation,
                                                                 math::BoundingBox<float> const & boundingBox) const;
        
        math::mat4 const &              get_projection_matrix   () const;
        
    private:
        
        math::OrthographicViewport<float>   viewport;
        
    };
    
}

#endif /* orthographic_camera_hpp */
