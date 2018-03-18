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
#include "../utils/loader.hpp"

namespace ms {
    
	class NGin {
      
    public:
        
												NGin        		(unsigned int screenWidth,
																	 unsigned int screenHeight,
																	 float camNear,
																	 float camFar,
																	 float fovDegrees,
																	 float aspect);
		virtual void							load_model			(std::string absolutePath);
		virtual void 							load 				() = 0;
		virtual void 							unload 				();
        virtual void							draw_scene  		();
		DeferredRender & 						get_deferred_render	() const;
		ForwardRender & 						get_forward_render	() const;
		virtual        							~NGin       		() = default;
		
		std::unique_ptr<Scene>                  scene;
		
	protected:
		
		virtual std::unique_ptr<Loader>			get_loader() = 0;
		virtual std::shared_ptr<SceneNode>		get_node() = 0;
		
		std::unique_ptr<Loader>					loader;
		std::unique_ptr<DeferredRender>         deferredRenderer;
		std::unique_ptr<ForwardRender>          forwardRenderer;
		unsigned int 							screenWidth;
		unsigned int 							screenHeight;
		
    };
    
}

#endif /* ngin_hpp */
