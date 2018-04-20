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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	
    switch(key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_Z: {
            auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(0.1f);
            engine->scene.get_camera().get_transformation() = rotation * engine->scene.get_camera().get_transformation();
        }
            break;
        case GLFW_KEY_X: {
            auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(-0.1f);
            engine->scene.get_camera().get_transformation() = rotation * engine->scene.get_camera().get_transformation();
        }
            break;

        case GLFW_KEY_C: {
            auto rotation = ms::math::transform::rotate_about_x_radians<float, 4>(0.1f);
            engine->scene.get_camera().get_transformation() = rotation * engine->scene.get_camera().get_transformation();
        }
            break;
        case GLFW_KEY_V: {
            auto rotation = ms::math::transform::rotate_about_x_radians<float, 4>(-0.1f);
            engine->scene.get_camera().get_transformation() = rotation * engine->scene.get_camera().get_transformation();
        }
            break;

        case GLFW_KEY_Q: {
            auto up = 0.2f * ms::math::up(engine->scene.get_camera().get_transformation());
            auto transform = translate<float, 4>(up);
            engine->scene.get_camera().get_transformation() = engine->scene.get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_E: {
            auto up = -0.2f * ms::math::up(engine->scene.get_camera().get_transformation());
            auto transform = translate<float, 4>(up);
            engine->scene.get_camera().get_transformation() = engine->scene.get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_A: {
            auto right = 0.2f * ms::math::right(engine->scene.get_camera().get_transformation());
            auto transform = translate<float, 4>(right);
            engine->scene.get_camera().get_transformation() = engine->scene.get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_D: {
            auto right = -0.2f * ms::math::right(engine->scene.get_camera().get_transformation());
            auto transform = translate<float, 4>(right);
            engine->scene.get_camera().get_transformation() = engine->scene.get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_W: {
            auto forward = 0.2f * ms::math::back(engine->scene.get_camera().get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->scene.get_camera().get_transformation() = engine->scene.get_camera().get_transformation() * transform;
        }
            break;

        case GLFW_KEY_S: {
            auto forward = -0.2f * ms::math::back(engine->scene.get_camera().get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->scene.get_camera().get_transformation() = engine->scene.get_camera().get_transformation() * transform;
        }
            break;
        case GLFW_KEY_T: {
            auto forward = 0.2f * ms::math::back(engine->scene.get_spot_lights()[0].get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->scene.get_spot_lights()[0].get_transformation() *= transform;
        }
            break;
            
        case GLFW_KEY_G: {
            auto forward = -0.2f * ms::math::back(engine->scene.get_spot_lights()[0].get_transformation());
            auto transform = translate<float, 4>(forward);
            engine->scene.get_spot_lights()[0].get_transformation() *= transform;
        }
            break;
        case GLFW_KEY_R: {
            auto up = 0.2f * ms::math::up(engine->scene.get_spot_lights()[0].get_transformation());
            auto transform = translate<float, 4>(up);
            engine->scene.get_spot_lights()[0].get_transformation() *= transform;
        }
            break;
            
        case GLFW_KEY_Y: {
            auto up = -0.2f * ms::math::up(engine->scene.get_spot_lights()[0].get_transformation());
            auto transform = translate<float, 4>(up);
            engine->scene.get_spot_lights()[0].get_transformation() *= transform;
        }
            break;
        case GLFW_KEY_F: {
            auto right = 0.2f * ms::math::right(engine->scene.get_spot_lights()[0].get_transformation());
            auto transform = translate<float, 4>(right);
            engine->scene.get_spot_lights()[0].get_transformation() *= transform;
        }
            break;
            
        case GLFW_KEY_H: {
            auto right = -0.2f * ms::math::right(engine->scene.get_spot_lights()[0].get_transformation());
            auto transform = translate<float, 4>(right);
            engine->scene.get_spot_lights()[0].get_transformation() *= transform;
        }
            break;
        case GLFW_KEY_B: {
            auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(0.1f);
            engine->scene.get_spot_lights()[0].get_transformation() = rotation * engine->scene.get_spot_lights()[0].get_transformation();
        }
            break;
        case GLFW_KEY_N: {
            auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(-0.1f);
            engine->scene.get_spot_lights()[0].get_transformation() = rotation * engine->scene.get_spot_lights()[0].get_transformation();
        }
            break;
        case GLFW_KEY_8:
        engine->set_renderer(ms::NGin::Renderer::deferred);
        break;
        case GLFW_KEY_9:
        engine->set_renderer(ms::NGin::Renderer::forward_fragment);
        break;
        case GLFW_KEY_0:
        engine->set_renderer(ms::NGin::Renderer::forward_vertex);
        break;
        default:
            break;
    }
	
}
