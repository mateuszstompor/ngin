//
//  utilsOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 09/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef utils_ogl_hpp
#define utils_ogl_hpp

#include <iostream>

#ifdef __WIN32__

	#include <glad/glad.h>

#endif

#if __APPLE__

	#ifdef ios_build

		#import <OpenGLES/ES3/gl.h>
		#import <OpenGLES/ES3/glext.h>

	#elif mac_build

		#include <OpenGL/gl3.h>
		#include <OpenGL/gl.h>

	#endif

#endif

namespace ms {
	
	namespace utils {
		
		void _check_gl_error(const char * fileName, int line);
		
		#define check_gl_error() _check_gl_error(__FILE__,__LINE__)
		
	}
	
}



#endif /* utils_ogl_hpp */
