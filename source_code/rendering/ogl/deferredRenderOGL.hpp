//
//  deferredRenderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferred_render_ogl_hpp
#define deferred_render_ogl_hpp

#include "../../utils/ogl/proxyOGL.hpp"
#include "../deferredRender.hpp"
#include "../shaders/ogl/deferredShaderOGL.hpp"
#include "../shaders/ogl/deferredLightingShaderOGL.hpp"
#include "../reusableGeometry.hpp"
#include "../../utils/ogl/utilsOGL.hpp"
#include "../../scene/ogl/textureOGL.hpp"

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
	
	class DeferredRenderOGL : public DeferredRender {
		
		public:
		
			DeferredRenderOGL			(std::shared_ptr<std::string> gBufferVertexShaderSource,
										 std::shared_ptr<std::string> gBufferFragmentShaderSource,
										 std::shared_ptr<std::string> lightingVertexShaderSource,
										 std::shared_ptr<std::string> lightingFragmentShaderSource,
										 unsigned int screenWidth,
										 unsigned int screenHeight,
										 unsigned int frameBufferWidth,
										 unsigned int frameBufferHeight);
		
			void	use     			() 										override;
			void 	clear_frame			()										override;
			void 	draw_scene			(const Scene * scene) 	override;
			void	load				() 										override;
			bool	is_loaded			() 										override;
			void 	unload				() 										override;
			void 	set_default_FBO		(GLuint defFBO);
			virtual	~DeferredRenderOGL 	() = default;
		
		protected:
		
			GLuint						defaultFBO;

//			GLuint						gPosition;
//			GLuint 						gNormal;
//			GLuint 						gAlbedo;
		
			GLuint						gFrameBuffer;
			GLuint						gRenderBuffer;
		
			GLuint						quadVAO;
			GLuint						quadVBO;
		
	};
	
}

#endif /* deferred_render_ogl_hpp */
