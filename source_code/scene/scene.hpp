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
#include "camera/orthographicCamera.hpp"
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
		
        using materials_type    = std::map<std::string, std::shared_ptr<Material>>;
        using textures_type     = std::map<std::string, std::shared_ptr<Texture>>;
        using spot_lights_type  = std::vector<std::shared_ptr<SpotLight>>;
        using point_lights_type = std::vector<std::shared_ptr<PointLight>>;
        using drawable_type     = std::vector<std::shared_ptr<Drawable>>;
        
                                                                    Scene ();
        
															        Scene (float nearPlan,
                                                                           float farPlan,
                                                                           float fovDegress,
                                                                           float cameraAspect);
        
                                                                    Scene (std::unique_ptr<Camera> && cam);

        constexpr Camera const &							        get_camera() const { return *cam; }
        constexpr Camera &                                          get_camera() { return *cam; }
        constexpr DirectionalLight *                                get_directional_light() { return directionalLight.get(); }
        constexpr DirectionalLight const *                          get_directional_light() const { return directionalLight.get(); }
        constexpr materials_type &                                  get_materials() { return materials; }
        constexpr textures_type &                                   get_textures() { return textures; }
        
        constexpr spot_lights_type &                                get_spot_lights() { return spotLights; }
        constexpr const spot_lights_type &                          get_spot_lights() const { return spotLights; }
        constexpr point_lights_type &                               get_point_lights() { return pointLights; }
        constexpr const point_lights_type &                         get_point_lights() const { return pointLights; }
        constexpr drawable_type &                                   get_nodes() { return nodes; }
        constexpr const drawable_type & 	                        get_nodes() const { return nodes; }
        
        void        												set_directional_light(float 		power,
                                                                                          math::vec3 	color,
                                                                                          math::vec3 	direction);
		
    protected:
        
        std::map<std::string, std::shared_ptr<Material>>    materials;
        std::map<std::string, std::shared_ptr<Texture>>     textures;
        std::vector<std::shared_ptr<Drawable>>              nodes;
        std::vector<std::shared_ptr<PointLight>>            pointLights;
        std::vector<std::shared_ptr<SpotLight>>             spotLights;
		std::unique_ptr<Camera> 							cam;
		std::unique_ptr<DirectionalLight>					directionalLight;
		
    };
    
}



#endif /* scene_hpp */
