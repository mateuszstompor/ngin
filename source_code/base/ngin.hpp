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
    
	class NGin : public Resource {
      
    public:
        
                        NGin        ();
        
        virtual void    draw_scene  () = 0;
        
		virtual         ~NGin       () = 0;
		
        
    protected:
        
        std::unique_ptr<Scene>                  scene;
        
        // Coordinator and renderer should be initialized by class
        // which inherits from NGin
        
        std::unique_ptr<ResourceCoordinator>    coordinator;
        
        std::unique_ptr<Render>                 deferredRenderer;
        
    };
    
}

#endif /* ngin_hpp */
