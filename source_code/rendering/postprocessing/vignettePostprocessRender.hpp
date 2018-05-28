//
//  vignettePostprocessRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 28/05/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <algorithm>

#include "postprocessRender.hpp"

namespace ms {
    
    class VignettePostprocessRender : public PostprocessRender {
        
        friend class NGin;
        
    public:
                            VignettePostprocessRender   (std::vector<std::weak_ptr<Texture2D>>  input,
                                                         std::unique_ptr<Framebuffer> &&        framebuffer,
                                                         std::unique_ptr<Shader> &&             shaderProgram,
                                                         float                                  innerCircleRadius = 0.25f,
                                                         float                                  outerCircleRadius = 0.8f,
                                                         float effectStrength = 0.7f);
        
        void                set_effect_strength         (float strength);
        constexpr float     get_effect_strength         () const { return outerCircleRadius; }
        void                set_inner_radius            (float innerRadius);
        constexpr float     get_inner_radius            () const { return innerCircleRadius; }
        void                set_outer_radius            (float outerRadius);
        constexpr float     get_outer_radius            () const { return outerCircleRadius; }
        
        
    private:
        void                draw                        () const override;
        // values should be in range [0, 1]
        float               effectStrength;
        float               innerCircleRadius;
        float               outerCircleRadius;
        
    };
    
}
