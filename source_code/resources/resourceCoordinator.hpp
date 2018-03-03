//
//  resourceCoordinator.h
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef resourceCoordinator_h
#define resourceCoordinator_h

#include <vector>
#include <memory>

#include "resource.hpp"
#include "../rendering/render.hpp"
#include "../rendering/shaders/shader.hpp"

namespace ms {
    
    class ResourceCoordinator {
    
    public:
        
        std::shared_ptr<Render>     get_render      () const;
        std::shared_ptr<Shader>     get_shader      () const;
        
    protected:
        
        std::vector<std::weak_ptr<Resource>> resources;
        
    };
    
}

#endif /* resourceCoordinator_h */
