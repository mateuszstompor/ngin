//
//  DLShadowRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 19/04/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//



#ifndef directional_shadow_render_hpp
#define directional_shadow_render_hpp

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
        
    private:
        
        void            draw                    (Drawable & node, Scene const  & scene) override {}
        
    };
    
}

#endif /* directional_shadow_render_hpp */
