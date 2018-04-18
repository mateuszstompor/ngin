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
#include <vector>
#include <tuple>

#include "render.hpp"
#include "shaders/deferredLightingShader.hpp"
#include "shaders/deferredShader.hpp"
#include "shaders/deferredLightingShader.hpp"
#include "../scene/texture.hpp"
#include "framebuffer.hpp"
#include "renderbuffer.hpp"

namespace ms {
	
	class DeferredRender : public Render {
		
	protected:
		
		using sm_spot_lights = std::vector<std::unique_ptr<Framebuffer>>;
		using lighting_shader 	= std::unique_ptr<DeferredLightingShader>;
		using g_pass_shader 	= std::unique_ptr<DeferredShader>;
		using shadow_shader 	= std::unique_ptr<Shader>;
		
	public:
		
		enum class DebugType;
		
										DeferredRender				(unsigned int 						maxPointLightsAmount,
																	 unsigned int 						maxSpotLightsAmount,
																	 std::unique_ptr<Framebuffer> && 	framebuffer,
																	 g_pass_shader && 					gShader,
																	 lighting_shader &&					lightingShader);
		
				void 					clear_frame					() override;
				void					use			     			() override;
				void 					draw						(Drawable & node, const Scene & scene) override;
		virtual void 					perform_light_pass			(const Scene * scene) = 0;
		virtual void 					setup_material_uniforms		(const Scene * scene, const Drawable * node);
		virtual void 					setup_lightpass_uniforms	(const Scene * scene);
		virtual void 					setup_g_buffer_uniforms		(const Scene * scene);
		virtual void 					set_render_type				(DebugType type);
		virtual 						~DeferredRender				() = default;
		
	protected:
		
		lighting_shader					lightingShader;
		
		unsigned int 					maxPointLightsAmount;
		unsigned int 					maxSpotLightsAmount;
		
		unsigned int					renderMode;
		bool							debugMode;
		DebugType						debugType;
		
		std::unique_ptr<Framebuffer>	gFramebuffer;
		std::shared_ptr<Drawable> 		quad;
		
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
