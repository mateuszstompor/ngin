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
			auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(0.1f);
			engine->scene->get_camera().pre_transform(rotation);
		}
			break;
		case GLFW_KEY_X: {
			auto rotation = ms::math::transform::rotate_about_y_radians<float, 4>(-0.1f);
			engine->scene->get_camera().pre_transform(rotation);
		}
			break;

		case GLFW_KEY_Q: {
			auto right = 0.2f * ms::math::up(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(right);
			engine->scene->get_camera().post_transform(transform);
		}
			break;

		case GLFW_KEY_E: {
			auto right = -0.2f * ms::math::up(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(right);
			engine->scene->get_camera().post_transform(transform);
		}
			break;

		case GLFW_KEY_A: {
			auto right = 0.2f * ms::math::right(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(right);
			engine->scene->get_camera().post_transform(transform);
		}
			break;

		case GLFW_KEY_D: {
			auto right = -0.2f * ms::math::right(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(right);
			engine->scene->get_camera().post_transform(transform);
		}
			break;

		case GLFW_KEY_W: {
			auto forward = 0.2f * ms::math::back(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->get_camera().post_transform(transform);
		}
			break;
			
		case GLFW_KEY_S: {
			auto forward = -0.2f * ms::math::back(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->get_camera().post_transform(transform);
		}
			break;
		case GLFW_KEY_J: {
			auto right = 1.6f * ms::math::right(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(right);
			engine->scene->pointLights[usedPointLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->pointLights[usedPointLightIndex]->position = get_position(engine->scene->pointLights[usedPointLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_L: {
			auto right = -1.6f * ms::math::right(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(right);
			engine->scene->pointLights[usedPointLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->pointLights[usedPointLightIndex]->position = get_position(engine->scene->pointLights[usedPointLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_K: {
			auto forward = 1.6f * ms::math::back(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->pointLights[usedPointLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->pointLights[usedPointLightIndex]->position = get_position(engine->scene->pointLights[usedPointLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_I: {
			auto forward = -1.6f * ms::math::back(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->pointLights[usedPointLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->pointLights[usedPointLightIndex]->position = get_position(engine->scene->pointLights[usedPointLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_U: {
			auto forward = 1.6f * ms::math::up(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->pointLights[usedPointLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->pointLights[usedPointLightIndex]->position = get_position(engine->scene->pointLights[usedPointLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_O: {
			auto forward = -1.6f * ms::math::up(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->pointLights[usedPointLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->pointLights[usedPointLightIndex]->position = get_position(engine->scene->pointLights[usedPointLightIndex]->modelTransformation.get_transformation());
		}
			break;
			
		case GLFW_KEY_F: {
			auto right = 1.6f * ms::math::right(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(right);
			engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->spotLights[usedSpotLightIndex]->position = get_position(engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_H: {
			auto right = -1.6f * ms::math::right(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(right);
			engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->spotLights[usedSpotLightIndex]->position = get_position(engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_G: {
			auto forward = 1.6f * ms::math::back(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->spotLights[usedSpotLightIndex]->position = get_position(engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_T: {
			auto forward = -1.6f * ms::math::back(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->spotLights[usedSpotLightIndex]->position = get_position(engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_R: {
			auto forward = 1.6f * ms::math::up(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->spotLights[usedSpotLightIndex]->position = get_position(engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_Y: {
			auto forward = -1.6f * ms::math::up(engine->scene->get_camera().get_transformation());
			auto transform = translate<float, 4>(forward);
			engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.post_transform(transform);
			engine->scene->spotLights[usedSpotLightIndex]->position = get_position(engine->scene->spotLights[usedSpotLightIndex]->modelTransformation.get_transformation());
		}
			break;
		case GLFW_KEY_8:
			engine->scene->pointLights[usedPointLightIndex]->color[0] += 0.1f;
			if(engine->scene->pointLights[usedPointLightIndex]->color[0] > 1.0f) {
				engine->scene->pointLights[usedPointLightIndex]->color[0] = 0.0f;
			}
			break;
		case GLFW_KEY_9:
			engine->scene->pointLights[usedPointLightIndex]->color[1] += 0.1f;
			if(engine->scene->pointLights[usedPointLightIndex]->color[1] > 1.0f) {
				engine->scene->pointLights[usedPointLightIndex]->color[1] = 0.0f;
			}
			break;
		case GLFW_KEY_0:
			engine->scene->pointLights[usedPointLightIndex]->color[2] += 0.1f;
			if(engine->scene->pointLights[usedPointLightIndex]->color[2] > 1.0f) {
				engine->scene->pointLights[usedPointLightIndex]->color[2] = 0.0f;
			}
			break;
		case GLFW_KEY_MINUS:
			if (usedPointLightIndex > 0) {
				usedPointLightIndex--;
			}
			break;
		case GLFW_KEY_EQUAL:
			if (usedPointLightIndex+1 < engine->scene->pointLights.size()) {
				usedPointLightIndex++;
			}
			break;
		case GLFW_KEY_LEFT_BRACKET:
			if (usedSpotLightIndex > 0) {
				usedSpotLightIndex--;
			}
			break;
		case GLFW_KEY_RIGHT_BRACKET:
			if (usedSpotLightIndex+1 < engine->scene->spotLights.size()) {
				usedSpotLightIndex++;
			}
			break;
        case GLFW_KEY_M:
        engine->set_renderer(ms::NGin::Renderer::deferred);
        break;
        case GLFW_KEY_N:
        engine->set_renderer(ms::NGin::Renderer::forward_fragment);
        break;
        case GLFW_KEY_B:
        engine->set_renderer(ms::NGin::Renderer::forward_vertex);
        break;
		default:
			break;
	}
	
}
