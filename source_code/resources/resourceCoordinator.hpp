//
//  resourceCoordinator.h
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef resource_coordinator_h
#define resource_coordinator_h

#include <set>
#include <memory>
#include <iostream>
#include <algorithm>
#include <string>

#include "resource.hpp"
#include "../rendering/render.hpp"
#include "../utils/utils.hpp"
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
		virtual void									unload_all_resources		();
														~ResourceCoordinator		() = default;
    protected:
		
														ResourceCoordinator			();
		std::set<std::string> 							loadedResources;
        std::set<std::string> 							allocatedResources;
	
	public:
		
		static std::shared_ptr<ResourceCoordinator> 	sharedInstance;
		
    };
    
}

#endif /* resource_coordinator_h */
