//
//  Created by Mateusz Stompór on 25/01/2018.
//  Copyright © 2017 Mateusz Stompór. All rights reserved.
//

#include <iostream>
#include <memory>

#ifdef __WIN32__
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <cstring>
#include <string>
#include <memory>

#include "../../source_code/base/ngin.hpp"
#include "../../source_code/base/ogl/nginOGL.hpp"

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

    engine->initialize();
    
    glfwSetKeyCallback(window, key_callback);
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        
        engine->draw_scene();
        
        glfwSwapBuffers(window);
    }
    
    engine->destroy();
    
    glfwTerminate();
    return 0;
}
