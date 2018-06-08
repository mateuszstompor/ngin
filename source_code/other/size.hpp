//
//  size.hpp
//  ngin
//
//  Created by Mateusz Stompór on 05/06/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

namespace ms {
    
    /**
     * Szablonowa struktura służąca do reprezentowania rozmiaru w formie dwuwymiarowej.
     */
    template<typename T>
    struct Size {
        /**
         * Szerokość
         */
        T width;
        /**
         * Wysokość
         */
        T height;
    };
    
}
