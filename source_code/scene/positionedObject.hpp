//
//  positionedObject.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef positioned_object_hpp
#define positioned_object_hpp

#include "../../glMath/source/glMath.h"

namespace ms {

	class PositionedObject {

	public:
		
		inline							PositionedObject	();
		inline							PositionedObject	(math::mat4 transformation);
		virtual 						~PositionedObject	() = default;
		
		inline math::mat4 const & 		get_transformation	() const;
		inline void						set_transformation	(math::mat4 transformation);
		inline void						post_transform		(math::mat4 transformation);
		inline void						pre_transform		(math::mat4 transformation);
		
	protected:
		
		math::mat4 						transformation;

	};

}

ms::PositionedObject::PositionedObject (math::mat4 t) : transformation(t) { }

ms::PositionedObject::PositionedObject () : PositionedObject(math::mat4::identity()) { }

void ms::PositionedObject::set_transformation (math::mat4 t) {
	transformation = t;
}

ms::math::mat4 const & ms::PositionedObject::get_transformation	() const {
	return transformation;
}

void ms::PositionedObject::post_transform (math::mat4 t) {
	transformation = transformation * t;
}

void ms::PositionedObject::pre_transform (math::mat4 t) {
	transformation = t * transformation;
}

#endif /* positioned_object_hpp */
