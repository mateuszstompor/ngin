//
//  forwardRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef forward_render_hpp
#define forward_render_hpp

#include <memory>

#include "render.hpp"
#include "shaders/forwardShader.hpp"
#include "shaders/lightSourceDrawerShader.hpp"

namespace ms {
	
	class ForwardRender : public Render {
	
	public:
		
		 						ForwardRender					(unsigned int 						maximalAmountOfLights,
																 std::unique_ptr<Framebuffer> && 	framebuffer,
																 std::unique_ptr<ForwardShader> && 	shader);
		
		virtual std::string 	get_class						() const override;
		virtual void 			draw  							(Drawable & node, const Scene & scene) override;
		virtual void 			setup_uniforms					(const Scene * scene);
		virtual void 			setup_material_uniforms_for		(const Scene * scene, const Drawable * node);
		
		virtual 				~ForwardRender() = default;
	
	protected:
		
		std::string 			vertexShaderSource;
		std::string 			fragmentShaderSource;
		unsigned int 			maximalAmountOfLights;
				
	};
	
}

#endif /* forward_render_hpp */
