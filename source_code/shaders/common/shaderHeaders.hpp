//
//  shaderHeaders.hpp
//  ngin
//
//  Created by Mateusz Stompór on 11/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef shader_headers_hpp
#define shader_headers_hpp

#include <string>

namespace ms {
	
	namespace shader {
		
		static std::string ios_header =
		"#version 300 es\n"
		#ifdef DEBUG
		"#define DEBUG 1\n"
		#endif
		"precision highp float;\n";

		
		static std::string mac_os_header =
		"#version 410 core\n"
		#ifdef DEBUG
		"#define DEBUG 1\n"
		#endif
		;
		
	}
	
}

#endif // shader_headers_hpp

