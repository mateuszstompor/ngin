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
    mglBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
}

void ms::Geometry::use_bitangents () {
    load();
    mglBindBuffer(GL_ARRAY_BUFFER, bitangentsBuffer);
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
        utils::map_buffer_range_for_writing<GLfloat>(GL_ARRAY_BUFFER, positionsBuffer, 3 * vertices.size(), GL_STATIC_DRAW, [&](auto vBuf){
            for (unsigned long long i = 0; i < vertices.size(); ++i) {
                std::memcpy(&vBuf[3 * i], vertices[i].position.c_array(), 3 * sizeof(GLfloat));
            }
        });
    }
    
    {
        mglGenBuffers(1, &normalsBuffer);
        utils::map_buffer_range_for_writing<GLfloat>(GL_ARRAY_BUFFER, normalsBuffer, 3 * vertices.size(), GL_STATIC_DRAW, [&](auto nBuf){
            for (unsigned long long i = 0; i < vertices.size(); ++i) {
                std::memcpy(&nBuf[3 * i], vertices[i].normal.c_array(), 3 * sizeof(GLfloat));
            }
        });
    }
    
    {
        mglGenBuffers(1, &tangentsBuffer);
        utils::map_buffer_range_for_writing<GLfloat>(GL_ARRAY_BUFFER, tangentsBuffer, 3 * vertices.size(), GL_STATIC_DRAW, [&](auto tBuf){
            for (unsigned long long i = 0; i < vertices.size(); ++i) {
                std::memcpy(&tBuf[3 * i], vertices[i].tangent.c_array(), 3 * sizeof(GLfloat));
            }
        });
    }
    
    {
        mglGenBuffers(1, &bitangentsBuffer);
        utils::map_buffer_range_for_writing<GLfloat>(GL_ARRAY_BUFFER, bitangentsBuffer, 3 * vertices.size(), GL_STATIC_DRAW, [&](auto bitBuf){
            for (unsigned long long i = 0; i < vertices.size(); ++i) {
                std::memcpy(&bitBuf[3 * i], vertices[i].bitangent.c_array(), 3 * sizeof(GLfloat));
            }
        });
    }
    
    {
        mglGenBuffers(1, &texturesCooridnatesBuffer);
        utils::map_buffer_range_for_writing<GLfloat>(GL_ARRAY_BUFFER, texturesCooridnatesBuffer, 2 * vertices.size(), GL_STATIC_DRAW, [&](auto texCoBuf){
            for (unsigned long long i = 0; i < vertices.size(); ++i) {
                std::memcpy(&texCoBuf[2 * i], vertices[i].textureCoordinates.c_array(), 2 * sizeof(GLfloat));
            }
        });
    }
    amountOfVertices = static_cast<int>(vertices.size());
    vertices.clear();
    {
        mglGenBuffers(1, &indiciesBuffer);
        mglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesBuffer);
        mglBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        mglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    amountOfIndices = static_cast<int>(indices.size());
    indices.clear();
}

std::string ms::Geometry::get_class () const {
    return "ms::Geometry";
}

void ms::Geometry::_unload() {
    indices.resize(amountOfIndices);
    vertices.resize(amountOfVertices);
    
    utils::map_buffer_range_for_read<GLfloat>(GL_ARRAY_BUFFER, texturesCooridnatesBuffer, 2 * vertices.size(), [&](auto tBuf) {
        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(vertices[i].textureCoordinates.c_array(), &tBuf[2 * i], 2 * sizeof(GLfloat));
        }
    });
    
    utils::map_buffer_range_for_read<GLfloat>(GL_ARRAY_BUFFER, positionsBuffer, 3 * vertices.size(), [&](auto vBuf) {
        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(vertices[i].position.c_array(), &vBuf[3 * i], 3 * sizeof(GLfloat));
        }
    });
    
    utils::map_buffer_range_for_read<GLfloat>(GL_ARRAY_BUFFER, normalsBuffer, 3 * vertices.size(), [&](auto nBuf) {
        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(vertices[i].normal.c_array(), &nBuf[3 * i], 3 * sizeof(GLfloat));
        }
    });

    utils::map_buffer_range_for_read<GLfloat>(GL_ARRAY_BUFFER, tangentsBuffer, 3 * vertices.size(), [&](auto tBuf) {
        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(vertices[i].tangent.c_array(), &tBuf[3 * i], 3 * sizeof(GLfloat));
        }
    });
    
    utils::map_buffer_range_for_read<GLfloat>(GL_ARRAY_BUFFER, bitangentsBuffer, 3 * vertices.size(), [&](auto bitBuf) {
        for (unsigned long long i = 0; i < vertices.size(); ++i) {
            std::memcpy(vertices[i].bitangent.c_array(), &bitBuf[3 * i], 3 * sizeof(GLfloat));
        }
    });
    
    utils::map_buffer_range_for_read<GLuint>(GL_ELEMENT_ARRAY_BUFFER, indiciesBuffer, amountOfIndices, [&](auto indicesBuf) {
        std::memcpy(&indices[0], indicesBuf, amountOfIndices * sizeof(GLuint));
    });
        
    mglDeleteBuffers(1, &bitangentsBuffer);
    mglDeleteBuffers(1, &tangentsBuffer);
    mglDeleteBuffers(1, &positionsBuffer);
    mglDeleteBuffers(1, &normalsBuffer);
    mglDeleteBuffers(1, &texturesCooridnatesBuffer);
    mglDeleteBuffers(1, &indiciesBuffer);
}
