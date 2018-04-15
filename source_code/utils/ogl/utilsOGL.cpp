//
//  utilsOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 09/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "utilsOGL.hpp"

#include <cassert>

void ms::utils::_check_gl_error(const char *file, int line) {
	
	GLenum err (glGetError());
	
	while(err!=GL_NO_ERROR) {
		
		std::string error;
		
		switch(err) {
			case GL_INVALID_OPERATION: 				error = "INVALID_OPERATION";      			break;
			case GL_INVALID_ENUM:          			error = "INVALID_ENUM";           			break;
			case GL_INVALID_VALUE:          		error = "INVALID_VALUE";          			break;
			case GL_OUT_OF_MEMORY:          		error = "OUT_OF_MEMORY";          			break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  	break;
		}
		
		std::cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<std::endl;
		err=glGetError();
        assert(false);
        
	}
	
}
