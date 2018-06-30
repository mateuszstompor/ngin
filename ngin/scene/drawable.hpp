//
//  drawable.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "../resources/resource.hpp"
#include "geometry.hpp"
#include "material.hpp"
#include "../utils/proxyOGL.hpp"
#include "../rendering/reusableGeometry.hpp"

namespace ms {

	/**
	 * Rysowalny obiekt, który możliwy jest do wyświetlenia na ekranie.
	 * Powiązuje siatkę geometrii, materiał oraz macierz transformacji.
	 * Obiekty tej klasy zorganizowane są na scenie.
	 * @see Scene
	 * @see Material
	 * @see Geometry
	 */
	class Drawable : public Resource {
		
	public:
		
        friend class Render;
        friend class NGin;
        friend class LightSourcesRender;
        friend class DLShadowRender;
        friend class ForwardRender;
        friend class DeferredRender;
        friend class PostprocessRender;

		/**
		 * W procesie tworzenia macierz trasnformacji każdego rysowalnego obiektu inicjowana jest macierzą identyczności.
		 * @param geometry wskaźnik do geometrii, która ma zostać powiązana z rysowalnym obiektem.
		 * @param material wskaźnik do materiału, który ma zostać powiązany z rysowalnym obiektem.
		 */
                                                Drawable                (std::shared_ptr<Geometry> geometry = nullptr, std::shared_ptr<Material> material = nullptr);

        std::string 	                        get_class	            () const override;
		/**
         * Metoda zwracająca przetrzymywaną wewnątrz klasy wartość, służacą do modyfikacjei położenia światła.
         */
        constexpr math::mat4 const &            get_transformation      () const { return transformation; }
		/**
         * Standardowa funkcja zwracająca przetrzymywaną wewnątrz klasy wartość, służacą do modyfikacji położenia rysowalnego obiektu.
         * Ze względu na wczesne stadium rozwoju oddano do dyspozycji użytkownikowi pełne zarządzanie macierzą.
         * Nie jest zaimplementowana żadnego rodzaju kontrola błędów.
         * Macierz przechowuje informację na temat rotacji, translacji, pochylenia oraz skali.
         */
		constexpr math::mat4 &                  get_transformation      () { return transformation; }
		/**
         * Dowiązuje geometrię do rysowalnego obiektu.
         */
		void                                    bind_geometry           (std::shared_ptr<Geometry> geometry);
		/**
		 * Dowiązuje materiał do rysowalnego obiektu.
		 */
        void                                    bind_material           (std::shared_ptr<Material> material);
		/**
         * Zwraca wskaźnik do materiału z którym powiązany jest rysowalny obiekt.
         */
		Material *                              get_material            ();
		/**
		 * Zwraca wskaźnik do geometrii z którą powiązany jest rysowalny obiekt.
		 */
        Geometry *                              get_geometry            ();
        
    private:

												Drawable                (Drawable const &) = delete;
		Drawable &                              operator =              (Drawable const &) = delete;
        bool                                    can_be_drawn            () const;
                void                            _load                   () override;
                void                            _unload                 () override;
        virtual void                            use                     ();
        virtual void                            draw                    ();
        static  std::unique_ptr<Drawable>       get_quad                ();
        bool                                    invalidated;
        std::shared_ptr<Geometry>               boundedGeometry;
        std::shared_ptr<Material>               boundedMaterial;
		math::mat4                              transformation;
        GLuint                                  vertexArray;
		
	};
	
}
