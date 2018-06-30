//
//  orthographicCamera.cpp
//  ngin
//
//  Created by Mateusz Stompór on 12/04/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "orthographicCamera.hpp"

ms::OrthographicCamera::OrthographicCamera (float far, float near,
                                            float top, float bottom,
                                            float right, float left) : ms::Camera(), viewport{far, near, top, bottom, right, left} {
}

bool ms::OrthographicCamera::is_in_camera_sight (math::mat4 const & boundingBoxTransformation, math::BoundingBox<float> const & boundingBox) const {
    return viewport.is_in_camera_sight(this->transformation * boundingBoxTransformation, boundingBox);
}

ms::math::mat4 const & ms::OrthographicCamera::get_projection_matrix () const {
    return viewport.get_projection_matrix();
}
