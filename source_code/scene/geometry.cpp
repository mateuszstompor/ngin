//
//  geometry.cpp
//  ngin
//
//  Created by Mateusz Stompór on 13/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "geometry.hpp"

ms::Geometry::Geometry() : vertices(std::vector<Vertex>()), indices(std::vector<unsigned int>()), associatedMaterial("") { }

int ms::Geometry::amount_of_vertices () const {
	return static_cast<int>(vertices.size());
}

int ms::Geometry::amount_of_indices	() const {
	return static_cast<int>(indices.size());
}

void ms::Geometry::set_material (std::string name) {
	associatedMaterial = name;
}

std::string ms::Geometry::get_material_name () const {
	return associatedMaterial;
}

bool ms::Geometry::has_material () const {
	return !associatedMaterial.empty();
}

ms::math::BoundingBox<float>* ms::Geometry::get_bounding_box () const {
    return boundingBox.get();
}
