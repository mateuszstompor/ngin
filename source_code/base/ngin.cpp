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

#define TRANSFORM -1.0f

ms::NGin::NGin(unsigned int                   	screenWidth,
               unsigned int                     screenHeight,
               unsigned int                     fbW,
               unsigned int                     fbH,
               unsigned int                     shadowsResolution,
               std::unique_ptr<Camera> &&       cam,
               std::unique_ptr<Framebuffer> &&  defaultFramebuffer) :
shouldDraw{true},
scene{std::move(cam)},
screenWidth{screenWidth},
screenHeight{screenHeight},
framebufferWidth{fbW},
framebufferHeight{fbH},
loader{},
shadowResolution{shadowsResolution}{    
        using tf = texture::Format;
        using tt = texture::AssociatedType;
    
        auto windowFramebuffer(defaultFramebuffer == nullptr ? Framebuffer::window_framebuffer(screenWidth, screenHeight) : std::move(defaultFramebuffer));
    
        auto vignetteFramebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(1, 0, fbW, fbH));
        auto hdrFramebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(1, 0, fbW, fbH));
        auto mainRenderFramebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(1, 1, fbW, fbH));
        auto lsFramebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(1, 1, fbW, fbH));
        auto bloomSplitFramebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(2, 0, fbW, fbH));
        //gaussian blur
        auto gbfFramebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(1, 0, fbH, fbW));
        auto gbsFramebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(1, 0, fbW, fbH));
        auto bloomMergeFramebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(1, 0, fbW, fbH));
    
        shadowsArray = std::unique_ptr<Texture2DArray>( new Texture2DArray{10, tf::depth_32, tt::FLOAT, shadowResolution, shadowResolution} );
        shadowsArray->load();
    
        for (std::size_t i = 0; i < 11; ++i) {
            shadows.push_back(std::unique_ptr<Framebuffer>(new Framebuffer(0, 1, shadowResolution, shadowResolution)));
            if(i == 0) {
                shadows[i]->bind_depth_buffer(std::make_unique<Texture2D>(tf::depth_32, tt::FLOAT, shadowResolution, shadowResolution));
            } else {
                shadows[i]->bind_depth_buffer(shadowsArray, i-1);
            }
            shadows[i]->configure();
        }
    
    for ( auto j{0}; j < 20; ++j) {
        auto cm = new CubeMap{texture::Format::depth_32, texture::AssociatedType::FLOAT, ms::Size<uint>{shadowResolution, shadowResolution}};
        pointLightShadowsBuffer.push_back(std::unique_ptr<CubeMap>{cm});
    }
    
    pointLightFramebuffer = std::unique_ptr<Framebuffer>{new Framebuffer(0, 1, shadowResolution, shadowResolution)};
    pointLightFramebuffer->bind_depth_buffer(pointLightShadowsBuffer[0], CubeMap::Face::back_positive_z);
    pointLightFramebuffer->configure();
    
        mainRenderFramebuffer->bind_depth_buffer(std::unique_ptr<Renderbuffer>(new Renderbuffer(tf::depth_32, tt::FLOAT, 0, fbW, fbH)));
        mainRenderFramebuffer->bind_color_buffer(0, std::make_unique<Texture2D>(tf::rgb_16_16_16, tt::FLOAT, fbW, fbH));
        mainRenderFramebuffer->configure();
    

        lsFramebuffer->bind_depth_buffer(std::unique_ptr<Renderbuffer>(new Renderbuffer(tf::depth_32, tt::FLOAT, 0, fbW, fbH)));
        lsFramebuffer->bind_color_buffer(0, std::make_unique<Texture2D>(tf::rgb_16_16_16, tt::FLOAT, fbW, fbH));
        lsFramebuffer->configure();
    
    
        vignetteFramebuffer->bind_color_buffer(0, std::make_unique<Texture2D>(tf::rgb_16_16_16, tt::FLOAT, fbW, fbH));
        vignetteFramebuffer->configure();
    
    
        gbfFramebuffer->bind_color_buffer(0, std::make_unique<Texture2D>(tf::rgb_16_16_16, tt::FLOAT, fbH, fbW));
        gbfFramebuffer->configure();
    
        gbsFramebuffer->bind_color_buffer(0, std::make_unique<Texture2D>(tf::rgb_16_16_16, tt::FLOAT, fbW, fbH));
        gbsFramebuffer->configure();
    
    
        hdrFramebuffer->bind_color_buffer(0, std::make_unique<Texture2D>(tf::rgb_16_16_16, tt::FLOAT, fbW, fbH));
        hdrFramebuffer->configure();
    
    
        bloomSplitFramebuffer->bind_color_buffer(0, std::make_unique<Texture2D>(tf::rgb_16_16_16, tt::FLOAT, fbW, fbH));
        bloomSplitFramebuffer->bind_color_buffer(1, std::make_unique<Texture2D>(tf::rgb_16_16_16, tt::FLOAT, fbW, fbH));
        bloomSplitFramebuffer->configure();
    
        bloomMergeFramebuffer->bind_color_buffer(0, std::make_unique<Texture2D>(tf::rgb_16_16_16, tt::FLOAT, fbW, fbH));
        bloomMergeFramebuffer->configure();
    
        unsigned int AOL = 100;
    
        auto shadowShader = Shader::vf_program(get_shader_of_type(Type::shadow_mapping_dir_vshader), get_shader_of_type(Type::shadow_mapping_dir_fshader));
        auto phongforwardShader = Shader::vf_program(get_shader_of_type(Type::forward_render_phong_vshader), get_shader_of_type(Type::forward_render_phong_fshader));
        auto gouraudforwardShader = Shader::vf_program(get_shader_of_type(Type::forward_render_gouraud_vshader), get_shader_of_type(Type::forward_render_gouraud_fshader));
        auto lightSourceforwardShader = Shader::vf_program(get_shader_of_type(Type::forward_render_light_drawer_vshader), get_shader_of_type(Type::forward_render_light_drawer_fshader));
        auto bloomSplitProgram = Shader::vf_program(get_shader_of_type(Type::post_process_bloom_splitter_vshader), get_shader_of_type(Type::post_process_bloom_splitter_fshader));
        auto bloomMergeProgram = Shader::vf_program(get_shader_of_type(Type::post_process_bloom_merger_vshader), get_shader_of_type(Type::post_process_bloom_merger_fshader));
        auto hdrProgram = Shader::vf_program(get_shader_of_type(Type::post_process_hdr_vshader), get_shader_of_type(Type::post_process_hdr_fshader));
        auto gaussianBlurProgram = Shader::vf_program(get_shader_of_type(Type::post_process_gaussian_blur_vshader), get_shader_of_type(Type::post_process_gaussian_blur_fshader));
        auto secondGaussianBlurProgram = Shader::vf_program(get_shader_of_type(Type::post_process_gaussian_blur_vshader), get_shader_of_type(Type::post_process_gaussian_blur_fshader));
        auto vignetteProgram = Shader::vf_program(get_shader_of_type(Type::post_process_vignette_vshader), get_shader_of_type(Type::post_process_vignette_fshader));
        auto scaleProgram = Shader::vf_program(get_shader_of_type(Type::post_process_scale_vshader), get_shader_of_type(Type::post_process_scale_fshader));
        auto defGshader = Shader::vf_program(get_shader_of_type(Type::deferred_render_g_buf_vertex_shader), get_shader_of_type(Type::deferred_render_g_buf_fragment_shader));
        auto defLightingShader = Shader::vf_program(get_shader_of_type(Type::deferred_render_light_pass_vshader), get_shader_of_type(Type::deferred_render_light_pass_fshader));
        auto plsmShader = Shader::vf_program(get_shader_of_type(Type::shadow_mapping_pl_vshader), get_shader_of_type(Type::shadow_mapping_pl_fshader));
        deferredRenderer = std::unique_ptr<DeferredRender> (new DeferredRender{AOL, AOL, std::move(mainRenderFramebuffer), std::move(defGshader), std::move(defLightingShader)});
        shadowRenderer = std::unique_ptr<DLShadowRender>(new DLShadowRender{nullptr, std::move(shadowShader)});
        phongForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRender{AOL, nullptr, std::move(phongforwardShader)});
        gouraudForwardRenderer = std::unique_ptr<ForwardRender>(new ForwardRender(AOL, nullptr, std::move(gouraudforwardShader)));
        lightSourceRenderer = std::unique_ptr<LightSourcesRender>(new LightSourcesRender{std::move(lsFramebuffer), std::move(lightSourceforwardShader)});
        bloomSplitRenderer = std::unique_ptr<PostprocessRender>(new PostprocessRender(lightSourceRenderer->get_framebuffer().get_colors(), std::move(bloomSplitFramebuffer), std::move(bloomSplitProgram)));
        pointLightShadowRenderer = std::unique_ptr<DLShadowRender>(new DLShadowRender{nullptr, std::move(plsmShader)});
        std::vector<std::weak_ptr<Texture2D>> textures;
        textures.push_back(bloomSplitRenderer->get_framebuffer().get_colors()[0]);
    
        gaussianBlurFirstStepRenderer = std::unique_ptr<GaussianBlurPostprocessRender>(new GaussianBlurPostprocessRender{textures, std::move(gbfFramebuffer), std::move(gaussianBlurProgram)});
    
        gaussianBlurSecondStepRenderer = std::unique_ptr<GaussianBlurPostprocessRender>(new GaussianBlurPostprocessRender(gaussianBlurFirstStepRenderer->get_framebuffer().get_colors(), std::move(gbsFramebuffer), std::move(secondGaussianBlurProgram)));
    
        std::vector<std::weak_ptr<Texture2D>> textures2;
        textures2.push_back(gaussianBlurSecondStepRenderer->get_framebuffer().get_colors()[0]);
        textures2.push_back(bloomSplitRenderer->get_framebuffer().get_colors()[1]);
    
        bloomMergeRenderer = std::unique_ptr<PostprocessRender>(new PostprocessRender(textures2, std::move(bloomMergeFramebuffer), std::move(bloomMergeProgram)));
        hdrRenderer = std::unique_ptr<PostprocessRender>(new PostprocessRender(bloomMergeRenderer->get_framebuffer().get_colors(), std::move(hdrFramebuffer), std::move(hdrProgram)));
        vignetteRenderer = std::unique_ptr<VignettePostprocessRender>(new VignettePostprocessRender(hdrRenderer->get_framebuffer().get_colors(), std::move(vignetteFramebuffer), std::move(vignetteProgram)));
        scaleRenderer = std::unique_ptr<PostprocessRender>(new PostprocessRender(vignetteRenderer->get_framebuffer().get_colors(), std::move(windowFramebuffer), std::move(scaleProgram)));
}

