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
	inline	virtual 	std::string 	get_class	() = 0;
			virtual		void			draw		() = 0;
			virtual 					~Drawable	() = default;
		
		PositionedObject				modelTransformation;
		std::shared_ptr<Geometry> 		geometry;
		//avoid reference cycles with weak pointers!!
		std::weak_ptr<Material>			boundedMaterial;

	protected:
		
			virtual 	void 			use			() = 0;
		
	};
	
}

ms::Drawable::Drawable() : modelTransformation(PositionedObject()) { }

std::string ms::Drawable::get_class () {
	return "ms::Drawable";
}

#endif /* scene_node_hpp */
