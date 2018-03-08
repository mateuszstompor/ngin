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

    glUseProgram(program);

    GLint gPosition = glGetUniformLocation(program, "gPosition");
    glUniform1i(gPosition, 0);

    GLint gNormal = glGetUniformLocation(program, "gNormal");
    glUniform1i(gNormal, 1);

    GLint gAlbedo = glGetUniformLocation(program, "gAlbedo");
    glUniform1i(gAlbedo, 2);

    glUseProgram(0);

}