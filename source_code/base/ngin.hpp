//
//  ngin.hpp
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <chrono>

#include "../rendering/forwardRender.hpp"
#include "../rendering/deferredRender.hpp"
#include "../rendering/lightSourcesRender.hpp"
#include "../rendering/shadowRender.hpp"
#include "../scene/scene.hpp"
#include "../resources/resource.hpp"
#include "../resources/resourceCoordinator.hpp"
#include "../utils/loader.hpp"
#include "../rendering/postprocessDrawer.hpp"
#include "../utils/loader.hpp"
#include "../scene/drawable.hpp"
#include "../shaders/shaderHeaders.hpp"

namespace ms {
    
	class NGin {
      
    public:
    
        enum class Renderer  {
            deferred,
            forward_fragment,
            forward_vertex
        };
        
                                                    NGin        		(unsigned int                       screenWidth,
                                                                         unsigned int                       screenHeight,
                                                                         unsigned int                       frameBufferWidth,
                                                                         unsigned int                       frameBufferHeight,
                                                                         unsigned int                       shadowsResolution,
                                                                         std::unique_ptr<Camera> &&			cam,
                                                                         std::unique_ptr<Framebuffer> &&    defaultFramebuffer);
        void                                        set_renderer        (Renderer r);
        void 							            load 				();
        void 							            unload 				();
        void							            draw_scene  		();
        DeferredRender & 		                    get_deferred_render	() const;
        void							            load_model			(std::string const & absolutePath);
        void                                        pause_drawing       () { shouldDraw = false; }
        void                                        resume_drawing      () { shouldDraw = true; }
		Scene                                       scene;
		
	private:
		
		void							            count_fps			();
		Loader					                    loader;
        
		std::unique_ptr<DeferredRender>             deferredRenderer;
        std::unique_ptr<DLShadowRender>             shadowRenderer;
		std::unique_ptr<ForwardRender>              gouraudForwardRenderer;
		std::unique_ptr<ForwardRender>              phongForwardRenderer;
		std::unique_ptr<LightSourcesRender>         lightSourceRenderer;
		std::unique_ptr<PostprocessDrawer>     		hdrRenderer;
		std::unique_ptr<PostprocessDrawer>     	    bloomSplitRenderer;
		std::unique_ptr<PostprocessDrawer>     	    bloomMergeRenderer;
		std::unique_ptr<PostprocessDrawer>     	    gaussianBlurFirstStepRenderer;
		std::unique_ptr<PostprocessDrawer>     	    gaussianBlurSecondStepRenderer;
        std::unique_ptr<PostprocessDrawer>          vignetteRenderer;
		std::unique_ptr<PostprocessDrawer>     	    scaleRenderer;
        std::vector<std::unique_ptr<Framebuffer>>   shadows;
        
        bool                                        shouldDraw;
        unsigned int                                shadowResolution;
		unsigned int							    screenWidth;
		unsigned int							    screenHeight;

		unsigned int 							    framebufferWidth;
		unsigned int 							    framebufferHeight;
        
        Renderer                                    chosenRenderer {Renderer::forward_fragment};
		
    };
    
}
