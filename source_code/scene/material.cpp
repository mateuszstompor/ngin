//
//  material.cpp
//  ngin
//
//  Created by Mateusz Stompór on 27/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "material.hpp"

//layout (std140) uniform  MaterialBlock {
//	vec3 	ambientColor; 	// 0  bytes padding, stride 4 (dispite fact it has 3 components)
//	vec3 	diffuseColor;	// 16 bytes padding
//	vec3 	specularColor;	// 32 bytes padding
//	float 	shininess;		// 48 bytes padding
//	float	opacity;		// 52 bytes padding
//} materialBlock;
// size in total 56 bytes

ms::Material::Material(math::vec3 const & ambient, math::vec3 const & diffuse, math::vec3 const & specular, float shin, float opac, std::string const & nam) :
ambientColor{ambient},
diffuseColor{diffuse},
specularColor{specular},
shininess{shin},
opacity{opac},
name{nam},
bufferId{0} { }

void ms::Material::_load () {
	mglGenBuffers(1, &bufferId);
	mglBindBuffer(GL_UNIFORM_BUFFER, bufferId);
	mglBufferData(GL_UNIFORM_BUFFER, 56 * sizeof(GLbyte), nullptr, GL_STATIC_DRAW);
	GLvoid* uniformBlock = glMapBufferRange(GL_UNIFORM_BUFFER, 0, 56 * sizeof(GLbyte), GL_MAP_WRITE_BIT);
	memcpy(static_cast<GLbyte*>(uniformBlock), ambientColor.c_array(), 3 * sizeof(GLfloat));
	memcpy((static_cast<GLbyte*>(uniformBlock) + 16), diffuseColor.c_array(), 3 * sizeof(GLfloat));
	memcpy((static_cast<GLbyte*>(uniformBlock) + 32), specularColor.c_array(), 3 * sizeof(GLfloat));
	memcpy((static_cast<GLbyte*>(uniformBlock) + 48), &shininess, sizeof(GLfloat));
	memcpy((static_cast<GLbyte*>(uniformBlock) + 52), &opacity, sizeof(GLfloat));
	mglUnmapBuffer(GL_UNIFORM_BUFFER);
	mglBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ms::Material::_unload () {
	mglDeleteBuffers(1, &bufferId);
}

void ms::Material::use () {
	if(!is_loaded())
		load();
	
	mglBindBuffer(GL_UNIFORM_BUFFER, bufferId);
	mglBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferId);
}
