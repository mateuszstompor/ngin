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
#include <string>

#include "vertex.hpp"
#include "../resources/resource.hpp"
#include "../../libs/source/glMath.h"

namespace ms {
	
	class Geometry : public Resource {
	
		friend class Drawable;
		friend class DrawableOGL;
		
	public:
		
		friend class Loader;
		
				virtual void 			            use_normals 		() = 0;
                virtual void                        use_tangents        () = 0;
                virtual void                        use_bitangents      () = 0;
				virtual void 			            use_vertices 		() = 0;
				virtual void 			            use_texture_coord 	() = 0;
		
						void			            set_material		(std::string name);
						std::string		            get_material_name	() const;
		inline	virtual	std::string		            get_class			() = 0;
                        math::BoundingBox<float>*   get_bounding_box    () const;
						bool			            has_material		() const;
		
						int				            amount_of_vertices	() const;
						int				            amount_of_indices	() const;
		
										            Geometry			();
				virtual					            ~Geometry			() = default;
		
	protected:

				virtual void 			            use_indicies 		() = 0;
				
		std::vector <Vertex> 			            vertices;
		std::vector <unsigned int> 		            indices;
		std::string						            associatedMaterial;
        std::unique_ptr<math::BoundingBox<float>>   boundingBox;

	};
	
}

std::string ms::Geometry::get_class () {
	return "ms::Geometry";
}

#endif /* geometry_hpp */
