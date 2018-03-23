//
//  resource.h
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef resource_h
#define resource_h

#include <memory>
#include <iostream>
#include <string>

#include "../../libs/source/glMath.h"

namespace ms {

	class ResourceCoordinator;

    class Resource {
		
	protected:
		
		 						Resource	();
		
    public:
	
		virtual void    		load  		() = 0;
		virtual bool    		is_loaded   ();
		virtual void 			unload 		() = 0;
		virtual std::string 	get_class	();
		virtual         		~Resource   ();
	
	private:
		
				bool			isLoaded;
	        
    };
    
}

#endif /* resource_h */
