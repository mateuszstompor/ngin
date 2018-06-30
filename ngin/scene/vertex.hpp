//
//  vertex.hpp
//  ngin
//
//  Created by Mateusz Stompór on 12/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <glMath/glMath.h>

namespace ms {
    
    /**
     * Wierzchołek jest najmniejszym elementem z którego złożona jest siatka.
     * Posiada pozycję określoną we współrzędnych modelu, określanych także mianem współrzędnych obiektu.
     * Z każdym wierzchołkiem powiązany jest także wektor normalny.
     * Te dwie składowe są wymagane, aby model generowany na ekranie mógł być poddany cieniowaniu.
     * W celu poprawienia walorów wizualnych wierzchołek może posiadać także koordynaty tekstury z którego będzie próbkowany kolor.
     * Tangens oraz bitangens wykorzystywane są wówczas, gdy wektory normalne siatki wierzchołków próbkowane są za pomocą normal mapy.
     * @see Geometry
     */
	struct Vertex {
		math::vec3 position;
		math::vec3 normal;
        math::vec3 tangent;
        math::vec3 bitangent;
		math::vec2 textureCoordinates;
	};

}
