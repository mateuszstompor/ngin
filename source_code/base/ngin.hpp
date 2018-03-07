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

#include "../rendering/forwardRender.hpp"
#include "../rendering/deferredRender.hpp"
#include "../scene/scene.hpp"
#include "../resources/resource.hpp"
#include "../resources/resourceCoordinator.hpp"

namespace ms {
    
	class NGin {
      
    public:
        
                        NGin        		(unsigned int screenWidth,
											 unsigned int screenHeight,
											 float camNear,
											 float camFar,
											 float fovDegrees,
											 float aspect);
		
		virtual void 	load 				() = 0;
		virtual void 	unload 				();
        virtual void    draw_scene  		() = 0;
		virtual         ~NGin       		() = default;
		
		std::shared_ptr<Scene>                  scene;
		
	protected:
		
		std::shared_ptr<DeferredRender>         deferredRenderer;
		std::shared_ptr<ForwardRender>          forwardRenderer;
		unsigned int 							screenWidth;
		unsigned int 							screenHeight;
		
    };
    
}

#endif /* ngin_hpp */
