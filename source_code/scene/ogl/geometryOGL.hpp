//
//  geometryOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef model_ogl_hpp
#define model_ogl_hpp

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

#include "../geometry.hpp"

namespace ms {
	
	class GeometryOGL : public Geometry {
		
	public:
		
		void 	use_normals 			() override;
		void 	use_vertices 			() override;
		void 	load					() override;
		void 	unload					() override;
		void 	load_vector_to_buffer	(const std::vector<float> &, GLuint*);
		
	protected:
		
		GLuint 	normalsBuffer;
		GLuint 	verticesBuffer;
		
	};
	
}

#endif /* model_ogl_hpp */
