//
//  loader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <memory>
#include <string>
#include <map>
#include <tuple>
#include <cassert>
#include <cmath>
#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "tree.hpp"
#include "../scene/material.hpp"
#include "../scene/texture.hpp"
#include "../scene/drawable.hpp"
#include "../scene/geometry.hpp"

namespace ms {

	/**
	 * Klasa odpowiedzialna za wczytywanie obiektów, może być użyta niezależnie od biblioteki.
	 * Na podstawie ścieżki jest w stanie wczytać geometrię przy zachowaniu lub z pominięciem hierarchii znajdujących się w niej siatek.
	 * Dodatkowo może wczytywać tekstury i materiały.
	 * @see NGin
	 * @see Geometry
	 * @see Texture2D
	 * @see Material
	 */
	class Loader {
		
	public:
	
		using geometries_vec            = std::vector<std::unique_ptr<Geometry>>;
        using geometries_vec_h          = ms::tree<std::unique_ptr<Geometry>>;
		using materials_map             = std::map<std::string, std::unique_ptr<Material>>;
		using textures_map              = std::map<std::string, std::unique_ptr<Texture2D>>;
        using model_data                = std::tuple<geometries_vec, materials_map, textures_map>;
        using textures_and_materials    = std::tuple<materials_map, textures_map>;
        using material_vec              = std::vector<std::unique_ptr<Material>>;
        using geometry                  = std::unique_ptr<Geometry>;
        using texture                   = std::unique_ptr<Texture2D>;
        using paths                     = std::vector<std::string>;

		/**
		 * @param absolutePath ścieżka do pliku
		 * @return zwraca płaski model geometrii
		 */
		model_data	                	load_flat_model                         (std::string const & absolutePath) const;
		/**
		 * @param absolutePath ścieżka do pliku
		 * @return zwraca dwuwymiarową teksturę
		 */
        texture                         load_texture                            (std::string const & absolutePath) const;
		/**
		 * @param absolutePath ścieżka do pliku
		 * @return wczytuje materiały osadzone w pliku
		 */
        materials_map                   load_materials                          (std::string const & absolutePath) const;
		/**
		 * Wczytuje materiał, ale w przypadku, gdy zawiera on także tekstury pobierze i zwróci je
		 * @param absolutePath ścieżka do pliku
		 */
        textures_map                    load_textures_for_materials             (std::string const & absolutePath) const;
		/**
		* @param absolutePath ścieżka do pliku
		* @return Zwraca model nie zachowując hierarchii jego siatek
		*/
        geometries_vec                  load_flat_geometry                      (std::string const & absolutePath) const;
		/**
		 * @param absolutePath ścieżka do pliku
		 * @return Zwraca model zachowując hierarchię jego siatek
		 */
        geometries_vec_h                load_hierarchical_geometry              (std::string const & absolutePath) const;
        
	private:
		
		geometries_vec                  process_node			        (aiNode const &	node, aiScene const & scene) const;
        void                            process_node                    (tree<geometry> & tree, ::ms::tree<geometry>::iterator parent, aiNode const & node, aiScene const & scene) const;
		geometry  		                process_geometry		        (aiMesh const & mesh, aiScene const & scene) const;
		textures_and_materials      	load_materials_with_textures	(aiScene const & scene, std::string const & directoryPath) const;
		texture    	                    load_embeded_texture	        (aiTexture const & texture, std::string const & withName) const;
		paths    	                    get_texture_paths		        (aiTextureType const & type, aiMaterial const & mat, std::string const & directoryPath) const;
		static math::vec3               to_vec3					        (aiColor3D const & color);
        
	};
	
}
