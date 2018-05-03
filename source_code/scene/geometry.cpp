//
//  geometry.cpp
//  ngin
//
//  Created by Mateusz Stompór on 13/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "geometry.hpp"

ms::Geometry::Geometry(std::vector <Vertex>  &&       vertices,
                       std::vector <unsigned int> &&  indices,
                       std::string &&                 associatedMaterial,
                       math::BoundingBox<float> &&    boundingBox,
                       std::string &&                 name) :
                                                                        vertices{std::move(vertices)},
                                                                        indices{std::move(indices)},
                                                                        associatedMaterial{std::move(associatedMaterial)},
                                                                        boundingBox{std::move(boundingBox)},
                                                                        name{std::move(name)} {}

int ms::Geometry::amount_of_vertices () const {
    return static_cast<int>(vertices.size());
}

int ms::Geometry::amount_of_indices () const {
    return static_cast<int>(indices.size());
}

void ms::Geometry::set_material (std::string const & name) {
    associatedMaterial = name;
}

bool ms::Geometry::has_material () const {
    return !associatedMaterial.empty();
}

void ms::Geometry::_load() {
    load_vertices_to_buffer();
}

void ms::Geometry::use_texture_coord () {
    load();
    mglBindBuffer(GL_ARRAY_BUFFER, texturesCooridnatesBuffer);
}

void ms::Geometry::use_tangents () {
    load();
    mglBindBuffer(GL_ARRAY_BUFFER, tangents);
}

void ms::Geometry::use_bitangents () {
    load();
    mglBindBuffer(GL_ARRAY_BUFFER, bitangents);
}

void ms::Geometry::use_normals () {
    load();
    mglBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
}

void ms::Geometry::use_vertices () {
    load();
    mglBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
}

void ms::Geometry::use_indicies () {
    load();
    mglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesBuffer);
}


void ms::Geometry::load_vertices_to_buffer () {
    
    {
        mglGenBuffers(1, &positionsBuffer);
        mglBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
        mglBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
        auto elementsAmount = 3 * vertices.size();
        auto rawBuffer = mglMapBufferRange(GL_ARRAY_BUFFER, 0, elementsAmount * sizeof(GLfloat),  GL_MAP_WRITE_BIT);
        auto vBuf = gsl::make_span(static_cast<GLfloat*>(rawBuffer), elementsAmount);

        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(&vBuf[3 * i], vertices[i].position.c_array(), 3 * sizeof(GLfloat));
        }
        
        mglUnmapBuffer(GL_ARRAY_BUFFER);
        mglBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    {
        mglGenBuffers(1, &normalsBuffer);
        mglBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
        mglBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
        auto elementsAmount = 3 * vertices.size();
        auto rawBuffer = mglMapBufferRange(GL_ARRAY_BUFFER, 0, elementsAmount * sizeof(GLfloat),  GL_MAP_WRITE_BIT);
        auto nBuf = gsl::make_span(static_cast<GLfloat*>(rawBuffer), elementsAmount);

        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(&nBuf[3 * i], vertices[i].normal.c_array(), 3 * sizeof(GLfloat));
        }
        
        mglUnmapBuffer(GL_ARRAY_BUFFER);
        mglBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    {
        mglGenBuffers(1, &tangents);
        mglBindBuffer(GL_ARRAY_BUFFER, tangents);
        mglBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
        auto elementsAmount = 3 * vertices.size();
        auto rawBuffer = mglMapBufferRange(GL_ARRAY_BUFFER, 0, elementsAmount * sizeof(GLfloat),  GL_MAP_WRITE_BIT);
        auto tBuf = gsl::make_span(static_cast<GLfloat*>(rawBuffer), elementsAmount);

        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(&tBuf[3 * i], vertices[i].tangent.c_array(), 3 * sizeof(GLfloat));
        }
        
        mglUnmapBuffer(GL_ARRAY_BUFFER);
        mglBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    {
        mglGenBuffers(1, &bitangents);
        mglBindBuffer(GL_ARRAY_BUFFER, bitangents);
        mglBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
        auto elementsAmount = 3 * vertices.size();
        auto rawBuffer = mglMapBufferRange(GL_ARRAY_BUFFER, 0, elementsAmount * sizeof(GLfloat),  GL_MAP_WRITE_BIT);
        auto bitBuf = gsl::make_span(static_cast<GLfloat*>(rawBuffer), elementsAmount);

        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(&bitBuf[3 * i], vertices[i].bitangent.c_array(), 3 * sizeof(GLfloat));
        }
        
        mglUnmapBuffer(GL_ARRAY_BUFFER);
        mglBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    {
        mglGenBuffers(1, &texturesCooridnatesBuffer);
        mglBindBuffer(GL_ARRAY_BUFFER, texturesCooridnatesBuffer);
        mglBufferData(GL_ARRAY_BUFFER, 2 * vertices.size() * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
        auto elementsAmount = 2 * vertices.size();
        auto rawBuffer = mglMapBufferRange(GL_ARRAY_BUFFER, 0, elementsAmount * sizeof(GLfloat),  GL_MAP_WRITE_BIT);
        auto texCoBuf = gsl::make_span(static_cast<GLfloat*>(rawBuffer), elementsAmount);

        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(&texCoBuf[2 * i], vertices[i].textureCoordinates.c_array(), 2 * sizeof(GLfloat));
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

std::string ms::Geometry::get_class () const {
    return "ms::Geometry";
}

void ms::Geometry::_unload() {
    mglDeleteBuffers(1, &bitangents);
    mglDeleteBuffers(1, &tangents);
    mglDeleteBuffers(1, &positionsBuffer);
    mglDeleteBuffers(1, &normalsBuffer);
    mglDeleteBuffers(1, &texturesCooridnatesBuffer);
    mglDeleteBuffers(1, &indiciesBuffer);
}
