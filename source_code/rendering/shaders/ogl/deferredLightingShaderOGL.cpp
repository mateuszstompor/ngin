//
//  deferredLightingShaderOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 07/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredLightingShaderOGL.hpp"

ms::DeferredLightingShaderOGL::DeferredLightingShaderOGL(str_ptr vertexShaderSource, str_ptr fragmentShaderSource) :
ms::ShaderOGL(vertexShaderSource, nullptr, nullptr, nullptr, fragmentShaderSource) { }
