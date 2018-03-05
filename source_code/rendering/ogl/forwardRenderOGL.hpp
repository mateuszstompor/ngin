//
//  forwardRenderOGL.hpp
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
#include "../shaders/ogl/shaderOGL.hpp"

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
    
    class ForwardRenderOGL : public Render {
		
	public:
		
		ForwardRenderOGL			(std::shared_ptr<std::string> vertexShaderSource, std::shared_ptr<std::string> fragmentShaderSource,
									 unsigned int screenWidth, unsigned int screenHeight, unsigned int frameBufferWidth, unsigned int frameBufferHeight);
		
		void	use     			() 										override;
		void 	clear_frame			()										override;
		void 	draw_scene			(const std::shared_ptr<Scene> &scene) 	override;
		void	load				() 										override;
		bool	is_loaded			() 										override;
		void 	unload				() 										override;
		virtual	~ForwardRenderOGL 	();
		
	private:
		
		std::shared_ptr<std::string> 	vertexShaderSource;
		std::shared_ptr<std::string> 	fragmentShaderSource;
		std::shared_ptr<Shader>			shader;
        
    };
    
}

#endif /* deferredRenderOGL_hpp */
