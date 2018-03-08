//
//  scene.hpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef scene_hpp
#define scene_hpp

#include "camera/camera.hpp"
#include "camera/perspectiveCamera.hpp"
#include "lights/directionalLight.hpp"
#include "lights/pointLight.hpp"
#include "lights/spotLight.hpp"
#include "sceneNode.hpp"
#include "geometry.hpp"

#include <vector>

namespace ms {
    
    class Scene {

	public:
		
		Scene (float nearPlan, float farPlan, float fovDegress, float cameraAspect);

		std::vector<std::shared_ptr<SceneNode>> 	nodes;
		Camera &									get_camera();
		DirectionalLight *							get_directional_light();
		void										set_directional_light(float power, math::vec4 color, math::vec3 direction);
		
	protected:
		
		std::unique_ptr<Camera> 					cam;
		std::unique_ptr<DirectionalLight>			directionalLight;
		
    };
    
}



#endif /* scene_hpp */
