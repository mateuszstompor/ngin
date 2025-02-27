//
//  framebuffer.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "../../resources/resource.hpp"

#include "../../scene/texture.hpp"
#include "../../scene/cubemap.hpp"
#include "../../scene/cubemapArray.hpp"
#include "../../scene/textureArray.hpp"

#include "renderbuffer.hpp"


				//											//
				//											//
				// 	DEPTH TESTING IS TURNED ON BY DEFAULT	//
				//											//
				//											//


namespace ms {
	
	class Framebuffer : public Resource {
        
        using fb_ptr = std::unique_ptr<Framebuffer>;
        
        friend class Render;
        friend class DeferredRender;
        friend class NGin;
        
    public:
        
                                        Framebuffer         (int                                colorAttachmentsAmount,
                                                             int                                renderbufferAttachmentsAmount,
                                                             int                                width,
                                                             int                                height);
        void                            bind_color_buffer   (int                                index,
                                                             std::unique_ptr<Texture2D> &&      Texture2D);
        void                            bind_color_buffer   (int                                index,
                                                             std::unique_ptr<Renderbuffer> &&   renderbuffer);
        void                            bind_depth_buffer   (std::unique_ptr<Renderbuffer> &&   renderbuffer);
        void                            bind_depth_buffer   (std::unique_ptr<Texture2D> &&      texture);
        void                            bind_depth_buffer   (std::unique_ptr<Texture2DArray> &  texture, std::size_t layer);
        void                            bind_depth_buffer   (std::unique_ptr<CubeMap> &         texture, /*CubeMap::Face*/ int face);
        void                            bind_depth_buffer   (std::unique_ptr<CubemapArray> &    texture, int face, std::size_t layerIndex);
        void                            configure           ();
        void                            use                 ();
                                        ~Framebuffer        () = default;
        
	private:
		
		using strong_color_texbuffers       = std::vector<std::shared_ptr<Texture2D>>;
        using strong_depth_texbuffer        = std::shared_ptr<Texture2D>;
        
		using strong_color_renderbuffers    = std::vector<std::shared_ptr<Renderbuffer>>;
        using strong_depth_renderbuffer     = std::shared_ptr<Renderbuffer>;
        
        using weak_color_texbuffers         = std::vector<std::weak_ptr<Texture2D>>;
        using weak_depth_texbuffer          = std::weak_ptr<Texture2D>;
    
        using weak_color_renderbuffers      = std::vector<std::weak_ptr<Renderbuffer>>;
        using weak_depth_renderbuffer       = std::weak_ptr<Renderbuffer>;
		
									    Framebuffer			(Framebuffer const &) = delete;
		Framebuffer & 				    operator = 			(Framebuffer const &) = delete;
		
		
					//														//
					// 	Checks completeness and merge all things together	//
					//														//
		
		void					        use_for_read		();
        std::string			            get_class			() const override;
        void 					        copy_depth_from		(Framebuffer & frame, texture::MagFilter filter = texture::MagFilter::nearest);
        void                            copy_color_from     (Framebuffer & frame, texture::MagFilter filter = texture::MagFilter::nearest);
        void                            copy_framebuffer    (Framebuffer & frame, texture::MagFilter filter = texture::MagFilter::nearest);
		void				            use_for_write		();
		bool				            is_complete			() const;
        constexpr bool                  is_configured		() const { return isConfigured; }
		void				            clear_color			();
		void				            clear_depth			();
		void				            clear_frame			();
        void                            set_blending        (bool enabled);
        constexpr bool                  get_blending        () const { return isBlendingEnabled; }
        void				            set_depth_test		(bool enabled);
        void				            set_clear_color		(math::vec4 const & color);
        constexpr int                   get_height			() const { return height; }
        constexpr int                   get_width			() const { return width; }
        void                            _load               () override;
        void                            _unload             () override;
        weak_color_texbuffers 	        get_colors          ();
        weak_depth_texbuffer            get_depth_texture   ();
        static  fb_ptr                  window_framebuffer  (int width, int height);
        constexpr GLuint                get_underlying_id   () const { return framebuffer; }
        void                            set_underlying_id   (GLuint framebufferID);
		
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
		bool							isConfigured;
        bool                            is_default_framebuffer;
        bool                            isBlendingEnabled;
        GLuint                          framebuffer;
	};
	
}

