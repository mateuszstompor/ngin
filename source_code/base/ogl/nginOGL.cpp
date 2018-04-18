
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
                          std::unique_ptr<Framebuffer> && defaultFramebuffer
                          ) : NGinOGL(screenWidth,
                                      screenHeight,
                                      frameBufferWidth,
                                      frameBufferHeight,
                                      std::make_unique<PerspectiveCamera>(camNear, camFar, fovDegrees, aspect),
                                      std::move(defaultFramebuffer)){ }

ms::NGinOGL::NGinOGL (  unsigned int screenWidth,
                        unsigned int screenHeight,
                        unsigned int frameBufferWidth,
						unsigned int frameBufferHeight,
                        std::unique_ptr<Camera> && cam,
						std::unique_ptr<Framebuffer> &&	defaultFramebuffer
                      ) : NGin(screenWidth, screenHeight, frameBufferWidth, frameBufferHeight, std::move(cam)) {
	
    std::unique_ptr<Framebuffer> windowFramebuffer(defaultFramebuffer == nullptr ? FramebufferOGL::window_framebuffer(screenWidth, screenHeight) : std::move(defaultFramebuffer));
    
    auto shadowMapperVertexShaderSource = get_shader_of_type(Type::shadow_mapping_dir_vshader);
    auto shadowMapperFragmentShaderSource = get_shader_of_type(Type::shadow_mapping_dir_fshader);

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

    auto vignetteFramebuffer = std::make_unique<FramebufferOGL>(1,
                                                                0,
                                                                frameBufferWidth,
                                                                frameBufferHeight);
    
    auto hdrFramebuffer = std::make_unique<FramebufferOGL>(1,
                                                           0,
                                                           frameBufferWidth,
                                                           frameBufferHeight);

    auto mainRenderFramebuffer = std::make_unique<FramebufferOGL>(1,
                                                                  1,
                                                                  frameBufferWidth,
                                                                  frameBufferHeight);
    
    auto lightSourceDrawerFramebuffer = std::make_unique<FramebufferOGL>(1,
                                                                         1,
                                                                         frameBufferWidth,
                                                                         frameBufferHeight);
    
    auto bloomTwoTexSplitFramebuffer = std::make_unique<FramebufferOGL>(2,
                                                                        0,
                                                                        frameBufferWidth,
                                                                        frameBufferHeight);
    
    auto gaussianBlurFirstStepFramebuffer = std::make_unique<FramebufferOGL>(1,
                                                                  0,
                                                                  frameBufferWidth,
                                                                  frameBufferHeight);
    
    auto gaussianBlurSecondStepFramebuffer = std::make_unique<FramebufferOGL>(1,
                                                                  0,
                                                                  frameBufferWidth,
                                                                  frameBufferHeight);
    
    
    auto bloomMergeFramebuffer = std::make_unique<FramebufferOGL>(1,
                                                                  0,
                                                                  frameBufferWidth,
                                                                  frameBufferHeight);



    mainRenderFramebuffer->bind_depth_buffer(std::make_unique<Renderbuffer>(Texture::Format::depth_32,
                                                                               Texture::AssociatedType::FLOAT,
                                                                               0,
                                                                               frameBufferWidth,
                                                                               frameBufferHeight));

    mainRenderFramebuffer->bind_color_buffer(0, std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                             Texture::Format::rgb_16_16_16,
                                                                             Texture::AssociatedType::FLOAT,
                                                                             frameBufferWidth,
                                                                             frameBufferHeight));

    mainRenderFramebuffer->configure();
    
    
    lightSourceDrawerFramebuffer->bind_depth_buffer(std::make_unique<Renderbuffer>(Texture::Format::depth_32,
                                                                                      Texture::AssociatedType::FLOAT,
                                                                                      0,
                                                                                      frameBufferWidth,
                                                                                      frameBufferHeight));

    lightSourceDrawerFramebuffer->bind_color_buffer(0, std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                                    Texture::Format::rgb_16_16_16,
                                                                                    Texture::AssociatedType::FLOAT,
                                                                                    frameBufferWidth,
                                                                                    frameBufferHeight));
    
    lightSourceDrawerFramebuffer->configure();
    

    vignetteFramebuffer->bind_color_buffer(0, std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                           Texture::Format::rgb_16_16_16,
                                                                           Texture::AssociatedType::FLOAT,
                                                                           frameBufferWidth,
                                                                           frameBufferHeight));

    vignetteFramebuffer->configure();
    
    
    gaussianBlurFirstStepFramebuffer->bind_color_buffer(0, std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                           Texture::Format::rgb_16_16_16,
                                                                           Texture::AssociatedType::FLOAT,
                                                                           frameBufferWidth,
                                                                           frameBufferHeight));
    
    gaussianBlurFirstStepFramebuffer->configure();
    
    gaussianBlurSecondStepFramebuffer->bind_color_buffer(0, std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                                        Texture::Format::rgb_16_16_16,
                                                                                        Texture::AssociatedType::FLOAT,
                                                                                        frameBufferWidth,
                                                                                        frameBufferHeight));
    
    gaussianBlurSecondStepFramebuffer->configure();
    
    
    hdrFramebuffer->bind_color_buffer(0, std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                           Texture::Format::rgb_16_16_16,
                                                                           Texture::AssociatedType::FLOAT,
                                                                           frameBufferWidth,
                                                                           frameBufferHeight));
    
    hdrFramebuffer->configure();
    
    
    bloomTwoTexSplitFramebuffer->bind_color_buffer(0, std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                              Texture::Format::rgb_16_16_16,
                                                                              Texture::AssociatedType::FLOAT,
                                                                              frameBufferWidth,
                                                                              frameBufferHeight));

    bloomTwoTexSplitFramebuffer->bind_color_buffer(1, std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                            Texture::Format::rgb_16_16_16,
                                                                            Texture::AssociatedType::FLOAT,
                                                                            frameBufferWidth,
                                                                            frameBufferHeight));
    bloomTwoTexSplitFramebuffer->configure();
    
    bloomMergeFramebuffer->bind_color_buffer(0, std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                                   Texture::Format::rgb_16_16_16,
                                                                                   Texture::AssociatedType::FLOAT,
                                                                                   frameBufferWidth,
                                                                                   frameBufferHeight));
    bloomMergeFramebuffer->configure();
    
    unsigned int AOL = 100;

    auto shadowShader = ShaderOGL::vf_program(shadowMappingVertexShader, shadowMappingFragmentShader);
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

    auto bloomSplitProgram = ShaderOGL::vf_program(bloomSplitterVertexShader, bloomSplitterFragmentShader);
    auto bloomMergeProgram = ShaderOGL::vf_program(bloomMergerVertexShader, bloomMergerFragmentShader);
    auto hdrProgram = ShaderOGL::vf_program(hdrVertexShader, hdrFragmentShader);
    auto gaussianBlurProgram = ShaderOGL::vf_program(gaussianBlurVertexShader, gaussianBlurFragmentShader);
    auto secondGaussianBlurProgram = ShaderOGL::vf_program(gaussianBlurVertexShader, gaussianBlurFragmentShader);
    auto vignetteProgram = ShaderOGL::vf_program(vignetteVertexShader, vignetteFragmentShader);
    auto scaleProgram = ShaderOGL::vf_program(scaleVertexShader, scaleFragmentShader);
    
    auto defGshader = std::make_unique<DeferredShaderOGL>(deferredRenderVertexShaderSource, deferredRenderFragmentShaderSource);
    auto defLightingShader = std::make_unique<DeferredLightingShaderOGL>(AOL, AOL, deferredRenderLightingVertexShaderSource, deferredRenderLightingFragmentShaderSource);

    deferredRenderer = std::make_unique<DeferredRenderOGL> (AOL,
                                                            AOL,
                                                            std::move(mainRenderFramebuffer),
                                                            std::move(defGshader),
                                                            std::move(defLightingShader));
    
    shadowRenderer = std::make_unique<Render>(nullptr, std::move(shadowShader));

