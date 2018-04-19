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

void ms::DLShadowRender::draw (Drawable & node, const Scene & scene) {
    shader->set_uniform("toWorld", node.modelTransformation.get_transformation());
    node.draw();
}

void ms::DLShadowRender::setup_uniforms (Scene const & scene) {
    shader->set_uniform("projection", scene.get_directional_light()->get_projection());
    shader->set_uniform("toLight", scene.get_directional_light()->get_transformation());
}
