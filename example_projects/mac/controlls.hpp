//
//  controlls.hpp
//  ngin_mac
//
//  Created by Mateusz Stompór on 28/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef controlls_hpp
#define controlls_hpp

#include <GLFW/glfw3.h>
#include "../../source_code/umbrellaHeader.hpp"

extern std::unique_ptr<ms::NGin> engine;
extern unsigned int usedPointLightIndex;
extern unsigned int usedSpotLightIndex;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif /* controlls_hpp */
