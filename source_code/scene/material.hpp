//
//  material.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <string>
#include <memory>
#include "gsl/span"

#include "texture.hpp"
#include "../resources/resource.hpp"

namespace ms {

	/**
	 * Jego celem jest nadanie wyglądu rysowalnemu obiektowi.
	 * Kolor może być specyfikowany zarówno przy wykorzystaniu wektora koloru, jak i tekstury.
	 * Materiały są elementem sceny.
	 * @see Scene
	 * @see Drawable
	 * @see Texture
	 */
	class Material : public Resource {
        
        friend class ForwardRender;
        friend class DeferredRender;
        
	public:
		
        
		 						        Material                (math::vec3 const & ambient,
                                                                 math::vec3 const & diffuse,
                                                                 math::vec3 const & specular,
                                                                 float shininess,
                                                                 float opacity,
                                                                 std::string const & name);

		/**
         * @return wektor koloru dla komponentu ambient.
         */
		constexpr math::vec3 const &    get_ambient_color       () const { return ambientColor; }
		/**
		 * Ustawia parametr ambient materiału.
		 */
        void                            set_ambient_color       (math::vec3 const & ambient);
		/**
		 * @return wektor koloru dla komponentu diffuse.
		 */
        constexpr math::vec3 const &    get_diffuse_color       () const { return diffuseColor; }
		/**
		 * Ustawia parametr diffuse materiału.
		 */
        void                            set_diffuse_color       (math::vec3 const & diffuse);
		/**
		 * @return wektor koloru dla komponentu specular.
		 */
        constexpr math::vec3 const &    get_specular_color      () const { return specularColor; }
		/**
		 * Ustawia parametr specular materiału.
		 * @param specular
		 */
        void                            set_specular_color      (math::vec3 const & specular);
		/**
		 * Oczekiwana wartość powinna leżeć w przedziale [0.0, 100.0].
		 * @return float Zwraca moc z jaką materiał odbłyskuje światło.
		 */
        constexpr float                 get_shininess           () const { return shininess; }
		/**
		 * Ustawia odbłyskliwość materiału.
		 * @param shininess wartość powinna leżeć w przedziale [0.0, 100.0].
		 */
        void                            set_shininess           (float shininess);
		/**
		 * @return Zwraca nieprzezroczystość materaiłu.
		 */
        constexpr float                 get_opacity             () const { return opacity; }
		/**
		 * Ustawia nieprzezroczystosć.
		 * @param opacity wartość powinna leżeć w przedziale [0.0, 1.0].
		 */
        void                            set_opacity             (float opacity);
		/**
		 * @return Zwraca nazwę materiału.
		 */
        std::string const &             get_name                () const { return name; }
		/**
		 * Ustawia nazwę materaiłu.
		 */
        void                            set_name                (std::string const & name);
		/**
		 * @return sprawdza, czy materiał choć w najmniejszym stopniu przepuszcza światło.
		 */
        bool                            is_translucent          () const;
		/**
		 * Ustawia, czy materiał powinien być cieniowany.
		 */
        void                            set_shading             (bool shouldBeShaded);
		/**
		 * @return zwraca, czy materiał powinien zostać poddany procesowi cieniowania.
		 */
        constexpr bool                  is_shaded               () const { return isShaded; }
        std::string                     get_class               () const override;
		/**
		 * Dowiązuje teksture diffuse do materiału, jeśli tektura będzie dowiążana będzie to źródło z którego czerpany będzie kolor.
		 */
        void                            bind_diffuse_texture    (std::shared_ptr<Texture2D> diffuse);
		/**
		 * Dowiązuje teksture specular do materiału, jeśli tektura będzie dowiążana będzie to źródło z którego czerpany będzie kolor.
		 */
        void                            bind_specular_texture   (std::shared_ptr<Texture2D> specular);
		/**
		 * Dowiązuje normal-mapę do materiału.
		 */
        void                            bind_normal_texture     (std::shared_ptr<Texture2D> normal);
		/**
		 * Dowiązuje mapę głębi do materiału.
		 * W obecnym stadium funkcjonalność korzystająca z metody nie jest zaimplementowana.
		 */
        void                            bind_height_texture     (std::shared_ptr<Texture2D> height);
        Texture2D *                     get_diffuse_texture     ();
        Texture2D *                     get_specular_texture    ();
        Texture2D *                     get_normal_texture      ();
        Texture2D *                     get_height_texture      ();
        
		std::vector <std::string> 	    diffuseTexturesNames;
		std::vector <std::string> 	    specularTexturesNames;
		std::vector <std::string> 	    normalTexturesNames;
		std::vector <std::string> 	    heightTexturesNames;

    private:
										Material                (Material const &) = delete;
		Material &                      operator =          	(Material const &) = delete;
        void                            use                        ();
        virtual void                    _load                   () override;
        virtual void                    _unload                 () override;
        
        bool                            invalidated;
        GLuint                          bufferId;
        math::vec3                      diffuseColor;
        math::vec3                      ambientColor;
        math::vec3                      specularColor;
        std::string                     name;
        float                           shininess;
        float                           opacity;
        bool                            isShaded;
        std::shared_ptr<Texture2D>      boundedDiffuseTexture;
        std::shared_ptr<Texture2D>      boundedSpecularTexture;
        std::shared_ptr<Texture2D>      boundedNormalTexture;
        std::shared_ptr<Texture2D>      boundedHeightTexture;
	};
	
}
