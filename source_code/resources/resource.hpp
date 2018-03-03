//
//  resource.h
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef resource_h
#define resource_h

namespace ms {

    class Resource {
        
    public:
        
        virtual void    initialize  () = 0;
        virtual bool    is_loaded   () = 0;
		virtual void 	destroy 	() = 0;
        virtual         ~Resource   () { }
        
    };
    
}

#endif /* resource_h */
