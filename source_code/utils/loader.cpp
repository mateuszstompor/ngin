//
//  loader.cpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "loader.hpp"

#include "stb_image.h"

ms::Loader::model_data ms::Loader::load_flat_model (std::string const & path) const {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
		assert(false);
	}
    model_data data{};
    std::get<0>(data) = process_node(*scene->mRootNode, *scene);
	textures_and_materials texmat = load_materials_with_textures(*scene, path.substr(0, path.find_last_of('/')));
    std::get<2>(data) = std::get<1>(std::move(texmat));

	if(scene->HasMaterials()) {
        std::get<1>(data) = std::get<0>(std::move(texmat));
	}
	
	return data;
}

ms::Loader::geometries_vec_h ms::Loader::load_hierarchical_geometry (std::string const & path) const {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        assert(false);
    }
    tree<std::unique_ptr<Geometry>> t {};
    process_node(t, t.begin(), *scene->mRootNode, *scene);
    return t;
}

void ms::Loader::process_node (tree<geometry> & tree, ::ms::tree<geometry>::iterator parent, aiNode const & node, aiScene const & scene) const {
    //check it
    for(auto i{0}; i < node.mNumMeshes; ++i) {
        auto const & mesh = *scene.mMeshes[node.mMeshes[i]];
        if(mesh.HasNormals() && mesh.HasFaces() && mesh.HasPositions()) {
            tree.insert_c(parent, i, process_geometry(mesh, scene));
        }
    }
    auto tmp{parent};
    for(auto i{0}; i < node.mNumChildren; ++i) {
        process_node(tree, ++tmp, *node.mChildren[i], scene);
    }
}

ms::Loader::geometries_vec ms::Loader::load_flat_geometry (std::string const & path) const {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        assert(false);
    }
    return process_node(*scene->mRootNode, *scene);
}

ms::Loader::materials_map ms::Loader::load_materials (std::string const & path) const {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        assert(false);
    }
    return std::get<0>(load_materials_with_textures(*scene, path.substr(0, path.find_last_of('/'))));
}

ms::Loader::textures_map ms::Loader::load_textures_for_materials (std::string const & path) const {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        assert(false);
    }
    return std::get<1>(load_materials_with_textures(*scene, path.substr(0, path.find_last_of('/'))));
}

