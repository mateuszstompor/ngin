//
//  spotLight.cpp
//  ngin
//
//  Created by Mateusz Stompór on 09/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "spotLight.hpp"

ms::SpotLight::SpotLight (math::vec3 const &    color,
                          float                 power,
                          math::vec3 const &    position,
                          float                 lAD,
                          math::vec3 const &    direction,
                          bool                  castsShadow,
                          float                 nearPlane,
                          float                 farPlane,
                          float                 aspectRatio) :
Light{color, power, math::mat4::identity(), castsShadow},
lightingAngleDegrees{lAD},
frustum {nearPlane, farPlane, lAD, aspectRatio} { }
