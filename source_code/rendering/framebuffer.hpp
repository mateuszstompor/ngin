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


				//											//
				//											//
				// 	DEPTH TESTING IS TURNED ON BY DEFAULT	//
				//											//
				//											//


namespace ms {
	
	class Framebuffer : public Resource {
		
	protected:
		
		using strong_color_texbuffers       = std::vector<std::shared_ptr<Texture>>;
        using strong_depth_texbuffer        = std::shared_ptr<Texture>;
        
		using strong_color_renderbuffers    = std::vector<std::shared_ptr<Renderbuffer>>;
        using strong_depth_renderbuffer     = std::shared_ptr<Renderbuffer>;
        
        using weak_color_texbuffers         = std::vector<std::weak_ptr<Texture>>;
        using weak_depth_texbuffer          = std::weak_ptr<Texture>;
    
        using weak_color_renderbuffers      = std::vector<std::weak_ptr<Renderbuffer>>;
        using weak_depth_renderbuffer       = std::weak_ptr<Renderbuffer>;
		
	public:
		
        inline 						    Framebuffer			(int    colorAttachmentsAmount,
                                                             int    renderbufferAttachmentsAmount,
                                                             int 	width,
                                                             int 	height);
		
									    Framebuffer			(const Framebuffer &) = delete;
		Framebuffer & 				    operator = 			(const Framebuffer &) = delete;
		
		virtual void 				    bind_color_buffer	(int index,
                                                            std::unique_ptr<Texture> && texture) = 0;
        
		virtual void 				    bind_color_buffer	(int index,
                                                             std::unique_ptr<Renderbuffer> && renderbuffer) = 0;
        
		virtual void 					bind_depth_buffer	(std::unique_ptr<Renderbuffer> && renderbuffer) = 0;
        virtual void                    bind_depth_buffer   (std::unique_ptr<Texture> && texture) = 0;
		
					//														//
					// 	Checks completeness and merge all things together	//
					//														//
		
inline  virtual void					configure			();
		virtual void					use					() = 0;
		virtual void					use_for_read		() = 0;
inline	virtual std::string			    get_class			() = 0;
        virtual void 					copy_depth_from		(Framebuffer & frame, Texture::MagFilter filter = Texture::MagFilter::nearest) = 0;
        virtual void                    copy_color_from     (Framebuffer & frame, Texture::MagFilter filter = Texture::MagFilter::nearest) = 0;
        virtual void                    copy_framebuffer    (Framebuffer & frame, Texture::MagFilter filter = Texture::MagFilter::nearest) = 0;
		virtual void				    use_for_write		() = 0;
		virtual bool				    is_complete			() const = 0;
inline  virtual bool				    is_configured		() const;
		virtual void				    clear_color			() = 0;
		virtual void				    clear_depth			() = 0;
		virtual void				    clear_frame			() = 0;
inline	virtual void				    set_depth_test		(bool enabled);
		inline	void				    set_clear_color		(math::vec4 const & color);
		inline 	int					    get_height			() const;
		inline 	int					    get_width			() const;
        
		inline weak_color_texbuffers 	get_colors();
        inline weak_depth_texbuffer     get_depth_texture();
		
		virtual						    ~Framebuffer		() = default;

	protected:
		
		int 						    width;
		int 							height;
		bool							isDepthTestEnabled;
		
		math::vec4						clearingColor;
        
		int 							colorTextureAttachmentsAmount;
		strong_color_texbuffers 		colorTextureAttachments;
        strong_depth_texbuffer          depthTexture;
        
        int                             colorRenderbufferAttachmentsAmount;
        strong_color_renderbuffers      colorRenderbufferAttachments;
        strong_depth_renderbuffer       depthRenderbuffer;
        
	private:
		
		bool							isConfigured;
		
	};
	
}

ms::Framebuffer::Framebuffer(int colorAttachmentsAmount,
							 int renderbufferAttachmentsAmount,
							 int width,
							 int height) : 	width(width),
											height(height),
											colorTextureAttachmentsAmount(colorAttachmentsAmount),
											colorRenderbufferAttachmentsAmount(renderbufferAttachmentsAmount),
											isDepthTestEnabled(true) {
												
	colorTextureAttachments.resize(colorAttachmentsAmount);
	colorRenderbufferAttachments.resize(renderbufferAttachmentsAmount);
}

std::string ms::Framebuffer::get_class () {
	return "ms::Framebuffer";
}

void ms::Framebuffer::set_clear_color (math::vec4 const & color) {
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

ms::Framebuffer::weak_depth_texbuffer ms::Framebuffer::get_depth_texture() {
    return std::weak_ptr<Texture>(depthTexture);
}

ms::Framebuffer::weak_color_texbuffers ms::Framebuffer::get_colors() {
    weak_color_texbuffers weak_colors;
    for(const auto & tex : this->colorTextureAttachments) {
        weak_colors.push_back(std::weak_ptr<Texture>(tex));
    }
	return weak_colors;
}

void ms::Framebuffer::set_depth_test (bool enabled) {
	isDepthTestEnabled = enabled;
}

#endif /* framebuffer_hpp */
