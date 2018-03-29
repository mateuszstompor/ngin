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
			   float camNear,
			   float camFar,
			   float fovDegrees,
			   float aspect) :
	scene(new Scene(camNear, camFar, fovDegrees, aspect)),
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
	bloomMergeRenderer(nullptr) {
		
}

void ms::NGin::unload() {
	ResourceCoordinator::sharedInstance->unload_all_resources();
}

ms::DeferredRender & ms::NGin::get_deferred_render () const {
	return *deferredRenderer;
}

//TODO this function is implemented three times
void ms::NGin::load_model (std::string absolutePath) {
	
	if(loader == nullptr) {
		this->loader = get_loader();
	}
	
	Loader::model_data loadedData = loader->load_model(absolutePath);
	Loader::geometries_vec & geo = std::get<0>(loadedData);
	Loader::materials_map & mat = std::get<1>(loadedData);
	Loader::textures_map & tex = std::get<2>(loadedData);
	
	for (auto geometry : geo) {
		std::shared_ptr<ms::Drawable> node = get_drawable();
		node->geometry=geometry;
		auto nodeMaterial = mat.find(geometry->get_material_name());
		if(nodeMaterial != mat.end()) {
			
			node->boundedMaterial = nodeMaterial->second;
			
			if(nodeMaterial->second->diffuseTexturesNames.size() > 0) {
				auto diffuseTexture = tex.find(nodeMaterial->second->diffuseTexturesNames[0]);
				if(diffuseTexture != tex.end()) {
					nodeMaterial->second->boundedDiffuseTexture = diffuseTexture->second;
				}
			}
			
			if(nodeMaterial->second->specularTexturesNames.size() > 0) {
				auto specularTexture = tex.find(nodeMaterial->second->specularTexturesNames[0]);
				if(specularTexture != tex.end()) {
					nodeMaterial->second->boundedSpecularTexture = specularTexture->second;
				}
			}
			
			if(nodeMaterial->second->heightTexturesNames.size() > 0) {
				auto heightTexture = tex.find(nodeMaterial->second->heightTexturesNames[0]);
				if(heightTexture != tex.end()) {
					nodeMaterial->second->boundedHeightTexture = heightTexture->second;
				}
			}
			
			if(nodeMaterial->second->normalTexturesNames.size() > 0) {
				auto normalTexture = tex.find(nodeMaterial->second->normalTexturesNames[0]);
				if(normalTexture != tex.end()) {
					nodeMaterial->second->boundedNormalTexture = normalTexture->second;
				}
			}
			
		}
		scene->nodes.push_back(node);
	}
	
	for (auto material : mat) {
		scene->materials.insert(material);
	}
	
	for (auto texture : tex) {
		scene->textures.insert(texture);
	}
	
}

