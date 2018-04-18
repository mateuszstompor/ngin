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
#include <memory>

#include "../framebuffer.hpp"
#include "../../scene/texture.hpp"
#include "../renderbuffer.hpp"
#include "../../utils/ogl/proxyOGL.hpp"

namespace ms {
	
	class FramebufferOGL : public Framebuffer {
	
	protected:
		
		typedef std::unique_ptr<FramebufferOGL> fb_ptr;
		
	public:
		
							FramebufferOGL					(int colorAttachmentsAmount,
												 			int renderbufferAttachmentsAmount,
												 			int width,
												 			int height);
		
							FramebufferOGL & operator = 	(const FramebufferOGL &) = delete;
		
		virtual void		use								() override;
		virtual void		use_for_read					() override;
		virtual void		use_for_write					() override;
		virtual void		configure						() override;
		virtual void 		bind_color_buffer				(int index,
															 std::unique_ptr<Texture> && texture) override;
		virtual void 		bind_color_buffer				(int index,
															 std::unique_ptr<Renderbuffer> && renderbuffer) override;
		virtual void 		bind_depth_buffer				(std::unique_ptr<Renderbuffer> && renderbuffer) override;
        virtual void        bind_depth_buffer               (std::unique_ptr<Texture> && texture) override;
		virtual void 		copy_depth_from					(Framebuffer & frame, Texture::MagFilter filter) override;
        virtual void        copy_color_from                 (Framebuffer & frame, Texture::MagFilter filter) override;
        virtual void        copy_framebuffer                (Framebuffer & frame, Texture::MagFilter filter) override;
		virtual bool		is_complete						() const override;
		virtual void    	_load  							() override;
		virtual void 		_unload 						() override;
		virtual std::string	get_class						() override;
		virtual void		clear_color						() override;
		virtual void		clear_depth						() override;
		virtual void		clear_frame						() override;
		static 	fb_ptr 		window_framebuffer				(int width, int height);
				GLuint		get_underlying_id				() const;
				void		set_underlying_id				(GLuint framebufferID);
	protected:
		
		bool				is_default_framebuffer;
		GLuint				framebuffer;
		
	};
	
}

#endif /* framebufferOGL_hpp */
