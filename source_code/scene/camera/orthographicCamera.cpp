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
                                            float left, float right) : ms::Camera() {
    
    projectionMatrix = math::projection::orthogonal(far, near, top, bottom, left, right);
    
    c = math::vec3(left, bottom, near);
    d = math::vec3(right, bottom, near);
    a = math::vec3(left, top, near);
    b = math::vec3(right, top, near);
    
    g = math::vec3(left, bottom, far);
    h = math::vec3(right, bottom, far);
    e = math::vec3(left, top, far);
    f = math::vec3(right, top, far);
    
    Camera::front = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::front));
    Camera::back = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::back));
    Camera::top = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::top));
    Camera::bottom = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::bottom));
    Camera::left = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::left));
    Camera::right = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::right));
    
}
