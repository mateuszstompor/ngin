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

    /**
     * Klasa odpowiadająca za nanoszenie efektu winiety na bufor ramki.
     * Efekt działa w trzch regionach, a jego zadaniem jest przyciemnienie części ekranu.
     * Obraz podzielony może zostać w oparciu o dwa okręgi.
     * Wnętrze pierwszego, wewnętrznego okręgu nie jest poddawana działaniu efektu w ogóle.
     * W regionie pomiędzy pierwszym okręgiem a drugim interpolowany jest kolor od przezroczystego do czarnego.
     * W końcu obszar będący poza drugim, zewnętrznym okręgiem wypełniany jest kolorem czarnym.
     * Klasa stanowi część fasady biblioteki - NGin, z której poziomu obiekty mogą być poddawane modyfikacjom.
     * @see NGin
     */
    class VignettePostprocessRender : public PostprocessRender {
        
        friend class NGin;
        
    public:

        /**
         * Zadaniem metody jest określić siłę działania efektu, funkcja przyjmuje wartości z przedziału [0.0, 1.0].
         * @param strength siła efektu.
         */
        void                set_effect_strength         (float strength);
        /**
         * @return obecna siła efektu.
         */
        constexpr float     get_effect_strength         () const { return effectStrength; }
        /**
         * Ustawia promień wewnętrznego efektu na określoną wartość
         */
        void                set_inner_radius            (float innerRadius);
        /**
         * @return promień wewnętrznego okręgu.
         */
        constexpr float     get_inner_radius            () const { return innerCircleRadius; }
        /**
         * Ustawia promień wewnętrznego efektu na określoną wartość
         */
        void                set_outer_radius            (float outerRadius);
        /**
         * @return promień zewnętrznego okręgu.
         */
        constexpr float     get_outer_radius            () const { return outerCircleRadius; }
        
    private:
                            VignettePostprocessRender   (std::vector<std::weak_ptr<Texture2D>>  input,
                                                         std::unique_ptr<Framebuffer> &&        framebuffer,
                                                         std::unique_ptr<Shader> &&             shaderProgram,
                                                         float                                  innerCircleRadius = 0.25f,
                                                         float                                  outerCircleRadius = 0.8f,
                                                         float                                  effectStrength = 0.7f);
        void                draw                        () const override;
        // values should be in range [0, 1]
        float               effectStrength;
        float               innerCircleRadius;
        float               outerCircleRadius;
        
    };
    
}
