//
//  shaderOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef shader_ogl_hpp
#define shader_ogl_hpp

#include <memory>
#include <iostream>
#include <cstdlib>

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

#include "../../../shaders/shaderHeaders.hpp"
#include "../../../utils/ogl/proxyOGL.hpp"
#include "../shader.hpp"

namespace ms {
    
	class ShaderOGL : public virtual Shader {
		
	public:
		
						ShaderOGL(std::string vertexShader,
								  std::string tessellationControlShader,
								  std::string tessellationEvaluationShader,
								  std::string geometryShader,
								  std::string fragmentShader);
		
		virtual void 	use			() override;
		virtual void 	load		() override;
		virtual void 	unload		() override;
		virtual 		~ShaderOGL	() = default;
		
	protected:
		
		void 			compile_program();
		void 			compile_shader(GLuint program, GLuint shader, GLenum shaderType, std::string source);
		int				get_shader_status(GLuint shader, GLenum statusType);
		
		std::string 		vertexSource;
		std::string 		tesselationControlSource;
		std::string 		tesselationEvalutationSource;
		std::string 		geometrySource;
		std::string 		fragmentSource;
		
		GLuint 			program;
		
	};
    
}

#endif /* shader_ogl_hpp */
