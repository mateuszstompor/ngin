//
//  shaderHeaders.hpp
//  ngin
//
//  Created by Mateusz Stompór on 11/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef shader_headers_hpp
#define shader_headers_hpp

#include <string>
#include <cassert>

namespace ms {
	
	namespace shader {
		
		enum class Type;
		
	#ifdef ios_build
		
		static std::string os_header =
		"#version 300 es\n"
		#ifdef DEBUG
		"#define DEBUG 1\n"
		#endif
		"precision highp float;\n";
		
	#else
		
		static std::string os_header =
		"#version 410 core\n"
		#ifdef DEBUG
		"#define DEBUG 1\n"
		#endif
		;
		
	#endif
		
		const std::string lightsDeclarations =
		#include "./lightDeclarations.glsl"
		
		const std::string constantsDefinitions =
		#include "./constantsDefinitions.glsl"
		
		const std::string materialsDeclarations =
		#include "./materialDeclarations.glsl"
		
		const std::string functionsDefinitions =
		#include "./functions.glsl"
		
		namespace deferredrenderer {
			
			const std::string gBufferPassVertexShader =
			#include "./deferred_render/g_buf_vshader.glsl"
			
			const std::string gBufferPassFragmentShader =
			#include "./deferred_render/g_buf_fshader.glsl"
			
			const std::string lightingPassVertexShader =
			#include "./deferred_render/lighting_vshader.glsl"
			
			const std::string lightingPassFragmentShader =
			#include "./deferred_render/lighting_fshader.glsl"
	
		}
		
		namespace forwardrenderer {
			
			const std::string phongVertexShader =
			#include "./forward_render/vshader.glsl"
			
			const std::string phongFragmentShader =
			#include "./forward_render/fshader.glsl"
			
			const std::string lightSourceDrawerVertexShader =
			#include "./forward_render/light_source_drawer_vshader.glsl"
			
			const std::string lightSourceDrawerFragmentShader =
			#include "./forward_render/light_source_drawer_fshader.glsl"
			
		}
		
		inline std::string get_shader_of_type(Type type);
		
	}
	
}

enum class ms::shader::Type {
	deferred_render_g_buf_vertex_shader,
	deferred_render_g_buf_fragment_shader,
	deferred_render_light_pass_vshader,
	deferred_render_light_pass_fshader,
	forward_render_phong_vshader,
	forward_render_phong_fshader,
	forward_render_light_drawer_vshader,
	forward_render_light_drawer_fshader
};


std::string ms::shader::get_shader_of_type(Type type) {
	
	std::string shaderContent = "";

	shaderContent += os_header;

	switch(type) {
		case ms::shader::Type::deferred_render_g_buf_vertex_shader:
			shaderContent += deferredrenderer::gBufferPassVertexShader;
			break;
		case ms::shader::Type::deferred_render_g_buf_fragment_shader:
			shaderContent += materialsDeclarations;
			shaderContent += deferredrenderer::gBufferPassFragmentShader;
			break;
		case ms::shader::Type::deferred_render_light_pass_vshader:
			shaderContent += deferredrenderer::lightingPassVertexShader;
			break;
		case ms::shader::Type::deferred_render_light_pass_fshader:
			shaderContent += constantsDefinitions;
			shaderContent += lightsDeclarations;
			shaderContent += functionsDefinitions;
			shaderContent += deferredrenderer::lightingPassFragmentShader;
			break;
		case ms::shader::Type::forward_render_phong_vshader:
			shaderContent += forwardrenderer::phongVertexShader;
			break;
		case ms::shader::Type::forward_render_phong_fshader:
			shaderContent += constantsDefinitions;
			shaderContent += lightsDeclarations;
			shaderContent += materialsDeclarations;
			shaderContent += functionsDefinitions;
			shaderContent += forwardrenderer::phongFragmentShader;
			break;
		case ms::shader::Type::forward_render_light_drawer_vshader:
			shaderContent += forwardrenderer::lightSourceDrawerVertexShader;
			break;
		case ms::shader::Type::forward_render_light_drawer_fshader:
			shaderContent += forwardrenderer::lightSourceDrawerFragmentShader;
			break;
		default:
			std::cout << "critical error, shader type not recognized" << std::endl;
			assert(false);
			break;
	}

	return shaderContent;
}

#endif // shader_headers_hpp

