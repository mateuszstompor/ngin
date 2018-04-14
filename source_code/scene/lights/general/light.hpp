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
		
		inline 			                            Light	            (math::vec3 const  & color);
        inline                                      Light               (math::vec3 const  & color, math::mat4 const & projection);
        inline                                      Light               (math::vec3 && color);
        inline                                      Light               (math::vec3 && color, math::mat4 && projection);
		virtual		 	                            ~Light	            () = default;
        inline constexpr math::vec3 const &         get_color           () const;
        inline constexpr math::vec3 &               get_color           ();
        inline constexpr ms::math::mat4 const &     get_projection      () const;
        inline constexpr ms::math::mat4 const &     get_transformation  () const;
        inline constexpr ms::math::mat4 &           get_transformation  ();
        
    protected:
        
		math::vec3 		                        color;
        math::mat4                              projection;
		math::mat4                              transformation;
        
	};
	
}

ms::Light::Light (math::vec3 const & c) : color(c) { }
ms::Light::Light (math::vec3 const  & c, math::mat4 const & p) : color(c), projection(p), transformation(math::mat4::identity()) { }
ms::Light::Light (math::vec3 && c) : color(std::move(c)) { }
ms::Light::Light (math::vec3 && c, math::mat4 && p) : color(std::move(c)), projection(std::move(p)), transformation(std::move(math::mat4::identity())) { }

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
