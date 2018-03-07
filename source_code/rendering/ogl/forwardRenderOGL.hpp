//
//  forwardRenderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef forward_render_ogl_hpp
#define forward_render_ogl_hpp

#include <memory>

#include "../render.hpp"
#include "../shaders/forwardShader.hpp"
#include "../forwardRender.hpp"
#include "../shaders/ogl/forwardShaderOGL.hpp"

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
    
    class ForwardRenderOGL : public ForwardRender {
		
	public:
		
		ForwardRenderOGL			(std::shared_ptr<std::string> vertexShaderSource, std::shared_ptr<std::string> fragmentShaderSource,
									 unsigned int screenWidth, unsigned int screenHeight, unsigned int frameBufferWidth, unsigned int frameBufferHeight);
		
		void	use     			() 										override;
		void 	clear_frame			()										override;
		void 	draw_scene			(const std::shared_ptr<Scene> &scene) 	override;
		void	load				() 										override;
		bool	is_loaded			() 										override;
		void 	unload				() 										override;
		virtual	~ForwardRenderOGL 	() = default;
        
    };
    
}

#endif /* forward_render_ogl_hpp */
