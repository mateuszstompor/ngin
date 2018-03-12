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

// LSB
// 1 bit - debug mode on/off
// 2 bit - albedo/normals, 1 for albedo and 0 for normals
// 3 bit - position/turn on second bit, 1 for turning on position, 0 for turing on albedo or normals
// MSB

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
lightingShader(nullptr),
gNormal(nullptr),
gPosition(nullptr),
gAlbedo(nullptr),
renderMode(0),
debugMode(false),
debugType(DeferredRender::DebugType::position) { }

void ms::DeferredRender::set_render_type (DebugType type) {
	switch (type) {
		case DeferredRender::DebugType::position:
			renderMode = 1;
			break;
		case DeferredRender::DebugType::albedo:
			renderMode = 2;
			break;
		case DeferredRender::DebugType::normals:
			renderMode = 3;
			break;
		case DeferredRender::DebugType::specular:
			renderMode = 4;
			break;
		case DeferredRender::DebugType::standard:
			renderMode = 0;
			break;
		default:
			assert(false);
			//fatal error
			break;
	}
}
