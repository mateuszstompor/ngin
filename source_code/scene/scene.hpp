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
#include "texture.hpp"
#include "drawable.hpp"
#include "geometry.hpp"

#include <map>
#include <vector>

namespace ms {
    
    class Scene {

	public:
		
															Scene (float nearPlan,
																   float farPlan,
																   float fovDegress,
																   float cameraAspect);

		Camera &											get_camera() const;
		DirectionalLight *									get_directional_light() const;
		const std::vector<std::shared_ptr<SpotLight>> &		get_spot_lights() const;
		const std::vector<std::shared_ptr<PointLight>> &	get_point_lights() const;
		const std::vector<std::shared_ptr<Drawable>> & 		get_nodes() const;
		void												set_directional_light(float 		power,
																				  math::vec3 	color,
																				  math::vec3 	direction);
//TODO make it protected
//		protected:
		std::vector<std::shared_ptr<Drawable>> 				nodes;
		std::map<std::string, std::shared_ptr<Material>>	materials;
		std::map<std::string, std::shared_ptr<Texture>>		textures;
		std::vector<std::shared_ptr<PointLight>>			pointLights;
		std::vector<std::shared_ptr<SpotLight>>				spotLights;
		std::unique_ptr<Camera> 							cam;
		std::unique_ptr<DirectionalLight>					directionalLight;
		
    };
    
}



#endif /* scene_hpp */
