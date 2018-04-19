//
//  ngin.cpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "ngin.hpp"

ms::NGin::NGin(unsigned int screenWidth,
			   unsigned int screenHeight,
			   unsigned int framebufferWidth,
			   unsigned int framebufferHeight,
               std::unique_ptr<Camera> && cam) :
    scene(std::make_unique<Scene>(std::move(cam))),
	screenWidth(screenWidth),
	screenHeight(screenHeight),
	framebufferWidth(framebufferWidth),
	framebufferHeight(framebufferHeight),
	phongForwardRenderer(nullptr),
	gouraudForwardRenderer(nullptr),
	deferredRenderer(nullptr),
	lightSourceRenderer(nullptr),
	hdrRenderer(nullptr),
	bloomSplitRenderer(nullptr),
	bloomMergeRenderer(nullptr),
    loader{} {
		
}

ms::DeferredRender & ms::NGin::get_deferred_render () const {
	return *deferredRenderer;
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
    
    if(chosenRenderer == Renderer::deferred) {
        deferredRenderer->use();
        deferredRenderer->setup_g_buffer_uniforms(scene.get());
        deferredRenderer->gFramebuffer->use();
        deferredRenderer->gFramebuffer->clear_frame();
//        deferredRenderer->get_framebuffer().use();
//        deferredRenderer->get_framebuffer().clear_frame();
        for(int i = 0; i < scene->get_nodes().size(); ++i) {
            auto node = scene->get_nodes()[i];
            if(scene->get_camera().is_in_camera_sight(node->modelTransformation.get_transformation(), node->geometry->get_bounding_box())) {
                deferredRenderer->draw(*scene->get_nodes()[i], *scene);
            }
        }
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


    lightSourceRenderer->use(deferredRenderer->get_framebuffer());
    for(int i = 0; i < scene->get_point_lights().size(); ++i) {
        lightSourceRenderer->draw(*scene->get_point_lights()[i], *scene);
    }
    for(int i = 0; i < scene->get_spot_lights().size(); ++i) {
        lightSourceRenderer->draw(*scene->get_spot_lights()[i], *scene);
    }

    bloomSplitRenderer->use(deferredRenderer->get_framebuffer());
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

