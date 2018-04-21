//
//  light.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include "../../../../glMath/source/glMath.h"

namespace ms {
	
	class Light {
	
	public:
		
		
            inline                                  Light                   (math::vec3 const & color,
                                                                             float              power,
                                                                             math::mat4 const & projection,
                                                                             math::mat4 const & transformation);
        inline virtual		 	                    ~Light                  () = 0;
        inline constexpr math::vec3 const  &        get_color               () const { return color; }
        inline constexpr math::mat4 const &         get_transformation      () const { return transformation; }
		inline constexpr math::mat4 &               get_transformation      () { return transformation; }
        inline constexpr math::mat4 const &         get_projection          () const { return projection; }
        inline constexpr float const &              get_power               () const { return power; }

        
    protected:
        
        float                                       power;
		math::vec3 		                            color;
        math::mat4 const                                  projection;
        math::mat4                                  transformation;
        
	};
	
}

ms::Light::Light (math::vec3 const & c,
                  float pow,
                  math::mat4 const & p,
                  math::mat4 const & t) : color{c}, projection{p}, transformation{t}, power{pow} { }

ms::Light::~Light () { }


#endif /* light_hpp */