//    phongForwardRenderer = std::make_unique<ForwardRender>(AOL,
//                                                           std::move(mainRenderFramebuffer),
//                                                           std::move(phongforwardShader));
//
//    gouraudForwardRenderer = std::make_unique<ForwardRender>(AOL,
//                                                             std::move(mainRenderFramebuffer),
//                                                             std::move(gouraudforwardShader));

    lightSourceRenderer = std::make_unique<LightSourcesRender>(std::move(lightSourceDrawerFramebuffer),
                                                               std::move(lightSourceforwardShader));

    bloomSplitRenderer = std::make_unique<PostprocessDrawerOGL>(lightSourceRenderer->get_framebuffer().get_colors(),
                                                                std::move(bloomTwoTexSplitFramebuffer),
                                                                std::move(bloomSplitProgram));

    std::vector<std::weak_ptr<Texture>> textures;
    textures.push_back(bloomSplitRenderer->get_framebuffer().get_colors()[0]);

    gaussianBlurFirstStepRenderer = std::make_unique<PostprocessDrawerOGL>(textures,
                                                                           std::move(gaussianBlurFirstStepFramebuffer),
                                                                           std::move(gaussianBlurProgram));

    gaussianBlurSecondStepRenderer = std::make_unique<PostprocessDrawerOGL>(gaussianBlurFirstStepRenderer->get_framebuffer().get_colors(),
                                                                            std::move(gaussianBlurSecondStepFramebuffer),
                                                                            std::move(secondGaussianBlurProgram));

    std::vector<std::weak_ptr<Texture>> textures2;
    textures2.push_back(gaussianBlurSecondStepRenderer->get_framebuffer().get_colors()[0]);
    textures2.push_back(bloomSplitRenderer->get_framebuffer().get_colors()[1]);
    
    bloomMergeRenderer = std::make_unique<PostprocessDrawerOGL>(textures2,
                                                                std::move(bloomMergeFramebuffer),
                                                                std::move(bloomMergeProgram));

    hdrRenderer = std::make_unique<PostprocessDrawerOGL>(lightSourceRenderer->get_framebuffer().get_colors(),
                                                         std::move(hdrFramebuffer),
                                                         std::move(hdrProgram));

    vignetteRenderer = std::make_unique<PostprocessDrawerOGL>(hdrRenderer->get_framebuffer().get_colors(),
                                                              std::move(vignetteFramebuffer),
                                                              std::move(vignetteProgram));

    scaleRenderer = std::make_unique<PostprocessDrawerOGL>(vignetteRenderer->get_framebuffer().get_colors(),
                                                           std::move(windowFramebuffer),
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
//    phongForwardRenderer->load();
    deferredRenderer->load();
//    gouraudForwardRenderer->load();
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
//    phongForwardRenderer->unload();
    deferredRenderer->unload();
//    gouraudForwardRenderer->unload();
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

std::shared_ptr<ms::SpotLight> ms::NGinOGL::get_spot_light (float 		power,
															math::vec3 	color,
															math::vec3 	position,
															float 		lightingAngleDegrees,
															math::vec3 	direction) {
	
	return std::make_shared<SpotLightOGL>(power, color, position, lightingAngleDegrees, direction);
	
}

