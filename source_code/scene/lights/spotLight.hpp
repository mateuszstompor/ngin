//
//  spotLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "pointLight.hpp"
#include "directionalLight.hpp"

namespace ms {
	
    /**
     * Jedna z dostępnych implementacji światła, która generuje promienie w zadanym kierunku.
     * Wektory promieni mają wspólny punkt przyłożenia.
     * Światło kierunkowe stożkowe umieszczone zostać może na scenie.
     * @see Scene
     * @see Light
     */
	class SpotLight : public Light {
		
        friend class ForwardRender;
        friend class DeferredRender;
        friend class NGin;
        
	public:
        /**
         * Kontruktor posiadający podstawowe dla światła parametry
         * @param color jest kolorem światła
         * @param power jest mocą światła z przedziału 0-300
         * @param position to położenie w którym światło ma zostać osadzone
         * @param lightingAngleDegrees to kąt określony w kątach na jaki mają rozchodzić się promienie
         * @param direction to kierunek światła, w którym promienie powinny być generowane
         * @param castsShadow określa, czy obiekty oświetlane przez źródło mają rzucać cień
         * @param nearPlane Odpowiada za określenie odległości w stosunku do położenia światła od którego miejsca obiekty będą widoczne. Parametr ten wykorzystywany jest w procesie generowania cienia.
         * @param farPlane Odpowiada za określenie odległości w stosunku do położenia światła do którego miejsca obiekty będą widoczne. Parametr ten wykorzystywany jest w procesie generowania cienia.
         * @param aspectRatio jest to stosunek szerokości do długości, określa w jakiej proporcji obraz ma być generowany. Parametr ten wykorzystywany jest w procesie generowania cienia.
         * Światła kierunkowe, stożkowe są elementem sceny
         * @see Scene
         * @see Light
         */
		 		                    		    SpotLight 	            (math::vec3 const & color,
                                                                         float 			    power,
                                                                         math::vec3 const & position,
                                                                         float 			    lightingAngleDegrees,
                                                                         math::vec3 const & direction,
                                                                         bool               castsShadow,
                                                                         float              nearPlane = 0.0001f,
                                                                         float              farPlane = 100.0f,
                                                                         float              aspectRatio = 1.0f);
        /**
         * Zwraca kąt w obrębie którego generowane jest światło.
         */
        constexpr float						    get_angle_degrees       () const { return lightingAngleDegrees; }
        
    private:
        
        constexpr math::frustum4f const &       get_frustum             () const { return frustum; }
        math::mat4 const &                      get_projection          () const override { return frustum.get_projection_matrix(); }
        math::frustum4f    						frustum;
		float 		                    		lightingAngleDegrees;
		
	};
	
}


