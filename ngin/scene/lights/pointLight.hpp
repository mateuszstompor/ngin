//
//  pointLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "light.hpp"
#include "../drawable.hpp"

namespace ms {

    /**
     * Światło ma za zadanie symulować źródło, które umieszczone jest w pewnym miejscu, a jego promienie rozchodzą się w każdą stronę.
     * Światło Punktowe jest elementem sceny i za pomocą odwołania się do tego elemetnu biblioteki światło może zostać dodane.
     * @see Scene
     * @see Light
     */
	class PointLight : public Light {
	
        friend class NGin;
        friend class DeferredRender;
        friend class ForwardRender;
        
	public:
		
        /**
         * Światło punktowe tworzone jest na podstawie kilku wartości.
         * @param color odpowiada za kolor generwoanego światła.
         * @param power odpowiada za moc światła generowaną przez żródło.
         * @param position określa gdzie we współrzędnych świata znajdują się obiekt
         * @param castsShadow określa czy obiekty w obrębie światła mają rzucać cień
         */
		 	                 	    PointLight              (math::vec3 const &	color,
															 float 		        power,
                                                             math::vec3 const &	position,
                                                             bool				castsShadow);
    private:

        math::mat4 const &        	get_projection          () const override { return projection; }
        math::mat4 					projection;

	};
	
}
