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
#include "../../source_code/scene/ogl/geometryOGL.hpp"
#include "../../source_code/scene/ogl/sceneNodeOGL.hpp"
#include "example_geometry.h"

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
			
		case GLFW_KEY_Q: {
			auto a = ms::math::transform::translate<float, 4>({0, 0.0, 0.1});
			engine->scene->get_camera()->set_transformation(engine->scene->get_camera()->get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_E: {
			auto a = ms::math::transform::translate<float, 4>({0, 0.0, -0.1});
			engine->scene->get_camera()->set_transformation(engine->scene->get_camera()->get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_A: {
			auto a = ms::math::transform::translate<float, 4>({0.1, 0.0, 0.0});
			engine->scene->get_camera()->set_transformation(engine->scene->get_camera()->get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_D: {
			auto a = ms::math::transform::translate<float, 4>({-0.1, 0.0, 0.0});
			engine->scene->get_camera()->set_transformation(engine->scene->get_camera()->get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_W: {
			auto a = ms::math::transform::translate<float, 4>({0.0, 0.1, 0.0});
			engine->scene->get_camera()->set_transformation(engine->scene->get_camera()->get_transformation() * a);
		}
			break;
			
		case GLFW_KEY_S: {
			auto a = ms::math::transform::translate<float, 4>({0.0, -0.1, 0.0});
			engine->scene->get_camera()->set_transformation(engine->scene->get_camera()->get_transformation() * a);
		}
			break;
			
        default:
            break;
    }
}

int main(int argc, const char * argv[]) {

    int width = 1200;
    int height = 800;
	int framebufferWidth = 1200;
	int framebufferHeight = 800;
	
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
	
	
	std::shared_ptr<ms::Geometry> m = std::shared_ptr<ms::Geometry>(new ms::GeometryOGL());
	std::shared_ptr<ms::SceneNode> node = std::shared_ptr<ms::SceneNode>(new ms::SceneNodeOGL());
	

	auto vertexSource = ms::utils::load_contents_of_file	("./vshader.glsl");
	auto fragmentSource = ms::utils::load_contents_of_file	("./fshader.glsl");
	
	std::shared_ptr<std::string> vS = std::shared_ptr<std::string>(new std::string(vertexSource));
	std::shared_ptr<std::string> fS = std::shared_ptr<std::string>(new std::string(fragmentSource));
	
	engine = std::unique_ptr<ms::NGin>(new ms::NGinOGL(vS, fS, 1200, 800, 1200, 800, 0.01, 100, 90, 1200.0/800.0));
	
	m->vertices.insert(m->vertices.end(), &cube::vertices[0], &cube::vertices[108]);
	m->normals.insert(m->normals.end(), &cube::normals[0], &cube::normals[108]);

	node->geometry = m;
	
	engine->scene->set_directional_light(50, ms::math::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, ms::math::vec3{ -1.0f, -1.0f, -1.0f });
	
	engine->scene->nodes.push_back(node);
	
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
    return 0;
}
