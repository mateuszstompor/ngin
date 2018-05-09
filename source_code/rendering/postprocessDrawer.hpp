//
//  postprocessDrawer.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>

#include "render.hpp"
#include "shaders/shader.hpp"

namespace ms {
	
	class PostprocessDrawer : public Render {
        
        friend class NGin;
        
	public:
        
									            PostprocessDrawer	(std::vector<std::weak_ptr<Texture2D>> 	input,
                                                                     std::unique_ptr<Framebuffer> &&        framebuffer,
                                                                     std::unique_ptr<Shader> &&             shaderProgram);
        
        std::string                             get_class           () const override;
        constexpr bool                          is_enabled          () const { return isOn; }
        void                                    set_enabled         (bool state);
        
	private:

        bool                                    isOn;
        void                                    draw                () const;
        void                                    _load               () override;
        void                                    _unload             () override;
		std::vector<std::weak_ptr<Texture2D>> 	inputTextures;
		std::unique_ptr<Drawable> 				quad;

	};
	
}
