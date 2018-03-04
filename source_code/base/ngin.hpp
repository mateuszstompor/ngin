//
//  ngin.hpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef ngin_hpp
#define ngin_hpp

#include <memory>

#include "../rendering/render.hpp"
#include "../scene/scene.hpp"
#include "../resources/resource.hpp"
#include "../resources/resourceCoordinator.hpp"

namespace ms {
    
	class NGin {
      
    public:
        
                        NGin        					();
		virtual void    sygnalize_loading_ability  		();
		virtual void 	unload 							() = 0;
        virtual void    draw_scene  					() = 0;
		virtual         ~NGin       					() = default;
		
        
//    protected:
		
		std::shared_ptr<Render>                 deferredRenderer;
        std::shared_ptr<Scene>                  scene;
        
    };
    
}

#endif /* ngin_hpp */