//TODO
void ms::NGin::load_point_light	(float power,
								 math::vec3 color,
								 math::vec3 position,
								 std::string absolutePath) {
	
	if(loader == nullptr) {
		this->loader = get_loader();
	}
	
	Loader::model_data loadedData = loader->load_model(absolutePath);
	Loader::geometries_vec & geo = std::get<0>(loadedData);
	Loader::materials_map & mat = std::get<1>(loadedData);
	Loader::textures_map & tex = std::get<2>(loadedData);
	
	for (auto geometry : geo) {
		std::shared_ptr<ms::PointLight> node = get_point_light(power, color, position);
		node->geometry=geometry;
		scene->pointLights.push_back(node);
	}
	
	for (auto material : mat) {
		scene->materials.insert(material);
	}
	
	for (auto texture : tex) {
		scene->textures.insert(texture);
	}
	
}
//TODO
void ms::NGin::load_spot_light (float power,
								math::vec3 color,
								math::vec3 position,
								float lightingAngleDegrees,
								math::vec3 direction,
								std::string absolutePath) {
	
	if(loader == nullptr) {
		this->loader = get_loader();
	}
	
	Loader::model_data loadedData = loader->load_model(absolutePath);
	Loader::geometries_vec & geo = std::get<0>(loadedData);
	Loader::materials_map & mat = std::get<1>(loadedData);
	Loader::textures_map & tex = std::get<2>(loadedData);
	
	for (auto geometry : geo) {
		std::shared_ptr<ms::SpotLight> node = get_spot_light(power, color, position, lightingAngleDegrees, direction);
		node->geometry=geometry;
		scene->spotLights.push_back(node);
	}
	
	for (auto material : mat) {
		scene->materials.insert(material);
	}
	
	for (auto texture : tex) {
		scene->textures.insert(texture);
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

    #ifdef CALLS_TIME_CONSUMPTION
        static int framesGenerated = 0;
        static int framesSpan = 150;
	#endif
    
        static long modelsDrawingTime,	        lightSourceDrawingTime, 	bloomSplitDrawingTime;
        static long firstStepBlurDrawingTime, 	secondStepBlurDrawingTime, 	bloomMergerDrawingTime;
        static long hdrDrawingTime,             vignetteDrawingTime,        upScallingTime;
	
	#ifdef NGIN_COUNT_FPS
		count_fps();
	#endif
    modelsDrawingTime = utils::measure_time<std::chrono::microseconds>([&](){
        if(chosenRenderer == Renderer::deferred) {
            deferredRenderer->use();
            deferredRenderer->setup_g_buffer_uniforms(scene.get());
            deferredRenderer->clear_frame();
            for(int i = 0; i < scene->nodes.size(); ++i) {
                deferredRenderer->draw(scene->nodes[i].get(), scene.get());
            }
            deferredRenderer->perform_light_pass(scene.get());
        } else if(chosenRenderer == Renderer::forward_fragment) {
            phongForwardRenderer->use();
            phongForwardRenderer->clear_frame();
            phongForwardRenderer->setup_uniforms(scene.get());
            for(int i = 0; i < scene->nodes.size(); ++i) {
                phongForwardRenderer->draw(scene->nodes[i].get(), scene.get());
            }
        } else {
            gouraudForwardRenderer->use();
            gouraudForwardRenderer->clear_frame();
            gouraudForwardRenderer->setup_uniforms(scene.get());
            for(int i = 0; i < scene->nodes.size(); ++i) {
                gouraudForwardRenderer->draw(scene->nodes[i].get(), scene.get());
            }

        }
    });
    
	secondOneColorFramebuffer->copy_depth_from(*oneColorDepthFramebuffer.get());

	lightSourceDrawingTime = utils::measure_time<std::chrono::microseconds>([&](){
		lightSourceRenderer->use();
		for(int i = 0; i < scene->pointLights.size(); ++i) {
			lightSourceRenderer->draw(scene->pointLights[i].get(), scene.get());
		}
		for(int i = 0; i < scene->spotLights.size(); ++i) {
			lightSourceRenderer->draw(scene->spotLights[i].get(), scene.get());
		}
	});
	
	bloomSplitDrawingTime = utils::measure_time<std::chrono::microseconds>([&](){
		bloomSplitRenderer->use();
		bloomSplitRenderer->clear_frame();
		bloomSplitRenderer->draw_quad();
	});
	
	firstStepBlurDrawingTime = utils::measure_time<std::chrono::microseconds>([&](){
		gaussianBlurFirstStepRenderer->use();
		gaussianBlurFirstStepRenderer->clear_frame();
		gaussianBlurFirstStepRenderer->draw_quad();
	});
	
	secondStepBlurDrawingTime = utils::measure_time<std::chrono::microseconds>([&](){
		gaussianBlurSecondStepRenderer->use();
		gaussianBlurSecondStepRenderer->clear_frame();
		gaussianBlurSecondStepRenderer->draw_quad();
	});
	
	bloomMergerDrawingTime = utils::measure_time<std::chrono::microseconds>([&](){
		bloomMergeRenderer->use();
		bloomMergeRenderer->clear_frame();
		bloomMergeRenderer->draw_quad();
	});
	
	hdrDrawingTime = utils::measure_time<std::chrono::microseconds>([&](){
		hdrRenderer->clear_frame();
		hdrRenderer->draw_quad();
	});
	
    vignetteDrawingTime = utils::measure_time<std::chrono::microseconds>([&](){
        vignetteRenderer->clear_frame();
        vignetteRenderer->draw_quad();
    });
    
    upScallingTime = utils::measure_time<std::chrono::microseconds>([&](){
        scaleRenderer->clear_frame();
        scaleRenderer->draw_quad();
    });
	
    #ifdef CALLS_TIME_CONSUMPTION
	framesGenerated += 1;
	if(framesGenerated > framesSpan) {
		std::cout << "Models Pass: " 			<< modelsDrawingTime 			<< std::endl;
		std::cout << "Light Sources Render: " 	<< lightSourceDrawingTime 		<< std::endl;
		std::cout << "Bloom Split: " 			<< bloomSplitDrawingTime 		<< std::endl;
		std::cout << "Blur first step: " 		<< firstStepBlurDrawingTime 	<< std::endl;
		std::cout << "Blur second step: " 		<< secondStepBlurDrawingTime 	<< std::endl;
		std::cout << "Bloom Merge: " 			<< bloomMergerDrawingTime 		<< std::endl;
		std::cout << "Hdr draw: " 				<< hdrDrawingTime 				<< std::endl;
        std::cout << "Vignette draw: "          << vignetteDrawingTime          << std::endl;
        std::cout << "Upscalling draw: "        << upScallingTime               << std::endl;
		framesGenerated = 0;
	}
    #endif
}

void ms::NGin::set_renderer (Renderer r) {
    this->chosenRenderer = r;
}

