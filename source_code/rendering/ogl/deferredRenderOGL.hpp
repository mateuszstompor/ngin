//
//  deferredRenderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef deferredRenderOGL_hpp
#define deferredRenderOGL_hpp

#include <memory>

#include "../render.hpp"
#include "../shaders/shader.hpp"

#if __APPLE__

	#if ios_build

		#import <OpenGLES/ES3/gl.h>
		#import <OpenGLES/ES3/glext.h>

	#else

		#include <OpenGL/gl3.h>
		#include <OpenGL/gl.h>

	#endif

#endif



namespace ms {
    
    class DeferredRenderOGL : public Render {
		
	public:
		
		
		void	use     			() 										override	;
		void 	clear_frame			()										override	;
		void 	draw_scene			(const std::unique_ptr<Scene> &scene) 	override	;
		
		void	initialize			() 										override	;
		bool	is_loaded			() 										override	;
		
		void 	destroy				() 										override	;
		
				~DeferredRenderOGL 	()													;
		
	private:
		
		std::unique_ptr<Shader>		gbufferShader;
        
    };
    
}

#endif /* deferredRenderOGL_hpp */
