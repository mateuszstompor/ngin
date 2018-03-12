//
//  deferredRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef defered_render_h
#define defered_render_h

#include <cassert>

#include "render.hpp"
#include "shaders/deferredLightingShader.hpp"
#include "shaders/deferredShader.hpp"
#include "shaders/deferredLightingShader.hpp"
#include "../scene/texture.hpp"

namespace ms {
	
	class DeferredRender : public Render {
		
	public:
		
		enum class DebugType;
		
			 			DeferredRender	(	unsigned int screenWidth,
										   unsigned int screenHeight,
										   unsigned int frameBufferWidth,
										   unsigned int frameBufferHeight,
										   std::shared_ptr<std::string> gBufferVertexShaderSource,
										   std::shared_ptr<std::string> gBufferFragmentShaderSource,
										   std::shared_ptr<std::string> lightingVertexShaderSource,
										   std::shared_ptr<std::string> lightingFragmentShaderSource
										 );
		
		virtual void 	set_render_type	(DebugType type);
		virtual 		~DeferredRender	() = default;
		
	protected:
		
		std::shared_ptr<std::string> 				gBufferVertexShaderSource;
		std::shared_ptr<std::string> 				gBufferFragmentShaderSource;
		std::shared_ptr<std::string> 				lightingVertexShaderSource;
		std::shared_ptr<std::string> 				lightingFragmentShaderSource;
		
		std::unique_ptr<Texture>					gPosition;
		std::unique_ptr<Texture>					gAlbedo;
		std::unique_ptr<Texture>					gNormal;
		
		std::unique_ptr<DeferredShader>				gShader;
		std::unique_ptr<DeferredLightingShader>		lightingShader;
		unsigned int								renderMode;
		bool										debugMode;
		DebugType									debugType;
		
	};
	
}

enum class ms::DeferredRender::DebugType {
	standard,
	position,
	normals,
	albedo,
	specular
};

#endif /* defered_render_h */
