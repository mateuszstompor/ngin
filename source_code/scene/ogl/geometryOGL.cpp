//
//  geometryOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "geometryOGL.hpp"

void ms::GeometryOGL::load() {
	
	if (!is_loaded()) {
		
		load_vector_to_buffer(vertices, &verticesBuffer);
		load_vector_to_buffer(normals, 	&normalsBuffer);
		
		Resource::load();
		
	}
	
}

void ms::GeometryOGL::use_normals () {
	if(!is_loaded()) {
		load();
	}
	mglBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
}

void ms::GeometryOGL::use_vertices () {
	if(!is_loaded()) {
		load();
	}
	mglBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
}

void ms::GeometryOGL::load_vector_to_buffer(const std::vector<float> & v, GLuint* b) {
	mglGenBuffers(1, b);
	mglBindBuffer(GL_ARRAY_BUFFER, *b);
	mglBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), nullptr, GL_STATIC_DRAW);
	
	float* vBuf = reinterpret_cast<float*>(mglMapBufferRange(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float),  GL_MAP_WRITE_BIT));

	for (unsigned long long i = 0; i < v.size(); ++i) {
		*(vBuf + i) = v[i];
	}

	mglUnmapBuffer(GL_ARRAY_BUFFER);
	mglBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ms::GeometryOGL::unload() {
	if(is_loaded()) {
		mglDeleteBuffers(1, &verticesBuffer);
		mglDeleteBuffers(1, &normalsBuffer);
		
		Resource::unload();
	}
}
