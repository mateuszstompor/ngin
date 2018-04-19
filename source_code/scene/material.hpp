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
		
        
		 						    Material                (math::vec3 const & ambient,
                                                             math::vec3 const & diffuse,
                                                             math::vec3 const & specular,
                                                             float shininess,
                                                             float opacity,
                                                             std::string const & name);
                                    Material                (Material const &) = delete;
        Material &                  operator =              (Material const &) = delete;
        void				        use		                ();
		
		math::vec3 					diffuseColor;
		math::vec3 					ambientColor;
		math::vec3 					specularColor;
        std::string                 get_class() const override;
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
        
    private:
        
        virtual void                _load                   () override;
        virtual void                _unload                 () override;
        
        GLuint                  bufferId;
		
	};
	
}

#endif /* material_hpp */
