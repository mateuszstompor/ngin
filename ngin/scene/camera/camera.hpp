//
//  camera.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <glMath/glMath.h>

namespace ms {
	
    /**
     * Zadaniem tej klasy abstrakcyjnej jest zapewnienie interfejsu dla obu możliwych typów kamer.
     * Samą ideą jest reprezentacja punktu widzenia na scenie z którego generowana jest klatka obrazu.
     * Kamera wykorzystyawna jest jedynie wewnątrz sceny.
     * @see OrthographicCamera
     * @see PerspectiveCamera
     * @see Scene
     */
	class Camera {
	
	public:
				
		virtual 					            ~Camera                 () = default;
        /**
         * Funkcja ma za zadanie sprawdzić czy bryła otaczająca, która należy do Siatki geometrii znajduje się w polu widzenia kamery.
         * Funkcja wykorzystywana jest bardzo często, za każdym razem przed wyrenderowaniem obiektu rysowalnego w celu optymalizacji.
         * @param boundingBoxTransformation to macierz transformacji, którą zastosować należy przed obliczeniami na bryle otacząjącej
         * @param boundingBox to pudełko otaczające
         */
        virtual bool                            is_in_camera_sight      (math::mat4 const & boundingBoxTransformation,
                                                                         math::BoundingBox<float> const & boundingBox) const = 0;
        /**
         * Z każdą kamerą powiązana jest macierz projekcji, której zadaniem jest przełożyć abstrakcyjne współrzędny na współrzędne obsługiwane przez API karty graficznej.
         */
        virtual math::mat4 const &              get_projection_matrix   () const = 0;
        /**
         * Standardowa funkcja zwracająca przetrzymywaną wewnątrz klasy wartość, służacą do modyfikacji położenia kamery.
         */
        constexpr math::mat4 const &            get_transformation      () const { return transformation; }
        /**
         * Standardowa funkcja zwracająca przetrzymywaną wewnątrz klasy wartość, służacą do modyfikacji położenia kamery.
         * Ze względu na wczesne stadium rozwoju oddano do dyspozycji użytkownikowi pełne zarządzanie macierzą.
         * Nie jest zaimplementowana żadnego rodzaju kontrola błędów.
         * Macierz przechowuje informację na temat rotacji, translacji, pochylenia oraz skali.
         */
        constexpr math::mat4 &                  get_transformation      () { return transformation; }
        
    protected:
        
        inline                                  Camera                  ();
        math::mat4                              transformation;
    
	};
	
}

ms::Camera::Camera() : transformation(math::mat4::identity()) {}
