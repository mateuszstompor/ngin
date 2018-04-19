//
//  ngin.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "ngin.hpp"

using namespace ms;
using namespace shader;

ms::NGin::NGin(unsigned int                   	screenWidth,
               unsigned int                     screenHeight,
               unsigned int                     frameBufferWidth,
               unsigned int                     frameBufferHeight,
               std::unique_ptr<Camera> &&       cam,
               std::unique_ptr<Framebuffer> &&  defaultFramebuffer) :
    scene(std::make_unique<Scene>(std::move(cam))),
    screenWidth{screenWidth},
    screenHeight{screenHeight},
    framebufferWidth{frameBufferWidth},
    framebufferHeight{frameBufferHeight},
    loader{} {
        std::unique_ptr<Framebuffer> windowFramebuffer(defaultFramebuffer == nullptr ? Framebuffer::window_framebuffer(screenWidth, screenHeight) : std::move(defaultFramebuffer));
        
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
    
        auto vignetteFramebuffer = std::make_unique<Framebuffer>(1,
                                                                 0,
                                                                 frameBufferWidth,
                                                                 frameBufferHeight);
    
        auto hdrFramebuffer = std::make_unique<Framebuffer>(1,
                                                            0,
                                                            frameBufferWidth,
                                                            frameBufferHeight);
    
        auto mainRenderFramebuffer = std::make_unique<Framebuffer>(1,
                                                                   1,
                                                                   frameBufferWidth,
                                                                   frameBufferHeight);
    
        auto lightSourceDrawerFramebuffer = std::make_unique<Framebuffer>(1,
                                                                          1,
                                                                          frameBufferWidth,
                                                                          frameBufferHeight);
    
        auto bloomTwoTexSplitFramebuffer = std::make_unique<Framebuffer>(2,
                                                                            0,
                                                                            frameBufferWidth,
                                                                            frameBufferHeight);
    
        auto gaussianBlurFirstStepFramebuffer = std::make_unique<Framebuffer>(1,
                                                                      0,
                                                                              frameBufferHeight,
                                                                              frameBufferWidth);
    
        auto gaussianBlurSecondStepFramebuffer = std::make_unique<Framebuffer>(1,
                                                                      0,
                                                                               frameBufferWidth,
                                                                               frameBufferHeight);
    
    
        auto bloomMergeFramebuffer = std::make_unique<Framebuffer>(1,
                                                                      0,
                                                                      frameBufferWidth,
                                                                      frameBufferHeight);
    
    
        for (int i = 0; i < 50; ++i) {
            shadows.push_back(std::make_unique<Framebuffer>(0,
                                                            0,
                                                            frameBufferWidth,
                                                            frameBufferHeight));
            shadows[i]->bind_depth_buffer(std::make_unique<Texture>(Texture::Type::tex_2d,
                                                                    Texture::Format::depth_32,
                                                                    Texture::AssociatedType::FLOAT,
                                                                    frameBufferWidth,
                                                                    frameBufferHeight));
            
            shadows[i]->configure();
        }
        
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
                                                                                         frameBufferHeight,
                                                                                         frameBufferWidth));
    
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
    
        auto shadowShader = Shader::vf_program(shadowMappingVertexShader, shadowMappingFragmentShader);
        
        auto phongforwardShader = Shader::vf_program(forwardRenderVertexShaderSource,
                                                     forwardRenderFragmentShaderSource);
    
        auto gouraudforwardShader = Shader::vf_program(gouraudVertexShaderSource,
                                                       gouraudRenderFragmentShaderSource);
    
        auto lightSourceforwardShader = Shader::vf_program(lightSourceDrawerVertexShader,
                                                           lightSourceDrawerFragmentShader);
    
        auto bloomSplitProgram = Shader::vf_program(bloomSplitterVertexShader, bloomSplitterFragmentShader);
        auto bloomMergeProgram = Shader::vf_program(bloomMergerVertexShader, bloomMergerFragmentShader);
        auto hdrProgram = Shader::vf_program(hdrVertexShader, hdrFragmentShader);
        auto gaussianBlurProgram = Shader::vf_program(gaussianBlurVertexShader, gaussianBlurFragmentShader);
        auto secondGaussianBlurProgram = Shader::vf_program(gaussianBlurVertexShader, gaussianBlurFragmentShader);
        auto vignetteProgram = Shader::vf_program(vignetteVertexShader, vignetteFragmentShader);
        auto scaleProgram = Shader::vf_program(scaleVertexShader, scaleFragmentShader);
    
        auto defGshader = Shader::vf_program(deferredRenderVertexShaderSource, deferredRenderFragmentShaderSource);
        auto defLightingShader = Shader::vf_program(deferredRenderLightingVertexShaderSource, deferredRenderLightingFragmentShaderSource);
    
        deferredRenderer = std::make_unique<DeferredRender> (AOL,
                                                             AOL,
                                                             std::move(mainRenderFramebuffer),
                                                             std::move(defGshader),
                                                             std::move(defLightingShader));
    
        shadowRenderer = std::make_unique<DLShadowRender>(nullptr, std::move(shadowShader));
    
        phongForwardRenderer = std::make_unique<ForwardRender>(AOL,
                                                               nullptr,
                                                               std::move(phongforwardShader));
    
        gouraudForwardRenderer = std::make_unique<ForwardRender>(AOL,
                                                                 nullptr,
                                                                 std::move(gouraudforwardShader));
    
        lightSourceRenderer = std::make_unique<LightSourcesRender>(std::move(lightSourceDrawerFramebuffer),
                                                                   std::move(lightSourceforwardShader));
    
        bloomSplitRenderer = std::make_unique<PostprocessDrawer>(lightSourceRenderer->get_framebuffer().get_colors(),
                                                                 std::move(bloomTwoTexSplitFramebuffer),
                                                                 std::move(bloomSplitProgram));
    
        std::vector<std::weak_ptr<Texture>> textures;
        textures.push_back(bloomSplitRenderer->get_framebuffer().get_colors()[0]);
    
        gaussianBlurFirstStepRenderer = std::make_unique<PostprocessDrawer>(textures,
                                                                            std::move(gaussianBlurFirstStepFramebuffer),
                                                                            std::move(gaussianBlurProgram));
    
        gaussianBlurSecondStepRenderer = std::make_unique<PostprocessDrawer>(gaussianBlurFirstStepRenderer->get_framebuffer().get_colors(),
                                                                             std::move(gaussianBlurSecondStepFramebuffer),
                                                                             std::move(secondGaussianBlurProgram));
    
        std::vector<std::weak_ptr<Texture>> textures2;
        textures2.push_back(gaussianBlurSecondStepRenderer->get_framebuffer().get_colors()[0]);
        textures2.push_back(bloomSplitRenderer->get_framebuffer().get_colors()[1]);
    
        bloomMergeRenderer = std::make_unique<PostprocessDrawer>(textures2,
                                                                 std::move(bloomMergeFramebuffer),
                                                                 std::move(bloomMergeProgram));
    
        hdrRenderer = std::make_unique<PostprocessDrawer>(bloomMergeRenderer->get_framebuffer().get_colors(),
                                                          std::move(hdrFramebuffer),
                                                          std::move(hdrProgram));
    
        vignetteRenderer = std::make_unique<PostprocessDrawer>(hdrRenderer->get_framebuffer().get_colors(),
                                                               std::move(vignetteFramebuffer),
                                                               std::move(vignetteProgram));
    
        scaleRenderer = std::make_unique<PostprocessDrawer>(vignetteRenderer->get_framebuffer().get_colors(),
                                                            std::move(windowFramebuffer),
                                                            std::move(scaleProgram));
            
}

