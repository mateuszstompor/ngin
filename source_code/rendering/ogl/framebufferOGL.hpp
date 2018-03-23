//
//  framebufferOGL.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef framebufferOGL_hpp
#define framebufferOGL_hpp

#include <iostream>
#include <cassert>

#include "../framebuffer.hpp"
#include "../../scene/ogl/textureOGL.hpp"
#include "renderbufferOGL.hpp"
#include "../../utils/ogl/proxyOGL.hpp"

namespace ms {
	
	class FramebufferOGL : public Framebuffer {
	
	public:
		
						FramebufferOGL		(int colorAttachmentsAmount,
									   		int renderbufferAttachmentsAmount,
									   		int width,
									   		int height);
		
		virtual void	use					() override;
		virtual void	use_for_read		() override;
		virtual void	use_for_write		() override;
		virtual void	configure			() override;
		virtual void 	bind_color_buffer	(int index, std::shared_ptr<Texture> texture) override;
		virtual void 	bind_color_buffer	(int index, std::shared_ptr<Renderbuffer> renderbuffer) override;
		virtual void 	bind_depth_buffer	(std::shared_ptr<Renderbuffer> renderbuffer) override;
		virtual void 	copy_depth_from		(Framebuffer & frame) override;
		virtual bool	is_complete			() const override;
		virtual void    load  				() override;
		virtual void 	unload 				() override;
		virtual void	clear_color			() override;
		virtual void	clear_depth			() override;
		virtual void	clear_frame			() override;
				GLuint	get_underlying_id	();
//	TODO make it protected
//	protected:
		
		GLuint			framebuffer;
		
	};
	
}

#endif /* framebufferOGL_hpp */
