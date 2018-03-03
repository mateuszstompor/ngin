//
//  nginOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef nginOGL_hpp
#define nginOGL_hpp

#include "../ngin.hpp"
#include "../../resources/resourceCoordinatorOGL.hpp"
#include "../../rendering/ogl/deferredRenderOGL.hpp"

namespace ms {
    
    class NGinOGL : public NGin {
        
    public:
                NGinOGL     ();
		
        void    draw_scene  () override;
		
		void 	initialize	() override;
		
		bool 	is_loaded	() override;
		
		void 	destroy		() override;
		
    };
    
}

#endif /* nginOGL_hpp */