//TODO this function is implemented three times
void ms::NGin::load_model (std::string absolutePath) {
	
    Loader::model_data loadedData = loader.load_model(absolutePath);
    Loader::geometries_vec & geo = std::get<0>(loadedData);
    
    {
        Loader::materials_map & mat = std::get<1>(loadedData);
        Loader::textures_map & tex = std::get<2>(loadedData);

        for (auto & material : mat) {
            scene->get_materials().insert(std::move(material));
        }
        
        for (auto & texture : tex) {
            scene->get_textures().insert(std::move(texture));
        }
    }
    
    for (auto & geometry : geo) {
        auto node = std::make_shared<Drawable>();
        node->geometry = std::move(geometry);
        auto nodeMaterial = scene->get_materials().find(node->geometry->get_material_name());
        if(nodeMaterial != scene->get_materials().end()) {

            node->boundedMaterial = nodeMaterial->second;

            if(nodeMaterial->second->diffuseTexturesNames.size() > 0) {
                auto diffuseTexture = scene->get_textures().find(nodeMaterial->second->diffuseTexturesNames[0]);
                if(diffuseTexture != scene->get_textures().end()) {
                    nodeMaterial->second->boundedDiffuseTexture = diffuseTexture->second;
                }
            }

            if(nodeMaterial->second->specularTexturesNames.size() > 0) {
                auto specularTexture = scene->get_textures().find(nodeMaterial->second->specularTexturesNames[0]);
                if(specularTexture != scene->get_textures().end()) {
                    nodeMaterial->second->boundedSpecularTexture = specularTexture->second;
                }
            }

            if(nodeMaterial->second->heightTexturesNames.size() > 0) {
                auto heightTexture = scene->get_textures().find(nodeMaterial->second->heightTexturesNames[0]);
                if(heightTexture != scene->get_textures().end()) {
                    nodeMaterial->second->boundedHeightTexture = heightTexture->second;
                }
            }

            if(nodeMaterial->second->normalTexturesNames.size() > 0) {
                auto normalTexture = scene->get_textures().find(nodeMaterial->second->normalTexturesNames[0]);
                if(normalTexture != scene->get_textures().end()) {
                    nodeMaterial->second->boundedNormalTexture = normalTexture->second;
                }
            }
            
        }
        scene->get_nodes().push_back(node);
    }
    
}

