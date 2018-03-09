//
//  sceneNodeOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "sceneNodeOGL.hpp"

ms::SceneNodeOGL::SceneNodeOGL () : ms::SceneNode() {
	modelTransformation = PositionedObject();
}

void ms::SceneNodeOGL::use () {
	if(!is_loaded())
		load();
	
	glBindVertexArray(vertexArray);
}

void ms::SceneNodeOGL::load	() {
	
	if (!is_loaded()) {
		
		glGenVertexArrays(1, &vertexArray);
		
		glBindVertexArray(vertexArray);
		
		if(geometry) {
			geometry->use_normals();
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
			glEnableVertexAttribArray(1);
			
			geometry->use_vertices();
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
			glEnableVertexAttribArray(0);
		}
		
		Resource::load();
		
	}
	
}

void ms::SceneNodeOGL::unload () {
	if(is_loaded()) {
		glDeleteVertexArrays(1, &vertexArray);
		Resource::unload();
	}
}
