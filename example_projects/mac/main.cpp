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
#include <initializer_list>
#include <cassert>

#include "../../source_code/umbrellaHeader.hpp"
#include "assimp/scene.h"

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
			auto a = ms::math::transform::rotateAboutYRadians<float, 4>(0.1);
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
		break;
		case GLFW_KEY_Y: {
			auto a = ms::math::transform::rotateAboutYRadians<float, 4>(-0.1);
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
		break;
		
		case GLFW_KEY_Q: {
			auto a = ms::math::transform::translate<float, 4>({0, 0.0, 0.1});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_E: {
			auto a = ms::math::transform::translate<float, 4>({0, 0.0, -0.1});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_A: {
			auto a = ms::math::transform::translate<float, 4>({0.1, 0.0, 0.0});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_D: {
			auto a = ms::math::transform::translate<float, 4>({-0.1, 0.0, 0.0});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_W: {
			auto a = ms::math::transform::translate<float, 4>({0.0, 0.1, 0.0});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_S: {
			auto a = ms::math::transform::translate<float, 4>({0.0, -0.1, 0.0});
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_J: {
			auto a = ms::math::transform::translate<float, 4>({0.0, -0.1, 0.0});
			engine->scene->pointLights[0].set_transformation(engine->scene->pointLights[0].get_transformation() * a);
		}
			break;
		case GLFW_KEY_L: {
			auto a = ms::math::transform::translate<float, 4>({0.0, 0.1, 0.0});
			engine->scene->pointLights[0].set_transformation(engine->scene->pointLights[0].get_transformation() * a);
		}
			break;
		case GLFW_KEY_K: {
			auto a = ms::math::transform::translate<float, 4>({-0.1, 0.0, 0.0});
			engine->scene->pointLights[0].set_transformation(engine->scene->pointLights[0].get_transformation() * a);
		}
			break;
		case GLFW_KEY_I: {
			auto a = ms::math::transform::translate<float, 4>({0.1, 0.0, 0.0});
			engine->scene->pointLights[0].set_transformation(engine->scene->pointLights[0].get_transformation() * a);
		}
			break;
		case GLFW_KEY_U: {
			auto a = ms::math::transform::translate<float, 4>({0.0, 0.0, -0.1});
			engine->scene->pointLights[0].set_transformation(engine->scene->pointLights[0].get_transformation() * a);
		}
			break;
		case GLFW_KEY_O: {
			auto a = ms::math::transform::translate<float, 4>({0.0, 0.0, 0.1});
			engine->scene->pointLights[0].set_transformation(engine->scene->pointLights[0].get_transformation() * a);
		}
			break;
			
        default:
            break;
    }
}

int main(int argc, const char * argv[]) { {
	
	if (argc < 7) {
		std::cerr << "Pass vertex and fragment shader source path for forward, deferred render and its lighting as parameter" << std::endl;
		exit(1);
	}
	
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
	
	auto forwardRenderVSource = ms::utils::load_contents_of_file	(argv[1]);
	auto forwardRenderFSource = ms::utils::load_contents_of_file	(argv[2]);
	auto deferredRenderVSource = ms::utils::load_contents_of_file	(argv[3]);
	auto deferredRenderFSource = ms::utils::load_contents_of_file	(argv[4]);
	auto deferredRenderLightingVSource = ms::utils::load_contents_of_file	(argv[5]);
	auto deferredRenderLightingFSource = ms::utils::load_contents_of_file	(argv[6]);
	
	std::shared_ptr<std::string> vSFR = std::shared_ptr<std::string>(new std::string(forwardRenderVSource));
	std::shared_ptr<std::string> fSFR = std::shared_ptr<std::string>(new std::string(forwardRenderFSource));
	std::shared_ptr<std::string> vSDR = std::shared_ptr<std::string>(new std::string(deferredRenderVSource));
	std::shared_ptr<std::string> fSDR = std::shared_ptr<std::string>(new std::string(deferredRenderFSource));
	std::shared_ptr<std::string> vSDLR = std::shared_ptr<std::string>(new std::string(deferredRenderLightingVSource));
	std::shared_ptr<std::string> fSDLR = std::shared_ptr<std::string>(new std::string(deferredRenderLightingFSource));
	
	
	
	engine = std::unique_ptr<ms::NGin>(new ms::NGinOGL(vSFR, fSFR, vSDR, fSDR, vSDLR, fSDLR, width, height, framebufferWidth, framebufferHeight, 0.01, 100, 90, float(width)/height, 0));
	
	std::unique_ptr<ms::Loader> loader = std::unique_ptr<ms::Loader>(new ms::LoaderOGL());
	
	for (auto a : loader->load_model("/Users/mateuszstompor/Desktop/classroom.obj")) {
		engine->scene->nodes.push_back(a);
	}
	
	engine->scene->set_directional_light(50, ms::math::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, ms::math::vec3{ -1.0f, -1.0f, -1.0f });
	
	engine->scene->pointLights.push_back(ms::PointLight(50, ms::math::vec4{1.0f, 1.0f, 1.0f, 1.0f}, ms::math::mat4::identity() * ms::math::transform::translate<float, 4>({-1.4, 4, -9})));
	
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
