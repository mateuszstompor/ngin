//
//  sceneNode.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef scene_node_hpp
#define scene_node_hpp

#include <memory>

#include "../resources/resource.hpp"
#include "geometry.hpp"
#include "material.hpp"
#include "positionedObject.hpp"

namespace ms {
	
	class SceneNode : public Resource {
	
	public:
		
		friend class Loader;
		
		inline					SceneNode		();
		virtual 	void 		use				() = 0;
		virtual 				~SceneNode		() = default;
		
		PositionedObject					modelTransformation;
		
		// Shared resources
		std::shared_ptr<Geometry> 			geometry;
		
	};
	
}

ms::SceneNode::SceneNode() : geometry(nullptr), modelTransformation(PositionedObject()) { }

#endif /* scene_node_hpp */
