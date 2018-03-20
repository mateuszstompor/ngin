//
//  deferredRender.cpp
//  ngin
//
//  Created by Mateusz Stompór on 11/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "deferredRender.hpp"

//	//	//	//	//	//	//	//	//	//	//	//
//		SETTINGS UNSIGNED INTEGER LAYOUT	//
//	//	//	//	//	//	//	//	//	//	//	//

// def. Najbardziej znaczący bit (ang. most significant bit, MSB), zwany też najstarszym bitem
// def. Najmniej znaczący bit (ang. least significant bit, LSB), zwany też najmłodszym bitem


namespace ms {

	#define RENDER_MODE_STANDARD	0
	#define RENDER_MODE_POSITION 	1
	#define RENDER_MODE_ALBEDO 		2
	#define RENDER_MODE_NORMALS 	3
	#define RENDER_MODE_SPECULAR 	4
	
}

ms::DeferredRender::DeferredRender(unsigned int maxAOL,
								   unsigned int sW,
								   unsigned int sH,
								   unsigned int fbW,
								   unsigned int fbH,
								   std::string gVS,
								   std::string gFS,
								   std::string lVS,
								   std::string lFS
								   ) : 	Render(sW, sH, fbW, fbH),
gBufferVertexShaderSource(gVS),
gBufferFragmentShaderSource(gFS),
lightingVertexShaderSource(lVS),
lightingFragmentShaderSource(lFS),
maximalAmountOfLights(maxAOL),
gShader(nullptr),
lightingShader(nullptr),
gNormal(nullptr),
gPosition(nullptr),
gAlbedo(nullptr),
renderMode(0),
debugMode(false),
debugType(DeferredRender::DebugType::position) { }

void ms::DeferredRender::set_render_type (DebugType type) {
	switch (type) {
		case DeferredRender::DebugType::standard:
			renderMode = RENDER_MODE_STANDARD;
			break;
		case DeferredRender::DebugType::position:
			renderMode = RENDER_MODE_POSITION;
			break;
		case DeferredRender::DebugType::albedo:
			renderMode = RENDER_MODE_ALBEDO;
			break;
		case DeferredRender::DebugType::normals:
			renderMode = RENDER_MODE_NORMALS;
			break;
		case DeferredRender::DebugType::specular:
			renderMode = RENDER_MODE_SPECULAR;
			break;
		default:
			assert(false);
			//fatal error
			break;
	}
}
