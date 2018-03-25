
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
	
	std::string bloomSplitterVertexShader = shader::get_shader_of_type(shader::Type::post_process_bloom_splitter_vshader);
	std::string bloomSplitterFragmentShader = shader::get_shader_of_type(shader::Type::post_process_bloom_splitter_fshader);
	
	std::string bloomMergerVertexShader = shader::get_shader_of_type(shader::Type::post_process_bloom_merger_vshader);
	std::string bloomMergerFragmentShader = shader::get_shader_of_type(shader::Type::post_process_bloom_merger_fshader);
	
	std::string gaussianBlurVertexShader = shader::get_shader_of_type(shader::Type::post_process_gaussian_blur_vshader);
	std::string gaussianBlurFragmentShader = shader::get_shader_of_type(shader::Type::post_process_gaussian_blur_fshader);
	
	
	windowFramebuffer = FramebufferOGL::window_framebuffer(screenWidth, screenHeight);
	auto aa = std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
													  Texture::Format::rgb_16_16_16,
													  Texture::AssociatedType::FLOAT,
													  frameBufferWidth,
													  frameBufferHeight));
	oneColorDepthFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																				  1,
																				  frameBufferWidth,
																				  frameBufferHeight));
	
	twoColorsDepthFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(2,
																				   1,
																				   frameBufferWidth,
																				   frameBufferHeight));
	
	thirdOneColorDepthFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																					   1,
																					   frameBufferWidth,
																					   frameBufferHeight));
	
	fourthOneColorDepthFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																						1,
																						frameBufferWidth,
																						frameBufferHeight));
	
	secondOneColorDepthFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																						1,
																						frameBufferWidth,
																						frameBufferHeight));
	
	oneColorDepthFramebuffer->bind_depth_buffer(std::shared_ptr<Renderbuffer>(new RenderbufferOGL(Texture::Format::depth_24,
																								  Texture::AssociatedType::UNSIGNED_BYTE,
																								  0,
																								  frameBufferWidth,
																								  frameBufferHeight)));
	
	oneColorDepthFramebuffer->bind_color_buffer(0, std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
																						   Texture::Format::rgb_16_16_16,
																						   Texture::AssociatedType::FLOAT,
																						   frameBufferWidth,
																						   frameBufferHeight)));
	
	
	
	oneColorDepthFramebuffer->configure();
	
	secondOneColorDepthFramebuffer->bind_color_buffer(0, std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
																								 Texture::Format::rgb_16_16_16,
																								 Texture::AssociatedType::FLOAT,
																								 frameBufferWidth,
																								 frameBufferHeight)));
	
	secondOneColorDepthFramebuffer->bind_depth_buffer(std::shared_ptr<Renderbuffer>(new RenderbufferOGL(Texture::Format::depth_24,
																										Texture::AssociatedType::UNSIGNED_BYTE,
																										0,
																										frameBufferWidth,
																										frameBufferHeight)));
	
	secondOneColorDepthFramebuffer->configure();
	
	fourthOneColorDepthFramebuffer->bind_color_buffer(0, aa);
	
	fourthOneColorDepthFramebuffer->bind_depth_buffer(std::shared_ptr<Renderbuffer>(new RenderbufferOGL(Texture::Format::depth_24,
																										Texture::AssociatedType::UNSIGNED_BYTE,
																										0,
																										frameBufferWidth,
																										frameBufferHeight)));
	
	fourthOneColorDepthFramebuffer->configure();
	
	thirdOneColorDepthFramebuffer->bind_color_buffer(0, std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
																								Texture::Format::rgb_16_16_16,
																								Texture::AssociatedType::FLOAT,
																								frameBufferWidth,
																								frameBufferHeight)));
	
	thirdOneColorDepthFramebuffer->bind_depth_buffer(std::shared_ptr<Renderbuffer>(new RenderbufferOGL(Texture::Format::depth_24,
																									   Texture::AssociatedType::UNSIGNED_BYTE,
																									   0,
																									   frameBufferWidth,
																									   frameBufferHeight)));
	
	thirdOneColorDepthFramebuffer->configure();
	
	twoColorsDepthFramebuffer->bind_depth_buffer(std::shared_ptr<Renderbuffer>(new RenderbufferOGL(Texture::Format::depth_24,
																								   Texture::AssociatedType::UNSIGNED_BYTE,
																								   0,
																								   frameBufferWidth,
																								   frameBufferHeight)));
	
	twoColorsDepthFramebuffer->bind_color_buffer(0, aa);
	
	twoColorsDepthFramebuffer->bind_color_buffer(1, std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
																							Texture::Format::rgb_16_16_16,
																							Texture::AssociatedType::FLOAT,
																							frameBufferWidth,
																							frameBufferHeight)));
	twoColorsDepthFramebuffer->configure();
	
	unsigned int AOL = 100;

	auto phongforwardShader = std::unique_ptr<ForwardShader>(new ForwardShaderOGL(AOL,
																				  forwardRenderVertexShaderSource,
																				  forwardRenderFragmentShaderSource));
	
	auto gouraudforwardShader = std::unique_ptr<ForwardShader>(new ForwardShaderOGL(AOL,
																					gouraudVertexShaderSource,
																					gouraudRenderFragmentShaderSource));
	
	auto lightSourceforwardShader = std::unique_ptr<LightSourceDrawerShader>(new LightSourceDrawerShaderOGL(lightSourceDrawerVertexShader,
																											lightSourceDrawerFragmentShader));
	
	auto deferredShader = std::unique_ptr<DeferredShader>(new DeferredShaderOGL(deferredRenderVertexShaderSource,
																				deferredRenderFragmentShaderSource));
	
	auto bloomSplitProgram = std::unique_ptr<Shader>(new ShaderOGL(bloomSplitterVertexShader, "", "", "", bloomSplitterFragmentShader));
	auto bloomMergeProgram = std::unique_ptr<Shader>(new ShaderOGL(bloomMergerVertexShader, "", "", "", bloomMergerFragmentShader));
	auto hdrProgram = std::unique_ptr<Shader>(new ShaderOGL(hdrVertexShader, "", "", "", hdrFragmentShader));
	auto gaussianBlurProgram = std::unique_ptr<Shader>(new ShaderOGL(gaussianBlurVertexShader, "", "", "", gaussianBlurFragmentShader));
	auto gaussianBlurProgram2 = std::unique_ptr<Shader>(new ShaderOGL(gaussianBlurVertexShader, "", "", "", gaussianBlurFragmentShader));

	deferredRenderer = std::unique_ptr<DeferredRenderOGL> (new DeferredRenderOGL( AOL,
																				  deferredRenderVertexShaderSource,
																				  deferredRenderFragmentShaderSource,
																				  deferredRenderLightingVertexShaderSource,
																				  deferredRenderLightingFragmentShaderSource,
																				  oneColorDepthFramebuffer));
	
	phongForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRender(AOL,
																			oneColorDepthFramebuffer,
																			std::move(phongforwardShader)));
	
	gouraudForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRender(AOL,
																			  oneColorDepthFramebuffer,
																			  std::move(gouraudforwardShader)));
	
	lightSourceRenderer = std::unique_ptr<LightSourcesRender>(new LightSourcesRender(oneColorDepthFramebuffer,
																					 std::move(lightSourceforwardShader)));
	
	bloomSplitRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(oneColorDepthFramebuffer->get_colors(),
																					 twoColorsDepthFramebuffer, std::move(bloomSplitProgram)));
	
	std::vector<std::shared_ptr<Texture>> tex;
	tex.push_back(twoColorsDepthFramebuffer->get_colors()[0]);
	
	gaussianBlurRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(tex,
																					   thirdOneColorDepthFramebuffer,
																					   std::move(gaussianBlurProgram)));
	
	gaussianBlurRenderer2 = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(thirdOneColorDepthFramebuffer->get_colors(),
																					   	fourthOneColorDepthFramebuffer,
																					   	std::move(gaussianBlurProgram2)));
	
	
	bloomMergeRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(twoColorsDepthFramebuffer->get_colors(),
																					 secondOneColorDepthFramebuffer,
																					 std::move(bloomMergeProgram)));
	
	hdrRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(secondOneColorDepthFramebuffer->get_colors(),
																			  windowFramebuffer,
																			  std::move(hdrProgram)));
	
}

void ms::NGinOGL::load () {
	phongForwardRenderer->load();
	gouraudForwardRenderer->load();
	deferredRenderer->load();
	lightSourceRenderer->load();
	hdrRenderer->load();
	bloomSplitRenderer->load();
	bloomMergeRenderer->load();
	gaussianBlurRenderer->load();
	gaussianBlurRenderer2->load();
}

void ms::NGinOGL::unload () {
	phongForwardRenderer->unload();
	gouraudForwardRenderer->unload();
	deferredRenderer->unload();
	lightSourceRenderer->unload();
	hdrRenderer->unload();
	bloomSplitRenderer->unload();
//	bloomMergeRenderer->unload();
//	gaussianBlurRenderer2->unload();
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

