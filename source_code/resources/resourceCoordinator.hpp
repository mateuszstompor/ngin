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
														ResourceCoordinator			(ResourceCoordinator const & rc) = delete;
		ResourceCoordinator& 							operator = 					(ResourceCoordinator const & rc) = delete;

		static 	std::shared_ptr<ResourceCoordinator> 	get_instance				();
		static 	void 									destroy_shared_instance		();
		
		virtual void 									register_load				(Resource* resource);
		virtual void 									register_unload				(Resource* resource);
		virtual void 									register_allocation			(Resource* resource);
		virtual void 									register_deallocation		(Resource* resource);
		
														~ResourceCoordinator		() = default;
    protected:
		
														ResourceCoordinator			();
		
		std::vector<std::weak_ptr<Resource>> 			loadedResources;
        std::vector<std::weak_ptr<Resource>> 			allocatedResources;
		static std::shared_ptr<ResourceCoordinator> 	sharedInstance;
    };
    
}

#endif /* resourceCoordinator_h */
