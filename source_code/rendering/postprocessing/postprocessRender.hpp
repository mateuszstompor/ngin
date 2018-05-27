//
//  postprocessRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 22/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>

#include "../render.hpp"
#include "../shaders/shader.hpp"

namespace ms {
	
	class PostprocessRender : public Render {
        
        friend class NGin;
        
    public:

        constexpr bool                          is_enabled          () const { return isOn; }
        void                                    set_enabled         (bool state);
        virtual                                 ~PostprocessRender  () = default;
        
    protected:
                                                PostprocessRender   (std::vector<std::weak_ptr<Texture2D>>  input,
                                                                     std::unique_ptr<Framebuffer> &&        framebuffer,
                                                                     std::unique_ptr<Shader> &&             shaderProgram);
        virtual void                            draw                () const;
        
	private:
        
        std::string                             get_class           () const override;
        bool                                    isOn;
        void                                    _load               () override;
        void                                    _unload             () override;
		std::vector<std::weak_ptr<Texture2D>> 	inputTextures;
		std::unique_ptr<Drawable> 				quad;

	};
	
}
