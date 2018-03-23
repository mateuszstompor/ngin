//
//  drawable.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef drawable_hpp
#define drawable_hpp

#include <memory>

#include "../resources/resource.hpp"
#include "geometry.hpp"
#include "material.hpp"
#include "positionedObject.hpp"

namespace ms {
	
	class Drawable : public virtual Resource {
	
	public:
		
		friend class Loader;
		
	inline								Drawable	();
			virtual 	void 			use			() = 0;
	inline	virtual 	std::string 	get_class	() = 0;

			virtual 					~Drawable	() = default;
		
		PositionedObject				modelTransformation;
		
		// Shared resources
		std::shared_ptr<Geometry> 		geometry;
		
	};
	
}

ms::Drawable::Drawable() : geometry(nullptr), modelTransformation(PositionedObject()) { }

std::string ms::Drawable::get_class () {
	return "ms::Drawable";
}

#endif /* scene_node_hpp */
