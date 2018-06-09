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

    /**
	 * Klasa służąca do generowania rysowalnych map cieni obiektów na scenie dla każdego ze świateł rzucających cień.
	 * W obencym stadium rozwoju nie może poddawana być modyfikacjom i z zewnątrz biblioteki nie ma do niej dostępu.
	 * @see NGin
	 */
    class DLShadowRender : public Render {
        
        friend class NGin;
        
    public:
    
        std::string     get_class               () const override;
        
    private:
                        DLShadowRender          (std::unique_ptr<Framebuffer> &&    framebuffer,
                                                 std::unique_ptr<Shader> &&         shader);
        void            draw                    (Drawable & node);
        virtual void    draw                    (Drawable & node, math::mat4 const & transformation);
        void            setup_uniforms          (math::mat4 const & projection, math::mat4 const & transformation);
        
    };
    
}
