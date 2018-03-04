//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2017 Mateusz Stompór. All rights reserved.
//

#ifdef __WIN32__
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>
#include <cstring>
#include <string>
#include <memory>
#include <iostream>

#include "../../source_code/umbrellaHeader.hpp"
#include "../../source_code/scene/ogl/geometryOGL.hpp"
#include "../../source_code/scene/ogl/sceneNodeOGL.hpp"
#include "../../source_code/rendering/shaders/ogl/shaderOGL.hpp"
#include "../../source_code/rendering/ogl/deferredRenderOGL.hpp"
#include "Geometry.h"

namespace ms {
    const std::string libInitializationError        = "Cannot initialize GLFW3";
    const std::string windowCreationError           = "Cannot create window";
    const std::string windowName                    = "NGIN";
    const std::string contextInitializationFailure  = "Failed to initialize OpenGL context";
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch(key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        default:
            break;
    }
}

int main(int argc, const char * argv[]) {

    std::unique_ptr<ms::NGin> engine = std::unique_ptr<ms::NGin>(new ms::NGinOGL());
    
    int width = 1200;
    int height = 800;
	int framebufferWidth;
	int framebufferHeight;

    if(glfwInit()==0){
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

	std::shared_ptr<ms::Geometry> m = std::shared_ptr<ms::Geometry>(new ms::GeometryOGL());
	std::shared_ptr<ms::SceneNode> node = std::shared_ptr<ms::SceneNode>(new ms::SceneNodeOGL());
	

	auto vertexSource = ms::utils::load_contents_of_file("/Users/mateuszstompor/Documents/ngin/source_code/shaders/forward_render/vshader.glsl");
	auto fragmentSource = ms::utils::load_contents_of_file("/Users/mateuszstompor/Documents/ngin/source_code/shaders/forward_render/fshader.glsl");
	
	std::shared_ptr<std::string> vS = std::shared_ptr<std::string>(new std::string(vertexSource));
	std::shared_ptr<std::string> fS = std::shared_ptr<std::string>(new std::string(fragmentSource));
	
	std::shared_ptr<ms::Shader> shader = std::shared_ptr<ms::Shader>(new ms::ShaderOGL(vS, nullptr, nullptr, nullptr, fS));
	
	std::shared_ptr<ms::Render> renderer = std::shared_ptr<ms::Render>(new ms::DeferredRenderOGL(shader));
	
	engine->deferredRenderer = renderer;
	
	engine->sygnalize_loading_ability();
	
	m->vertices.insert(m->vertices.end(), &cube::vertices[0], &cube::vertices[108]);
	m->normals.insert(m->normals.end(), &cube::normals[0], &cube::normals[108]);
	m->load();
	
	node->geometry = m;
	
	engine->scene->nodes.push_back(node);
	
    glfwSetKeyCallback(window, key_callback);
    
    while(!glfwWindowShouldClose(window)){
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColorMask(0.0f, 0.0f, 0.0f, 1.0f);
		glfwPollEvents();
		
        engine->draw_scene();
        
        glfwSwapBuffers(window);
    }
    
    engine->unload();
    
    glfwTerminate();
    return 0;
}
