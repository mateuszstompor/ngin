//
//  shader.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <iostream>
#include <cstdlib>

#include "../../shaders/shaderHeaders.hpp"
#include "../../utils/proxyOGL.hpp"
#include "../../resources/resource.hpp"
#include "../../scene/texture.hpp"

namespace ms {
    
	class Shader : public Resource {
		
		using program_ptr = std::unique_ptr<Shader>;
		
		friend class NGin;
		friend class Render;
		friend class PostprocessDrawer;
		friend class DLShadowRender;
		friend class ForwardRender;
		friend class DeferredRender;
		friend class LightSourcesRender;
		
	public:
		
		virtual 					~Shader					() = default;

	private:
		
									Shader					(std::string const & vertexShader,
															 std::string const & tessellationControlShader,
															 std::string const & tessellationEvaluationShader,
															 std::string const & geometryShader,
															 std::string const & fragmentShader);
		
									Shader					(Shader const &) = delete;
					Shader & 		operator = 				(Shader const &) = delete;
		
		virtual		void 			bind_texture			(std::size_t index, Texture2D & texture) ;
		virtual 	void 			use						();
		virtual 	void 			_load					() override;
		virtual 	void 			_unload					() override;
		virtual 	std::string		get_class				() const override;
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
		void 						compile_program			();
		void 						compile_shader			(GLuint program,
															 GLuint shader,
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
