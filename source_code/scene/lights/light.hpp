//
//  light.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <glMath.h>

namespace ms {
	
    /**
     * Abstrakcyjna klasa reprezuntująca źródło światła.
     * Bez względu na typ pochodny każde ze świateł zawiera informację na temat koloru światła, mocy z jaką jest ono generowane,
     * położenia źródła oraz informacji, czy oświetlane przez nie obiekty powinny rzucać cień.
     * Dostępne są trzy implementacje
     * @see PointLight
     * @see SpotLight
     * @see DirectionalLight
     */
	class Light {
	
	public:

        /**
         * Metoda zwracająca kolor światła generowanego przez źródło.
         */
        inline constexpr    math::vec3 const  &       	get_color              	() const { return color; }
        /**
         * Metoda zwracająca przetrzymywaną wewnątrz klasy wartość, służacą do modyfikacji położenia światła.
         */
        inline constexpr    math::mat4 const &        	get_transformation     	() const { return transformation; }
		/**
         * Standardowa funkcja zwracająca przetrzymywaną wewnątrz klasy wartość, służacą do modyfikacji położenia światła.
         * Ze względu na wczesne stadium rozwoju oddano do dyspozycji użytkownikowi pełne zarządzanie macierzą.
         * Nie jest zaimplementowana żadnego rodzaju kontrola błędów.
         * Macierz przechowuje informację na temat rotacji, translacji, pochylenia oraz skali.
         */
		inline constexpr    math::mat4 &              	get_transformation     	() { return transformation; }
        /**
         * Zwraca moc źródła.
         * Jej wartość mieścić powinna się w przedziale 0 - 300.
         */
        inline constexpr    float const &             	get_power               () const { return power; }
        /**
         * Informuje czy źródło światła rzuca cień
         */
		inline constexpr    bool             	      	casts_shadow            () const { return castsShadow; }

	protected:

        virtual		 	                      	        ~Light                 	() = 0;
		                                      	        Light                   (math::vec3 const & color,
																			   	 float              power,
																			   	 math::mat4 const & transformation,
																			   	 bool               castsShadow);
        virtual math::mat4 const &                      get_projection          () const = 0;
        //  power should be in range from 0 to 200
        float                                          	power;
		math::vec3 		                                color;
        math::mat4                                      transformation;
        bool                                            castsShadow;
        
	};
	
}
