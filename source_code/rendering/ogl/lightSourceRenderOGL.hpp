//
//  lightSourceRenderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 19/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef lightSourceRenderOGL_hpp
#define lightSourceRenderOGL_hpp

#include <memory>

#include "../../utils/ogl/proxyOGL.hpp"
#include "../lightSourcesRender.hpp"
#include "../shaders/ogl/lightSourceDrawerShaderOGL.hpp"

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



namespace ms {
	
	class LightSourceRenderOGL : public LightSourcesRender {
		
	public:
		
		LightSourceRenderOGL			(std::string vertexShaderSource,
										 std::string fragmentShaderSource,
										 unsigned int screenWidth,
										 unsigned int screenHeight,
										 unsigned int frameBufferWidth,
										 unsigned int frameBufferHeight);
		
		void	use     				() 												override;
		void 	clear_frame				()												override;
		void	draw  					(Drawable * node, const Scene * scene)			override;
		void 	setup_uniforms			(const Scene * scene)							override;
		void	load					() 												override;
		bool	is_loaded				() 												override;
		void 	unload					() 												override;
		virtual	~LightSourceRenderOGL 	() = default;
		
	};
	
}

#endif /* lightSourceRenderOGL_hpp */
