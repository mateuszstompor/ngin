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
		
			 			DeferredRender		(	unsigned int maximalAmountOfLights,
											 	unsigned int screenWidth,
												unsigned int screenHeight,
												unsigned int frameBufferWidth,
												unsigned int frameBufferHeight,
												std::string gBufferVertexShaderSource,
												std::string gBufferFragmentShaderSource,
												std::string lightingVertexShaderSource,
												std::string lightingFragmentShaderSource
											 );
		virtual void 	perform_light_pass	(const Scene * scene) = 0;
		virtual void 	set_render_type		(DebugType type);
		virtual 		~DeferredRender		() = default;
		
	protected:
		
		std::string 								gBufferVertexShaderSource;
		std::string 								gBufferFragmentShaderSource;
		std::string 								lightingVertexShaderSource;
		std::string 								lightingFragmentShaderSource;
		
		std::unique_ptr<Texture>					gPosition;
		std::unique_ptr<Texture>					gAlbedo;
		std::unique_ptr<Texture>					gNormal;
		
		std::unique_ptr<DeferredShader>				gShader;
		std::unique_ptr<DeferredLightingShader>		lightingShader;
		
		unsigned int 								maximalAmountOfLights;
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
