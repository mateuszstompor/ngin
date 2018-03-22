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

unsigned int usedIndex = 0;
unsigned int spotLightIndex = 0;

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
		
		case GLFW_KEY_Z: {
			auto a = ms::math::transform::rotateAboutYRadians<float, 4>(0.1f);
			engine->scene->get_camera().set_transformation(engine->scene->get_camera().get_transformation() * a);
		}
		break;
		case GLFW_KEY_X: {
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
		case GLFW_KEY_J: {
			auto a = ms::math::vec3{0.0f, -0.1f, 0.0f};
			engine->scene->pointLights[usedIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.0f, -0.1f, 0.0f});
			engine->scene->pointLights[usedIndex]->modelTransformation.set_transformation(b * engine->scene->pointLights[usedIndex]->modelTransformation.get_transformation());
			
		}
			break;
		case GLFW_KEY_L: {
			auto a = ms::math::vec3{0.0f, 0.1f, 0.0f};
			engine->scene->pointLights[usedIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.0f, 0.1f, 0.0f});
			engine->scene->pointLights[usedIndex]->modelTransformation.set_transformation(b * engine->scene->pointLights[usedIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_K: {
			auto a = ms::math::vec3{-0.1f, 0.0f, 0.0f};
			engine->scene->pointLights[usedIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({-0.1f, 0.0f, 0.0f});
			engine->scene->pointLights[usedIndex]->modelTransformation.set_transformation(b * engine->scene->pointLights[usedIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_I: {
			auto a = ms::math::vec3{0.1f, 0.0f, 0.0f};
			engine->scene->pointLights[usedIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.1f, 0.0f, 0.0f});
			engine->scene->pointLights[usedIndex]->modelTransformation.set_transformation(b * engine->scene->pointLights[usedIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_U: {
			auto a = ms::math::vec3{0.0f, 0.0f, 0.1f};
			engine->scene->pointLights[usedIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.0f, 0.0f, 0.1f});
			engine->scene->pointLights[usedIndex]->modelTransformation.set_transformation(b * engine->scene->pointLights[usedIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_O: {
			auto a = ms::math::vec3{0.0f, 0.0f, -0.1f};
			engine->scene->pointLights[usedIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.0f, 0.0f, -0.1f});
			engine->scene->pointLights[usedIndex]->modelTransformation.set_transformation(b * engine->scene->pointLights[usedIndex]->modelTransformation.get_transformation());
		}
			break;
			
		case GLFW_KEY_F: {
			auto a = ms::math::vec3{0.0f, -0.1f, 0.0f};
			engine->scene->spotLights[spotLightIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.0, -0.1f, 0.0});
			engine->scene->spotLights[spotLightIndex]->modelTransformation.set_transformation(b * engine->scene->spotLights[spotLightIndex]->modelTransformation.get_transformation());

		}
			break;
		case GLFW_KEY_H: {
			auto a = ms::math::vec3{0.0f, 0.1f, 0.0f};
			engine->scene->spotLights[spotLightIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.0f, 0.1f, 0.0f});
			engine->scene->spotLights[spotLightIndex]->modelTransformation.set_transformation(b * engine->scene->spotLights[spotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_G: {
			auto a = ms::math::vec3{-0.1f, 0.0f, 0.0f};
			engine->scene->spotLights[spotLightIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({-0.1f, 0.0f, 0.0f});
			engine->scene->spotLights[spotLightIndex]->modelTransformation.set_transformation(b * engine->scene->spotLights[spotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_T: {
			auto a = ms::math::vec3{0.1f, 0.0f, 0.0f};
			engine->scene->spotLights[spotLightIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.1f, 0.0f, 0.0f});
			engine->scene->spotLights[spotLightIndex]->modelTransformation.set_transformation(b * engine->scene->spotLights[spotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_R: {
			auto a = ms::math::vec3{0.0f, 0.0f, 0.1f};
			engine->scene->spotLights[spotLightIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.0f, 0.0f, 0.f});
			engine->scene->spotLights[spotLightIndex]->modelTransformation.set_transformation(b * engine->scene->spotLights[spotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_Y: {
			auto a = ms::math::vec3{0.0f, 0.0f, -0.1f};
			engine->scene->spotLights[spotLightIndex]->position += a;
			auto b = ms::math::transform::translate<float, 4>({0.0f, 0.0f, -0.1f});
			engine->scene->spotLights[spotLightIndex]->modelTransformation.set_transformation(b * engine->scene->spotLights[spotLightIndex]->modelTransformation.get_transformation());
		}
			break;
			
		case GLFW_KEY_8:
			engine->scene->pointLights[usedIndex]->color[0] += 0.1f;
			if(engine->scene->pointLights[usedIndex]->color[0] > 1.0f) {
				engine->scene->pointLights[usedIndex]->color[0] = 0.0f;
			}
			break;
		case GLFW_KEY_9:
			engine->scene->pointLights[usedIndex]->color[1] += 0.1f;
			if(engine->scene->pointLights[usedIndex]->color[1] > 1.0f) {
				engine->scene->pointLights[usedIndex]->color[1] = 0.0f;
			}
			break;
		case GLFW_KEY_0:
			engine->scene->pointLights[usedIndex]->color[2] += 0.1f;
			if(engine->scene->pointLights[usedIndex]->color[2] > 1.0f) {
				engine->scene->pointLights[usedIndex]->color[2] = 0.0f;
			}
			break;
		case GLFW_KEY_MINUS:
			if (usedIndex > 0) {
				usedIndex--;
			}
			break;
		case GLFW_KEY_EQUAL:
			if (usedIndex+1 < engine->scene->pointLights.size()) {
				usedIndex++;
			}
			break;
		case GLFW_KEY_LEFT_BRACKET:
			if (spotLightIndex > 0) {
				spotLightIndex--;
			}
			break;
		case GLFW_KEY_RIGHT_BRACKET:
			if (spotLightIndex+1 < engine->scene->spotLights.size()) {
				spotLightIndex++;
			}
			break;
			
        default:
            break;
    }
	
}

int main(int argc, const char * argv[]) { {

	
	// pass path to scene with building as first param
	// pass path to nanosuit as second param
	// pass path to cone as third param
	// pass path to sphere as fourth param
	
	bool useCommandLineArguments = argc == 5;

	int width = 1500;
	int height = 1000;
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
	glfwSwapInterval(1);
	
	GLFWwindow *window = glfwCreateWindow(width, height, ms::windowName.c_str(), nullptr, nullptr);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwGetFramebufferSize(window, &width, &height);
	
	framebufferWidth = width/4;
	framebufferHeight = height/4;
	
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
	
	engine = std::unique_ptr<ms::NGin>(new ms::NGinOGL(width, height, framebufferWidth, framebufferHeight, 0.01f, 100, 90, float(framebufferWidth)/framebufferHeight, 0));
		
	engine->load_model(useCommandLineArguments ? argv[1] : "./sponza/sponza.obj");

	engine->scene->set_directional_light(50, ms::math::vec3{ 1.0f, 1.0f, 1.0f}, ms::math::vec3{ 1.0f, -1.0f, -1.0f });
	
	auto scale = ms::math::transform::scale<float, 4> ({0.05f, 0.05f, 0.05f});
	
	for (int i = 0; i < 15; ++i) {
		auto translation = ms::math::transform::translate<float, 4>({-6 + (i * 1.0f), 1.0f, 0.0f});
		auto result = translation * ms::math::vec4{0.0f, -1.0f, 0.0f, 1.0f};
		auto res = ms::math::vec3{result.x(), result.y(), result.z()};
		
		engine->load_point_light(50, ms::math::vec3{1.0f, 1.0f, 0.0f},
								 res, useCommandLineArguments ? argv[4] : "./sphere/sphere.obj");
		engine->scene->pointLights[i]->modelTransformation.set_transformation(translation * scale * engine->scene->pointLights[i]->modelTransformation.get_transformation());
	}
	
	for (int i = 0; i < 3; ++i) {
		auto translation = ms::math::transform::translate<float, 4>({-4.0f + (i * 2.0f), 3.0f, 0.0f});
		auto result = translation * ms::math::vec4{0.0f, -1.0f, 0.0f, 1.0f};
		auto res = ms::math::vec3{result.x(), result.y(), result.z()};
	
		engine->load_spot_light(50, ms::math::vec3{0.0f, 1.0f, 1.0f}, res,
								50, ms::math::vec3{0.0f, -1.0f, 0.0f},  useCommandLineArguments ? argv[3] : "./cone/cone.obj");
		engine->scene->spotLights[i]->modelTransformation.set_transformation(translation * scale * engine->scene->spotLights[i]->modelTransformation.get_transformation());
	}
	
	for(int a = 0; a < engine->scene->get_nodes().size(); ++a) {
		engine->scene->get_nodes()[a]->modelTransformation.set_transformation(ms::math::transform::scale<float, 4>({0.02f, 0.02f, 0.02f}) * engine->scene->get_nodes()[a]->modelTransformation.get_transformation());
	}
	
	
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
