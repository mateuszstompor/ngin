//
//  material.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#ifndef material_hpp
#define material_hpp

#include <string>
#include <memory>

#include "texture.hpp"
#include "../resources/resource.hpp"

namespace ms {
	
	class Material : public Resource {
	public:
		
		inline 						Material(math::vec3 ambient,
											 math::vec3 diffuse,
											 math::vec3 specular,
											 float shininess,
											 float opacity,
											 std::string name);
		
		virtual	void				use		() = 0;
		
		
		
		math::vec3 					diffuseColor;
		math::vec3 					ambientColor;
		math::vec3 					specularColor;
		
		float 						shininess;
		float						opacity;
		
		std::string					name;
		
		std::vector <std::string> 	diffuseTexturesNames;
		std::vector <std::string> 	specularTexturesNames;
		std::vector <std::string> 	normalTexturesNames;
		std::vector <std::string> 	heightTexturesNames;
		
		std::weak_ptr<Texture> 		boundedDiffuseTexture;
		std::weak_ptr<Texture> 		boundedSpecularTexture;
		std::weak_ptr<Texture> 		boundedNormalTexture;
		std::weak_ptr<Texture> 		boundedHeightTexture;
		
	};
	
}

ms::Material::Material(math::vec3 ambient, math::vec3 diffuse, math::vec3 specular, float shin, float opac, std::string nam) :
	ambientColor(ambient),
	diffuseColor(diffuse),
	specularColor(specular),
	shininess(shin),
	opacity(opac),
	name(nam) { }

#endif /* material_hpp */
