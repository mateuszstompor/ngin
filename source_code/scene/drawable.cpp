//
//  drawable.cpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#include "drawable.hpp"

ms::Drawable::Drawable (std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material) :
transformation {math::mat4::identity()},
boundedGeometry{geometry},
boundedMaterial{material},
invalidated{false},
isShaded{true} { }

//
//  Invalidation mechanism
//
//  Object can be invalidated only when is loaded to graphics
//  Object unload make object valid
//  If object is going to be used and has already been loaded should be unloaded - which make it valid again and loaded once again
//

void ms::Drawable::use () {
    if(invalidated) {
        unload();
    }
    
	if(!is_loaded())
		load();
	
	mglBindVertexArray(vertexArray);
}

std::string ms::Drawable::get_class () const {
	return "ms::Drawable";
}

void ms::Drawable::draw () {
	use();
    if(boundedGeometry) {
        boundedGeometry->use_indicies();
        mglDrawElements(GL_TRIANGLES, boundedGeometry->amount_of_indices(), GL_UNSIGNED_INT, nullptr);
    }
}

std::unique_ptr<ms::Drawable> ms::Drawable::get_quad () {
	
    auto drawable = std::make_unique<Drawable>();
	
    std::vector <Vertex>                vertices;
    std::vector <unsigned int>          indices;
    std::string                         associatedMaterial;
    math::BoundingBox<float>            boundingBox{0,0,0,0,0,0};
    
	for(int i = 0; i < 4; ++i) {
		int positionOffset = 3 * i;
		int textureOffset = 2 * i;
		
		math::vec3 position = math::vec3{	gsl::at(quad_indicies::vertices, positionOffset),
											gsl::at(quad_indicies::vertices, positionOffset+1),
											gsl::at(quad_indicies::vertices, positionOffset+2)};
		
		math::vec2 textureCoordinate = math::vec2{gsl::at(quad_indicies::textureCoordinates, textureOffset),
                                                  gsl::at(quad_indicies::textureCoordinates, textureOffset+1)};
		
        math::vec3 normal{};
        math::vec3 tangent{};
        math::vec3 bitangent{};
        
        vertices.push_back(Vertex{std::move(position), std::move(normal), std::move(tangent), std::move(bitangent), std::move(textureCoordinate)});
	}
	
	for(int i = 0; i < 6; ++i) {
		indices.push_back(gsl::at(quad_indicies::indicies, i));
	}
    
	drawable->boundedGeometry = std::make_shared<Geometry>(std::move(vertices), std::move(indices), std::move(associatedMaterial), std::move(boundingBox));
	
	return drawable;
	
}

ms::Material * ms::Drawable::get_material () {
    return boundedMaterial.get();
}

ms::Geometry * ms::Drawable::get_geometry () {
    return boundedGeometry.get();
}

void ms::Drawable::bind_geometry (std::shared_ptr<Geometry> geometry) {
    boundedGeometry = geometry;
    if(is_loaded()) {
        invalidated = true;
    }
}

void ms::Drawable::bind_material (std::shared_ptr<Material> material) {
    boundedMaterial = material;
    if(is_loaded()) {
        invalidated = true;
    }
}

 bool ms::Drawable::can_be_drawn () const {
     return boundedGeometry != nullptr && boundedGeometry != nullptr;
 }

void ms::Drawable::set_shading (bool shouldBeShaded) {
    isShaded = shouldBeShaded;
}

void ms::Drawable::_load () {

	mglGenVertexArrays(1, &vertexArray);
	
	mglBindVertexArray(vertexArray);
	
	if(boundedGeometry) {
		
		boundedGeometry->use_vertices();
		mglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		mglEnableVertexAttribArray(0);
		
		boundedGeometry->use_normals();
		mglVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		mglEnableVertexAttribArray(1);
		
		boundedGeometry->use_texture_coord();
		mglVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
		mglEnableVertexAttribArray(2);
		
        boundedGeometry->use_tangents();
        mglVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        mglEnableVertexAttribArray(3);
        
        boundedGeometry->use_bitangents();
        mglVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        mglEnableVertexAttribArray(4);
        
	}
    
    mglBindVertexArray(0);

}

void ms::Drawable::_unload () {
	mglDeleteVertexArrays(1, &vertexArray);
    invalidated = false;
}
