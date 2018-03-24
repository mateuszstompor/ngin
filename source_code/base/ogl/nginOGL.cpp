
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
	
	std::string bloomVertexShader = shader::get_shader_of_type(shader::Type::post_process_bloom_vshader);
	std::string bloomFragmentShader = shader::get_shader_of_type(shader::Type::post_process_bloom_fshader);
	
	
	windowFramebuffer = FramebufferOGL::window_framebuffer(screenWidth, screenHeight);
	
	tempBuffer1 = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																									1,
																									frameBufferWidth,
																									frameBufferHeight));
	
	tempBuffer2 = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(2,
																									1,
																									frameBufferWidth,
																									frameBufferHeight));
	
	
	std::shared_ptr<Renderbuffer> renderbuf1 = std::shared_ptr<Renderbuffer>(new RenderbufferOGL(Texture::Format::depth_24,
																								Texture::AssociatedType::UNSIGNED_BYTE,
																								0,
																								frameBufferWidth,
																								frameBufferHeight));
	
	std::shared_ptr<Renderbuffer> renderbuf2 = std::shared_ptr<Renderbuffer>(new RenderbufferOGL(Texture::Format::depth_24,
																								Texture::AssociatedType::UNSIGNED_BYTE,
																								0,
																								frameBufferWidth,
																								frameBufferHeight));
	
	
	std::shared_ptr<Texture> color1 = std::shared_ptr<Texture>(new TextureOGL(	 Texture::Type::tex_2d, "",
																				 Texture::Format::rgb_16_16_16,
																				 Texture::AssociatedType::FLOAT,
																				 Texture::MinFilter::nearest,
																				 Texture::MagFilter::nearest,
																				 Texture::Wrapping::clamp_to_edge,
																				 Texture::Wrapping::clamp_to_edge,
																				 0,
																				 frameBufferWidth,
																			 	 frameBufferHeight));
	
	std::shared_ptr<Texture> color2 = std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d, "",
																			  Texture::Format::rgb_16_16_16,
																			  Texture::AssociatedType::FLOAT,
																			  Texture::MinFilter::nearest,
																			  Texture::MagFilter::nearest,
																			  Texture::Wrapping::clamp_to_edge,
																			  Texture::Wrapping::clamp_to_edge,
																			  0,
																			  frameBufferWidth,
																			  frameBufferHeight));
//
//	std::shared_ptr<Texture> color22 = std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d, "",
//																			  Texture::Format::rgb_16_16_16,
//																			  Texture::AssociatedType::FLOAT,
//																			  Texture::MinFilter::nearest,
//																			  Texture::MagFilter::nearest,
//																			  Texture::Wrapping::clamp_to_edge,
//																			  Texture::Wrapping::clamp_to_edge,
//																			  0,
//																			  frameBufferWidth,
//																			  frameBufferHeight));
	
	
	tempBuffer1->bind_depth_buffer(renderbuf1);
	tempBuffer1->bind_color_buffer(0, color1);
	tempBuffer1->configure();
	
	tempBuffer2->bind_depth_buffer(renderbuf2);
	tempBuffer2->bind_color_buffer(0, color2);
//	tempBuffer2->bind_color_buffer(1, color22);
	tempBuffer2->configure();
	
	unsigned int AOL = 200;
	


	std::unique_ptr<ForwardShader> phongforwardShader = std::unique_ptr<ForwardShader>(new ForwardShaderOGL(AOL, forwardRenderVertexShaderSource, forwardRenderFragmentShaderSource));
	std::unique_ptr<ForwardShader> gouraudforwardShader = std::unique_ptr<ForwardShader>(new ForwardShaderOGL(AOL, gouraudVertexShaderSource, gouraudRenderFragmentShaderSource));
	std::unique_ptr<LightSourceDrawerShader> lightSourceforwardShader = std::unique_ptr<LightSourceDrawerShader>(new LightSourceDrawerShaderOGL(lightSourceDrawerVertexShader, lightSourceDrawerFragmentShader));
	std::unique_ptr<DeferredShader> deferredShader = std::unique_ptr<DeferredShader>(new DeferredShaderOGL(deferredRenderVertexShaderSource, deferredRenderFragmentShaderSource));
	
	
	lightSourceRenderer = std::unique_ptr<LightSourcesRender>(new LightSourcesRender(windowFramebuffer, std::move(lightSourceforwardShader)));
	
	phongForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRender(AOL, tempBuffer1, std::move(phongforwardShader)));
	
	gouraudForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRender(AOL, tempBuffer1, std::move(gouraudforwardShader)));

	deferredRenderer = std::unique_ptr<DeferredRenderOGL> (new DeferredRenderOGL( AOL,
																				  deferredRenderVertexShaderSource,
																				  deferredRenderFragmentShaderSource,
																				  deferredRenderLightingVertexShaderSource,
																				  deferredRenderLightingFragmentShaderSource,
																				  tempBuffer1));
	
	auto hdrProgram = std::unique_ptr<Shader>(new ShaderOGL(hdrVertexShader, "", "", "", hdrFragmentShader));
//	auto bloomProgram = std::unique_ptr<Shader>(new ShaderOGL(bloomVertexShader, "", "", "", bloomFragmentShader));

	

	hdrRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(tempBuffer1->get_colors(), windowFramebuffer, std::move(hdrProgram)));
//	bloomRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(tempBuffer2->get_colors(), windowFramebuffer, std::move(bloomProgram)));

	
}

void ms::NGinOGL::load () {
	phongForwardRenderer->load();
	gouraudForwardRenderer->load();
	deferredRenderer->load();
	lightSourceRenderer->load();
	hdrRenderer->load();
//	bloomRenderer->load();
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

