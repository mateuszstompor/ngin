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

void ms::DrawableOGL::load	() {
	
	if (!is_loaded()) {
		
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
			
		}
		
		Resource::load();
		
	}
	
}

void ms::DrawableOGL::unload () {
	if(is_loaded()) {
		mglDeleteVertexArrays(1, &vertexArray);
		Resource::unload();
	}
}
