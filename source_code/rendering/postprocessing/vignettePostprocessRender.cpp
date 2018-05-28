//
//  vignettePostprocessRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 28/05/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "vignettePostprocessRender.hpp"

ms::VignettePostprocessRender::VignettePostprocessRender   (std::vector<std::weak_ptr<Texture2D>>   input,
                                                            std::unique_ptr<Framebuffer> &&         framebuffer,
                                                            std::unique_ptr<Shader> &&              shaderProgram,
                                                            float                                   innerCircleRadius,
                                                            float                                   outerCircleRadius,
                                                            float                                   effectStrength) :
PostprocessRender{input, std::move(framebuffer), std::move(shaderProgram)},
innerCircleRadius{innerCircleRadius},
outerCircleRadius{outerCircleRadius},
effectStrength{effectStrength} {}


void ms::VignettePostprocessRender::draw() const {
    shader->set_uniform("innerCircleRadius", innerCircleRadius);
    shader->set_uniform("outerCircleRadius", outerCircleRadius);
    shader->set_uniform("effectIntensity", effectStrength);
    PostprocessRender::draw();
}

void ms::VignettePostprocessRender::set_effect_strength (float strength) {
    effectStrength = std::clamp(strength, 0.0f, 1.0f);
}

void ms::VignettePostprocessRender::set_inner_radius (float innerRadius) {
    innerCircleRadius = std::clamp(innerRadius, 0.0f, 1.0f);
}

void ms::VignettePostprocessRender::set_outer_radius (float outerRadius) {
    outerCircleRadius = std::clamp(outerRadius, 0.0f, 1.0f);
}
