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

namespace ms {
	
	class SceneNode : public Resource {
	
	public:
		
							SceneNode	();
		virtual 	void 	use			() = 0;
		virtual 			~SceneNode	() = default;
		
		std::shared_ptr<Geometry> geometry;
		
	};
	
}

#endif /* scene_node_hpp */
