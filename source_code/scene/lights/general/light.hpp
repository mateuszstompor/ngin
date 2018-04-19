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
		
		
        inline                                      Light               (math::vec3 const & color,
                                                                         float              power,
                                                                         math::mat4 const & projection,
                                                                         math::mat4 const & transformation);
        
        inline                                      Light               (math::vec3 &&      color,
                                                                         float              power,
                                                                         math::mat4 &&      projection,
                                                                         math::mat4 &&      transformation);
        
		virtual		 	                            ~Light                  () = default;
        inline constexpr math::vec3 const &         get_color               () const { return color; }
        inline constexpr math::vec3 &               get_color               () { return color; }
        inline constexpr math::mat4 const &         get_projection          () const { return projection; }
        inline constexpr PositionedObject const &   get_positionedObject    () const { return positionedObject; }
        inline constexpr PositionedObject &         get_positionedObject    () { return positionedObject; }
        inline constexpr float const &              get_power               () const { return power; }
        inline constexpr float &                    get_power               () { return power; }
        
    protected:
        
        float                                       power;
		math::vec3 		                            color;
        math::mat4                                  projection;
        PositionedObject                            positionedObject;
        
	};
	
}

ms::Light::Light (math::vec3 const & c,
                  float pow,
                  math::mat4 const & p,
                  math::mat4 const & t) : color{c}, projection{p}, positionedObject{t}, power{pow} { }

ms::Light::Light (math::vec3 && c,
                  float pow,
                  math::mat4 && p,
                  math::mat4 && t) : color{std::move(c)}, projection{std::move(p)}, positionedObject{std::move(t)}, power{pow} { }

#endif /* light_hpp */
