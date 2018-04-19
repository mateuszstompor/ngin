//
//  shader.hpp
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

#include "../../shaders/shaderHeaders.hpp"
#include "../../utils/ogl/proxyOGL.hpp"
#include "../../resources/resource.hpp"
#include "../../scene/texture.hpp"

namespace ms {
	
	#define AMOUNT_SPOT_LIGHT_PROPERTIES 	5
	#define AMOUNT_POINT_LIGHT_PROPERTIES 	3
	
	namespace spotlight {
		
		#define SL_POWER			0
		#define SL_COLOR			1
		#define SL_POSITION			2
		#define SL_ANGLE_DEGREES	3
		#define SL_DIRECTION		4
		
	}
	
	namespace pointlight {
		
		#define PL_POWER			0
		#define PL_COLOR			1
		#define PL_POSITION			2
		
	}
	
}

namespace ms {
    
	class Shader : public Resource {
		
		using program_ptr = std::unique_ptr<Shader>;
		
	public:
		
		
									Shader					(std::string const & vertexShader,
															 std::string const & tessellationControlShader,
															 std::string const & tessellationEvaluationShader,
															 std::string const & geometryShader,
															 std::string const & fragmentShader);
		
									Shader					(Shader const &) = delete;
					Shader & 		operator = 				(Shader const &) = delete;
		
		virtual		void 			bind_texture			(unsigned int index, Texture & texture) ;
		virtual 	void 			use						();
		virtual 	void 			_load					() override;
		virtual 	void 			_unload					() override;
		virtual 	std::string		get_class				() const override;
		virtual 					~Shader					() = default;
		constexpr 	GLuint 			get_gl_id				() const { return program; }
					GLint			set_uniform				(std::string const & name, int value);
					GLint			set_uniform				(std::string const & name, unsigned int value);
					GLint			set_uniform				(std::string const & name, float value);
					GLint			set_uniform				(std::string const & name, math::mat4 const & m);
					GLint			set_uniform				(std::string const & name, math::mat3 const & m);
					GLint			set_uniform				(std::string const & name, math::vec4 const & v);
					GLint			set_uniform				(std::string const & name, math::vec3 const & v);
		static 		program_ptr		vf_program 				(std::string const & vertexSource,
															 std::string const & fragmentSource);

	protected:
		
		void 						compile_program			();
		void 						compile_shader			(GLuint program,
															 GLuint shader,
															 GLenum shaderType,
															 std::string const & source);
		
		int							get_shader_status		(GLuint shader,
															 GLenum statusType);
		
		std::string 	vertexSource;
		std::string 	tesselationControlSource;
		std::string		tesselationEvalutationSource;
		std::string 	geometrySource;
		std::string 	fragmentSource;
		
		GLuint 			program;
		
	};
    
}

#endif /* shader_ogl_hpp */
