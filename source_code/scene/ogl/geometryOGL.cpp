//
//  geometryOGL.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "geometryOGL.hpp"

void ms::GeometryOGL::_load() {
	load_vertices_to_buffer();
}

void ms::GeometryOGL::use_texture_coord () {
	if(!is_loaded()) {
		load();
	}
	
	mglBindBuffer(GL_ARRAY_BUFFER, texturesCooridnatesBuffer);
}

void ms::GeometryOGL::use_tangents () {
    if(!is_loaded()) {
        load();
    }
    
    mglBindBuffer(GL_ARRAY_BUFFER, tangents);
}

void ms::GeometryOGL::use_bitangents () {
    if(!is_loaded()) {
        load();
    }
    
    mglBindBuffer(GL_ARRAY_BUFFER, bitangents);
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
	mglBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
}

void ms::GeometryOGL::use_indicies () {
	if(!is_loaded()) {
		load();
	}
	mglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesBuffer);
}


void ms::GeometryOGL::load_vertices_to_buffer () {
	
	{
		mglGenBuffers(1, &positionsBuffer);
		mglBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
		mglBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);

		float* vBuf = reinterpret_cast<float*>(mglMapBufferRange(GL_ARRAY_BUFFER, 0, 3 * vertices.size() * sizeof(GLfloat),  GL_MAP_WRITE_BIT));

		for (unsigned long long i = 0; i < vertices.size(); ++i) {
			*(vBuf +  3 * i) 		= vertices[i].position[0];
			*(vBuf + (3 * i) + 1) 	= vertices[i].position[1];
			*(vBuf + (3 * i) + 2) 	= vertices[i].position[2];
		}
		
		mglUnmapBuffer(GL_ARRAY_BUFFER);
		mglBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	{
		mglGenBuffers(1, &normalsBuffer);
		mglBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		mglBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
		
		float* nBuf = reinterpret_cast<float*>(mglMapBufferRange(GL_ARRAY_BUFFER, 0, 3 * vertices.size() * sizeof(GLfloat),  GL_MAP_WRITE_BIT));
		
		for (unsigned long long i = 0; i < vertices.size(); ++i) {
			*(nBuf +  3 * i) 		= vertices[i].normal[0];
			*(nBuf + (3 * i) + 1) 	= vertices[i].normal[1];
			*(nBuf + (3 * i) + 2) 	= vertices[i].normal[2];
		}
		
		mglUnmapBuffer(GL_ARRAY_BUFFER);
		mglBindBuffer(GL_ARRAY_BUFFER, 0);
	}
    
    {
        mglGenBuffers(1, &tangents);
        mglBindBuffer(GL_ARRAY_BUFFER, tangents);
        mglBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
        
        float* nBuf = reinterpret_cast<float*>(mglMapBufferRange(GL_ARRAY_BUFFER, 0, 3 * vertices.size() * sizeof(GLfloat),  GL_MAP_WRITE_BIT));
        
        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            *(nBuf +  3 * i)         = vertices[i].tangent[0];
            *(nBuf + (3 * i) + 1)     = vertices[i].tangent[1];
            *(nBuf + (3 * i) + 2)     = vertices[i].tangent[2];
        }
        
        mglUnmapBuffer(GL_ARRAY_BUFFER);
        mglBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    {
        mglGenBuffers(1, &bitangents);
        mglBindBuffer(GL_ARRAY_BUFFER, bitangents);
        mglBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
        
        float* nBuf = reinterpret_cast<float*>(mglMapBufferRange(GL_ARRAY_BUFFER, 0, 3 * vertices.size() * sizeof(GLfloat),  GL_MAP_WRITE_BIT));
        
        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            *(nBuf +  3 * i)         = vertices[i].bitangent[0];
            *(nBuf + (3 * i) + 1)     = vertices[i].bitangent[1];
            *(nBuf + (3 * i) + 2)     = vertices[i].bitangent[2];
        }
        
        mglUnmapBuffer(GL_ARRAY_BUFFER);
        mglBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
	{
		mglGenBuffers(1, &texturesCooridnatesBuffer);
		mglBindBuffer(GL_ARRAY_BUFFER, texturesCooridnatesBuffer);
		mglBufferData(GL_ARRAY_BUFFER, 2 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
		
		float* nBuf = reinterpret_cast<float*>(mglMapBufferRange(GL_ARRAY_BUFFER, 0, 2 * vertices.size() * sizeof(GLfloat),  GL_MAP_WRITE_BIT));
		
		for (unsigned long long i = 0; i < vertices.size(); ++i) {
			*(nBuf +  2 * i) 		= vertices[i].textureCoordinates[0];
			*(nBuf + (2 * i) + 1) 	= vertices[i].textureCoordinates[1];
		}
		
		mglUnmapBuffer(GL_ARRAY_BUFFER);
		mglBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	{
		mglGenBuffers(1, &indiciesBuffer);
		mglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesBuffer);
		mglBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		mglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

std::string ms::GeometryOGL::get_class () {
	return "ms::GeometryOGL";
}

void ms::GeometryOGL::_unload() {
    mglDeleteBuffers(1, &bitangents);
    mglDeleteBuffers(1, &tangents);
	mglDeleteBuffers(1, &positionsBuffer);
	mglDeleteBuffers(1, &normalsBuffer);
	mglDeleteBuffers(1, &texturesCooridnatesBuffer);
	mglDeleteBuffers(1, &indiciesBuffer);
}