void ms::NGin::load_model (std::string const & absolutePath) {
	
    Loader::model_data loadedData = loader.load_flat_model(absolutePath);
    
    {
        auto & geo = std::get<0>(loadedData);
        auto & mat = std::get<1>(loadedData);
        auto & tex = std::get<2>(loadedData);
    
        scene.get_materials().insert(std::make_move_iterator(mat.begin()), std::make_move_iterator(mat.end()));
        scene.get_textures().insert(std::make_move_iterator(tex.begin()), std::make_move_iterator(tex.end()));
        scene.get_geometries().insert(scene.get_geometries().end(), std::make_move_iterator(geo.begin()), std::make_move_iterator(geo.end()));
    }
    for (auto & geometry : scene.get_geometries()) {
        auto node = std::make_shared<Drawable>();
        node->bind_geometry(geometry);
        auto nodeMaterial = scene.get_materials().find(node->get_geometry()->get_preferred_material_name());
        if(nodeMaterial != scene.get_materials().end()) {

            node->bind_material(nodeMaterial->second);

            if(nodeMaterial->second->diffuseTexturesNames.size() > 0) {
                auto diffuseTexture = scene.get_textures().find(nodeMaterial->second->diffuseTexturesNames[0]);
                if(diffuseTexture != scene.get_textures().end()) {
                    node->get_material()->bind_diffuse_texture(diffuseTexture->second);
                }
            }

            if(nodeMaterial->second->specularTexturesNames.size() > 0) {
                auto specularTexture = scene.get_textures().find(nodeMaterial->second->specularTexturesNames[0]);
                if(specularTexture != scene.get_textures().end()) {
                    node->get_material()->bind_specular_texture(specularTexture->second);
                }
            }

            if(nodeMaterial->second->heightTexturesNames.size() > 0) {
                auto heightTexture = scene.get_textures().find(nodeMaterial->second->heightTexturesNames[0]);
                if(heightTexture != scene.get_textures().end()) {
                    node->get_material()->bind_height_texture(heightTexture->second);
                }
            }

            if(nodeMaterial->second->normalTexturesNames.size() > 0) {
                auto normalTexture = scene.get_textures().find(nodeMaterial->second->normalTexturesNames[0]);
                if(normalTexture != scene.get_textures().end()) {
                    node->get_material()->bind_normal_texture(normalTexture->second);
                }
            }
            
        }
        scene.get_nodes().insert_s(scene.get_nodes().begin(), node);
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
    shadowRenderer->load();
    pointLightShadowRenderer->load();
    vignetteRenderer->load();
}

void ms::NGin::unload () {
    pause_drawing();
    pointLightFramebuffer->unload();
    shadowsArray->unload();
    pointLightShadowRenderer->unload();
    phongForwardRenderer->unload();
    deferredRenderer->unload();
    gouraudForwardRenderer->unload();
    lightSourceRenderer->unload();
    hdrRenderer->unload();
    bloomSplitRenderer->unload();
    shadowRenderer->unload();
    bloomMergeRenderer->unload();
    gaussianBlurFirstStepRenderer->unload();
    gaussianBlurSecondStepRenderer->unload();
    scaleRenderer->unload();
    vignetteRenderer->unload();
    
    std::for_each(pointLightShadowsBuffer.begin(), pointLightShadowsBuffer.end(), [](auto const & res){ res->unload(); });
    std::for_each(scene.get_geometries().begin(), scene.get_geometries().end(), [](auto const & geo) { geo->unload(); } );
    std::for_each(scene.get_nodes().begin(), scene.get_nodes().end(), [](auto const & node) { node->unload(); } );
    std::for_each(scene.get_textures().begin(), scene.get_textures().end(), [](auto const & texture) { texture.second->unload(); });
    std::for_each(scene.get_materials().begin(), scene.get_materials().end(), [](auto const & material) { material.second->unload(); });
    std::for_each(shadows.begin(), shadows.end(), [](auto const & fb) { fb->unload(); } );
}

void ms::NGin::count_fps () {
	static int fps = 0;
	static auto start = std::chrono::high_resolution_clock::now();
	
	auto now = std::chrono::high_resolution_clock::now();
	auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
	
	fps += 1;
	
	if(timeElapsed > 1000.0f){
		std::cout << fps/(timeElapsed/1000.0f) << '\n';
		fps = 0;
		start = now;
	}

}

void ms::NGin::draw_scene() {
	
    if(shouldDraw) {
        
    #ifdef NGIN_COUNT_FPS
            count_fps();
    #endif
        
        draw_models();
        draw_postprocess();
        
    #ifdef DRAW_LIGHT_POV
        draw_sl_pov(0, 0, 500, 500);
        draw_pl_pov(0, 0, 500, 500);
    #endif
        
    }
    
}

void ms::NGin::draw_models () {
    std::stack<math::mat4> s{};
    std::function<void(tree<std::shared_ptr<Drawable>>::depth_change, tree<std::shared_ptr<Drawable>>::iterator)> l = [&](auto direction, auto it){
        if (direction == tree<std::shared_ptr<Drawable>>::depth_change::up) {
            if(!s.empty())
                s.pop();
        } else {
            s.push(s.empty() ? (*it.parent())->get_transformation() : (*it.parent())->get_transformation() * s.top());
        }
    };
    
    if (auto dirLight = scene.get_directional_light()) {
        if(dirLight->casts_shadow()) {
            shadows[0]->use();
            shadows[0]->clear_frame();
            shadowRenderer->use(*shadows[0]);
            shadowRenderer->setup_uniforms(dirLight->get_projection(), dirLight->get_transformation());
            s = std::stack<math::mat4>{};
            auto it = scene.get_nodes().begin(l);
            while(it != scene.get_nodes().end()) {
                shadowRenderer->draw(*(*it), s.empty() ? (*it)->get_transformation() : (*it)->get_transformation() * s.top());
                ++it;
            }
        }
    }
    
    lightSourceRenderer->use();
    lightSourceRenderer->get_framebuffer().clear_frame();
    
    for (size_t i = 0; i < scene.get_spot_lights().size(); ++i) {
        SpotLight & spotLight = scene.get_spot_lights()[i];
        if(spotLight.casts_shadow()) {
            shadows[1 + i]->use();
            shadows[1 + i]->clear_frame();
            shadowRenderer->use(*shadows[1 + i]);
            shadowRenderer->setup_uniforms(spotLight.get_frustum().get_projection_matrix(), spotLight.get_transformation());
            s = std::stack<math::mat4>{};
            auto it = scene.get_nodes().begin(l);
            
            while(it != scene.get_nodes().end()) {
                auto node = *it;
                auto transform =  s.empty() ? node->get_transformation() : node->get_transformation() * s.top();
                
                if(node->can_be_drawn() && spotLight.get_frustum().is_in_camera_sight( spotLight.get_transformation() * transform, node->get_geometry()->get_bounding_box())) {
                    shadowRenderer->draw(*(*it), transform);
                }
                ++it;
            }
            
        }
    }
    
    //point light shadows
    s = std::stack<math::mat4>{};
    for(auto i{0}; i < scene.get_point_lights().size(); ++i) {
        PointLight & pointLight = scene.get_point_lights()[i];
        pointLightShadowRenderer->use(*pointLightFramebuffer);
        if(pointLight.casts_shadow()) {
            pointLightFramebuffer->use();
            pointLightFramebuffer->clear_frame();
            for(int j{0}; j < 6; ++j) {
                pointLightFramebuffer->bind_depth_buffer(pointLightShadowsBuffer[i], (CubeMap::Face::right_positive_x + j));
                pointLightFramebuffer->clear_frame();
                auto transform = math::transform4f::translate(math::vec3{0.0f, 0.0f, TRANSFORM});
                switch (j) {
                    case 0:
                        pointLightShadowRenderer->setup_uniforms(pointLight.get_projection(), transform * math::transform4f::rotate_about_y_radians(M_PI/2) * pointLight.get_transformation() );
                        break;
                    case 1:
                        pointLightShadowRenderer->setup_uniforms(pointLight.get_projection(), transform * math::transform4f::rotate_about_y_radians(-M_PI/2) * pointLight.get_transformation());
                        break;
                    case 2:
                        pointLightShadowRenderer->setup_uniforms(pointLight.get_projection(), transform * math::transform4f::rotate_about_x_radians(-M_PI/2) * pointLight.get_transformation());
                        break;
                    case 3:
                        pointLightShadowRenderer->setup_uniforms(pointLight.get_projection(), transform * math::transform4f::rotate_about_x_radians(M_PI/2) * pointLight.get_transformation());
                        break;
                    case 4:
                        pointLightShadowRenderer->setup_uniforms(pointLight.get_projection(), transform * pointLight.get_transformation());
                        break;
                    case 5:
                        pointLightShadowRenderer->setup_uniforms(pointLight.get_projection(), transform * math::transform4f::rotate_about_y_radians(M_PI) * pointLight.get_transformation());
                        break;
                    default:
                        break;
                }
                s = std::stack<math::mat4>{};
                auto it = scene.get_nodes().begin(l);
                
                while(it != scene.get_nodes().end()) {
                    auto node = *it;
                    auto transform =  s.empty() ? node->get_transformation() : node->get_transformation() * s.top();
                    if(node->can_be_drawn()) {
                        shadowRenderer->draw(*(*it), transform);
                    }
                    ++it;
                }
            }
        }
    }
    
    //point light shadows end
    
    s = std::stack<math::mat4>{};
    if(chosenRenderer == Renderer::deferred) {
        deferredRenderer->use();
        deferredRenderer->set_camera(scene.get_camera());
        deferredRenderer->gFramebuffer->use();
        deferredRenderer->gFramebuffer->clear_frame();
        
        tree<std::shared_ptr<Drawable>>::iterator it = scene.get_nodes().begin(l);
        
        while(it != scene.get_nodes().end()) {
            auto node = *it;
            auto transform =  s.empty() ? node->get_transformation() : node->get_transformation() * s.top();
            if(node->can_be_drawn() && node->get_material()->is_shaded() && !node->boundedMaterial->is_translucent()) {
                if(scene.get_camera().is_in_camera_sight(transform, node->get_geometry()->get_bounding_box())) {
                    deferredRenderer->draw(*node, transform);
                }
            }
            ++it;
        }
        
        deferredRenderer->get_framebuffer().use();
        deferredRenderer->get_framebuffer().clear_frame();
        deferredRenderer->lightingShader->use();
        deferredRenderer->lightingShader->bind_texture(3, *shadows[0]->get_depth_texture().lock());
        deferredRenderer->get_shader().bind_texture(4, *shadowsArray);
        deferredRenderer->get_shader().bind_texture(5, *pointLightShadowsBuffer[0]);
        deferredRenderer->perform_light_pass(scene);
        lightSourceRenderer->get_framebuffer().copy_framebuffer(deferredRenderer->get_framebuffer());
    } else if(chosenRenderer == Renderer::forward_fragment) {
        phongForwardRenderer->use(deferredRenderer->get_framebuffer());
        deferredRenderer->get_framebuffer().clear_frame();
        phongForwardRenderer->set_camera(scene.get_camera());
        phongForwardRenderer->set_spot_lights(scene.get_spot_lights());
        phongForwardRenderer->set_directionallight(scene.get_directional_light());
        phongForwardRenderer->set_point_lights(scene.get_point_lights());
        phongForwardRenderer->get_shader().bind_texture(3, *shadows[0]->get_depth_texture().lock());
        phongForwardRenderer->get_shader().bind_texture(4, *shadowsArray);
        tree<std::shared_ptr<Drawable>>::iterator it = scene.get_nodes().begin(l);
        
        while(it != scene.get_nodes().end()) {
            auto node = *it;
            auto transform =  s.empty() ? node->get_transformation() : node->get_transformation() * s.top();
            if(node->can_be_drawn() && node->get_material()->is_shaded() && !node->boundedMaterial->is_translucent()) {
                if(scene.get_camera().is_in_camera_sight(transform, node->get_geometry()->get_bounding_box())) {
                    phongForwardRenderer->set_material(node->get_material());
                    phongForwardRenderer->draw(*node, transform);
                }
            }
            ++it;
        }
        
        lightSourceRenderer->get_framebuffer().copy_framebuffer(deferredRenderer->get_framebuffer());
    } else {
        gouraudForwardRenderer->use(deferredRenderer->get_framebuffer());
        deferredRenderer->get_framebuffer().clear_frame();
        gouraudForwardRenderer->set_camera(scene.get_camera());
        gouraudForwardRenderer->set_spot_lights(scene.get_spot_lights());
        gouraudForwardRenderer->set_directionallight(scene.get_directional_light());
        gouraudForwardRenderer->set_point_lights(scene.get_point_lights());
        gouraudForwardRenderer->get_shader().bind_texture(3, *shadows[0]->get_depth_texture().lock());
        gouraudForwardRenderer->get_shader().bind_texture(4, *shadowsArray);
        tree<std::shared_ptr<Drawable>>::iterator it = scene.get_nodes().begin(l);
        
        while(it != scene.get_nodes().end()) {
            auto node = *it;
            auto transform =  s.empty() ? node->get_transformation() : node->get_transformation() * s.top();
            if(node->can_be_drawn() && node->get_material()->is_shaded() && !node->boundedMaterial->is_translucent()) {
                if(scene.get_camera().is_in_camera_sight(transform, node->get_geometry()->get_bounding_box())) {
                    gouraudForwardRenderer->set_material(node->get_material());
                    gouraudForwardRenderer->draw(*node, transform);
                }
            }
            ++it;
        }
        lightSourceRenderer->get_framebuffer().copy_framebuffer(deferredRenderer->get_framebuffer());
    }
    
    {
        lightSourceRenderer->use();
        lightSourceRenderer->set_camera(scene.get_camera());
        s = std::stack<math::mat4>{};
        tree<std::shared_ptr<Drawable>>::iterator it = scene.get_nodes().begin(l);
        
        while(it != scene.get_nodes().end()) {
            auto node = *it;
            auto transform =  s.empty() ? node->get_transformation() : node->get_transformation() * s.top();
            if(node->can_be_drawn() && !node->get_material()->is_shaded() && !node->boundedMaterial->is_translucent()) {
                if(scene.get_camera().is_in_camera_sight(transform, node->get_geometry()->get_bounding_box())) {
                    lightSourceRenderer->draw(*node, transform);
                }
            }
            ++it;
        }
    }
    
    {
        //        translucency
        lightSourceRenderer->get_framebuffer().set_blending(true);
        phongForwardRenderer->use(lightSourceRenderer->get_framebuffer());
        phongForwardRenderer->set_camera(scene.get_camera());
        phongForwardRenderer->set_spot_lights(scene.get_spot_lights());
        phongForwardRenderer->set_directionallight(scene.get_directional_light());
        phongForwardRenderer->set_point_lights(scene.get_point_lights());
        phongForwardRenderer->get_shader().bind_texture(3, *shadows[0]->get_depth_texture().lock());
        phongForwardRenderer->get_shader().bind_texture(4, *shadowsArray);
        s = std::stack<math::mat4>{};
        auto it = scene.get_nodes().begin(l);
        
        while(it != scene.get_nodes().end()) {
            auto node = *it;
            auto transform =  s.empty() ? node->get_transformation() : node->get_transformation() * s.top();
            if(node->can_be_drawn() && node->get_material()->is_shaded() && node->get_material()->is_translucent()) {
                if(scene.get_camera().is_in_camera_sight(transform, node->get_geometry()->get_bounding_box())) {
                    phongForwardRenderer->set_material(node->get_material());
                    phongForwardRenderer->draw(*node, transform);
                }
            }
            ++it;
        }
        
        lightSourceRenderer->get_framebuffer().set_blending(false);
    }
}

void ms::NGin::draw_sl_pov (std::uint16_t x, std::uint16_t y, std::uint16_t tileWidth, std::uint16_t tileHeight) {
    scaleRenderer->get_framebuffer().use();
    shadowRenderer->use(scaleRenderer->get_framebuffer());
    for(int i = 0; i < scene.get_spot_lights().size(); ++i) {
        mglViewport(x + tileWidth * i, y, tileWidth, tileWidth);
        scaleRenderer->get_framebuffer().clear_depth();
        shadowRenderer->setup_uniforms(scene.get_spot_lights()[i].get_frustum().get_projection_matrix(), scene.get_spot_lights()[i].get_transformation());
        for(auto & node : scene.get_nodes()) {
            shadowRenderer->draw(*node);
        }
    }
}

void ms::NGin::draw_pl_pov (std::uint16_t x, std::uint16_t y, std::uint16_t tileWidth, std::uint16_t tileHeight) {
    scaleRenderer->get_framebuffer().use();
    shadowRenderer->use(scaleRenderer->get_framebuffer());
    for(int i = 0; i < scene.get_point_lights().size(); ++i) {
        //render front view, side number 4
        auto transform = math::transform4f::translate(math::vec3{0.0f, 0.0f, TRANSFORM});
        mglViewport(x + tileWidth * 3 * i, y, tileWidth, tileWidth);
        scaleRenderer->get_framebuffer().clear_depth();
        shadowRenderer->setup_uniforms(scene.get_point_lights()[i].get_projection(), transform * scene.get_point_lights()[i].get_transformation());
        for(auto & node : scene.get_nodes()) {
            shadowRenderer->draw(*node);
        }
        //render right view, side number 0
        mglViewport(x + tileWidth * 3 * i + tileWidth, y, tileWidth, tileWidth);
        scaleRenderer->get_framebuffer().clear_depth();
        shadowRenderer->setup_uniforms(scene.get_point_lights()[i].get_projection(),transform * math::transform4f::rotate_about_y_radians(M_PI/2) * scene.get_point_lights()[i].get_transformation());
        for(auto & node : scene.get_nodes()) {
            shadowRenderer->draw(*node);
        }
        //render up view, side number 2
        mglViewport(x + tileWidth * 3 * i + 2 * tileWidth, y, tileWidth, tileWidth);
        scaleRenderer->get_framebuffer().clear_depth();
        shadowRenderer->setup_uniforms(scene.get_point_lights()[i].get_projection(), transform * math::transform4f::rotate_about_x_radians(-M_PI/2) * scene.get_point_lights()[i].get_transformation());
        for(auto & node : scene.get_nodes()) {
            shadowRenderer->draw(*node);
        }
    }
}

void ms::NGin::draw_postprocess () {
    bloomSplitRenderer->use();
    bloomSplitRenderer->get_framebuffer().clear_frame();
    bloomSplitRenderer->draw();
    
    gaussianBlurFirstStepRenderer->use();
    gaussianBlurFirstStepRenderer->get_framebuffer().clear_frame();
    gaussianBlurFirstStepRenderer->draw();
    
    gaussianBlurSecondStepRenderer->use();
    gaussianBlurSecondStepRenderer->get_framebuffer().clear_frame();
    gaussianBlurSecondStepRenderer->draw();
    
    bloomMergeRenderer->use();
    bloomMergeRenderer->get_framebuffer().clear_frame();
    bloomMergeRenderer->draw();
    
    hdrRenderer->use();
    hdrRenderer->get_framebuffer().clear_frame();
    hdrRenderer->draw();
    
    vignetteRenderer->use();
    vignetteRenderer->get_framebuffer().clear_frame();
    vignetteRenderer->draw();
    
    scaleRenderer->use();
    scaleRenderer->get_framebuffer().clear_frame();
    scaleRenderer->draw();
}

void ms::NGin::set_renderer (Renderer r) {
    this->chosenRenderer = r;
}

ms::DeferredRender & ms::NGin::get_deferred_render () {
    return *deferredRenderer;
}

ms::VignettePostprocessRender & ms::NGin::get_vignette_render () {
    return *vignetteRenderer;
}

ms::GaussianBlurPostprocessRender & ms::NGin::get_bloom_v_render () {
    return *gaussianBlurSecondStepRenderer;
}

ms::GaussianBlurPostprocessRender & ms::NGin::get_bloom_h_render () {
    return *gaussianBlurFirstStepRenderer;
}

ms::PostprocessRender & ms::NGin::get_hdr_render () {
    return *hdrRenderer;
}