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
        
	public:
        
									            PostprocessDrawer	(std::vector<std::weak_ptr<Texture2D>> 	input,
                                                                     std::unique_ptr<Framebuffer> &&        framebuffer,
                                                                     std::unique_ptr<Shader> &&             shaderProgram);
        void                                    draw			    () const;
        std::string                             get_class			() const override;
		
	private:
        
        void                                    _load               () override;
        void                                    _unload             () override;
		std::vector<std::weak_ptr<Texture2D>> 	inputTextures;
		std::unique_ptr<Drawable> 				quad;

	};
	
}
