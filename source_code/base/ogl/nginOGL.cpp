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
	std::string hdrVertexShader = shader::get_shader_of_type(shader::Type::post_process_hdr_vshader);
	std::string hdrFragmentShader = shader::get_shader_of_type(shader::Type::post_process_hdr_fshader);
	
	
	std::shared_ptr<Framebuffer> windowFramebuffer = FramebufferOGL::window_framebuffer(screenWidth, screenHeight);
	
	std::shared_ptr<FramebufferOGL> tempBuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																									1,
																									frameBufferWidth,
																									frameBufferHeight));
	
	
	std::shared_ptr<Renderbuffer> renderbuf = std::shared_ptr<Renderbuffer>(new RenderbufferOGL(Texture::Format::depth_24,
																								Texture::AssociatedType::UNSIGNED_BYTE,
																								0,
																								frameBufferWidth,
																								frameBufferHeight));
	tempBuffer->bind_depth_buffer(renderbuf);
	std::shared_ptr<Texture> color = std::shared_ptr<Texture>(new TextureOGL(	 Texture::Type::tex_2d, "",
																				 Texture::Format::rgb_16_16_16,
																				 Texture::AssociatedType::FLOAT,
																				 Texture::MinFilter::nearest,
																				 Texture::MagFilter::nearest,
																				 Texture::Wrapping::clamp_to_edge,
																				 Texture::Wrapping::clamp_to_edge,
																				 0,
																				 frameBufferWidth,
																			 	 frameBufferHeight));
	tempBuffer->bind_depth_buffer(renderbuf);
	tempBuffer->bind_color_buffer(0, color);
	
	tempBuffer->configure();
	
	unsigned int AOL = 200;
	
	lightSourceRenderer = std::unique_ptr<LightSourcesRender>(new LightSourceRenderOGL(lightSourceDrawerVertexShader,
																					   lightSourceDrawerFragmentShader,
																					   tempBuffer));

    phongForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRenderOGL(AOL,
																			   forwardRenderVertexShaderSource,
																			   forwardRenderFragmentShaderSource,
																			   tempBuffer));

	gouraudForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRenderOGL(AOL,
																				 gouraudVertexShaderSource,
																				 gouraudRenderFragmentShaderSource,
																				 tempBuffer));

	deferredRenderer = std::unique_ptr<DeferredRenderOGL> (new DeferredRenderOGL( AOL,
																				  deferredRenderVertexShaderSource,
																				  deferredRenderFragmentShaderSource,
																				  deferredRenderLightingVertexShaderSource,
																				  deferredRenderLightingFragmentShaderSource,
																				  tempBuffer));
	
	std::shared_ptr<Framebuffer> framebuffer = std::shared_ptr<Framebuffer>(new FramebufferOGL(1, 0, frameBufferWidth, frameBufferHeight));

	auto hdrprogram = std::shared_ptr<Shader>(new ShaderOGL(hdrVertexShader, "", "", "", hdrFragmentShader));
	
	hdrRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(tempBuffer->get_colors()[0], windowFramebuffer));
	hdrRenderer->shaderProgram = hdrprogram;
	
}

void ms::NGinOGL::load () {
	phongForwardRenderer->load();
	gouraudForwardRenderer->load();
	deferredRenderer->load();
	lightSourceRenderer->load();
	hdrRenderer->load();
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

