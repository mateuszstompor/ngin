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
                                                                         math::mat4 const & projection = math::mat4::identity(),
                                                                         math::mat4 const & transformation = math::mat4::identity());
        
        inline                                      Light               (math::vec3 && color,
                                                                         math::mat4 && projection = math::mat4::identity(),
                                                                         math::mat4 && transformation = math::mat4::identity());
		virtual		 	                            ~Light	            () = default;
        inline constexpr math::vec3 const &         get_color           () const;
        inline constexpr math::vec3 &               get_color           ();
        inline constexpr math::mat4 const &         get_projection      () const;
        inline constexpr math::mat4 const &         get_transformation  () const;
        inline constexpr math::mat4 &               get_transformation  ();
        
    protected:
        
		math::vec3 		                            color;
        math::mat4                                  projection;
		math::mat4                                  transformation;
        
	};
	
}

ms::Light::Light (math::vec3 const & c, math::mat4 const & p, math::mat4 const & t) : color(c), projection(p), transformation(t) { }
ms::Light::Light (math::vec3 && c, math::mat4 && p, math::mat4 && t) : color(std::move(c)), projection(std::move(p)), transformation(std::move(t)) { }

constexpr ms::math::vec3 const & ms::Light::get_color() const {
    return color;
}

constexpr ms::math::vec3 & ms::Light::get_color() {
    return color;
}

constexpr ms::math::mat4 const &  ms::Light::get_projection () const {
    return projection;
}

constexpr ms::math::mat4 const &  ms::Light::get_transformation () const {
    return transformation;
}

constexpr ms::math::mat4 &  ms::Light::get_transformation () {
    return transformation;
}

#endif /* light_hpp */
