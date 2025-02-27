//
//  lightSourcesRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 19/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "render.hpp"

namespace ms {

	/**
	 * Klasa służąca do generowania rysowalnych obiektów na scenie, które nie są poddawane cieniowaniu.
	 * W obencym stadium rozwoju nie może poddawana być modyfikacjom.
	 * Klasa stanowi część fasady biblioteki - NGin.
	 * @see Scene
	 * @see NGin
	 */
	class LightSourcesRender : public Render {
		
        friend class NGin;
        
    public:
		
                        ~LightSourcesRender     () = default;
        std::string     get_class               () const override;
    
    private:
                        LightSourcesRender        (std::unique_ptr<Framebuffer> &&    framebuffer,
                                                   std::unique_ptr<Shader> &&         shader);
        void	        draw  					(Drawable & node);
        void            draw                    (Drawable & node, math::mat4 const & transformation);
        void            set_camera              (Camera const & camera);
		
	};
	
}
