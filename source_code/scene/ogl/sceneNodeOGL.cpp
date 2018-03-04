//
//  sceneNodeOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "sceneNodeOGL.hpp"

void ms::SceneNodeOGL::use () {
	glBindVertexArray(vertexArray);
}

void ms::SceneNodeOGL::load	() {
	glGenVertexArrays(1, &vertexArray);
	
	if (geometry && !geometry->is_loaded()) {
		geometry->load();
		
	}
	
	this->use();
//	geometry->use_normals();s
	
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
//	glEnableVertexAttribArray(1);
	
	geometry->use_vertices();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	
	isLoaded = true;
	
	Resource::load();
}

void ms::SceneNodeOGL::unload () {
	glDeleteVertexArrays(1, &vertexArray);
	
	isLoaded = false;
	
	Resource::unload();
}
