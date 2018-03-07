//
//  deferredRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef defered_render_h
#define defered_render_h

#include "render.hpp"
#include "shaders/deferredLightingShader.hpp"
#include "shaders/deferredShader.hpp"
#include "shaders/deferredLightingShader.hpp"

namespace ms {
	
	class DeferredRender : public Render {
		
	public:
		
		inline 	DeferredRender(unsigned int screenWidth,
							   unsigned int screenHeight,
							   unsigned int frameBufferWidth,
							   unsigned int frameBufferHeight,
							   std::shared_ptr<std::string> gBufferVertexShaderSource,
							   std::shared_ptr<std::string> gBufferFragmentShaderSource,
							   std::shared_ptr<std::string> lightingVertexShaderSource,
							   std::shared_ptr<std::string> lightingFragmentShaderSource
							  );
		
		virtual ~DeferredRender() = default;
		
	protected:
		
		std::shared_ptr<std::string> 				gBufferVertexShaderSource;
		std::shared_ptr<std::string> 				gBufferFragmentShaderSource;
		std::shared_ptr<std::string> 				lightingVertexShaderSource;
		std::shared_ptr<std::string> 				lightingFragmentShaderSource;
		std::shared_ptr<DeferredShader>				gShader;
		std::shared_ptr<DeferredLightingShader>		lightingShader;
		
	};
	
}

ms::DeferredRender::DeferredRender(unsigned int sW,
								   unsigned int sH,
								   unsigned int fbW,
								   unsigned int fbH,
								   std::shared_ptr<std::string> gVS,
								   std::shared_ptr<std::string> gFS,
								   std::shared_ptr<std::string> lVS,
								   std::shared_ptr<std::string> lFS
								 ) : 	Render(sW, sH, fbW, fbH),
										gBufferVertexShaderSource(gVS),
										gBufferFragmentShaderSource(gFS),
										gShader(nullptr),
										lightingShader(nullptr) { }

#endif /* defered_render_h */
