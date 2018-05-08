//
//  scene.hpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "camera/camera.hpp"
#include "camera/perspectiveCamera.hpp"
#include "camera/orthographicCamera.hpp"
#include "lights/directionalLight.hpp"
#include "lights/pointLight.hpp"
#include "lights/spotLight.hpp"
#include "texture.hpp"
#include "drawable.hpp"
#include "geometry.hpp"
#include "../../third-party-libs/aTree/aTree.hpp"

#include <map>
#include <vector>

namespace ms {
    
    class Scene {

        using materials_type    = std::map<std::string, std::shared_ptr<Material>>;
        using textures_type     = std::map<std::string, std::shared_ptr<Texture2D>>;
        using geometries_vec    = std::vector<std::shared_ptr<Geometry>>;
        using spot_lights_type  = std::vector<SpotLight>;
        using point_lights_type = std::vector<PointLight>;
        using drawable_type     = ms::aTree<std::shared_ptr<Drawable>>;
        
	public:
        
                                                Scene (std::unique_ptr<Camera> && cam);
        Camera const &                          get_camera() const;
		Camera &                                get_camera();
		DirectionalLight const *                get_directional_light() const;
		DirectionalLight *                      get_directional_light();
        
        constexpr materials_type &              get_materials() { return materials; }
        
        constexpr geometries_vec &              get_geometries() { return geometries; }
        
        constexpr textures_type &               get_textures() { return textures; }
        
        constexpr spot_lights_type &            get_spot_lights() { return spotLights; }
        constexpr spot_lights_type const &      get_spot_lights() const { return spotLights; }
        
        constexpr point_lights_type &           get_point_lights() { return pointLights; }
        constexpr point_lights_type const &     get_point_lights() const { return pointLights; }
        
        constexpr drawable_type &               get_nodes() { return nodes; }
        constexpr drawable_type const &         get_nodes() const { return nodes; }
        void        							set_directional_light(std::unique_ptr<DirectionalLight> && dl) { directionalLight = std::move(dl); }
		
    private:
        
        //resources to store
        materials_type                          materials{};
        textures_type                           textures{};
        geometries_vec                          geometries{};
        
        //nodes
        drawable_type                           nodes{};
        
        std::unique_ptr<Camera>                 cam{};
        
        //lights
        std::vector<PointLight>                 pointLights{};
        std::vector<SpotLight>                  spotLights{};
        std::unique_ptr<DirectionalLight>       directionalLight{};
		
    };
    
}

