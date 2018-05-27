//
//  gaussianBlurPostprocessRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 27/05/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "gaussianBlurPostprocessRender.hpp"

ms::GaussianBlurPostprocessRender::GaussianBlurPostprocessRender    (std::vector<std::weak_ptr<Texture2D>>  input,
                                                                     std::unique_ptr<Framebuffer> &&        framebuffer,
                                                                     std::unique_ptr<Shader> &&             shaderProgram,
                                                                     int                                    amountOfSamples) :
                                                                                                    PostprocessRender{  input,
                                                                                                                        std::move(framebuffer),
                                                                                                                        std::move(shaderProgram)},
                                                                                                                        amountOfSamplesToTake{amountOfSamples} {}

void ms::GaussianBlurPostprocessRender::draw() const {
    shader->set_uniform("amountOfSamplesToTake", amountOfSamplesToTake);
    PostprocessRender::draw();
}
