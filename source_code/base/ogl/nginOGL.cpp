
//  nginOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "nginOGL.hpp"

ms::NGinOGL::NGinOGL (  unsigned int screenWidth,
                        unsigned int screenHeight,
                        unsigned int frameBufferWidth,
						unsigned int frameBufferHeight,
						float camNear,
						float camFar,
						float fovDegrees,
						float aspect,
						std::shared_ptr<Framebuffer>	defaultFramebuffer
					  ) : NGin(screenWidth, screenHeight, frameBufferWidth, frameBufferHeight, camNear, camFar, fovDegrees, aspect) {
	
	if(defaultFramebuffer == nullptr) {
		windowFramebuffer = FramebufferOGL::window_framebuffer(screenWidth, screenHeight);
	} else {
		windowFramebuffer = defaultFramebuffer;
	}
	
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
	
	std::string scaleVertexShader = shader::get_shader_of_type(shader::Type::post_process_scale_vshader);
	std::string scaleFragmentShader = shader::get_shader_of_type(shader::Type::post_process_scale_fshader);
	
	auto colorTexture = std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
																Texture::Format::rgb_16_16_16,
																Texture::AssociatedType::FLOAT,
																frameBufferWidth,
																frameBufferHeight));
	
	oneColorDepthFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																				  1,
																				  frameBufferWidth,
																				  frameBufferHeight));
	
	twoColorsFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(2,
																			  0,
																			  frameBufferWidth,
																			  frameBufferHeight));
	fifthOneColorFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																				  0,
																				  frameBufferWidth,
																				  frameBufferHeight));
	
	thirdOneColorFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																				  0,
																				  frameBufferWidth,
																				  frameBufferHeight));
	
	fourthOneColorFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																				   0,
																				   frameBufferWidth,
																				   frameBufferHeight));
	
	secondOneColorFramebuffer = std::shared_ptr<FramebufferOGL>(new FramebufferOGL(1,
																				   0,
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
	
	fifthOneColorFramebuffer->bind_color_buffer(0, std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
																						   Texture::Format::rgb_16_16_16,
																						   Texture::AssociatedType::FLOAT,
																						   frameBufferWidth,
																						   frameBufferHeight)));
	
	fifthOneColorFramebuffer->configure();
	
	secondOneColorFramebuffer->bind_color_buffer(0, std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
																								 Texture::Format::rgb_16_16_16,
																								 Texture::AssociatedType::FLOAT,
																								 frameBufferWidth,
																								 frameBufferHeight)));
	
	secondOneColorFramebuffer->configure();
	
	fourthOneColorFramebuffer->bind_color_buffer(0, colorTexture);
	
	
	fourthOneColorFramebuffer->configure();
	
	thirdOneColorFramebuffer->bind_color_buffer(0, std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
																								Texture::Format::rgb_16_16_16,
																								Texture::AssociatedType::FLOAT,
																								frameBufferWidth,
																								frameBufferHeight)));
	
	
	thirdOneColorFramebuffer->configure();
	
	twoColorsFramebuffer->bind_color_buffer(0, colorTexture);
	
	twoColorsFramebuffer->bind_color_buffer(1, std::shared_ptr<Texture>(new TextureOGL(Texture::Type::tex_2d,
																							Texture::Format::rgb_16_16_16,
																							Texture::AssociatedType::FLOAT,
																							frameBufferWidth,
																							frameBufferHeight)));
	twoColorsFramebuffer->configure();
	
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
	auto secondGaussianBlurProgram = std::unique_ptr<Shader>(new ShaderOGL(gaussianBlurVertexShader, "", "", "", gaussianBlurFragmentShader));
	auto scaleProgram = std::unique_ptr<Shader>(new ShaderOGL(scaleVertexShader, "", "", "", scaleFragmentShader));

	
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
																					 twoColorsFramebuffer, std::move(bloomSplitProgram)));
	
	std::vector<std::shared_ptr<Texture>> textures;
	textures.push_back(twoColorsFramebuffer->get_colors()[0]);
	
	gaussianBlurFirstStepRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(textures,
																								thirdOneColorFramebuffer,
																								std::move(gaussianBlurProgram)));
	
	gaussianBlurSecondStepRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(thirdOneColorFramebuffer->get_colors(),
																								 fourthOneColorFramebuffer,
																								 std::move(secondGaussianBlurProgram)));
	
	
	bloomMergeRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(twoColorsFramebuffer->get_colors(),
																					 secondOneColorFramebuffer,
																					 std::move(bloomMergeProgram)));
	
	hdrRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(secondOneColorFramebuffer->get_colors(),
																			  fifthOneColorFramebuffer,
																			  std::move(hdrProgram)));
	
	scaleRenderer = std::unique_ptr<PostprocessDrawer>(new PostprocessDrawerOGL(fifthOneColorFramebuffer->get_colors(),
																				  windowFramebuffer,
																				  std::move(scaleProgram)));
	
}


ms::NGinOGL::NGinOGL (	unsigned int screenWidth,
                        unsigned int screenHeight,
                        unsigned int frameBufferWidth,
						unsigned int frameBufferHeight,
						float camNear,
						float camFar,
						float fovDegrees,
						float aspect
						) : NGinOGL(screenWidth, screenHeight, frameBufferWidth, frameBufferHeight, camNear, camFar, fovDegrees, aspect, nullptr) {
	
	

	
}

void ms::NGinOGL::load () {
	phongForwardRenderer->load();
	gouraudForwardRenderer->load();
	deferredRenderer->load();
	lightSourceRenderer->load();
	hdrRenderer->load();
	bloomSplitRenderer->load();
	bloomMergeRenderer->load();
	gaussianBlurFirstStepRenderer->load();
	gaussianBlurSecondStepRenderer->load();
	scaleRenderer->load();
}

void ms::NGinOGL::unload () {
	NGin::unload();
	phongForwardRenderer->unload();
	gouraudForwardRenderer->unload();
	deferredRenderer->unload();
	lightSourceRenderer->unload();
	hdrRenderer->unload();
	bloomSplitRenderer->unload();
	bloomMergeRenderer->unload();
	gaussianBlurFirstStepRenderer->unload();
	gaussianBlurSecondStepRenderer->unload();
	scaleRenderer->unload();
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