ms::Loader::textures_and_materials ms::Loader::load_materials_with_textures (aiScene const & scene, std::string const & directoryPath) const {
	
	textures_and_materials  texmat;
	
	materials_map & 	    materials 	= std::get<0>(texmat);
	textures_map & 		    textures 	= std::get<1>(texmat);
	
	for(unsigned int i = 0; i < scene.mNumMaterials; ++i) {
		auto const & mat = *scene.mMaterials[i];
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
		
		auto name (aiName.C_Str());
		
        auto msMaterial = std::make_unique<Material>(to_vec3(ambient),
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
            std::cout << aiName.C_Str() << 	" DIFFUSE: " 	<< msMaterial->diffuseTexturesNames.size() 	<< '\n';
			std::cout << aiName.C_Str() << 	" SPECULAR: " 	<< msMaterial->specularTexturesNames.size()	<< '\n';
			std::cout << aiName.C_Str() <<  " NORMALS: " 	<< msMaterial->normalTexturesNames.size() 	<< '\n';
			std::cout << aiName.C_Str() <<  " HEIGHT: " 	<< msMaterial->heightTexturesNames.size() 	<< '\n';
		#endif
		
        for (auto absolutePath : msMaterial->diffuseTexturesNames) {
            if(auto texture = load_texture(absolutePath))
                textures.insert(std::make_pair(absolutePath, std::move(texture)));
        }
    
        for (auto absolutePath : msMaterial->specularTexturesNames) {
            if(auto texture = load_texture(absolutePath))
                textures.insert(std::make_pair(absolutePath, std::move(texture)));
        }
        
        for (auto absolutePath : msMaterial->normalTexturesNames) {
            if(auto texture = load_texture(absolutePath))
                textures.insert(std::make_pair(absolutePath, std::move(texture)));
        }
        
        for (auto absolutePath : msMaterial->heightTexturesNames) {
            if(auto texture = load_texture(absolutePath))
                textures.insert(std::make_pair(absolutePath, std::move(texture)));
        }
		
        materials.insert(std::make_pair(name, std::move(msMaterial)));
		
	}
	
	#ifdef LD_MAT_SUMMARY
		std::cout << "materials count: " << materials.size() << '\n';
		std::cout << "textures count: " << textures.size() << '\n';
	#endif
	
	return texmat;
	
}

std::unique_ptr<ms::Texture2D> ms::Loader::load_texture (std::string const & absolutePath) const {
	int width, height, bpp;
	unsigned char* data = stbi_load(absolutePath.c_str(), &width, &height, &bpp, 0);
	if (data != nullptr) {
		Texture2D::Format format;
		switch (bpp) {
			case 3:
				format = Texture2D::Format::rgb_8_8_8;
				break;
			case 4:
				format = Texture2D::Format::rgba_8_8_8_8;
				break;
			case 1:
				format = Texture2D::Format::r_8;
				break;
			default:
				std::cerr << "FORMAT NOT SUPPORTED" << '\n';
				assert(false);
		}

        auto texture = std::make_unique<ms::Texture2D>(format,
													   Texture2D::AssociatedType::UNSIGNED_BYTE,
                                                       static_cast<unsigned int>(width),
                                                       static_cast<unsigned int>(height),
                                                       absolutePath,
													   Texture2D::MinFilter::linear,
													   Texture2D::MagFilter::linear,
													   Texture2D::Wrapping::repeat,
													   Texture2D::Wrapping::repeat,
                                                       0);
		
		texture->copy_data(data, static_cast<size_t>(width * height * bpp));
		stbi_image_free(data);
		return texture;
	} else {
		return nullptr;
	}
}

ms::Loader::geometries_vec ms::Loader::process_node (aiNode const & node, aiScene const & scene) const {
		
    geometries_vec geometries{};
	
		for(auto i = 0; i < node.mNumMeshes; ++i) {
			aiMesh const & mesh = *scene.mMeshes[node.mMeshes[i]];
			if(mesh.HasNormals() && mesh.HasFaces() && mesh.HasPositions()) {
                geometries.push_back(process_geometry(mesh, scene));
			}
		}
	
		for(auto i = 0; i < node.mNumChildren; ++i) {
			geometries_vec proceededGeo = process_node(*node.mChildren[i], scene);
			geometries.insert(geometries.end(),
							  std::make_move_iterator(proceededGeo.begin()),
							  std::make_move_iterator(proceededGeo.end()));
		}
	
	return geometries;
}

std::unique_ptr<ms::Geometry> ms::Loader::process_geometry(aiMesh const & mesh, aiScene const & scene) const {

    std::vector <Vertex> vertices;
    std::vector <unsigned int> indices;
    std::string associatedMaterial;

    for(unsigned int i {0}; i < mesh.mNumVertices; ++i) {

        Vertex vertex;

        std::memcpy(vertex.position.c_array(), &mesh.mVertices[i], 3 * sizeof(float));
        std::memcpy(vertex.normal.c_array(), &mesh.mNormals[i], 3 * sizeof(float));

        if(mesh.HasTangentsAndBitangents()) {
            std::memcpy(vertex.bitangent.c_array(), &mesh.mBitangents[i], 3 * sizeof(float));
            std::memcpy(vertex.tangent.c_array(), &mesh.mTangents[i], 3 * sizeof(float));
        }

        if(mesh.HasTextureCoords(0)) {
            std::memcpy(vertex.textureCoordinates.c_array(), &mesh.mTextureCoords[0][i], 2 * sizeof(float));

        } else {
            vertex.textureCoordinates = math::vec2{0.0f, 0.0f};
        }

        vertices.push_back(vertex);

    }
    
    if(mesh.mMaterialIndex > 0) {
        aiString aiName;
        scene.mMaterials[mesh.mMaterialIndex]->Get(AI_MATKEY_NAME, aiName);
        assert(strcmp(aiName.C_Str(), "") != 0);
        associatedMaterial = aiName.C_Str();
    }

    for(unsigned int i = 0; i < mesh.mNumFaces; ++i) {
        auto face = mesh.mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    return std::make_unique<ms::Geometry>(std::move(vertices), std::move(indices), std::move(associatedMaterial), mesh.mName.C_Str());

}

std::vector<std::string> ms::Loader::get_texture_paths (aiTextureType const & type, aiMaterial const & mat, std::string const & directoryPath) const {
	std::vector<std::string> paths;
	auto texCount = mat.GetTextureCount(type);
	for (auto i = 0; i < texCount; ++i) {
		aiString path;
		mat.GetTexture(type, i, &path);
		paths.push_back(directoryPath + "/" + path.C_Str());
	}
	return paths;
}

std::unique_ptr<ms::Texture2D> ms::Loader::load_embeded_texture (aiTexture const & texture, std::string const & withName) const {
	//TODO implement embeded textures
	assert(false);
	return nullptr;
}

ms::math::vec3 ms::Loader::to_vec3 (aiColor3D const & color) {
	return ms::math::vec3 {color.r, color.g, color.b};
}
