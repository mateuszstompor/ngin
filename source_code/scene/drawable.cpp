//
//  drawable.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "drawable.hpp"

ms::Drawable::Drawable() : modelTransformation{ } { }

void ms::Drawable::use () {
	if(!is_loaded())
		load();
	
	mglBindVertexArray(vertexArray);
}

std::string ms::Drawable::get_class () const {
	return "ms::Drawable";
}

void ms::Drawable::draw () {
	use();
	geometry->use_indicies();
	mglDrawElements(GL_TRIANGLES, geometry->amount_of_indices(), GL_UNSIGNED_INT, nullptr);
}

std::unique_ptr<ms::Drawable> ms::Drawable::get_quad () {
	
    auto drawable = std::make_unique<Drawable>();
	
    std::vector <Vertex>                vertices;
    std::vector <unsigned int>          indices;
    std::string                         associatedMaterial;
    math::BoundingBox<float>            boundingBox{0,0,0,0,0,0};
    
	for(int i = 0; i < 4; ++i) {
		int positionOffset = 3 * i;
		int textureOffset = 2 * i;
		
		math::vec3 position = math::vec3{	quad_indicies::vertices[positionOffset],
											quad_indicies::vertices[positionOffset+1],
											quad_indicies::vertices[positionOffset+2]};
		
		math::vec2 textureCoordinate = math::vec2{quad_indicies::textureCoordinates[textureOffset],
												  quad_indicies::textureCoordinates[textureOffset+1]};
		
        math::vec3 normal;
        math::vec3 tangent;
        math::vec3 bitangent;
        
        vertices.push_back(Vertex{std::move(position), std::move(normal), std::move(tangent), std::move(bitangent), std::move(textureCoordinate)});
	}
	
	for(int i = 0; i < 6; ++i) {
		indices.push_back(quad_indicies::indicies[i]);
	}
    
	drawable->geometry = std::make_shared<Geometry>(std::move(vertices), std::move(indices), std::move(associatedMaterial), std::move(boundingBox));
	
	return drawable;
	
}

void ms::Drawable::_load	() {

	mglGenVertexArrays(1, &vertexArray);
	
	mglBindVertexArray(vertexArray);
	
	if(geometry) {
		
		geometry->use_vertices();
		mglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		mglEnableVertexAttribArray(0);
		
		geometry->use_normals();
		mglVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		mglEnableVertexAttribArray(1);
		
		geometry->use_texture_coord();
		mglVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
		mglEnableVertexAttribArray(2);
		
        geometry->use_tangents();
        mglVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        mglEnableVertexAttribArray(3);
        
        geometry->use_bitangents();
        mglVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        mglEnableVertexAttribArray(4);
        
	}
    
    mglBindVertexArray(0);

}

void ms::Drawable::_unload () {
	mglDeleteVertexArrays(1, &vertexArray);
}