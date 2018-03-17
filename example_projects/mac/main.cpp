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

std::unique_ptr<ms::NGin> engine;

namespace ms {
    const std::string libInitializationError        = "Cannot initialize GLFW3";
    const std::string windowCreationError           = "Cannot create window";
    const std::string windowName                    = "NGIN";
    const std::string contextInitializationFailure  = "Failed to initialize OpenGL context";
}

void prepareRenderDoc() {
	
#ifdef __WIN32__
	
    HINSTANCE hGetProcIDDLL = LoadLibrary("renderdoc.dll");

    if (!hGetProcIDDLL) {
        std::cout << "could not load the dynamic library" << std::endl;
        return exit(0);
    }

    HMODULE mod = GetModuleHandleA("renderdoc.dll");
    if(mod) {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");

        RENDERDOC_API_1_0_0 *rdoc_api = NULL;
        int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_0_0, (void **)&rdoc_api);

        assert(ret == 1);
        assert(rdoc_api->StartFrameCapture != NULL && rdoc_api->EndFrameCapture != NULL);

        int major = 999, minor = 999, patch = 999;
        std::cout<<rdoc_api->GetLogFilePathTemplate()<<std::endl;
        rdoc_api->GetAPIVersion(&major, &minor, &patch);

        assert(major == 1 && minor >= 0 && patch >= 0);
    }
	
#endif
	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	
	switch(key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
			
		case GLFW_KEY_1: {
			if (action == GLFW_PRESS) {
				engine->get_deferred_render().set_render_type(ms::DeferredRender::DebugType::standard);
			}
		}
			break;
			
		case GLFW_KEY_2: {
			if (action == GLFW_PRESS) {
				engine->get_deferred_render().set_render_type(ms::DeferredRender::DebugType::position);
			}
		}
			break;
		
		case GLFW_KEY_3: {
			if (action == GLFW_PRESS) {
				engine->get_deferred_render().set_render_type(ms::DeferredRender::DebugType::albedo);
			}
		}
			break;
			
		case GLFW_KEY_4: {
			if (action == GLFW_PRESS) {
				engine->get_deferred_render().set_render_type(ms::DeferredRender::DebugType::normals);
			}
		}
			break;
			
		case GLFW_KEY_5: {
			if (action == GLFW_PRESS) {
				engine->get_deferred_render().set_render_type(ms::DeferredRender::DebugType::specular);
			}
		}
			break;
		
		case GLFW_KEY_T: {
			auto a = ms::math::transform::rotateAboutYRadians<float, 4>(0.1f);
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
		break;
		case GLFW_KEY_Y: {
			auto a = ms::math::transform::rotateAboutYRadians<float, 4>(-0.1f);
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
		break;
		
		case GLFW_KEY_Q: {
			auto a = ms::math::transform::translate<float, 4>({0.0f, 0.0f, 0.1f});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_E: {
			auto a = ms::math::transform::translate<float, 4>({0.0f, 0.0f, -0.1f});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_A: {
			auto a = ms::math::transform::translate<float, 4>({0.1f, 0.0f, 0.0f});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_D: {
			auto a = ms::math::transform::translate<float, 4>({-0.1f, 0.0f, 0.0f});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_W: {
			auto a = ms::math::transform::translate<float, 4>({0.0f, 0.1f, 0.0f});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_S: {
			auto a = ms::math::transform::translate<float, 4>({0.0f, -0.1f, 0.0f});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;			
        default:
            break;
    }
}

int main(int argc, const char * argv[]) { {

	bool useCommandLineArguments = argc == 8;

	int width = 1200;
	int height = 800;
	int framebufferWidth = 600;
	int framebufferHeight = 400;
	
	prepareRenderDoc();
	
	if(glfwInit()==0) {
		std::cerr<<ms::libInitializationError<<std::endl;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwSwapInterval(1);
	
	GLFWwindow *window = glfwCreateWindow(width, height, ms::windowName.c_str(), nullptr, nullptr);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwGetFramebufferSize(window, &width, &height);
	
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
	
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
	auto forwardRenderVSource = ms::utils::load_contents_of_file (useCommandLineArguments ? argv[1] : "./vshader.glsl");
	auto forwardRenderFSource = ms::utils::load_contents_of_file (useCommandLineArguments ? argv[2] :"./fshader.glsl");
	auto deferredRenderVSource = ms::utils::load_contents_of_file (useCommandLineArguments ? argv[3] :"./g_buf_vshader.glsl");
	auto deferredRenderFSource = ms::utils::load_contents_of_file (useCommandLineArguments ? argv[4] :"./g_buf_fshader.glsl");
	auto deferredRenderLightingVSource = ms::utils::load_contents_of_file (useCommandLineArguments ? argv[5] :"./lighting_vshader.glsl");
	auto deferredRenderLightingFSource = ms::utils::load_contents_of_file (useCommandLineArguments ? argv[6] :"./lighting_fshader.glsl");
	
	std::shared_ptr<std::string> vSFR = std::shared_ptr<std::string>(new std::string(forwardRenderVSource));
	std::shared_ptr<std::string> fSFR = std::shared_ptr<std::string>(new std::string(forwardRenderFSource));
	std::shared_ptr<std::string> vSDR = std::shared_ptr<std::string>(new std::string(deferredRenderVSource));
	std::shared_ptr<std::string> fSDR = std::shared_ptr<std::string>(new std::string(deferredRenderFSource));
	std::shared_ptr<std::string> vSDLR = std::shared_ptr<std::string>(new std::string(deferredRenderLightingVSource));
	std::shared_ptr<std::string> fSDLR = std::shared_ptr<std::string>(new std::string(deferredRenderLightingFSource));
	
	engine = std::unique_ptr<ms::NGin>(new ms::NGinOGL(vSFR, fSFR, vSDR, fSDR, vSDLR, fSDLR, width, height, framebufferWidth, framebufferHeight, 0.01f, 100, 90, float(width)/height, 0));
		
	engine->load_model(useCommandLineArguments ? argv[7] : "./classroom/classroom.obj");

	engine->scene->set_directional_light(50, ms::math::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, ms::math::vec3{ -1.0f, -1.0f, -1.0f });

	engine->scene->pointLights.push_back(ms::PointLight(50, ms::math::vec4{1.0f, 1.0f, 1.0f, 1.0f}, ms::math::vec3{-1.4f, 4.0f, -9.0f}));
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

	engine = nullptr;	
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
	glfwTerminate();
	
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
	ms::ResourceCoordinator::destroy_shared_instance();
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
    return 0;
}
