
//  nginOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "nginOGL.hpp"

using namespace ms;
using namespace shader;

ms::NGinOGL::NGinOGL     (unsigned int                  screenWidth,
                          unsigned int                  screenHeight,
                          unsigned int                  frameBufferWidth,
                          unsigned int                  frameBufferHeight,
                          float                         camNear,
                          float                         camFar,
                          float                         fovDegrees,
                          float                         aspect,
                          std::shared_ptr<Framebuffer>  defaultFramebuffer
                          ) : NGinOGL(screenWidth,
                                      screenHeight,
                                      frameBufferWidth,
                                      frameBufferHeight,
                                      std::make_unique<PerspectiveCamera>(camNear, camFar, fovDegrees, aspect),
                                      defaultFramebuffer){ }

ms::NGinOGL::NGinOGL (  unsigned int screenWidth,
                        unsigned int screenHeight,
                        unsigned int frameBufferWidth,
						unsigned int frameBufferHeight,
                        std::unique_ptr<Camera> && cam,
						std::shared_ptr<Framebuffer>	defaultFramebuffer
                      ) : NGin(screenWidth, screenHeight, frameBufferWidth, frameBufferHeight, std::move(cam)) {
	
	if(defaultFramebuffer == nullptr) {
		windowFramebuffer = FramebufferOGL::window_framebuffer(screenWidth, screenHeight);
	} else {
		windowFramebuffer = defaultFramebuffer;
	}
	
	auto forwardRenderVertexShaderSource = get_shader_of_type(Type::forward_render_phong_vshader);
	auto forwardRenderFragmentShaderSource = get_shader_of_type(Type::forward_render_phong_fshader);
	
	auto gouraudVertexShaderSource = get_shader_of_type(Type::forward_render_gouraud_vshader);
	auto gouraudRenderFragmentShaderSource = get_shader_of_type(Type::forward_render_gouraud_fshader);
	
	auto deferredRenderVertexShaderSource = get_shader_of_type(Type::deferred_render_g_buf_vertex_shader);
	auto deferredRenderFragmentShaderSource = get_shader_of_type(Type::deferred_render_g_buf_fragment_shader);
	
	auto deferredRenderLightingVertexShaderSource = get_shader_of_type(Type::deferred_render_light_pass_vshader);
	auto deferredRenderLightingFragmentShaderSource = get_shader_of_type(Type::deferred_render_light_pass_fshader);
	
	auto lightSourceDrawerVertexShader = get_shader_of_type(Type::forward_render_light_drawer_vshader);
	auto lightSourceDrawerFragmentShader = get_shader_of_type(Type::forward_render_light_drawer_fshader);
    
    auto shadowMappingVertexShader = get_shader_of_type(Type::shadow_mapping_dir_vshader);
    auto shadowMappingFragmentShader = get_shader_of_type(Type::shadow_mapping_dir_fshader);
	
	auto hdrVertexShader = get_shader_of_type(Type::post_process_hdr_vshader);
	auto hdrFragmentShader = get_shader_of_type(Type::post_process_hdr_fshader);
	
	auto bloomSplitterVertexShader = get_shader_of_type(Type::post_process_bloom_splitter_vshader);
	auto bloomSplitterFragmentShader = get_shader_of_type(Type::post_process_bloom_splitter_fshader);
	
	auto bloomMergerVertexShader = get_shader_of_type(Type::post_process_bloom_merger_vshader);
	auto bloomMergerFragmentShader = get_shader_of_type(Type::post_process_bloom_merger_fshader);
	
	auto gaussianBlurVertexShader = get_shader_of_type(Type::post_process_gaussian_blur_vshader);
	auto gaussianBlurFragmentShader = get_shader_of_type(Type::post_process_gaussian_blur_fshader);
	
    auto vignetteVertexShader = get_shader_of_type(Type::post_process_vignette_vshader);
    auto vignetteFragmentShader = get_shader_of_type(Type::post_process_vignette_fshader);
    
	auto scaleVertexShader = get_shader_of_type(Type::post_process_scale_vshader);
	auto scaleFragmentShader = get_shader_of_type(Type::post_process_scale_fshader);
	
	auto colorTexture = std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                                    Texture::Format::rgb_16_16_16,
                                                    Texture::AssociatedType::FLOAT,
                                                    frameBufferWidth,
                                                    frameBufferHeight);
	
    vignetteFramebuffer = std::make_shared<FramebufferOGL>(1,
                                                           0,
                                                           frameBufferWidth,
                                                           frameBufferHeight);
    
	oneColorDepthFramebuffer = std::make_shared<FramebufferOGL>(1,
                                                                1,
                                                                frameBufferWidth,
                                                                frameBufferHeight);
	
	twoColorsFramebuffer = std::make_shared<FramebufferOGL>(2,
                                                            0,
                                                            frameBufferWidth,
                                                            frameBufferHeight);
    
	fifthOneColorFramebuffer = std::make_shared<FramebufferOGL>(1,
                                                                0,
                                                                frameBufferWidth,
                                                                frameBufferHeight);
	
	thirdOneColorFramebuffer = std::make_shared<FramebufferOGL>(1,
																0,
                                                                frameBufferHeight,
																frameBufferWidth);
	
	fourthOneColorFramebuffer = std::make_shared<FramebufferOGL>(1,
                                                                 0,
                                                                 frameBufferWidth,
                                                                 frameBufferHeight);
	
	secondOneColorFramebuffer = std::make_shared<FramebufferOGL>(1,
                                                                 0,
                                                                 frameBufferWidth,
                                                                 frameBufferHeight);
	
	oneColorDepthFramebuffer->bind_depth_buffer(std::make_shared<RenderbufferOGL>(Texture::Format::depth_24,
                                                                                  Texture::AssociatedType::UNSIGNED_BYTE,
                                                                                  0,
                                                                                  frameBufferWidth,
                                                                                  frameBufferHeight));
	
	oneColorDepthFramebuffer->bind_color_buffer(0, std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                                                                Texture::Format::rgb_16_16_16,
                                                                                Texture::AssociatedType::FLOAT,
                                                                                frameBufferWidth,
                                                                                frameBufferHeight));
	
	
	
	oneColorDepthFramebuffer->configure();
	
	fifthOneColorFramebuffer->bind_color_buffer(0, std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                                                                Texture::Format::rgb_16_16_16,
                                                                                Texture::AssociatedType::FLOAT,
                                                                                frameBufferWidth,
                                                                                frameBufferHeight));
	
	fifthOneColorFramebuffer->configure();
    
    vignetteFramebuffer->bind_color_buffer(0, std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                                                           Texture::Format::rgb_16_16_16,
                                                                           Texture::AssociatedType::FLOAT,
                                                                           frameBufferWidth,
                                                                           frameBufferHeight));
    
    vignetteFramebuffer->configure();
    
	
	secondOneColorFramebuffer->bind_color_buffer(0, std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                                                                 Texture::Format::rgb_16_16_16,
                                                                                 Texture::AssociatedType::FLOAT,
                                                                                 frameBufferWidth,
                                                                                 frameBufferHeight));
	
	secondOneColorFramebuffer->configure();
	
	fourthOneColorFramebuffer->bind_color_buffer(0, colorTexture);
	
	
	fourthOneColorFramebuffer->configure();
	
	thirdOneColorFramebuffer->bind_color_buffer(0, std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                                                                Texture::Format::rgb_16_16_16,
                                                                                Texture::AssociatedType::FLOAT,
																				frameBufferHeight,
                                                                                frameBufferWidth));
	
	
	thirdOneColorFramebuffer->configure();
	
	twoColorsFramebuffer->bind_color_buffer(0, colorTexture);
	
	twoColorsFramebuffer->bind_color_buffer(1, std::make_shared<TextureOGL>(Texture::Type::tex_2d,
                                                                            Texture::Format::rgb_16_16_16,
                                                                            Texture::AssociatedType::FLOAT,
                                                                            frameBufferWidth,
                                                                            frameBufferHeight));
	twoColorsFramebuffer->configure();
	
	unsigned int AOL = 100;
	
	auto phongforwardShader = std::make_unique<ForwardShaderOGL>(AOL,
                                                                 forwardRenderVertexShaderSource,
                                                                 forwardRenderFragmentShaderSource);
	
	auto gouraudforwardShader = std::make_unique<ForwardShaderOGL>(AOL,
                                                                   gouraudVertexShaderSource,
                                                                   gouraudRenderFragmentShaderSource);
	
	auto lightSourceforwardShader = std::make_unique<LightSourceDrawerShaderOGL>(lightSourceDrawerVertexShader,
                                                                                 lightSourceDrawerFragmentShader);
	
	auto deferredShader = std::make_unique<DeferredShaderOGL>(deferredRenderVertexShaderSource,
                                                              deferredRenderFragmentShaderSource);
	
    auto bloomSplitProgram = std::make_unique<ShaderOGL>(bloomSplitterVertexShader, "", "", "", bloomSplitterFragmentShader);
	auto bloomMergeProgram = std::make_unique<ShaderOGL>(bloomMergerVertexShader, "", "", "", bloomMergerFragmentShader);
	auto hdrProgram = std::make_unique<ShaderOGL>(hdrVertexShader, "", "", "", hdrFragmentShader);
	auto gaussianBlurProgram = std::make_unique<ShaderOGL>(gaussianBlurVertexShader, "", "", "", gaussianBlurFragmentShader);
	auto secondGaussianBlurProgram = std::make_unique<ShaderOGL>(gaussianBlurVertexShader, "", "", "", gaussianBlurFragmentShader);
	auto vignetteProgram = std::make_unique<ShaderOGL>(vignetteVertexShader, "", "", "", vignetteFragmentShader);
    auto scaleProgram = std::make_unique<ShaderOGL>(scaleVertexShader, "", "", "", scaleFragmentShader);
    
	
	deferredRenderer = std::make_unique<DeferredRenderOGL> (AOL,
                                                            deferredRenderVertexShaderSource,
                                                            deferredRenderFragmentShaderSource,
                                                            deferredRenderLightingVertexShaderSource,
                                                            deferredRenderLightingFragmentShaderSource,
                                                            shadowMappingVertexShader,
                                                            shadowMappingFragmentShader,
                                                            oneColorDepthFramebuffer);
	
	phongForwardRenderer = std::make_unique<ForwardRender>(AOL,
                                                           oneColorDepthFramebuffer,
                                                           std::move(phongforwardShader));
	
	gouraudForwardRenderer = std::make_unique<ForwardRender>(AOL,
                                                             oneColorDepthFramebuffer,
                                                             std::move(gouraudforwardShader));
	
    lightSourceRenderer = std::make_unique<LightSourcesRender>(oneColorDepthFramebuffer,
                                                               std::move(lightSourceforwardShader));
	
	bloomSplitRenderer = std::make_unique<PostprocessDrawerOGL>(oneColorDepthFramebuffer->get_colors(),
                                                                twoColorsFramebuffer,
                                                                std::move(bloomSplitProgram));
	
	std::vector<std::shared_ptr<Texture>> textures;
	textures.push_back(twoColorsFramebuffer->get_colors()[0]);
	
	gaussianBlurFirstStepRenderer = std::make_unique<PostprocessDrawerOGL>(textures,
                                                                           thirdOneColorFramebuffer,
                                                                           std::move(gaussianBlurProgram));
	
	gaussianBlurSecondStepRenderer = std::make_unique<PostprocessDrawerOGL>(thirdOneColorFramebuffer->get_colors(),
                                                                           fourthOneColorFramebuffer,
                                                                           std::move(secondGaussianBlurProgram));
	
	
	bloomMergeRenderer = std::make_unique<PostprocessDrawerOGL>(twoColorsFramebuffer->get_colors(),
                                                                secondOneColorFramebuffer,
                                                                std::move(bloomMergeProgram));
	
	hdrRenderer = std::make_unique<PostprocessDrawerOGL>(secondOneColorFramebuffer->get_colors(),
                                                         fifthOneColorFramebuffer,
                                                         std::move(hdrProgram));
	
    vignetteRenderer = std::make_unique<PostprocessDrawerOGL>(fifthOneColorFramebuffer->get_colors(),
                                                              vignetteFramebuffer,
                                                              std::move(vignetteProgram));
    
	scaleRenderer = std::make_unique<PostprocessDrawerOGL>(vignetteFramebuffer->get_colors(),
                                                           windowFramebuffer,
                                                           std::move(scaleProgram));
	
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
    vignetteRenderer->load();
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
    vignetteRenderer->unload();
}

std::shared_ptr<ms::PointLight> ms::NGinOGL::get_point_light(float 		power,
														   math::vec3 	color,
														   math::vec3 	position) {
	
	return std::make_shared<PointLightOGL>(power, color, position);
	
}

std::shared_ptr<ms::Drawable> ms::NGinOGL::get_drawable() {
	return std::make_shared<DrawableOGL>();
}

std::unique_ptr<ms::Loader> ms::NGinOGL::get_loader () {
	return std::make_unique<LoaderOGL>();
}

std::shared_ptr<ms::SpotLight> ms::NGinOGL::get_spot_light (float 		power,
															math::vec3 	color,
															math::vec3 	position,
															float 		lightingAngleDegrees,
															math::vec3 	direction) {
	
	return std::make_shared<SpotLightOGL>(power, color, position, lightingAngleDegrees, direction);
	
}

