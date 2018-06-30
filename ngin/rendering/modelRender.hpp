//
//  modelRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 05/05/2018.
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
    class ModelRender : public Render {

        friend class ForwardRender;
        friend class DeferredRender;

    private:

                        ModelRender             (std::unique_ptr<Framebuffer> && framebuffer, std::unique_ptr<Shader> && shader);
        virtual void    draw                    (Drawable & node) = 0;
        virtual void    draw                    (Drawable & node, math::mat4 const & transformation) = 0;
        virtual void    set_material            (Material * material) = 0;
        virtual void    set_spot_lights         (std::vector<SpotLight> const & spotLights) = 0;
        virtual void    set_point_lights        (std::vector<PointLight> const & pointLights) = 0;
        virtual void    set_directionallight    (DirectionalLight const * directionalLight) = 0;
        virtual void    set_camera              (Camera const & camera) = 0;
        virtual         ~ModelRender            () = default;
        
    };
    
}

