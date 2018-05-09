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

#include "../scene/material.hpp"
#include "../scene/texture.hpp"
#include "../scene/drawable.hpp"
#include "../scene/geometry.hpp"

namespace ms {
	
	class Loader {
		
	public:
	
		using geometries_vec            = std::vector<std::unique_ptr<Geometry>>;
		using materials_map             = std::map<std::string, std::unique_ptr<Material>>;
		using textures_map              = std::map<std::string, std::unique_ptr<Texture2D>>;
        using textures_and_materials    = std::tuple<materials_map, textures_map>;
		using model_data                = std::tuple<geometries_vec, materials_map, textures_map>;
		
		model_data	                	load_model              (std::string const & path);
        std::unique_ptr<Texture2D>      load_texture_from_file  (std::string const & absolutePath);
        
	private:
		
		model_data                  	process_node			(aiNode const &	node, aiScene const & scene);
		std::unique_ptr<Geometry>  		process_geometry		(aiMesh const & mesh, aiScene const & scene);
		textures_and_materials      	load_materials			(aiScene const & scene, std::string const & directoryPath);
		std::unique_ptr<Texture2D>    	load_embeded_texture	(aiTexture const & texture, std::string const & withName);
		std::vector<std::string>    	get_texture_paths		(aiTextureType const & type, aiMaterial const & mat, std::string const & directoryPath);
		ms::math::vec3              	to_vec3					(aiColor3D const & color);
        
	};
	
}
