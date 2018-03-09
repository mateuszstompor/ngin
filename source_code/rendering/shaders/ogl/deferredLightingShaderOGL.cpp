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

void  ms::DeferredLightingShaderOGL::load () {
    ShaderOGL::load();

    mglUseProgram(program);

    GLint gPosition = mglGetUniformLocation(program, "gPosition");
    mglUniform1i(gPosition, 0);

    GLint gNormal = mglGetUniformLocation(program, "gNormal");
    mglUniform1i(gNormal, 1);

    GLint gAlbedo = mglGetUniformLocation(program, "gAlbedo");
    mglUniform1i(gAlbedo, 2);

    mglUseProgram(0);

}
