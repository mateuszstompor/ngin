//
//  DLShadowRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 19/04/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "DLShadowRender.hpp"

ms::DLShadowRender::DLShadowRender(std::unique_ptr<Framebuffer> &&    framebuffer,
                                   std::unique_ptr<Shader> &&         shader) : Render{std::move(framebuffer), std::move(shader)} {}

std::string ms::DLShadowRender::get_class () const {
    return "ms::DLShadowRender";
}

void ms::DLShadowRender::draw (Drawable & node) {
    shader->set_uniform("toWorld", node.modelTransformation.get_transformation());
    node.draw();
}

void ms::DLShadowRender::setup_uniforms (math::mat4 const & projection, math::mat4 const & transformation) {
    shader->set_uniform("projection", projection);
    shader->set_uniform("toLight", transformation);
}

