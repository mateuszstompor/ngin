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
                                                                
    e = farPlaneOrigin - right * (farPlaneWidth/2.0f) + up * (farPlaneHeight/2.0f);
    f = farPlaneOrigin + right * (farPlaneWidth/2.0f) + up * (farPlaneHeight/2.0f);
    g = farPlaneOrigin - right * (farPlaneWidth/2.0f) - up * (farPlaneHeight/2.0f);
    h = farPlaneOrigin + right * (farPlaneWidth/2.0f) - up * (farPlaneHeight/2.0f);
                                                                
    Camera::front = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::front));
    Camera::back = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::back));
    Camera::top = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::top));
    Camera::bottom = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::bottom));
    Camera::left = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::left));
    Camera::right = std::make_unique<math::Plane<float>>(get_camera_plane(Camera::FrustumPlane::right));
    
}
