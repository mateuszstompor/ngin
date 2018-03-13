//
//  loader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef loader_hpp
#define loader_hpp

#include <memory>
#include <string>
#include <map>
#include <tuple>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../scene/texture.hpp"
#include "../scene/sceneNode.hpp"

namespace ms {
	
	class Loader {
		
		typedef std::vector<std::shared_ptr<Geometry>> 						geometries_vec;
		typedef std::map<std::string, std::shared_ptr<Material>> 			materials_map;
		typedef std::map<std::string, std::shared_ptr<Texture>> 			textures_map;
		
		typedef std::tuple<geometries_vec, materials_map, textures_map>		model_data;
		
	public:
		
		model_data	load_model(std::string path);
		
	protected:
		
		model_data 							process_node			(aiNode *		node,
																 	const aiScene *	scene);
		//TODO load textures
//		std::vector<Texture> 				load_material_textures	(aiMaterial *mat, aiTextureType type, std::string typeName);
		std::shared_ptr<Geometry> 			process_geometry		(aiMesh * mesh, const aiScene * scene);
		materials_map 						load_materials			(const aiScene * scene);
		ms::math::vec3						to_vec3					(aiColor3D color);
		virtual std::shared_ptr<Geometry> 	get_geometry			() = 0;
		
	};
	
}

#endif /* loader_hpp */
