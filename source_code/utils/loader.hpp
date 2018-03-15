//
//  loader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef loader_hpp
#define loader_hpp

#define STB_IMAGE_IMPLEMENTATION

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
		typedef std::tuple<materials_map, textures_map>						textures_and_materials;
		
		typedef std::tuple<geometries_vec, materials_map, textures_map>		model_data;
		
	public:
		
		model_data	load_model(std::string path);
		
	protected:
		
		model_data 							process_node			(aiNode *		node,
																	 const aiScene *	scene);
		
		std::shared_ptr<Geometry> 			process_geometry		(aiMesh * mesh, const aiScene * scene);
		textures_and_materials 				load_materials			(const aiScene * scene, std::string directoryPath);
		std::shared_ptr<Texture>			load_embeded_texture	(aiTexture * texture, std::string withName);
		std::shared_ptr<Texture>			load_texture_from_file	(std::string absolutePath);
		std::vector<std::string>			get_texture_paths		(aiTextureType type, aiMaterial * mat, std::string directoryPath);
		ms::math::vec3						to_vec3					(aiColor3D color);
		virtual std::shared_ptr<Geometry> 	get_geometry			() = 0;
		virtual std::shared_ptr<Texture> 	get_texture				(std::string				name,
																	 Texture::Format			format,
																	 Texture::AssociatedType	associatedType,
																	 Texture::MinFilter			minFilter,
																	 Texture::MagFilter			magFilter,
																	 Texture::Wrapping			sWrapping,
																	 Texture::Wrapping			tWrapping,
																	 unsigned int 				mipMapLevel,
																	 unsigned int 				width,
																	 unsigned int 				height) = 0;
		
	};
	
}

#endif /* loader_hpp */
