//
//  pointLight.cpp
//  ngin
//
//  Created by Mateusz Stompór on 09/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "pointLight.hpp"

ms::PointLight::PointLight (math::vec3 const &  col,
                            float               pow,
                            math::vec3 const &  pos,
                            bool                castsShadow) :
Light{col, pow,  math::transform4f::translate(pos), castsShadow},
projection{math::projection4f::perspective(0.001f, 100.0f, 90.0f, 1.0f)}{ }

