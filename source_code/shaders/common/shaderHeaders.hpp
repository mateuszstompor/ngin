//
//  shaderHeaders.hpp
//  ngin
//
//  Created by Mateusz Stompór on 11/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef shader_headers
#define shader_headers

#include <string>

namespace ms {
	
	namespace shader {
		
		static std::string ios_header =
		"#version 300 es\n"
		#ifdef DEBUG
		"#define DEBUG 1"
		#endif
		"precision highp float;";

		
		static std::string mac_os_header =
		"#version 410 core"
		#ifdef DEBUG
		"#define DEBUG 1"
		#endif
		;
		
	}
	
}

#endif

