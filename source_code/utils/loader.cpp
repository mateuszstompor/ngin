//
//  loader.cpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "loader.hpp"

std::vector<std::shared_ptr<ms::SceneNode>> ms::Loader::load_model(std::string path) {
	std::vector<std::shared_ptr<ms::SceneNode>> vec;
	
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		//critical error
		assert(false);
	}
	process_node(scene->mRootNode, scene, vec);
	return vec;
	
}

void ms::Loader::process_node(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<SceneNode>> & listOfNodes) {
	
	for(unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		if(mesh->HasNormals() && mesh->HasFaces() && mesh->HasPositions()) {
			
			std::shared_ptr<SceneNode> node = get_node();
			node->geometry = process_geometry(mesh, scene);
			listOfNodes.push_back(node);
		}
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene, listOfNodes);
	}
}

std::shared_ptr<ms::Geometry> ms::Loader::process_geometry(aiMesh *mesh, const aiScene *scene) {
	
	std::shared_ptr<ms::Geometry> geometry = get_geometry();
	
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		
		if (mesh->mVertices != nullptr && mesh->mNormals != nullptr) {
			
		}
		
		Vertex vertex;
		
		vertex.position.x() = mesh->mVertices[i].x;
		vertex.position.y() = mesh->mVertices[i].y;
		vertex.position.z() = mesh->mVertices[i].z;
	
		vertex.normal.x() = mesh->mNormals[i].x;
		vertex.normal.y() = mesh->mNormals[i].y;
		vertex.normal.z() = mesh->mNormals[i].z;
		
		geometry->vertices.push_back(vertex);
	}
	
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			geometry->indices.push_back(face.mIndices[j]);
	}
	
	return geometry;
}
