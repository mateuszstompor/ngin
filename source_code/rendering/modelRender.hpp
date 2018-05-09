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
    
    class ModelRender : public Render {
    
        private:
        
        friend class ForwardRender;
        friend class DeferredRender;
        
        inline          ModelRender             (std::unique_ptr<Framebuffer> && framebuffer, std::unique_ptr<Shader> && shader);
        virtual void    draw                    (Drawable & node) = 0;
        virtual void    set_material            (Material * material) = 0;
        virtual void    set_spot_lights         (std::vector<SpotLight> const & spotLights) = 0;
        virtual void    set_point_lights        (std::vector<PointLight> const & pointLights) = 0;
        virtual void    set_directionallight    (DirectionalLight const * directionalLight) = 0;
        virtual void    set_camera              (Camera const & camera) = 0;
        virtual         ~ModelRender            () = default;
        
    };
    
}

ms::ModelRender::ModelRender (std::unique_ptr<Framebuffer> && framebuffer, std::unique_ptr<Shader> && shader) : Render{std::move(framebuffer), std::move(shader)} {}
