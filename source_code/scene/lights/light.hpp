//
//  light.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "../../../glMath/source/glMath.h"

namespace ms {
	
	class Light {
	
	public:
		
		
            inline                                      Light                   (math::vec3 const & color,
                                                                                 float              power,
                                                                                 math::mat4 const & transformation,
                                                                                 bool               castsShadow);
        inline virtual		 	                        ~Light                 	() = 0;
        inline constexpr    math::vec3 const  &        get_color              	() const { return color; }
        inline constexpr    math::mat4 const &         get_transformation     	() const { return transformation; }
		inline constexpr    math::mat4 &               get_transformation     	() { return transformation; }
        inline constexpr    float const &              get_power                () const { return power; }
		inline constexpr    int             	       casts_shadow             () const { return castsShadow; }

	protected:
        
        float                                       power;
		math::vec3 		                            color;
        math::mat4                                  transformation;
        bool                                        castsShadow;
        
	};
	
}

ms::Light::Light (math::vec3 const & c,
                  float pow,
                  math::mat4 const & t,
                  bool               cs) : color{c}, transformation{t}, power{pow}, castsShadow{cs} { }

ms::Light::~Light () { }

