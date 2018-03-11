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
settings(0),
debugMode(false),
debugType(DeferredRender::DebugType::position) { }

void ms::DeferredRender::set_debug_mode	(bool isOn) {
	isOn ? settings |= 1 : settings &= ~1;
}

void ms::DeferredRender::set_debug_Type	(DebugType type) {
	switch (type) {
		case DeferredRender::DebugType::position:
			settings |= (1 << 2);
			break;
		case DeferredRender::DebugType::albedo:
			settings |= (1 << 1);
			settings &= ~(1 << 2);
			break;
		case DeferredRender::DebugType::normals:
			settings &= ~(1 << 1);
			settings &= ~(1 << 2);
			break;
		default:
			assert(false);
			//fatal error
			break;
	}
}

bool ms::DeferredRender::is_in_debug () {
	return (this->settings & 1) == 1;
}
