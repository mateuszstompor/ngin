//
//  directionalLight.cpp
//  ngin
//
//  Created by Mateusz Stompór on 09/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "directionalLight.hpp"

ms::DirectionalLight::DirectionalLight(math::vec3 const & col,
                                       float              pow,
                                       math::vec3 const & dir,
                                       bool               castsShadow,
                                       math::mat4 const & projection) :
Light{col, pow, math::mat4::identity(), castsShadow},
projection{projection} { }
