//
//  loader.cpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "loader.hpp"

#include "../../third-party-libs/stb_image.h"

ms::Loader::model_data ms::Loader::load_model(std::string path) {
	
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		assert(false);
	}
	
	model_data data = process_node(*scene->mRootNode, *scene);
	textures_and_materials texmat = load_materials(*scene, path.substr(0, path.find_last_of('/')));
	std::get<2>(data) = std::get<1>(texmat);
	
	if(scene->HasMaterials()) {
		std::get<1>(data) = std::get<0>(texmat);
	}
	
	return data;
}

ms::Loader::textures_and_materials ms::Loader::load_materials (aiScene const & scene, std::string const & directoryPath) {
	
	textures_and_materials texmat;
	
	materials_map & 	materials 	= std::get<0>(texmat);
	textures_map & 		textures 	= std::get<1>(texmat);
	
	for(unsigned int i = 0; i < scene.mNumMaterials; ++i) {
		aiMaterial const & mat = *scene.mMaterials[i];
		aiColor3D ambient, diffuse, specular;
		float opacity, shininess;
		aiString aiName;
		
		mat.Get(AI_MATKEY_NAME, aiName);
		mat.Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		mat.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		mat.Get(AI_MATKEY_COLOR_SPECULAR, specular);
		mat.Get(AI_MATKEY_OPACITY, opacity);
		mat.Get(AI_MATKEY_SHININESS, shininess);
		
		//TODO handle embedded textures
		
		std::string name (aiName.C_Str());
		
        std::shared_ptr<Material> msMaterial = std::make_shared<MaterialOGL>(to_vec3(ambient),
                                                              to_vec3(diffuse),
                                                              to_vec3(specular),
                                                              shininess,
                                                              opacity,
                                                              name);
		
		msMaterial->diffuseTexturesNames 	= get_texture_paths(aiTextureType_DIFFUSE, mat, directoryPath);
		msMaterial->specularTexturesNames 	= get_texture_paths(aiTextureType_SPECULAR, mat, directoryPath);
		msMaterial->normalTexturesNames 	= get_texture_paths(aiTextureType_NORMALS, mat, directoryPath);
		msMaterial->heightTexturesNames 	= get_texture_paths(aiTextureType_HEIGHT, mat, directoryPath);
		
		#ifdef LD_MAT_COMP_COUNT
			std::cout << aiName.C_Str() << 	" DIFFUSE: " 	<< msMaterial->diffuseTexturesNames.size() 	<< std:: endl;
			std::cout << aiName.C_Str() << 	" SPECULAR: " 	<< msMaterial->specularTexturesNames.size()	<< std:: endl;
			std::cout << aiName.C_Str() <<  " NORMALS: " 	<< msMaterial->normalTexturesNames.size() 	<< std:: endl;
			std::cout << aiName.C_Str() <<  " HEIGHT: " 	<< msMaterial->heightTexturesNames.size() 	<< std:: endl;
		#endif
		
		for (auto absolutePath : msMaterial->diffuseTexturesNames) {
			if(auto texture = load_texture_from_file(absolutePath)) {
				textures.insert(std::make_pair(absolutePath, texture));
			}
		}
	
		for (auto absolutePath : msMaterial->specularTexturesNames) {
			if(auto texture = load_texture_from_file(absolutePath)) {
				textures.insert(std::make_pair(absolutePath, texture));
			}
		}
		
		for (auto absolutePath : msMaterial->normalTexturesNames) {
			if(auto texture = load_texture_from_file(absolutePath)) {
				textures.insert(std::make_pair(absolutePath, texture));
			}
		}
		
		for (auto absolutePath : msMaterial->heightTexturesNames) {
			if(auto texture = load_texture_from_file(absolutePath)) {
				textures.insert(std::make_pair(absolutePath, texture));
			}
		}
		
		materials.insert(std::make_pair(name, msMaterial));
		
	}
	
	#ifdef LD_MAT_SUMMARY
	
		std::cout << "materials count: " << materials.size() << std::endl;
		std::cout << "textures count: " << textures.size() << std::endl;
	
	#endif
	
	return texmat;
	
}

std::shared_ptr<ms::Texture> ms::Loader::load_texture_from_file (std::string const & absolutePath) {
	int width, height, bpp;
	unsigned char* data = stbi_load(absolutePath.c_str(), &width, &height, &bpp, 0);
	if (data) {
		Texture::Format format;
		std::shared_ptr<Texture> texture;
		
		switch (bpp) {
			case 3:
				format = Texture::Format::rgb_8_8_8;
				break;
			case 4:
				format = Texture::Format::rgba_8_8_8_8;
				break;
			case 1:
				format = Texture::Format::r_8;
				break;
			default:
				std::cerr << "FORMAT NOT SUPPORTED" << std::endl;
				assert(false);
		}

        texture = std::make_shared<ms::Texture>(Texture::Type::tex_2d,
                                                format,
                                                Texture::AssociatedType::UNSIGNED_BYTE,
                                                static_cast<unsigned int>(width),
                                                static_cast<unsigned int>(height),
                                                absolutePath,
                                                Texture::MinFilter::linear,
                                                Texture::MagFilter::linear,
                                                Texture::Wrapping::repeat,
                                                Texture::Wrapping::repeat,
                                                0);
		
		texture->copy_data(data, width * height * bpp);
		stbi_image_free(data);
		return texture;
	} else {
		return nullptr;
	}
}

