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
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../scene/sceneNode.hpp"


namespace ms {
	
	class Loader {
		
	public:
		
		std::vector<std::shared_ptr<SceneNode>> load_model(std::string path);
//		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	
	protected:
		
			void 							process_node		(aiNode *									node,
																 const aiScene *							scene,
																 std::vector<std::shared_ptr<SceneNode>> & 	listOfNodes);
		
		std::shared_ptr<Geometry> 			process_geometry	(aiMesh *mesh, const aiScene *scene);
		virtual std::shared_ptr<SceneNode> 	get_node			() = 0;
		virtual std::shared_ptr<Geometry> 	get_geometry		() = 0;
		
	};
	
}

#endif /* loader_hpp */
