//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2017 Mateusz Stompór. All rights reserved.
//

#ifdef __WIN32__
	#include <glad/glad.h>
	#include "../../third-party-libs/renderdoc.h"
#endif

#include <GLFW/glfw3.h>
#include <cstring>
#include <string>
#include <memory>
#include <iostream>
#include <cassert>

#include "../../source_code/umbrellaHeader.hpp"
#include "controlls.hpp"
#include "utils.hpp"

std::unique_ptr<ms::NGin> engine 	= nullptr;
unsigned int usedPointLightIndex 	= 0;
unsigned int usedSpotLightIndex 	= 0;

using namespace ms;
using namespace math;
using namespace transform;

namespace ms {
    const std::string libInitializationError        = "Cannot initialize GLFW3";
    const std::string windowCreationError           = "Cannot create window";
    const std::string windowName                    = "NGIN";
    const std::string contextInitializationFailure  = "Failed to initialize OpenGL context";
}

int main(int argc, const char * argv[]) { {

	
	// pass path to scene with building as first param
	// pass path to nanosuit as second param
	// pass path to cone as third param
	// pass path to sphere as fourth param
	
	bool useCommandLineArguments = argc == 5;


	#ifdef __WIN32__
		int width = 2650;
		int height = 1400;
	#else
		int width = 1650;
		int height = 1000;
	#endif

	int actualScreenWidth;
	int actualScreenHeight;
	int framebufferWidth;
	int framebufferHeight;
	
	prepareRenderDoc();
	
	if(glfwInit()==0) {
		std::cerr<<ms::libInitializationError<<std::endl;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwSwapInterval(0);
	
	GLFWwindow *window = glfwCreateWindow(width, height, ms::windowName.c_str(), nullptr, nullptr);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	
	actualScreenWidth = framebufferWidth;
	actualScreenHeight = framebufferHeight;
	
    unsigned int shadowResolution = 1024 * 3;
    
	//Configure rendering resoultion here
	#ifndef __WIN32__
        framebufferWidth /= 2;
        framebufferHeight /= 2;
	#endif
	
	if(window == nullptr){
		std::cerr << ms::windowCreationError << std::endl;
		glfwTerminate();
	}
	
	glfwMakeContextCurrent(window);
	
#ifdef __WIN32__
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << ms::contextInitializationFailure << std::endl;
		return -1;
	}
#endif
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
    auto cam = std::make_unique<ms::PerspectiveCamera>(0.01f, 100, 90, float(framebufferWidth)/framebufferHeight);

    engine = std::make_unique<NGin>(actualScreenWidth, actualScreenHeight, framebufferWidth, framebufferHeight, shadowResolution, std::move(cam), nullptr);
		
    engine->load_model(useCommandLineArguments ? argv[1] : "./sponza/sponza.obj");

//    engine->scene.set_directional_light(std::make_unique<DirectionalLight>(ms::math::vec3{ 1.0f, 1.0f, 1.0f}, 50, vec3(0.0f, -1.0f, 0.0f).normalized(), true));
//
//    mat4 lookat = transform::look_at(vec3(0.0f, 4.0f, 0.0f),
//                                     vec3( 0.0f, 0.0f,  0.0f),
//                                     vec3( 0.0f, 0.0f,  1.0f));
//
//    engine->scene.get_directional_light()->get_transformation() = lookat;

    SpotLight sl1({1.0f, 0.0f, 0.0f}, 50.0f, {0.0f, 0.0f, 0.0f}, 50.0f, {1.0f, 0.0f, 0.0f}, true);
//    SpotLight sl2({1.0f, 1.0f, 0.0f}, 50.0f, {0.0f, 0.0f, 0.0f}, 80.0f, {1.0f, 0.0f, 0.0f}, true);
    engine->scene.get_spot_lights().push_back(sl1);
//    engine->scene.get_spot_lights().push_back(sl2);
    
//    PointLight pl1({1.0f, 1.0f, 0.0f}, 50.0f, {0.0f, 0.0f, 0.0f}, true);
//    engine->scene.get_point_lights().push_back(pl1);

    for(int i = 0; i < engine->scene.get_nodes().size(); ++i) {
        engine->scene.get_nodes()[i]->transformation = ms::math::transform::scale<float, 4>({0.02f, 0.02f, 0.02f}) * engine->scene.get_nodes()[i]->transformation;
    }

    engine->load_model(useCommandLineArguments ? argv[2] : "./nanosuit/nanosuit.obj");
	
	engine->load();
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
	glfwSetKeyCallback(window, key_callback);
	
	while(!glfwWindowShouldClose(window)) {
		
		glfwPollEvents();
		
		//////////////////////////////////////////////////////////////////////////////////////////
		
		engine->draw_scene();
		
		//////////////////////////////////////////////////////////////////////////////////////////
		
		glfwSwapBuffers(window);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////

	engine->unload();

    std::cout<<get_allocation_statistics()<<std::endl;
    
    engine = nullptr;
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
	glfwTerminate();
	
	}
		
    return 0;
}
