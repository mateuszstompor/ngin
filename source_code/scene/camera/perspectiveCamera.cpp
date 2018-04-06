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
                                         float aspectRatio) :
                                                            nearPlane(nearPlane),
                                                            farPlane(farPlane),
                                                            fovDegrees(fovDegrees),
                                                            aspectRatio(aspectRatio) {
    
	projectionMatrix = ms::math::projection::perspective(nearPlane, farPlane, fovDegrees, aspectRatio);
    
    math::vec3 direction = math::back(this->transformation).normalized();
    math::vec3 up = math::up(this->transformation).normalized();
    math::vec3 right = math::right(this->transformation).normalized();
                                                        
    nearPlaneOrigin = nearPlane * direction;
    farPlaneOrigin = farPlane * direction;
            
    float nearPlaneWidth = nearPlane * direction.length() * tan(math::radians(fovDegrees)) * 2;
    float nearPlaneHeight = nearPlaneWidth / aspectRatio;
    float farPlaneWidth = farPlane * direction.length() * tan(math::radians(fovDegrees)) * 2;
    float farPlaneHeight = farPlaneWidth / aspectRatio;
                                                                
    c = nearPlaneOrigin - right * (nearPlaneWidth/2.0f) - up * (nearPlaneHeight/2.0f);
    d = nearPlaneOrigin + right * (nearPlaneWidth/2.0f) - up * (nearPlaneHeight/2.0f);
    a = nearPlaneOrigin - right * (nearPlaneWidth/2.0f) + up * (nearPlaneHeight/2.0f);
    b = nearPlaneOrigin + right * (nearPlaneWidth/2.0f) + up * (nearPlaneHeight/2.0f);
                                                                
    e = farPlaneOrigin - right * (farPlaneWidth/2.0f) - up * (farPlaneHeight/2.0f);
    f = farPlaneOrigin + right * (farPlaneWidth/2.0f) - up * (farPlaneHeight/2.0f);
    g = farPlaneOrigin - right * (farPlaneWidth/2.0f) + up * (farPlaneHeight/2.0f);
    h = farPlaneOrigin + right * (farPlaneWidth/2.0f) + up * (farPlaneHeight/2.0f);
                                                                
    front = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::front));
    back = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::back));
    top = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::top));
    bottom = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::bottom));
    left = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::left));
    Camera::right = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::right));
    
}

ms::math::Plane<float> ms::PerspectiveCamera::get_camera_plane (FrustumPlane plane) {
    
    switch (plane) {
        case Camera::FrustumPlane::front:
            return math::Plane<float>::from_points(c, a, b);
        case Camera::FrustumPlane::back:
            return math::Plane<float>::from_points(f, e, g);
        case Camera::FrustumPlane::top:
            return math::Plane<float>::from_points(b, f, e);
        case Camera::FrustumPlane::bottom:
            return math::Plane<float>::from_points(g, h, d);
        case Camera::FrustumPlane::left:
            return math::Plane<float>::from_points(e, a, c);
        case Camera::FrustumPlane::right:
            return math::Plane<float>::from_points(d, h, f);
        default:
            assert(false);
            break;
    }
}
