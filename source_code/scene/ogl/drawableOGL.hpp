//
//  drawableOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef drawable_ogl_hpp
#define drawable_ogl_hpp

#ifdef __WIN32__

	#include <glad/glad.h>

#endif

#if __APPLE__

	#ifdef ios_build

		#import <OpenGLES/ES3/gl.h>
		#import <OpenGLES/ES3/glext.h>

	#elif mac_build

		#include <OpenGL/gl3.h>

	#endif

#endif

#include "../drawable.hpp"
#include "../../utils/ogl/proxyOGL.hpp"

namespace ms {
	
	class DrawableOGL : public virtual Drawable {
		
	public:
		
		friend class Loader;
		
				DrawableOGL	();
		void 	use				() override;
		void	load			() override;
		void 	unload			() override;
		virtual	~DrawableOGL()	= default;
		
	protected:
		
		GLuint 	vertexArray;
		
	};
	
}

#endif /* drawable_ogl_hpp */
