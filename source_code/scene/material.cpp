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
bufferId{0},
invalidated{false} { }

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
    invalidated = false;
}

std::string ms::Material::get_class () const {
    return "ms::Material";
}

void ms::Material::set_name (std::string const & n) {
    name = n;
}

void ms::Material::set_opacity (float o) {
    opacity = o;
    if(is_loaded()) {
        invalidated = true;
    }
}

void ms::Material::set_shininess (float s) {
    shininess = s;
    if(is_loaded()) {
        invalidated = true;
    }
}

void ms::Material::set_ambient_color (math::vec3 const & ambient) {
    ambientColor = ambient;
    if(is_loaded()) {
        invalidated = true;
    }
}

bool ms::Material::is_translucent () const {
    return opacity < 1.0f;
}

void ms::Material::set_diffuse_color (math::vec3 const & diffuse) {
    diffuseColor = diffuse;
    if(is_loaded()) {
        invalidated = true;
    }
}

void ms::Material::set_specular_color (math::vec3 const & specular) {
    specularColor = specular;
    if(is_loaded()) {
        specularColor = specular;
    }
}

void ms::Material::bind_diffuse_texture (std::shared_ptr<Texture2D> diffuse) {
    boundedDiffuseTexture = diffuse;
}

void ms::Material::bind_specular_texture (std::shared_ptr<Texture2D> specular) {
    boundedSpecularTexture = specular;
}

void ms::Material::bind_normal_texture (std::shared_ptr<Texture2D> normal) {
    boundedNormalTexture = normal;
}

void ms::Material::bind_height_texture (std::shared_ptr<Texture2D> height) {
    boundedHeightTexture = height;
}

ms::Texture2D * ms::Material::get_diffuse_texture () {
    return boundedDiffuseTexture.get();
}

ms::Texture2D * ms::Material::get_specular_texture () {
    return boundedSpecularTexture.get();
}

ms::Texture2D * ms::Material::get_normal_texture () {
    return boundedNormalTexture.get();
}

ms::Texture2D * ms::Material::get_height_texture () {
    return boundedHeightTexture.get();
}

void ms::Material::use () {
    if(invalidated) {
        unload();
    }
    
	if(!is_loaded())
		load();
	
	mglBindBuffer(GL_UNIFORM_BUFFER, bufferId);
	mglBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferId);
}
