//
//  geometry.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <gsl/span>

#include "vertex.hpp"
#include "../utils/proxyOGL.hpp"
#include "../utils/utils.hpp"
#include "../resources/resource.hpp"

namespace ms {
	
	class Geometry : public Resource {
	
        friend class Drawable;
        
	public:
		
                                                        Geometry                    (std::vector <Vertex>  &&       vertices,
                                                                                     std::vector <unsigned int> &&  indices,
                                                                                     std::string &&                 associatedMaterial,
                                                                                     math::BoundingBox<float> &&    boundingBox,
                                                                                     std::string &&                 name = "");
        
                                                        Geometry                    (std::vector <Vertex>  &&       vertices,
                                                                                     std::vector <unsigned int> &&  indices,
                                                                                     std::string &&                 associatedMaterial,
                                                                                     std::string &&                 name = "");
        
                                                        Geometry                    (Geometry const &) = delete;
        Geometry &                                      operator =                  (Geometry const &) = delete;
                                                        ~Geometry                   () = default;
        
        void			                                set_preferred_material      (std::string const & name);
        constexpr std::string const &                   get_preferred_material_name () const { return preferredMaterialName; }
        std::string		                                get_class			        () const override;
        bool			                                has_preferred_material      () const;
        constexpr size_t				                amount_of_vertices	        () const { return amountOfVertices; }
        constexpr size_t				                amount_of_indices	        () const { return amountOfIndices; }
        constexpr math::BoundingBox<float> const &      get_bounding_box            () const { return boundingBox; }
        
        void                                            add_index                   (unsigned int index, std::vector<unsigned int>::iterator position);
        void                                            add_vertex                  (Vertex vertex, std::vector<Vertex>::iterator position, bool recalculateBoundingBox = true);
        constexpr std::vector<Vertex> const &           get_vertices                () const { return vertices; }
        constexpr std::vector<unsigned int> const &     get_indices                 () const { return indices; }
        static math::BoundingBox<float>                 calculate_bounding_box      (std::vector<Vertex> const & vertices);
        
	private:
        void                                            load_vertices_to_buffer     ();
        void                                            _load                       () override;
        void                                            _unload                     () override;
        void 			                                use_indicies 		        ();
        void                                            use_normals                 ();
        void                                            use_tangents                ();
        void                                            use_bitangents              ();
        void                                            use_vertices                ();
        void                                            use_texture_coord           ();
        size_t                                          amountOfVertices;
		size_t                                          amountOfIndices;
        
		std::vector <Vertex> 			                vertices;
		std::vector <unsigned int>                      indices;
        std::string                                     name;
		std::string		    	                        preferredMaterialName;
        math::BoundingBox<float>                        boundingBox;
        
        GLuint                                          normalsBuffer;
        GLuint                                          tangentsBuffer;
        GLuint                                          bitangentsBuffer;
        GLuint                                          positionsBuffer;
        GLuint                                          texturesCooridnatesBuffer;
        GLuint                                          indiciesBuffer;

	};
	
}
