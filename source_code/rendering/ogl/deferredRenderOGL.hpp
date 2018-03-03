//
//  deferredRenderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferredRenderOGL_hpp
#define deferredRenderOGL_hpp

#include "../render.hpp"

namespace ms {
    
    class DeferredRenderOGL : public Render {
		
	public:
					~DeferredRenderOGL 	() {}
		
		void		use     			() {}
		void		initialize			() {}
		bool		is_loaded			() { return true; }
        
    };
    
}

#endif /* deferredRenderOGL_hpp */