ms::Loader::model_data ms::Loader::process_node (aiNode const & node, aiScene const &	scene) {
	
	model_data data;
	
	geometries_vec & geometries = std::get<0>(data);
	
		for(unsigned int i = 0; i < node.mNumMeshes; ++i) {
			aiMesh *mesh = scene.mMeshes[node.mMeshes[i]];
			if(mesh->HasNormals() && mesh->HasFaces() && mesh->HasPositions()) {
				std::shared_ptr<Geometry> geometry = std::make_shared<ms::GeometryOGL>();
				geometry = process_geometry(*mesh, scene);
				geometries.push_back(geometry);
			}
		}
	
		for(unsigned int i = 0; i < node.mNumChildren; ++i) {
			model_data proceededNodes = process_node(*node.mChildren[i], scene);
			geometries_vec & proceededGeo = std::get<0>(proceededNodes);
			geometries.insert(geometries.end(),
							  std::make_move_iterator(proceededGeo.begin()),
							  std::make_move_iterator(proceededGeo.end()));
		}
	
	return data;
}

std::shared_ptr<ms::Geometry> ms::Loader::process_geometry(aiMesh const & mesh, aiScene const & scene) {

    std::shared_ptr<ms::Geometry> geometry = std::make_shared<ms::GeometryOGL>();

    float minX = 0, maxX = 0;
    float minY = 0, maxY = 0;
    float minZ = 0, maxZ = 0;

    if(mesh.mNumVertices > 0) {
        maxX = minX = mesh.mVertices[0].x;
        maxY = minY = mesh.mVertices[0].y;
        maxZ = minZ = mesh.mVertices[0].z;
    }
    
	for(unsigned int i = 0; i < mesh.mNumVertices; ++i) {

			Vertex vertex;

            if(mesh.mVertices[i].x > maxX) {
                maxX = mesh.mVertices[i].x;
            }
        
            if(mesh.mVertices[i].x < minX) {
                minX = mesh.mVertices[i].x;
            }
        
            if(mesh.mVertices[i].y > maxY) {
                maxY = mesh.mVertices[i].y;
            }
        
            if(mesh.mVertices[i].y < minY) {
                minY = mesh.mVertices[i].y;
            }
        
            if(mesh.mVertices[i].z > maxZ) {
                maxZ = mesh.mVertices[i].z;
            }
        
            if(mesh.mVertices[i].z < minZ) {
                minZ = mesh.mVertices[i].z;
            }
        
        
			vertex.position.x() = mesh.mVertices[i].x;
			vertex.position.y() = mesh.mVertices[i].y;
			vertex.position.z() = mesh.mVertices[i].z;

            if(mesh.HasNormals()) {
                vertex.normal.x() = mesh.mNormals[i].x;
                vertex.normal.y() = mesh.mNormals[i].y;
                vertex.normal.z() = mesh.mNormals[i].z;
            }
        
            if(mesh.HasTangentsAndBitangents()) {
                vertex.bitangent.x() = mesh.mBitangents[i].x;
                vertex.bitangent.y() = mesh.mBitangents[i].y;
                vertex.bitangent.z() = mesh.mBitangents[i].z;
                
                vertex.tangent.x() = mesh.mTangents[i].x;
                vertex.tangent.y() = mesh.mTangents[i].y;
                vertex.tangent.z() = mesh.mTangents[i].z;
            }
        
			if(mesh.mTextureCoords[0]) {
				vertex.textureCoordinates.x() = mesh.mTextureCoords[0][i].x;
				vertex.textureCoordinates.y() = mesh.mTextureCoords[0][i].y;
			} else {
				vertex.textureCoordinates = math::vec2{0.0f, 0.0f};
			}
		
			geometry->vertices.push_back(vertex);

	}
    
    geometry->boundingBox = std::make_unique<math::BoundingBox<float>>(minX, maxX, minY, maxY, minZ, maxZ);

	if(mesh.mMaterialIndex > 0) {
		
		aiMaterial * material = scene.mMaterials[mesh.mMaterialIndex];
		
		aiString aiName;
		
		material->Get(AI_MATKEY_NAME, aiName);
		
		#ifdef DEBUG
		assert(strcmp(aiName.C_Str(), "") != 0);
		#endif
		
		geometry->set_material(std::string(aiName.C_Str()));
		
	}
	
	for(unsigned int i = 0; i < mesh.mNumFaces; ++i) {
		aiFace face = mesh.mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; ++j) {
			geometry->indices.push_back(face.mIndices[j]);
		}
	}

	return geometry;
}

std::vector<std::string> ms::Loader::get_texture_paths (aiTextureType const & type, aiMaterial const & mat, std::string const & directoryPath) {
	std::vector<std::string> paths;
	unsigned int texCount = mat.GetTextureCount(type);
	for (unsigned int i = 0; i < texCount; ++i) {
		aiString path;
		mat.GetTexture(type, i, &path);
		paths.push_back(directoryPath + "/" + path.C_Str());
	}
	return paths;
}

std::shared_ptr<ms::Texture> ms::Loader::load_embeded_texture (aiTexture const & texture, std::string const & withName) {
	//TODO implement embeded textures
	assert(false);
}

ms::math::vec3 ms::Loader::to_vec3 (aiColor3D const & color) {
	return ms::math::vec3 {color.r, color.g, color.b};
}
