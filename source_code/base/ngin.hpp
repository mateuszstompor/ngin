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
#include <chrono>

#include "../rendering/forwardRender.hpp"
#include "../rendering/deferredRender.hpp"
#include "../rendering/lightSourcesRender.hpp"
#include "../scene/scene.hpp"
#include "../resources/resource.hpp"
#include "../resources/resourceCoordinator.hpp"
#include "../utils/loader.hpp"
#include "../rendering/postprocessDrawer.hpp"

namespace ms {
    
	class NGin {
      
    public:
        
												NGin        		(unsigned int 	screenWidth,
																	 unsigned int 	screenHeight,
																	 float 			camNear,
																	 float 			camFar,
																	 float 			fovDegrees,
																	 float 			aspect);
		
		virtual void							load_model			(std::string 	absolutePath);
		virtual void							load_point_light	(float 			power,
																	 math::vec3 	color,
																	 math::vec3 	position,
																	 std::string 	absolutePath);
		
		virtual void							load_spot_light		(float 			power,
																	 math::vec3 	color,
																	 math::vec3 	position,
																	 float			lightingAngleDegrees,
																	 math::vec3 	direction,
																	 std::string 	absolutePath);
		
		virtual void 							load 				() = 0;
		virtual void 							unload 				();
        virtual void							draw_scene  		();
		DeferredRender & 						get_deferred_render	() const;
		virtual        							~NGin       		() = default;
		
		std::unique_ptr<Scene>                  scene;
		
	protected:
		
		virtual std::unique_ptr<Loader>			get_loader			() = 0;
		virtual std::shared_ptr<Drawable>		get_drawable		() = 0;
		
		virtual std::shared_ptr<PointLight>		get_point_light		(float 			power,
																	 math::vec3 	color,
																	 math::vec3 	position) = 0;
		
		virtual std::shared_ptr<SpotLight>		get_spot_light		(float 			power,
																	 math::vec3 	color,
																	 math::vec3 	position,
																	 float 			lightingAngleDegrees,
																	 math::vec3 	direction) = 0;
		
		void									count_fps			();
		
		
		std::unique_ptr<Loader>					loader;
		std::unique_ptr<DeferredRender>         deferredRenderer;
		std::unique_ptr<ForwardRender>          gouraudForwardRenderer;
		std::unique_ptr<ForwardRender>          phongForwardRenderer;
		std::unique_ptr<LightSourcesRender>     lightSourceRenderer;
		std::unique_ptr<PostprocessDrawer>     	hdrRenderer;
		
		unsigned int 							screenWidth;
		unsigned int 							screenHeight;
		
    };
    
}

#endif /* ngin_hpp */
