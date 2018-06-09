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

	/**
	 * Klasa zawiera w sobie zbiór wierzchołków oraz uporządkowaną listę indeksów, na podstawie których tworzona jest siatka trójkątów.
	 * Dodatkowo, z każdą siatką powiązana jest uproszczona struktura reprezentująca jej wymiary - bryła otaczająca.
	 * Geometria jest częścią sceny i może być użyta do stworzenia rysowalnego obiektu, sama w sobie nie posiada żadnych innych informacji niż definicja kształtu.
	 * @see Scene
	 * @see Drawable
	 */
	class Geometry : public Resource {
	
        friend class Drawable;
        
	public:
		/**
         * @param vertices wierzchołki, których należy użyć do stworzenia siatki.
         * @param indices uporządkowana lista indeksów do tablicy wierzchołków, której użyć aby stworzyć siatkętrójkątów.
         * @param associatedMaterial nazwa materiału, który został wybrany przez grafika tworzącego model, wykorzystujący siatkę.
         * @param boundingBox bryła otaczająca, którą należy wykorzystać podczas tworzenia geometrii.
         * @param name nazwa siatki
         */
                                                        Geometry                    (std::vector <Vertex>  &&       vertices,
                                                                                     std::vector <unsigned int> &&  indices,
                                                                                     std::string &&                 associatedMaterial,
                                                                                     math::BoundingBox<float> &&    boundingBox,
                                                                                     std::string &&                 name = "");

		/**
		 * W przypadku tego konstruktora bryła otaczająca siatki obliczana jest automatycznie.
         * @param vertices wierzchołki, których należy użyć do stworzenia siatki.
         * @param indices uporządkowana lista indeksów do tablicy wierzchołków, której użyć aby stworzyć siatkętrójkątów.
         * @param associatedMaterial nazwa materiału, który został wybrany przez grafika tworzącego model, wykorzystujący siatkę.
         * @param name nazwa siatki
         */
                                                        Geometry                    (std::vector <Vertex>  &&       vertices,
                                                                                     std::vector <unsigned int> &&  indices,
                                                                                     std::string &&                 associatedMaterial,
                                                                                     std::string &&                 name = "");
		/**
         * Zmienia domyślny, preferowany materiał na podany przez użytkownika
         */
        void			                                set_preferred_material      (std::string const & name);
		/**
		 * @return zwraca nazwę materiału, z którym grafik-artysta powiązał geometrię podczas jej tworzenia
		 */
        constexpr std::string const &                   get_preferred_material_name () const { return preferredMaterialName; }
        std::string		                                get_class			        () const override;
		/**
		 * Służy do sprawdzenia, czy geometria podczas tworzenia miała powiązany ze sobą jakiś materiał.
		 */
        bool			                                has_preferred_material      () const;
		/**
		 * @return zwraca liczbę wierzchołków w siatce.
		 */
        constexpr size_t				                amount_of_vertices	        () const { return amountOfVertices; }
		/**
		 * @return zwraca liczbę indeksów w siatce.
		 */
        constexpr size_t				                amount_of_indices	        () const { return amountOfIndices; }
		/**
		 * @return zwraca bryłę otaczającą.
		 */
        constexpr math::BoundingBox<float> const &      get_bounding_box            () const { return boundingBox; }
        /**
         * Dodaje na zadany indeks wierzchołka do wektora indeksów przechowywanego przez geometrię.
         */
        void                                            add_index                   (unsigned int index, std::vector<unsigned int>::iterator position);
		/**
         * Dodaje wierzchołek do wektora wierzchołków przechowywanego przez geometrię, użytkownik sam określa czy pudełko otaczające siatkę powinno być rekalkulowane.
         */
        void                                            add_vertex                  (Vertex vertex, std::vector<Vertex>::iterator position, bool recalculateBoundingBox = true);
		/**
		 * @return zwraca wszystkie wierzchołki zawarte w siatce.
		 */
        constexpr std::vector<Vertex> const &           get_vertices                () const { return vertices; }
		/**
		 * @return zwraca uporządkowaną listę indeksów określającą kolejność w jakiej wierzchołki powinny być łączone w trójki.
		 */
        constexpr std::vector<unsigned int> const &     get_indices                 () const { return indices; }
		/**
		 * Oblicza bryłę otaczającą dla zadanego wektora wierzchołków
		 * @return zwraca bryłę otaczającą
		 */
        static math::BoundingBox<float>                 calculate_bounding_box      (std::vector<Vertex> const & vertices);
        
	private:

														Geometry                    (Geometry const &) = delete;
		Geometry &                                      operator =                  (Geometry const &) = delete;
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