void ms::NGin::load () {
    phongForwardRenderer->load();
    deferredRenderer->load();
    gouraudForwardRenderer->load();
    lightSourceRenderer->load();
    hdrRenderer->load();
    bloomSplitRenderer->load();
    bloomMergeRenderer->load();
    gaussianBlurFirstStepRenderer->load();
    gaussianBlurSecondStepRenderer->load();
    scaleRenderer->load();
    vignetteRenderer->load();
}

void ms::NGin::unload () {
    phongForwardRenderer->unload();
    deferredRenderer->unload();
    gouraudForwardRenderer->unload();
    lightSourceRenderer->unload();
    hdrRenderer->unload();
    bloomSplitRenderer->unload();
    bloomMergeRenderer->unload();
    gaussianBlurFirstStepRenderer->unload();
    gaussianBlurSecondStepRenderer->unload();
    scaleRenderer->unload();
    vignetteRenderer->unload();
}

//TODO
void ms::NGin::load_point_light	(float power,
								 math::vec3 color,
								 math::vec3 position,
								 std::string absolutePath) {
    
    Loader::model_data loadedData = loader.load_model(absolutePath);
    Loader::geometries_vec & geo = std::get<0>(loadedData);
    Loader::materials_map & mat = std::get<1>(loadedData);
    Loader::textures_map & tex = std::get<2>(loadedData);

    for (auto & geometry : geo) {
        auto node = std::make_shared<PointLight>(power, color, position);
        node->geometry=std::move(geometry);
        scene->get_point_lights().push_back(std::move(node));
    }

    for (auto & material : mat) {
        scene->get_materials().insert(std::move(material));
    }

    for (auto & texture : tex) {
        scene->get_textures().insert(std::move(texture));
    }
	
}
//TODO
void ms::NGin::load_spot_light (float power,
								math::vec3 color,
								math::vec3 position,
								float lightingAngleDegrees,
								math::vec3 direction,
								std::string absolutePath) {

    Loader::model_data loadedData = loader.load_model(absolutePath);
    Loader::geometries_vec & geo = std::get<0>(loadedData);
    Loader::materials_map & mat = std::get<1>(loadedData);
    Loader::textures_map & tex = std::get<2>(loadedData);
    
    for (auto & geometry : geo) {
        auto node = std::make_shared<SpotLight>(power, color, position, lightingAngleDegrees, direction);
        node->geometry=std::move(geometry);
        scene->get_spot_lights().push_back(std::move(node));
    }
    
    for (auto & material : mat) {
        scene->get_materials().insert(std::move(material));
    }
    
    for (auto & texture : tex) {
        scene->get_textures().insert(std::move(texture));
    }
	
}

void ms::NGin::count_fps () {
	static int fps = 0;
	static auto start = std::chrono::high_resolution_clock::now();

	
	auto now = std::chrono::high_resolution_clock::now();
	auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
	
	fps += 1;
	
	if(timeElapsed > 1000.0f){
		std::cout << fps/(timeElapsed/1000.0f) << std::endl;
		fps = 0;
		start = now;
	}

}

