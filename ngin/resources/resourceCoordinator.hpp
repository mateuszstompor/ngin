//
//  resourceCoordinator.h
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <set>
#include <memory>
#include <iostream>
#include <algorithm>
#include <string>

#include "resource.hpp"
#include "../rendering/render.hpp"
#include "../utils/utils.hpp"
#include "../rendering/shaders/shader.hpp"
#include "../config.hpp"

namespace ms {

	/**
	 * Klasa jest zwierzchnikiem czuwający nad wszystkimi alokowanymi zasobami - pochodnymi Resource.
	 * W chwili obecnej jej najważniejszym zadaniem jest pełnienie funkcji diagnostycznych.
	 * Niemożliwa jest jej konfiugracja, jest całkowicie zamknięta na ingerencje użytkownika.
	 * Stanowi element fasady biblioteki - klasy NGin.
	 * Alokowana jest wraz z tworzeniem biblioteki, a dealokowana wraz jej destrukcją
	 *
	 * @see Resource
	 * @see NGin
	 */
    class ResourceCoordinator {
		
        friend class NGin;
        friend class Resource;
    
    public:
        
                                                        ~ResourceCoordinator        () = default;

    private:
														ResourceCoordinator			(ResourceCoordinator const & rc) = delete;
		ResourceCoordinator & 							operator = 					(ResourceCoordinator const & rc) = delete;

		static 	std::shared_ptr<ResourceCoordinator> 	get_instance				();
		static 	void 									destroy_shared_instance		();
		
		virtual void 									register_load				(Resource * resource);
		virtual void 									register_unload				(Resource * resource);
		virtual void 									register_allocation			(Resource * resource);
		virtual void 									register_deallocation		(Resource * resource);
		virtual void									unload_all_resources		();
														ResourceCoordinator			();
		std::set<std::string> 							loadedResources;
        std::set<std::string> 							allocatedResources;
			
		static std::shared_ptr<ResourceCoordinator> 	sharedInstance;
		
    };
    
}
