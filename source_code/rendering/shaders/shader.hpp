//
//  shader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include "../../resources/resource.hpp"

namespace ms {
    
	class Shader : public Resource {
		
	public:
		
		virtual void 	use 	() = 0;
		virtual 		~Shader	() = default;
    };
    
}

#endif /* shader_hpp */
