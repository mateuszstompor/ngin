//
//  nginOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "nginOGL.hpp"

ms::NGinOGL::NGinOGL (	unsigned int screenWidth,
						unsigned int screenHeight,
						unsigned int frameBufferWidth,
						unsigned int frameBufferHeight,
						float camNear,
						float camFar,
						float fovDegrees,
						float aspect,
					  	GLuint defaultFBO
						) : NGin(screenWidth, screenHeight, camNear, camFar, fovDegrees, aspect) {
	
	
	std::string forwardRenderVertexShaderSource = shader::get_shader_of_type(shader::Type::forward_render_phong_vshader);
	std::string forwardRenderFragmentShaderSource = shader::get_shader_of_type(shader::Type::forward_render_phong_fshader);
	std::string gouraudVertexShaderSource = shader::get_shader_of_type(shader::Type::forward_render_gouraud_vshader);
	std::string gouraudRenderFragmentShaderSource = shader::get_shader_of_type(shader::Type::forward_render_gouraud_fshader);
	std::string deferredRenderVertexShaderSource = shader::get_shader_of_type(shader::Type::deferred_render_g_buf_vertex_shader);
	std::string deferredRenderFragmentShaderSource = shader::get_shader_of_type(shader::Type::deferred_render_g_buf_fragment_shader);
	std::string deferredRenderLightingVertexShaderSource = shader::get_shader_of_type(shader::Type::deferred_render_light_pass_vshader);
	std::string deferredRenderLightingFragmentShaderSource = shader::get_shader_of_type(shader::Type::deferred_render_light_pass_fshader);
	std::string lightSourceDrawerVertexShader = shader::get_shader_of_type(shader::Type::forward_render_light_drawer_vshader);
	std::string lightSourceDrawerFragmentShader = shader::get_shader_of_type(shader::Type::forward_render_light_drawer_fshader);
	
	unsigned int AOL = 200;
	
	lightSourceRenderer = std::unique_ptr<LightSourcesRender>(new LightSourceRenderOGL(lightSourceDrawerVertexShader,
																					   lightSourceDrawerFragmentShader,
																					   screenWidth,
																					   screenHeight,
																					   frameBufferWidth,
																					   frameBufferHeight));
	
    phongForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRenderOGL(AOL,
																			   forwardRenderVertexShaderSource,
																			   forwardRenderFragmentShaderSource,
																			   screenWidth,
																			   screenHeight,
																			   frameBufferWidth,
																			   frameBufferHeight));

	gouraudForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRenderOGL(AOL,
																				 gouraudVertexShaderSource,
																				 gouraudRenderFragmentShaderSource,
																				 screenWidth,
																				 screenHeight,
																				 frameBufferWidth,
																				 frameBufferHeight));
	
	auto defPtr = std::unique_ptr<DeferredRenderOGL> (new DeferredRenderOGL(  AOL,
																			  deferredRenderVertexShaderSource,
																			  deferredRenderFragmentShaderSource,
																			  deferredRenderLightingVertexShaderSource,
																			  deferredRenderLightingFragmentShaderSource,
																			  screenWidth,
																			  screenHeight,
																			  frameBufferWidth,
																			  frameBufferHeight));
	defPtr->set_default_FBO(defaultFBO);
	deferredRenderer = std::move(defPtr);
	
}

void ms::NGinOGL::load () {
//	phongForwardRenderer->load();
//	gouraudForwardRenderer->load();
	deferredRenderer->load();
	lightSourceRenderer->load();
}

std::shared_ptr<ms::PointLight> ms::NGinOGL::get_point_light(float 		power,
														   math::vec3 	color,
														   math::vec3 	position) {
	
	return std::shared_ptr<PointLight>(new PointLightOGL(power, color, position));
	
}

std::shared_ptr<ms::Drawable> ms::NGinOGL::get_drawable() {
	return std::shared_ptr<Drawable>(new DrawableOGL());
}

std::unique_ptr<ms::Loader> ms::NGinOGL::get_loader () {
	return std::unique_ptr<Loader>(new LoaderOGL());
}

std::shared_ptr<ms::SpotLight> ms::NGinOGL::get_spot_light (float 		power,
															math::vec3 	color,
															math::vec3 	position,
															float 		lightingAngleDegrees,
															math::vec3 	direction) {
	
	return std::shared_ptr<SpotLight>(new SpotLightOGL(power, color, position, lightingAngleDegrees, direction));
	
}

