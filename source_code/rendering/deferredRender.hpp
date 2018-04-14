//
//  deferredRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef defered_render_h
#define defered_render_h

#include <cassert>

#include "render.hpp"
#include "shaders/deferredLightingShader.hpp"
#include "shaders/deferredShader.hpp"
#include "shaders/deferredLightingShader.hpp"
#include "../scene/texture.hpp"
#include "framebuffer.hpp"
#include "renderbuffer.hpp"

namespace ms {
	
	class DeferredRender : public Render {
		
	public:
		
		enum class DebugType;
		
													DeferredRender					(unsigned int maximalAmountOfLights,
																					std::shared_ptr<Framebuffer> framebuffer,
																					std::string gBufferVertexShaderSource,
																					std::string gBufferFragmentShaderSource,
																					std::string lightingVertexShaderSource,
																					std::string lightingFragmentShaderSource,
																					std::string	shadowMappingVertexShader,
																					std::string shadowMappingFragmentShader);
		
				void 								clear_frame						() override;
				void								use				     			() override;
				void 								draw							(Drawable * node, const Scene * scene) override;
		virtual void 								perform_light_pass				(const Scene * scene) = 0;
		virtual void 								setup_material_uniforms			(const Scene * scene, const Drawable * node);
		virtual void 								setup_lightpass_uniforms		(const Scene * scene);
		virtual void 								setup_g_buffer_uniforms			(const Scene * scene);
		virtual void 								set_render_type					(DebugType type);
		virtual 									~DeferredRender					() = default;
		
	protected:
		
		std::string 								gBufferVertexShaderSource;
		std::string 								gBufferFragmentShaderSource;
		
		std::string 								lightingVertexShaderSource;
		std::string 								lightingFragmentShaderSource;
		
		std::string 								shadowMappingVertexShaderSource;
		std::string 								shadowMappingFragmentShaderSource;
		
		std::shared_ptr<Texture>					gPosition;
		std::shared_ptr<Texture>					gAlbedo;
		std::shared_ptr<Texture>					gNormal;
		std::shared_ptr<Renderbuffer>				depthRenderbuffer;
		
		std::shared_ptr<Texture>					shadowTexture;
		
		std::unique_ptr<Shader>						shadowShader;
		std::unique_ptr<DeferredShader>				gShader;
		std::unique_ptr<DeferredLightingShader>		lightingShader;
		
		unsigned int 								maximalAmountOfLights;
		unsigned int								renderMode;
		bool										debugMode;
		DebugType									debugType;
		
		std::unique_ptr<Framebuffer>				shadowFramebuffer;
		
		std::unique_ptr<Framebuffer>				gFramebuffer;
		std::shared_ptr<Drawable> 					quad;
		
	};
	
}

enum class ms::DeferredRender::DebugType {
	standard,
	position,
	normals,
	albedo,
	specular
};



#endif /* defered_render_h */
