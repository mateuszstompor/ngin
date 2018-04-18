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
    
        enum class Renderer  {
            deferred,
            forward_fragment,
            forward_vertex
        };
        
												NGin        		(unsigned int 	screenWidth,
																	 unsigned int 	screenHeight,
																	 unsigned int 	framebuffecrWidth,
																	 unsigned int 	framebufferHeight,
																	 std::unique_ptr<Camera> && cam);
		
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
        
        virtual void                            set_renderer        (Renderer r);
		virtual void 							load 				() = 0;
		virtual void 							unload 				() = 0;
        virtual void							draw_scene  		();
		DeferredRender & 						get_deferred_render	() const;
		virtual void							load_model			(std::string 	absolutePath);
		virtual        							~NGin       		() = default;
		std::unique_ptr<Scene>                  scene;
		
	protected:
		
		void									count_fps			();
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
		
		std::unique_ptr<Loader>					loader;
        
		std::unique_ptr<DeferredRender>         deferredRenderer;
        std::unique_ptr<Render>                 shadowRenderer;
		std::unique_ptr<ForwardRender>          gouraudForwardRenderer;
		std::unique_ptr<ForwardRender>          phongForwardRenderer;
		std::unique_ptr<LightSourcesRender>     lightSourceRenderer;
		std::unique_ptr<PostprocessDrawer>     	hdrRenderer;
		std::unique_ptr<PostprocessDrawer>     	bloomSplitRenderer;
		std::unique_ptr<PostprocessDrawer>     	bloomMergeRenderer;
		std::unique_ptr<PostprocessDrawer>     	gaussianBlurFirstStepRenderer;
		std::unique_ptr<PostprocessDrawer>     	gaussianBlurSecondStepRenderer;
        std::unique_ptr<PostprocessDrawer>      vignetteRenderer;
		std::unique_ptr<PostprocessDrawer>     	scaleRenderer;
		
		unsigned int							screenWidth;
		unsigned int							screenHeight;

		unsigned int 							framebufferWidth;
		unsigned int 							framebufferHeight;
        
        Renderer                                chosenRenderer {Renderer::deferred};
		
    };
    
}



#endif /* ngin_hpp */
