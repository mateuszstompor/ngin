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
#include "geometry.hpp"
#include "../utils/ogl/proxyOGL.hpp"
#include "../resources/resource.hpp"

namespace ms {
	
	class Geometry : public Resource {
	
		friend class Drawable;
		friend class Loader;
        using bounding_box_f = math::BoundingBox<float>;
        
	public:
		
		
                                            Geometry                    (std::vector <Vertex>  &&       vertices,
                                                                         std::vector <unsigned int> &&  indices,
                                                                         std::string &&                 associatedMaterial,
                                                                         math::BoundingBox<float> &&    boundingBox);
        
                                            ~Geometry                   () = default;
        void 			                    use_normals          		();
        void                                use_tangents                ();
        void                                use_bitangents              ();
        void 			                    use_vertices         		();
        void 			                    use_texture_coord        	();
        void			                    set_material		        (std::string const & name);
        std::string const &                 get_material_name	        () const;
        std::string		                    get_class			        () const override;
        bool			                    has_material        		() const;
        int				                    amount_of_vertices	        () const;
        int				                    amount_of_indices	        () const;
		void                                load_vertices_to_buffer     ();
        constexpr bounding_box_f const &    get_bounding_box            () const { return boundingBox; }
        
	private:
        
        void                                _load                       () override;
        void                                _unload                     () override;
        void 			                    use_indicies 		        () ;
				
		std::vector <Vertex> 			    vertices;
		std::vector <unsigned int>          indices;
		std::string		    	            associatedMaterial;
        math::BoundingBox<float>            boundingBox;
        
        GLuint                              normalsBuffer;
        GLuint                              tangents;
        GLuint                              bitangents;
        GLuint                              positionsBuffer;
        GLuint                              texturesCooridnatesBuffer;
        GLuint                              indiciesBuffer;

	};
	
}

#endif /* geometry_hpp */
