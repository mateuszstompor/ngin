//
//  geometry.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef geometry_hpp
#define geometry_hpp

#include <vector>

#include "vertex.hpp"
#include "../resources/resource.hpp"

namespace ms {
	
	class Geometry : public Resource {
	
	public:
		
		friend class Loader;
		
		virtual void 	use_normals 		() = 0;
		virtual void 	use_vertices 		() = 0;
		virtual void 	use_indicies 		() = 0;
		
		inline int		amount_of_vertices	() const;
		inline int		amount_of_indices	() const;
		
		inline			Geometry			();
		virtual			~Geometry			() = default;
		
	protected:
		
		std::vector <Vertex> 		vertices;
		std::vector <unsigned int> 	indices;

	};
	
}

ms::Geometry::Geometry() : vertices(std::vector<Vertex>()), indices(std::vector<unsigned int>()) { }

int ms::Geometry::amount_of_vertices () const {
	return static_cast<int>(vertices.size());
}

int ms::Geometry::amount_of_indices	() const {
	return static_cast<int>(indices.size());
}


#endif /* geometry_hpp */
