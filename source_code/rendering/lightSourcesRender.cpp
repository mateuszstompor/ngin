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
                                           std::unique_ptr<LightSourceDrawerShader> && shader) : Render(std::move(framebuffer), std::move(shader)) { }

void ms::LightSourcesRender::draw (Drawable & node, const Scene & scene) {
	LightSourceDrawerShader * drawer = dynamic_cast<LightSourceDrawerShader*>(shader.get());
	drawer->set_camera_transformation(scene.get_camera().get_transformation());
	drawer->set_projection_matrix(scene.get_camera().get_projection_matrix());
	drawer->set_model_transformation(node.modelTransformation.get_transformation());
	auto pointLight = dynamic_cast<PointLight*>(&node);
	
	if(pointLight) {
		drawer->set_light_source_color(pointLight->get_color().c_array());
	} else {
		drawer->set_light_source_color(ms::math::vec3{0.0f, 1.0f, 0.0f}.c_array());
	}
	
	node.draw();
	
}
