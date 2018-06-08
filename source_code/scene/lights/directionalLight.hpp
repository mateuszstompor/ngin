//
//  directionalLight.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "light.hpp"

namespace ms {

	/**
	 * Światło, które symuluje nieskończonie oddalone źródło.
	 * Jego promienie są równoległe do siebie, a więc powierzchnie są oświetlane w równy sposób.
	 * W teorii światło nie ma położenia.
	 * Z uwagi, iż istnieje możliwość użycia map cieni i sprawienia, że obiekty w jego zasięgu rzucały będą cień, konieczne jest określenie ram w jakich mapa cienia będzie generowana.
	 * W wyniku tego światło kierunkowe korzysta zarówno z macierzy transformacji, jak i projekcji.
	 * Światła kierunkowe jest pochodną światła i może umieszczone zostać na scenie.
	 * @see Scene
	 * @see Light
	 */
	class DirectionalLight : public Light {
        
        friend class ForwardRender;
        friend class DeferredRender;
        friend class NGin;
        
	public:

		/**
         * @param color odpowiada za kolor generwoanego światła.
         * @param power odpowiada za moc światła generowaną przez żródło.
         * @param direction określa kierunek w którym generowane światło świeci
         * @param castsShadow określa czy obiekty w obrębie światła mają rzucać cień
         * @param projection specyfikuje jaka macierz projekcji ma być użyta do generowania mapy cieni w przypadku, gdy cienie mają być rzucane
         */
		                                DirectionalLight    (math::vec3 const & color,
                                                             float              power,
                                                             math::vec3 const & direction,
                                                             bool               castsShadow,
                                                             math::mat4 const & projection = math::projection4f::orthogonal_cube(44.0f));

    private:

        math::mat4 const &              get_projection      () const override { return projection; }
        math::mat4                      projection;
        
	};
	
}

