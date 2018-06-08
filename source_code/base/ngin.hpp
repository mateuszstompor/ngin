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
#include "../rendering/postprocessing/postprocessRender.hpp"
#include "../rendering/postprocessing/gaussianBlurPostprocessRender.hpp"
#include "../rendering/postprocessing/vignettePostprocessRender.hpp"

#include "../scene/scene.hpp"
#include "../scene/textureArray.hpp"
#include "../scene/cubemap.hpp"

#include "../utils/loader.hpp"

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
        void                                            set_renderer        (Renderer r);
        void 							                load 				();
        void 							                unload 				();
        void							                draw_scene  		();
        constexpr Loader const &                        get_loader          () const { return loader; }
        DeferredRender & 		                        get_deferred_render	() const;
        void							                load_model			(std::string const & absolutePath);
        void                                            pause_drawing       () { shouldDraw = false; }
        void                                            resume_drawing      () { shouldDraw = true; }
        constexpr Scene &                               get_scene           () { return scene; }
		
	private:
		
        Scene                                           scene;
		void							                count_fps			();
        void                                            draw_models         ();
        void                                            draw_postprocess    ();
        void                                            draw_sl_pov         (std::uint16_t x,
                                                                             std::uint16_t y,
                                                                             std::uint16_t tileWidth,
                                                                             std::uint16_t tileHeight);
        void                                            draw_pl_pov         (std::uint16_t x,
                                                                             std::uint16_t y,
                                                                             std::uint16_t tileWidth,
                                                                             std::uint16_t tileHeight);
		Loader					                        loader;
        
		std::unique_ptr<DeferredRender>                 deferredRenderer;
        std::unique_ptr<DLShadowRender>                 shadowRenderer;
        std::unique_ptr<DLShadowRender>                 pointLightShadowRenderer;
		std::unique_ptr<ForwardRender>                  gouraudForwardRenderer;
		std::unique_ptr<ForwardRender>                  phongForwardRenderer;
		std::unique_ptr<LightSourcesRender>             lightSourceRenderer;
		std::unique_ptr<PostprocessRender>     		    hdrRenderer;
		std::unique_ptr<PostprocessRender>     	        bloomSplitRenderer;
		std::unique_ptr<PostprocessRender>     	        bloomMergeRenderer;
		std::unique_ptr<GaussianBlurPostprocessRender>  gaussianBlurFirstStepRenderer;
		std::unique_ptr<GaussianBlurPostprocessRender>  gaussianBlurSecondStepRenderer;
        std::unique_ptr<VignettePostprocessRender>      vignetteRenderer;
		std::unique_ptr<PostprocessRender>     	        scaleRenderer;
        std::vector<std::unique_ptr<Framebuffer>>       shadows;
        std::unique_ptr<Texture2DArray>                 shadowsArray;
        std::vector<std::unique_ptr<CubeMap>>           pointLightShadowsBuffer;
        std::unique_ptr<Framebuffer>                    pointLightFramebuffer;
        
        bool                                            shouldDraw;
        unsigned int                                    shadowResolution;
		unsigned int							        screenWidth;
		unsigned int							        screenHeight;

		unsigned int 							        framebufferWidth;
		unsigned int 							        framebufferHeight;
        
        Renderer                                        chosenRenderer {Renderer::deferred};
		
    };
    
}
