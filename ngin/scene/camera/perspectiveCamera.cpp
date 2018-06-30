//
//  perspectiveCamera_hpp.cpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "perspectiveCamera.hpp"

ms::PerspectiveCamera::PerspectiveCamera(float nearPlane,
                                         float farPlane,
                                         float fovDegrees,
                                         float aspectRatio) :frustum{nearPlane, farPlane, fovDegrees, aspectRatio} {
    
}

bool ms::PerspectiveCamera::is_in_camera_sight (math::mat4 const & boundingBoxTransformation,
                                                math::BoundingBox<float> const & boundingBox) const {
    return frustum.is_in_camera_sight(this->transformation * boundingBoxTransformation, boundingBox);
}

ms::math::mat4 const & ms::PerspectiveCamera::get_projection_matrix () const {
    return frustum.get_projection_matrix();
}
