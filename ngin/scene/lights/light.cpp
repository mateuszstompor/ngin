//
//  light.cpp
//  ngin
//
//  Created by Mateusz Stompór on 09/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "light.hpp"

ms::Light::Light (math::vec3 const & c,
                  float pow,
                  math::mat4 const & t,
                  bool               cs) : color{c}, transformation{t}, power{pow}, castsShadow{cs} { }

ms::Light::~Light () { }
