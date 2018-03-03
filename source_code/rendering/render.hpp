//
//  render.h
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef render_h
#define render_h

#include "../resources/resource.hpp"
#include "../scene/scene.hpp"

namespace ms {
    
    class Render : public Resource {
        
    public:
        
        virtual void	use     	() = 0;
        virtual void	draw_scene  (Scene scene);
        virtual			~Render 	() {}
    };
    
}

#endif /* render_h */
