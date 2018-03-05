//
//  perspectiveCamera_hpp.cpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "perspectiveCamera.hpp"

ms::PerspectiveCamera::PerspectiveCamera(float nearPlane, float farPlane, float fovDegrees, float aspectRatio) {
	projectionMatrix = ms::math::projection::perspective(nearPlane, farPlane, fovDegrees, aspectRatio);
}
