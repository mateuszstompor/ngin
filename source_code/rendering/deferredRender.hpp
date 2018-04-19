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
#include "../scene/texture.hpp"
#include "framebuffer.hpp"
#include "renderbuffer.hpp"
#include "../utils/ogl/proxyOGL.hpp"
#include "reusableGeometry.hpp"
#include "../utils/ogl/utilsOGL.hpp"
#include "../scene/drawable.hpp"

namespace ms {
	
	class DeferredRender : public Render {
		
	public:
		
		enum class DebugType;
		
										DeferredRender				(unsigned int 						maxPointLightsAmount,
																	 unsigned int 						maxSpotLightsAmount,
																	 std::unique_ptr<Framebuffer> && 	framebuffer,
																	 std::unique_ptr<Shader> && 		gShader,
																	 std::unique_ptr<Shader> &&			lightingShader);
		
		void							use			     			() override;
		void 							draw						(Drawable & node, const Scene & scene) override;
		void 							perform_light_pass			(const Scene * scene);
		void 							setup_material_uniforms		(const Scene * scene, const Drawable * node);
		void 							setup_lightpass_uniforms	(const Scene * scene);
		void 							setup_g_buffer_uniforms		(const Scene * scene);
		void 							set_render_type				(DebugType type);
		void							_load						() override;
		void 							_unload						() override;

//	private:
		
		std::unique_ptr<Shader>			lightingShader;
		
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
