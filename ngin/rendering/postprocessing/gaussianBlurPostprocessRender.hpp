//
//  gaussianBlurPostprocessRender.hpp
//  ngin
//
//  Created by Mateusz Stompór on 27/05/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "postprocessRender.hpp"

namespace ms {

    /**
     * Odpowiada za naniesienie poziomego rozmycia gaussowskiego na klatkę obrazu.
     * Klasa stanowi część fasady biblioteki - NGin, z której poziomu obiekty mogą być poddawane modyfikacjom.
     * @see NGin
     */
    class GaussianBlurPostprocessRender : public PostprocessRender {
    
        friend class NGin;
        
    public:

        /**
         * Ustawia ilość próbek, która brana będzie pod uwagę podczas obliczania efektu.
         */
        constexpr void      set_amount_of_samples_to_take   (int amount) { amountOfSamplesToTake = amount; }
        /**
         * @return Zwraca ilość próbek, która brana jest pod uwagę podczas obliczania efektu.
         */
        constexpr int       get_amount_of_samples_to_take   () { return amountOfSamplesToTake; }
    
    private:
        
                            GaussianBlurPostprocessRender   (std::vector<std::weak_ptr<Texture2D>>  input,
                                                             std::unique_ptr<Framebuffer> &&        framebuffer,
                                                             std::unique_ptr<Shader> &&             shaderProgram,
                                                             int                                    amountOfSamples = 25);
        void                draw                            () const override;
        int                 amountOfSamplesToTake;
        
    };
    
}
