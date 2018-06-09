//
//  controlls.cpp
//  ngin_mac
//
//  Created by Mateusz Stompór on 28/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "controlls.hpp"

using namespace ms;
using namespace math;
using namespace transform;

int usedSpotLight = 0;
int usedPointLight = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	
    switch(key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_Z: {
            auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(0.1f);
            engine->get_scene().get_camera().get_transformation() = rotation * engine->get_scene().get_camera().get_transformation();
        }
            break;
        case GLFW_KEY_X: {
            auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(-0.1f);
            engine->get_scene().get_camera().get_transformation() = rotation * engine->get_scene().get_camera().get_transformation();
        }
            break;

        case GLFW_KEY_C: {
            auto rotation = ms::math::transform::rotate_about_x_radians<float, 4>(0.1f);
            engine->get_scene().get_camera().get_transformation() = rotation * engine->get_scene().get_camera().get_transformation();
        }
            break;
        case GLFW_KEY_V: {
            auto rotation = ms::math::transform::rotate_about_x_radians<float, 4>(-0.1f);
            engine->get_scene().get_camera().get_transformation() = rotation * engine->get_scene().get_camera().get_transformation();
        }
            break;

        case GLFW_KEY_Q: {
            auto up = 0.2f * ms::math::up(engine->get_scene().get_camera().get_transformation());
            auto transform = translate<float, 4>(up);
            engine->get_scene().get_camera().get_transformation() = engine->get_scene().get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_E: {
            auto up = -0.2f * ms::math::up(engine->get_scene().get_camera().get_transformation());
            auto transform = translate<float, 4>(up);
            engine->get_scene().get_camera().get_transformation() = engine->get_scene().get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_A: {
            auto right = 0.2f * ms::math::right(engine->get_scene().get_camera().get_transformation());
            auto transform = translate<float, 4>(right);
            engine->get_scene().get_camera().get_transformation() = engine->get_scene().get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_D: {
            auto right = -0.2f * ms::math::right(engine->get_scene().get_camera().get_transformation());
            auto transform = translate<float, 4>(right);
            engine->get_scene().get_camera().get_transformation() = engine->get_scene().get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_W: {
            auto forward = 0.2f * ms::math::back(engine->get_scene().get_camera().get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->get_scene().get_camera().get_transformation() = engine->get_scene().get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_S: {
            auto forward = -0.2f * ms::math::back(engine->get_scene().get_camera().get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->get_scene().get_camera().get_transformation() = engine->get_scene().get_camera().get_transformation() * transform;
        }
            break;
        case GLFW_KEY_T: {
            auto forward = 0.2f * ms::math::back(engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() *= transform;
        }
            break;
            
        case GLFW_KEY_G: {
            auto forward = -0.2f * ms::math::back(engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() *= transform;
        }
            break;
        case GLFW_KEY_R: {
            auto up = 0.2f * ms::math::up(engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation());
            auto transform = translate<float, 4>(up);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() *= transform;
        }
            break;
            
        case GLFW_KEY_Y: {
            auto up = -0.2f * ms::math::up(engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation());
            auto transform = translate<float, 4>(up);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() *= transform;
        }
            break;
        case GLFW_KEY_F: {
            auto right = 0.2f * ms::math::right(engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation());
            auto transform = translate<float, 4>(right);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() *= transform;
        }
            break;
            
        case GLFW_KEY_H: {
            auto right = -0.2f * ms::math::right(engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation());
            auto transform = translate<float, 4>(right);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() *= transform;
        }
            break;
        case GLFW_KEY_B: {
            auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(0.1f);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() = rotation * engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation();
        }
            break;
        case GLFW_KEY_N: {
            auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(-0.1f);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() = rotation * engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation();
        }
            break;
        case GLFW_KEY_U: {
            auto rotation = ms::math::transform::rotate_about_x_radians<float, 4>(0.1f);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() = rotation * engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation();
        }
            break;
        case GLFW_KEY_J: {
            auto rotation = ms::math::transform::rotate_about_x_radians<float, 4>(-0.1f);
            engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation() = rotation * engine->get_scene().get_spot_lights()[usedSpotLight].get_transformation();
        }
        case GLFW_KEY_O: {
            auto up = -0.2f * ms::math::up(engine->get_scene().get_point_lights()[usedPointLight].get_transformation());
            auto transform = translate<float, 4>(up);
            engine->get_scene().get_point_lights()[usedPointLight].get_transformation() *= transform;
            break;
        }
        case GLFW_KEY_P: {
            auto up = 0.2f * ms::math::up(engine->get_scene().get_point_lights()[usedPointLight].get_transformation());
            auto transform = translate<float, 4>(up);
            engine->get_scene().get_point_lights()[usedPointLight].get_transformation() *= transform;
            break;
        }
        case GLFW_KEY_UP: {
            auto forward = 0.2f * ms::math::back(engine->get_scene().get_point_lights()[usedPointLight].get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->get_scene().get_point_lights()[usedPointLight].get_transformation() *= transform;
        }
        break;
        case GLFW_KEY_DOWN: {
            auto forward = -0.2f * ms::math::back(engine->get_scene().get_point_lights()[usedPointLight].get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->get_scene().get_point_lights()[usedPointLight].get_transformation() *= transform;
        }
            break;
        case GLFW_KEY_LEFT: {
            auto forward = 0.2f * ms::math::right(engine->get_scene().get_point_lights()[usedPointLight].get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->get_scene().get_point_lights()[usedPointLight].get_transformation() *= transform;
        }
            break;

        case GLFW_KEY_RIGHT: {
            auto forward = -0.2f * ms::math::right(engine->get_scene().get_point_lights()[usedPointLight].get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->get_scene().get_point_lights()[usedPointLight].get_transformation() *= transform;
        }
            break;
        case GLFW_KEY_1:
            usedSpotLight = std::clamp((usedSpotLight - 1), 0, static_cast<int>(engine->get_scene().get_spot_lights().size()));
            break;
        case GLFW_KEY_2:
            usedSpotLight = std::clamp((usedSpotLight + 1), 0, static_cast<int>(engine->get_scene().get_spot_lights().size()));
            break;
        case GLFW_KEY_3:
            usedPointLight = std::clamp((usedPointLight - 1), 0, static_cast<int>(engine->get_scene().get_point_lights().size()));
            break;
        case GLFW_KEY_4:
            usedPointLight = std::clamp((usedPointLight + 1), 0, static_cast<int>(engine->get_scene().get_point_lights().size()));
            break;
        default:
            break;
    }
	
}
