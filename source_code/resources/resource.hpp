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

#include "../../glMath/source/glMath.h"

namespace ms {

	class ResourceCoordinator;

    class Resource {
		
	protected:
		
		 						Resource		();

    public:
	
		virtual void    		load  			() final;
		virtual void 			unload 			() final;
		
		virtual bool    		is_loaded   	() const final;
		
		virtual std::string 	get_class		() const;
		virtual         		~Resource   	();
	
	protected:
		
		virtual void			_load			() = 0;
		virtual void			_unload			() = 0;
		
	private:
		
				bool			isLoaded;
		
    };
    
}

#endif /* resource_h */
