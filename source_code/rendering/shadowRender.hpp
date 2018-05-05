//
//  shadowRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 19/04/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "render.hpp"

namespace ms {
    
    class DLShadowRender : public Render {
        
    public:
        
                        DLShadowRender          (std::unique_ptr<Framebuffer> &&    framebuffer,
                                                 std::unique_ptr<Shader> &&         shader);
        
        std::string     get_class               () const override;
        void            draw                    (Drawable & node);
        void            setup_uniforms          (math::mat4 const & projection, math::mat4 const & transformation);
        
    };
    
}
