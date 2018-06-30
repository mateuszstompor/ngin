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
#include "tree.hpp"

#include <map>
#include <vector>

namespace ms {

    /**
     * Główny element biblioteki służący do reprezentowania sceny.
     * Na scenie w formie drzewa zorganizowane są rysowalne obiekty.
     * Bezpośrednio, bez użycia hierarchii, w wektorze przechowywane są światła punktowe oraz kierunkowe stożkowe.
     * Na scenie może występować tylko jedno światło kierunkowe, które umiejscowione jest bezpośrednio do współrzędnych świata.
     * Scena musi zawierać dokładnie jedną kamerę.
     * Dodatkowo scena pełni też rolę kontenera na wszystkie załadowane siatki, materiały oraz tesktury i odpowiada za ich dealokację.
     * @see NGin
     * @see Drawable
     * @see SpotLight
     * @see PointLight
     * @see DirectionalLight
     */
    class Scene {

        using materials_type    = std::map<std::string, std::shared_ptr<Material>>;
        using textures_type     = std::map<std::string, std::shared_ptr<Texture2D>>;
        using geometries_vec    = std::vector<std::shared_ptr<Geometry>>;
        using spot_lights_type  = std::vector<SpotLight>;
        using point_lights_type = std::vector<PointLight>;
        using drawable_type     = ms::tree<std::shared_ptr<Drawable>>;
        
	public:
        /**
         * Tworzy scene, dodając na nią kamerę i umiejscawiajac ją w początku układu współrzędnych.
         */
                                                Scene (std::unique_ptr<Camera> && cam);
        /**
         * @return Zwraca kamerę umiejscowiona na scenie.
         */
        Camera const &                          get_camera() const;
        /**
         * @return Zwraca kamerę umiejscowiona na scenie.
         */
		Camera &                                get_camera();
        /**
         * @return Zwraca załadowane do sceny materiały.
         * Użytkownik może użyć tej metody w celu dodania nowego zasobu do sceny.
         */
        constexpr materials_type &              get_materials() { return materials; }
        /**
         * @return Zwraca załadowane do sceny siatki.
         * Użytkownik może użyć tej metody w celu dodania nowego zasobu do sceny.
         */
        constexpr geometries_vec &              get_geometries() { return geometries; }
        /**
         * @return Zwraca załadowane do sceny tekstury.
         * Użytkownik może użyć tej metody w celu dodania nowego zasobu do sceny.
         */
        constexpr textures_type &               get_textures() { return textures; }
        /**
         * @return Zwraca wszystkie światła kierunkowe, stożkowe w scenie.
         * Użytkownik może użyć tej metody w celu dodania nowego zasobu do sceny.
         */
        constexpr spot_lights_type &            get_spot_lights() { return spotLights; }
        /**
         * @return Zwraca wszystkie światła kierunkowe, stożkowe w scenie.
         */
        constexpr spot_lights_type const &      get_spot_lights() const { return spotLights; }
        /**
         * @return Zwraca wszystkie światła punktowe umiejscowione na scenie.
         * Użytkownik może użyć tej metody w celu dodania nowego zasobu do sceny.
         */
        constexpr point_lights_type &           get_point_lights() { return pointLights; }
        /**
         * @return Zwraca wszystkie światła punktowe umiejscowione na scenie.
         */
        constexpr point_lights_type const &     get_point_lights() const { return pointLights; }
        /**
         * Zwraca drzewo rysowalnych obiektów na scenie.
         * Użytkownik może użyć tej metody w celu dodania nowego zasobu do sceny.
         */
        constexpr drawable_type &               get_nodes() { return nodes; }
        /**
         * Zwraca drzewo rysowalnych obiektów na scenie.
         */
        constexpr drawable_type const &         get_nodes() const { return nodes; }
        /**
         * @return Zwraca światło kierunkowe umiejscowione na scenie.
         */
        DirectionalLight const *                get_directional_light() const;
        /**
         * @return Zwraca światło kierunkowe umiejscowione na scenie.
         */
        DirectionalLight *                      get_directional_light();
        /**
         * Dodaje na scenę światło kierunkowe.
         */
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

