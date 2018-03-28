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
		
		const std::string constantsDefinitions =
		#include "./constantsDefinitions.glsl"
		
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
			#include "./forward_render/phong/phong_vshader.glsl"
			
			const std::string phongFragmentShader =
			#include "./forward_render/phong/phong_fshader.glsl"
			
			const std::string gouraudVertexShader =
			#include "./forward_render/gouraud/gouraud_vshader.glsl"
			
			const std::string gouraudFragmentShader =
			#include "./forward_render/gouraud/gouraud_fshader.glsl"
			
			const std::string lightSourceDrawerVertexShader =
			#include "./forward_render/light_source_drawer/light_source_drawer_vshader.glsl"
			
			const std::string lightSourceDrawerFragmentShader =
			#include "./forward_render/light_source_drawer/light_source_drawer_fshader.glsl"
			
		}
		
		namespace postprocess {
			
			const std::string passTroughVshader =
			#include "./postprocess/pass_trough_vshader.glsl"
			
			const std::string hdrFshader =
			#include "./postprocess/hdr/hdr_fshader.glsl"
			
			const std::string bloom_merger_fshader =
			#include "./postprocess/bloom/bloom_merger_fshader.glsl"
			
			const std::string bloom_splitter_fshader =
			#include "./postprocess/bloom/bloom_splitter_fshader.glsl"
			
			const std::string gaussian_blur_fshader =
			#include "./postprocess/gaussian_blur/gaussian_blur_fshader.glsl"
			
			const std::string scale_fshader =
			#include "./postprocess/scale_fshader.glsl"
            
            const std::string vignette_fshader =
            #include "./postprocess/vignette_fshader.glsl"
			
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
	forward_render_gouraud_vshader,
	forward_render_gouraud_fshader,
	forward_render_light_drawer_vshader,
	forward_render_light_drawer_fshader,
	post_process_hdr_vshader,
	post_process_hdr_fshader,
	post_process_bloom_merger_vshader,
	post_process_bloom_merger_fshader,
	post_process_bloom_splitter_vshader,
	post_process_bloom_splitter_fshader,
	post_process_gaussian_blur_vshader,
	post_process_gaussian_blur_fshader,
	post_process_scale_fshader,
	post_process_scale_vshader,
    post_process_vignette_vshader,
    post_process_vignette_fshader
};


std::string ms::shader::get_shader_of_type(Type type) {
	
	std::string shaderContent = "";

	shaderContent += os_header;

	switch(type) {
		case ms::shader::Type::deferred_render_g_buf_vertex_shader:
			shaderContent += deferredrenderer::gBufferPassVertexShader;
			break;
		case ms::shader::Type::deferred_render_g_buf_fragment_shader:
			shaderContent += deferredrenderer::gBufferPassFragmentShader;
			break;
		case ms::shader::Type::deferred_render_light_pass_vshader:
			shaderContent += deferredrenderer::lightingPassVertexShader;
			break;
		case ms::shader::Type::deferred_render_light_pass_fshader:
			shaderContent += constantsDefinitions;
			shaderContent += functionsDefinitions;
			shaderContent += deferredrenderer::lightingPassFragmentShader;
			break;
		case ms::shader::Type::forward_render_phong_vshader:
			shaderContent += forwardrenderer::phongVertexShader;
			break;
		case ms::shader::Type::forward_render_phong_fshader:
			shaderContent += constantsDefinitions;
			shaderContent += functionsDefinitions;
			shaderContent += forwardrenderer::phongFragmentShader;
			break;
		case ms::shader::Type::forward_render_light_drawer_vshader:
			shaderContent += forwardrenderer::lightSourceDrawerVertexShader;
			break;
		case ms::shader::Type::forward_render_light_drawer_fshader:
			shaderContent += forwardrenderer::lightSourceDrawerFragmentShader;
			break;
		case ms::shader::Type::forward_render_gouraud_vshader:
			shaderContent += constantsDefinitions;
			shaderContent += functionsDefinitions;
			shaderContent += forwardrenderer::gouraudVertexShader;
			break;
		case ms::shader::Type::forward_render_gouraud_fshader:
			shaderContent += forwardrenderer::gouraudFragmentShader;
			break;
		case ms::shader::Type::post_process_hdr_fshader:
			shaderContent += functionsDefinitions;
			shaderContent += postprocess::hdrFshader;
			break;
		case ms::shader::Type::post_process_bloom_merger_vshader:
			shaderContent += postprocess::passTroughVshader;
			break;
		case ms::shader::Type::post_process_bloom_merger_fshader:
			shaderContent += functionsDefinitions;
			shaderContent += postprocess::bloom_merger_fshader;
			break;
		case ms::shader::Type::post_process_bloom_splitter_vshader:
			shaderContent += functionsDefinitions;
			shaderContent += postprocess::passTroughVshader;
			break;
		case ms::shader::Type::post_process_bloom_splitter_fshader:
			shaderContent += functionsDefinitions;
			shaderContent += postprocess::bloom_splitter_fshader;
			break;
		case ms::shader::Type::post_process_hdr_vshader:
		case ms::shader::Type::post_process_scale_vshader:
        case ms::shader::Type::post_process_vignette_vshader:
		case ms::shader::Type::post_process_gaussian_blur_vshader:
			shaderContent += postprocess::passTroughVshader;
			break;
        case ms::shader::Type::post_process_vignette_fshader:
            shaderContent += functionsDefinitions;
            shaderContent += postprocess::vignette_fshader;
            break;
		case ms::shader::Type::post_process_gaussian_blur_fshader:
			shaderContent += postprocess::gaussian_blur_fshader;
			break;
		case ms::shader::Type::post_process_scale_fshader:
			shaderContent += postprocess::scale_fshader;
			break;
		default:
			std::cout << "critical error, shader type not recognized" << std::endl;
			assert(false);
			break;
	}

	return shaderContent;
}

#endif // shader_headers_hpp

