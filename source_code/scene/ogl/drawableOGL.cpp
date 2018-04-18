//
//  drawableOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "drawableOGL.hpp"

ms::DrawableOGL::DrawableOGL () : ms::Drawable() {
	modelTransformation = PositionedObject();
}

void ms::DrawableOGL::use () {
	if(!is_loaded())
		load();
	
	mglBindVertexArray(vertexArray);
}

std::string ms::DrawableOGL::get_class () {
	return "ms::DrawableOGL";
}

void ms::DrawableOGL::draw () {
	use();
	geometry->use_indicies();
	mglDrawElements(GL_TRIANGLES, geometry->amount_of_indices(), GL_UNSIGNED_INT, nullptr);
}

std::unique_ptr<ms::DrawableOGL> ms::DrawableOGL::get_quad () {
	
    auto drawable = std::make_unique<DrawableOGL>();
    auto quad = std::make_shared<GeometryOGL>();
	
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
        
        quad->vertices.push_back(Vertex{std::move(position), std::move(normal), std::move(tangent), std::move(bitangent), std::move(textureCoordinate)});
	}
	
	for(int i = 0; i < 6; ++i) {
		quad->indices.push_back(quad_indicies::indicies[i]);
	}
	
	drawable->geometry = std::move(quad);
	
	return drawable;
	
}

void ms::DrawableOGL::_load	() {

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

}

void ms::DrawableOGL::_unload () {
	mglDeleteVertexArrays(1, &vertexArray);
}
