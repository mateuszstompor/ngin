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
        
        friend class NGin;
        
    public:
    
                        ~DLShadowRender         () = default;
        std::string     get_class               () const override;
        
    private:
                        DLShadowRender          (std::unique_ptr<Framebuffer> &&    framebuffer,
                                                 std::unique_ptr<Shader> &&         shader);
        void            draw                    (Drawable & node);
        virtual void    draw                    (Drawable & node, math::mat4 const & transformation);
        void            setup_uniforms          (math::mat4 const & projection, math::mat4 const & transformation);
        
    };
    
}
