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
#include "lights/directionalLight.hpp"
#include "lights/pointLight.hpp"
#include "lights/spotLight.hpp"
#include "sceneNode.hpp"
#include "geometry.hpp"

#include <vector>

namespace ms {
    
    class Scene {

	public:
		
		Scene 	();
		
		std::vector<std::shared_ptr<SceneNode>> nodes;
		std::unique_ptr<Camera> cam;
		
    };
    
}

#endif /* scene_hpp */
