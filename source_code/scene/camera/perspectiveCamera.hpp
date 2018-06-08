//
//  perspectiveCamera.hpp
//  ngin
//
//  Created by Mateusz Stompór on 03/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "camera.hpp"

namespace ms {
    
    /**
     * Jedna z dostępnych implementacji kamery, której zadaniem jest przedstawiać obraz symulając zniekształcenie perspektywiczne, spotykane na przykład u ludzi i zwierząt.
     * Opis jej zadania przedstawiony jest w klasie bazowej
     * @see Camera
     */
	class PerspectiveCamera : public Camera {

	public:
        /**
         * Ponieważ pole widzenia kamery jest skończone konieczne określenie jest parametrów na podstawie których będzie ono ograniczane.
         * @param nearPlane Odpowiada za określenie odległości w stosunku do położenia kamery od którego miejsca obiekty będą widoczne.
         * @param farPlane Odpowiada za określenie odległości w stosunku do położenia kamery do którego miejsca obiekty będą widoczne.
         * @param fovDegrees to kąt widzenia kamery określony w stopniach.
         * @param aspectRatio jest to stosunek szerokości do długości, określa w jakiej proporcji obraz ma być generowany.
         */
                                        PerspectiveCamera       (float nearPlane,
                                                                 float farPlane,
                                                                 float fovDegrees,
                                                                 float aspectRatio);
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
        
        math::FrustumViewport<float>    frustum;
                
	};
	
}
