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

#include "../../libs/source/glMath.h"
#include "../resources/resource.hpp"
#include "../scene/texture.hpp"
#include "renderbuffer.hpp"

namespace ms {
	
	class Framebuffer : public Resource {
		
	protected:
		
		typedef std::vector<std::shared_ptr<Texture>> 		colors_att;
		typedef std::vector<std::shared_ptr<Renderbuffer>> 	render_att;
		
	public:
		
		inline 						Framebuffer			(int colorAttachmentsAmount,
														int renderbufferAttachmentsAmount,
														int width,
														int height);
									Framebuffer			(const Framebuffer &) = delete;
		Framebuffer & 				operator = 			(const Framebuffer &) = delete;
		
		virtual void 				bind_color_buffer	(int index,
														 std::shared_ptr<Texture> texture) = 0;
		virtual void 				bind_color_buffer	(int index,
														 std::shared_ptr<Renderbuffer> renderbuffer) = 0;
		virtual void 				bind_depth_buffer	(std::shared_ptr<Renderbuffer> renderbuffer) = 0;
		// Checks completeness and merge all things together
inline  virtual void				configure			();
		virtual void				use					() = 0;
		virtual void				use_for_read		() = 0;
		virtual void 				copy_depth_from		(Framebuffer & frame) = 0;
		virtual void				use_for_write		() = 0;
		virtual bool				is_complete			() const = 0;
inline  virtual bool				is_configured		() const;
		virtual void				clear_color			() = 0;
		virtual void				clear_depth			() = 0;
		virtual void				clear_frame			() = 0;
		inline	void				set_clear_color		(math::vec4 color);
		inline 	int					get_height			() const;
		inline 	int					get_width			() const;
		inline 	const colors_att & 	get_colors();
		
		virtual						~Framebuffer		() = default;

	protected:
		
		int 										width;
		int 										height;
		
		math::vec4									clearingColor;
		int 										colorAttachmentsAmount;
		colors_att 									colorAttachments;

		int											renderbufferAttachmentsAmount;
		render_att 									renderbufferAttachments;
		
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

void ms::Framebuffer::set_clear_color (math::vec4 color) {
	this->clearingColor = color;
}

void ms::Framebuffer::configure () {
	isConfigured = true;
}

bool ms::Framebuffer::is_configured () const {
	return isConfigured;
}

int ms::Framebuffer::get_height () const {
	return height;
}

int ms::Framebuffer::get_width () const {
	return width;
}

const ms::Framebuffer::colors_att & ms::Framebuffer::get_colors() {
	return colorAttachments;
}

#endif /* framebuffer_hpp */
