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
	mglBufferData(GL_UNIFORM_BUFFER, 52 * sizeof(GLbyte), nullptr, GL_STATIC_DRAW);

    auto rawBuffer = mglMapBufferRange(GL_UNIFORM_BUFFER, 0, 52 * sizeof(GLbyte), GL_MAP_WRITE_BIT);
    gsl::span<GLbyte, 52> uniformBlock = gsl::make_span(static_cast<GLbyte*>(rawBuffer), 52);

    std::memcpy(&uniformBlock[0], ambientColor.c_array(), 3 * sizeof(GLfloat));
	std::memcpy(&uniformBlock[16], diffuseColor.c_array(), 3 * sizeof(GLfloat));
	std::memcpy(&uniformBlock[32], specularColor.c_array(), 3 * sizeof(GLfloat));
	std::memcpy(&uniformBlock[44], &shininess, sizeof(GLfloat));
	std::memcpy(&uniformBlock[48], &opacity, sizeof(GLfloat));

	mglUnmapBuffer(GL_UNIFORM_BUFFER);
	mglBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ms::Material::_unload () {
	mglDeleteBuffers(1, &bufferId);
}

std::string ms::Material::get_class () const {
    return "ms::Material";
}

void ms::Material::use () {
	if(!is_loaded())
		load();
	
	mglBindBuffer(GL_UNIFORM_BUFFER, bufferId);
	mglBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferId);
}