void ms::NGin::draw_scene() {
	
    #ifdef NGIN_COUNT_FPS
        count_fps();
    #endif
    
    if (auto dirLight = scene->get_directional_light()) {
        shadowRenderer->use(*shadows[0]);
        shadowRenderer->setup_uniforms(*scene);
        for(int i = 0; i < scene->get_nodes().size(); ++i) {
            shadowRenderer->draw(*scene->get_nodes()[i], *scene);
        }
    }
    
    if(chosenRenderer == Renderer::deferred) {
        deferredRenderer->use();
        deferredRenderer->setup_g_buffer_uniforms(scene.get());
        deferredRenderer->gFramebuffer->use();
        deferredRenderer->gFramebuffer->clear_frame();
        for(int i = 0; i < scene->get_nodes().size(); ++i) {
            auto node = scene->get_nodes()[i];
            if(scene->get_camera().is_in_camera_sight(node->modelTransformation.get_transformation(), node->geometry->get_bounding_box())) {
                deferredRenderer->draw(*scene->get_nodes()[i], *scene);
            }
        }
        deferredRenderer->get_framebuffer().use();
        deferredRenderer->get_framebuffer().clear_frame();
        deferredRenderer->lightingShader->use();
        deferredRenderer->lightingShader->bind_texture(3, *shadows[0]->get_depth_texture().lock());
        deferredRenderer->perform_light_pass(scene.get());
        lightSourceRenderer->get_framebuffer().copy_framebuffer(deferredRenderer->get_framebuffer());
    } else if(chosenRenderer == Renderer::forward_fragment) {
        phongForwardRenderer->use(deferredRenderer->get_framebuffer());
        deferredRenderer->get_framebuffer().clear_frame();
        phongForwardRenderer->setup_uniforms(scene.get());
        for(int i = 0; i < scene->get_nodes().size(); ++i) {
            phongForwardRenderer->draw(*scene->get_nodes()[i], *scene);
        }
        lightSourceRenderer->get_framebuffer().copy_framebuffer(deferredRenderer->get_framebuffer());
    } else {
        gouraudForwardRenderer->use(deferredRenderer->get_framebuffer());
        deferredRenderer->get_framebuffer().clear_frame();
        gouraudForwardRenderer->setup_uniforms(scene.get());
        for(int i = 0; i < scene->get_nodes().size(); ++i) {
            gouraudForwardRenderer->draw(*scene->get_nodes()[i], *scene);
        }
        lightSourceRenderer->get_framebuffer().copy_framebuffer(deferredRenderer->get_framebuffer());
    }


    lightSourceRenderer->use();
    for(int i = 0; i < scene->get_point_lights().size(); ++i) {
        lightSourceRenderer->draw(*scene->get_point_lights()[i], *scene);
    }
    for(int i = 0; i < scene->get_spot_lights().size(); ++i) {
        lightSourceRenderer->draw(*scene->get_spot_lights()[i], *scene);
    }

    bloomSplitRenderer->use();
    bloomSplitRenderer->get_framebuffer().clear_frame();
    bloomSplitRenderer->draw_quad();

    gaussianBlurFirstStepRenderer->use();
    gaussianBlurFirstStepRenderer->get_framebuffer().clear_frame();
    gaussianBlurFirstStepRenderer->draw_quad();

    gaussianBlurSecondStepRenderer->use();
    gaussianBlurSecondStepRenderer->get_framebuffer().clear_frame();
    gaussianBlurSecondStepRenderer->draw_quad();

    bloomMergeRenderer->use();
    bloomMergeRenderer->get_framebuffer().clear_frame();
    bloomMergeRenderer->draw_quad();

    hdrRenderer->get_framebuffer().use();
    hdrRenderer->get_framebuffer().clear_frame();
    hdrRenderer->draw_quad();

    vignetteRenderer->get_framebuffer().use();
    vignetteRenderer->get_framebuffer().clear_frame();
    vignetteRenderer->draw_quad();

    scaleRenderer->get_framebuffer().use();
    scaleRenderer->get_framebuffer().clear_frame();
    scaleRenderer->draw_quad();
    
}

void ms::NGin::set_renderer (Renderer r) {
    this->chosenRenderer = r;
}

ms::DeferredRender & ms::NGin::get_deferred_render	() const {
    return *deferredRenderer;
}

