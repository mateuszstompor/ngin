//
//  lightSourcesRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 24/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "lightSourcesRender.hpp"

ms::LightSourcesRender::LightSourcesRender(
										   std::unique_ptr<Framebuffer> && framebuffer,
                                           std::unique_ptr<Shader> && shader) : Render(std::move(framebuffer), std::move(shader)) { }

void ms::LightSourcesRender::draw (Drawable & node) {
    shader->set_uniform("modelTransformation", node.get_transformation());
    if(auto material = node.get_material()) {
        shader->set_uniform("lightSourceColor", material->get_diffuse_color());
    } else {
        shader->set_uniform("lightSourceColor", math::vec3{0.0f, 1.0f, 0.0f});
    }
    node.draw();
}

void ms::LightSourcesRender::set_camera (Camera const & camera) {
    shader->set_uniform("cameraTransformation", camera.get_transformation());
    shader->set_uniform("perspectiveProjection", camera.get_projection_matrix());
}

