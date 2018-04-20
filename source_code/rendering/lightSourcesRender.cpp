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

void ms::LightSourcesRender::draw (Drawable & node, const Scene & scene) {
    
    shader->set_uniform("cameraTransformation", scene.get_camera().get_transformation());
    shader->set_uniform("perspectiveProjection", scene.get_camera().get_projection_matrix());
    shader->set_uniform("modelTransformation", node.transformation);
    
    auto pointLight = dynamic_cast<PointLight*>(&node);

	if(pointLight) {
        shader->set_uniform("lightSourceColor", pointLight->get_color());
	} else {
        shader->set_uniform("lightSourceColor", math::vec3{0.0f, 1.0f, 0.0f});
	}
	
	node.draw();
	
}
