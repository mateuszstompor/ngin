//
//  orthographicCamera.hpp
//  ngin
//
//  Created by Mateusz Stompór on 12/04/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "camera.hpp"

namespace ms {
    
    /**
    * Jedna z dostępnych implementacji kamery, której zadaniem jest przedstawiać obraz zachowują równoległość linii projekci.
    * Efekt podobny można zaobserwować spoglądając na obiekty wyjątkowo odległe od obserwatora.
    * Opis jej zadania przedstawiony jest w klasie bazowej
    * @see Camera
    */
    class OrthographicCamera : public Camera {
        
    public:
        /**
         * Ponieważ pole widzenia kamery jest skończone konieczne określenie jest parametrów na podstawie których będzie ono ograniczane.
         * @param far Odpowiada za określenie oddelenia od położenia kamery w którym ma być stworzona daleka, prostopadłą to kierunku widzenia płaszczyzna ograniczająca
         * @param near Odpowiada za określenie oddelenia od położenia kamery w którym ma być stworzona bliska, prostopadłą to kierunku widzenia płaszczyzna ograniczająca
         * @param top Odpowiada za określenie oddelenia od położenia kamery w którym ma być stworzona góra płaszczyzna ograniczająca
         * @param bottom Odpowiada za określenie oddelenia od położenia kamery w którym ma być stworzona dolna płaszczyzna ograniczająca
         * @param left Odpowiada za określenie oddelenia od położenia kamery w którym ma być stworzona lewa, równoległa do kierunku widzenia płaszczyzna ograniczająca
         * @param right Odpowiada za określenie oddelenia od położenia kamery w którym ma być stworzona prawa, równoległa do kierunku widzenia płaszczyzna ograniczającay.
         */
                                        OrthographicCamera      (float far, float near,
                                                                 float top, float bottom,
                                                                 float right, float left);
        /**
         * Metoda opisana została w klasie bazowej.
         * @see Camera
         */
        bool                            is_in_camera_sight      (math::mat4 const & boundingBoxTransformation,
                                                                 math::BoundingBox<float> const & boundingBox) const override;
        /**
         * Metoda opisana została w klasie bazowej.
         * @see Camera
         */
        math::mat4 const &              get_projection_matrix   () const override;
    private:
        
        math::OrthographicViewport<float>   viewport;
        
    };
    
}
