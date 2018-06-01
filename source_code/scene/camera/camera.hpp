//
//  camera.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <glMath.h>

namespace ms {
	
	class Camera {
	
	public:
		
		inline 						            Camera                  ();
		virtual 					            ~Camera                 () = default;
        virtual bool                            is_in_camera_sight      (math::mat4 const & boundingBoxTransformation,
                                                                         math::BoundingBox<float> const & boundingBox) const = 0;
        virtual math::mat4 const &              get_projection_matrix   () const = 0;
        constexpr math::mat4 const &            get_transformation      () const { return transformation; }
        constexpr math::mat4 &                  get_transformation      () { return transformation; }
        
    protected:
        
        math::mat4                              transformation;
    
	};
	
}

ms::Camera::Camera() : transformation(math::mat4::identity()) {}
