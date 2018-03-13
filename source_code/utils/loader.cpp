//
//  loader.cpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "loader.hpp"

ms::Loader::model_data ms::Loader::load_model(std::string path) {
	
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		assert(false);
	}
	
	model_data data = process_node(scene->mRootNode, scene);
	
	if(scene->HasMaterials()) {
		materials_map & materials = std::get<1>(data);
		materials = load_materials(scene);
	}
	
	return data;
}

ms::Loader::materials_map ms::Loader::load_materials (const aiScene * scene) {
	
	materials_map materials;
	
	for(unsigned int i = 0; i < scene->mNumMaterials; ++i) {
		aiMaterial * mat = scene->mMaterials[i];
		aiColor3D ambient, diffuse, specular;
		float opacity, shininess;
		aiString aiName;
		
		mat->Get(AI_MATKEY_NAME, aiName);
		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		mat->Get(AI_MATKEY_OPACITY, opacity);
		mat->Get(AI_MATKEY_SHININESS, shininess);
		
		std::string name (aiName.C_Str());
		
		std::shared_ptr<Material> msMaterial(new Material(to_vec3(ambient),
														  to_vec3(diffuse),
														  to_vec3(specular),
														  shininess,
														  opacity,
														  name));
		
		materials.insert(std::make_pair(name, msMaterial));
		
	}
	
	return materials;
	
}

ms::Loader::model_data ms::Loader::process_node (aiNode * node, const aiScene *	scene) {
	
	model_data data;
	
	geometries_vec & geometries = std::get<0>(data);
	
		for(unsigned int i = 0; i < node->mNumMeshes; ++i) {
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			if(mesh->HasNormals() && mesh->HasFaces() && mesh->HasPositions()) {
				std::shared_ptr<Geometry> geometry = get_geometry();
				geometry = process_geometry(mesh, scene);
				std::get<0>(data).push_back(geometry);
			}
		}
	
		for(unsigned int i = 0; i < node->mNumChildren; ++i) {
			model_data proceededNodes = process_node(node->mChildren[i], scene);
			geometries_vec & proceededGeo = std::get<0>(proceededNodes);
			geometries.insert(geometries.end(), std::make_move_iterator(proceededGeo.begin()), std::make_move_iterator(proceededGeo.end()));
		}
	
	return data;
}

std::shared_ptr<ms::Geometry> ms::Loader::process_geometry(aiMesh *mesh, const aiScene *scene) {

	std::shared_ptr<ms::Geometry> geometry = get_geometry();

	for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {

			Vertex vertex;

			vertex.position.x() = mesh->mVertices[i].x;
			vertex.position.y() = mesh->mVertices[i].y;
			vertex.position.z() = mesh->mVertices[i].z;

			vertex.normal.x() = mesh->mNormals[i].x;
			vertex.normal.y() = mesh->mNormals[i].y;
			vertex.normal.z() = mesh->mNormals[i].z;

			geometry->vertices.push_back(vertex);

			if(mesh->mTextureCoords[0]) {
				vertex.textureCoordinates.x() = mesh->mTextureCoords[0][i].x;
				vertex.textureCoordinates.y() = mesh->mTextureCoords[0][i].y;
			} else {
				vertex.textureCoordinates = math::vec2{0.0f, 0.0f};
			}

			if(mesh->mMaterialIndex > 0) {
				
				aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
				
				aiString aiName;
				
				material->Get(AI_MATKEY_NAME, aiName);
				
				#ifdef DEBUG
				assert(strcmp(aiName.C_Str(), "") != 0);
				#endif
				
				geometry->set_material(std::string(aiName.C_Str()));
				//TODO load textures
//				material->Get(AI_MATKEY_SHININESS, shininess);
//				std::cout<<"shininess: "<<shininess<<std::endl;
//				std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
//																   aiTextureType_DIFFUSE, "texture_diffuse");
//				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//				vector<Texture> specularMaps = loadMaterialTextures(material,
//																	aiTextureType_SPECULAR, "texture_specular");
//				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

	}

	for(unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; ++j) {
			geometry->indices.push_back(face.mIndices[j]);
		}
	}

	return geometry;
}

ms::math::vec3 ms::Loader::to_vec3 (aiColor3D color) {
	return ms::math::vec3 {color.r, color.g, color.b};
}

//TODO load textures
//std::vector<ms::Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string typeName) {
//	std::vector<ms::Texture> textures;
////	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
////	{
////		aiString str;
////		mat->GetTexture(type, i, &str);
////		Texture texture;
////		texture.id = TextureFromFile(str.C_Str(), directory);
////		texture.type = typeName;
////		texture.path = str;
////		textures.push_back(texture);
////	}
//	return textures;
//}

