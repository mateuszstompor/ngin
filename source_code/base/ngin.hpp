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
#include "../scene/cubemapArray.hpp"

#include "../utils/loader.hpp"

#include "../shaders/shaderHeaders.hpp"

namespace ms {

	/**
	 * Zewnętrzna klasa, będąca jednocześnie fasadą biblioteki.
	 * Z jej poziomu użytkownik ma dostęp do wszystkich funkcjonalności.
	 * Zapewnione są w niej takie możliwości jak pobranie instancji sceny, a następnie dokonanie modyfikacji obiektów na niej się znajdujących.
	 * Dodatkowo, odwołanie do instancji loader umożliwi pobranie z plików obiektów, które mogą zostać umieszczone na scenie.
	 * Użytkownik ma możliwość także odwołać się do części z klas występujących w potoku renderowania i dokonać ich nieznacznej modyfikacji.
	 * @see Scene
	 * @see Loader
	 * @see PostprocessRender
	 */
	class NGin {
      
    public:
    
        enum class Renderer  {
            deferred,
            forward_fragment,
            forward_vertex
        };
        /**
         *
         * @param screenWidth Szerokość okna w którym należy generować obraz
         * @param screenHeight Wysokość okna w którym należy generować obraz
         * @param frameBufferWidth Szerokość obrazu, którą należy generować w zadanej szerokości okna
         * @param frameBufferHeight Wysokość obrazu, którą należy generować w zadanej szerokości okna
         * @param shadowsResolution Rozdzielczość cieni
         * @param cam kamera, którą należy dodać na scenę
         * @param defaultFramebuffer w przypadku urządzeń, które nie korzystają z domyślnego bufora ramki, np. smartfony z iOS, należy podać bufor ramki na który generować obraz
         */
                                                        NGin        		(unsigned int                       screenWidth,
                                                                             unsigned int                       screenHeight,
                                                                             unsigned int                       frameBufferWidth,
                                                                             unsigned int                       frameBufferHeight,
                                                                             unsigned int                       shadowsResolution,
                                                                             std::unique_ptr<Camera> &&			cam,
                                                                             std::unique_ptr<Framebuffer> &&    defaultFramebuffer = nullptr);
        /**
         *
         * @param frameWidth Szerokość obrazu i okna w której należy generować grafikę
         * @param frameHeight Wysokość obrazu i okna w której należy generować grafikę
         * @param shadowsResolution Rozdzielczość cieni
         * @param cam kamera, którą należy dodać na scenę
         * @param defaultFramebuffer w przypadku urządzeń, które nie korzystają z domyślnego bufora ramki, np. smartfony z iOS, należy podać bufor ramki na który generować obraz
         */
                                                        NGin                (unsigned int                       frameWidth,
                                                                             unsigned int                       frameHeight,
                                                                             unsigned int                       shadowsResolution,
                                                                             std::unique_ptr<Camera> &&         cam,
                                                                             std::unique_ptr<Framebuffer> &&    defaultFramebuffer = nullptr);

		/**
		 * Ładuje do karty graficznej wszystkie komponenty silnika jak renderery, shadery, itd.
		 */
        void 							                load 				();
		/**
		 * Dealokuje wszystkie zasoby.
		 */
        void 							                unload 				();
		/**
		 * Wyrysowuje pojedynczą klatkę obrazu.
		 */
        void							                draw_scene  		();
		/**
		 * Zwraca loader, za pomocą którego użytkownik może pobierać z pamięciu urządzenia zasoby.
		 */
        constexpr Loader const &                        get_loader          () const { return loader; }
        DeferredRender & 		                        get_deferred_render	();
		/**
		 * Zwraca renderer odpowiadający za winietowanie.
		 */
		VignettePostprocessRender &						get_vignette_render ();
		/**
		 * @return Zwraca renderer odpowiedzialny za wertykalny blur gaussowski podczas nakładania efektu bloom.
		 */
		GaussianBlurPostprocessRender &					get_bloom_v_render	();
		/**
		 * @return Zwraca renderer odpowiedzialny za horyzontalny blur gaussowski podczas nakładania efektu bloom.
		 */
		GaussianBlurPostprocessRender &					get_bloom_h_render	();
		/**
		 * @return Zwraca renderer odpowiedzialny za mapowanie klatki hdr do sdr.
		 */
		PostprocessRender &								get_hdr_render		();
		/**
		 * Wczytuje model do sceny, powiązuje jego komponenty ze sobą.
		 * @param absolutePath ścieżka do pliku
		 */
        void							                load_model			(std::string const & absolutePath);
		/**
		 * Wstrzymuje pracę biblioteki.
		 */
        void                                            pause_drawing       () { shouldDraw = false; }
		/**
		 * Kontynuuje pracę biblioteki.
		 */
        void                                            resume_drawing      () { shouldDraw = true; }
		/**
		 * Zwraca scenę, którą użytkownik może swobodnie modyfikować
		 */
        constexpr Scene &                               get_scene           () { return scene; }
		
	private:

		void                                            set_renderer        (Renderer r);
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
        std::unique_ptr<Framebuffer>                    directionalLightFramebuffer;
        std::unique_ptr<Texture2DArray>                 spotLightShadows;
        #ifndef ios_build
        std::unique_ptr<CubemapArray>                   pointLightMaps;
        #else
        std::unique_ptr<CubeMap>                        pointLightMap;
        #endif
        std::unique_ptr<Framebuffer>                    pointLightFramebuffer;
        std::unique_ptr<Framebuffer>                    spotLightFramebuffer;
        
        bool                                            shouldDraw;
        unsigned int                                    shadowResolution;
		unsigned int							        screenWidth;
		unsigned int							        screenHeight;
		unsigned int 							        framebufferWidth;
		unsigned int 							        framebufferHeight;
        Renderer                                        chosenRenderer {Renderer::deferred};
		Loader					                        loader;
		
    };
    
}
