//
//  framebuffer.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef framebuffer_hpp
#define framebuffer_hpp

#include <vector>
#include <memory>

#include "../resources/resource.hpp"
#include "../scene/texture.hpp"
#include "renderbuffer.hpp"

namespace ms {
	
	class Framebuffer : public Resource {
		
	public:
		
		inline 			Framebuffer			(int colorAttachmentsAmount,
										 	int renderbufferAttachmentsAmount,
										 	int width,
										 	int height);
						Framebuffer			(const Framebuffer &) = delete;
		Framebuffer & 	operator = 			(const Framebuffer &) = delete;
		
		virtual void 	bind_color_buffer	(int index, std::shared_ptr<Texture> texture) = 0;
		virtual void 	bind_color_buffer	(int index, std::shared_ptr<Renderbuffer> renderbuffer) = 0;
		virtual void 	bind_depth_buffer	(std::shared_ptr<Renderbuffer> renderbuffer) = 0;
		// Checks completeness and merge all things together
inline  virtual void	configure			();
		virtual void	use					() = 0;
		virtual void	use_for_read		() = 0;
		virtual void	use_for_write		() = 0;
		virtual bool	is_complete			() const = 0;
inline  virtual bool	is_configured		() const;
		
	protected:
		
		int 										colorAttachmentsAmount;
		std::vector<std::shared_ptr<Texture>> 		colorAttachments;
		
		int											renderbufferAttachmentsAmount;
		std::vector<std::shared_ptr<Renderbuffer>> 	renderbufferAttachments;
		
		int 										width;
		int 										height;
	
	private:
		
		bool										isConfigured;
		
	};
	
}

ms::Framebuffer::Framebuffer(int colorAttachmentsAmount,
							 int renderbufferAttachmentsAmount,
							 int width,
							 int height) : 	width(width),
											height(height),
											colorAttachmentsAmount(colorAttachmentsAmount),
											renderbufferAttachmentsAmount(renderbufferAttachmentsAmount) {
	colorAttachments.resize(colorAttachmentsAmount);
	renderbufferAttachments.resize(renderbufferAttachmentsAmount);
}

void ms::Framebuffer::configure () {
	isConfigured = true;
}

bool ms::Framebuffer::is_configured () const {
	return isConfigured;
}

#endif /* framebuffer_hpp */
