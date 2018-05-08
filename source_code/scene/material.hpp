//
//  material.hpp
//  ngin
//
//  Created by Mateusz Stompór on 06/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <string>
#include <memory>
#include "gsl/span"

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
        Material &                      operator =          	(Material const &) = delete;
        void				            use		                ();
        constexpr math::vec3 const &    get_ambient_color       () const { return ambientColor; }
        void                            set_ambient_color       (math::vec3 const & ambient);
        constexpr math::vec3 const &    get_diffuse_color       () const { return diffuseColor; }
        void                            set_diffuse_color       (math::vec3 const & diffuse);
        constexpr math::vec3 const &    get_specular_color      () const { return specularColor; }
        void                            set_specular_color      (math::vec3 const & specular);
        constexpr float                 get_shininess           () const { return shininess; }
        void                            set_shininess           (float shininess);
        constexpr float                 get_opacity             () const { return opacity; }
        void                            set_opacity             (float opacity);
        std::string const &             get_name                () const { return name; }
        void                            set_name                (std::string const & name);
        
        std::string                     get_class               () const override;
		
        void                            bind_diffuse_texture    (std::shared_ptr<Texture2D> diffuse);
        void                            bind_specular_texture   (std::shared_ptr<Texture2D> specular);
        void                            bind_normal_texture     (std::shared_ptr<Texture2D> normal);
        void                            bind_height_texture     (std::shared_ptr<Texture2D> height);
		
        Texture2D *                     get_diffuse_texture     ();
        Texture2D *                     get_specular_texture    ();
        Texture2D *                     get_normal_texture      ();
        Texture2D *                     get_height_texture      ();
        
		std::vector <std::string> 	    diffuseTexturesNames;
		std::vector <std::string> 	    specularTexturesNames;
		std::vector <std::string> 	    normalTexturesNames;
		std::vector <std::string> 	    heightTexturesNames;
        
        
        
    private:
        virtual void                    _load                   () override;
        virtual void                    _unload                 () override;
        
        bool                            invalidated;
        
        GLuint                          bufferId;
        math::vec3                      diffuseColor;
        math::vec3                      ambientColor;
        math::vec3                      specularColor;
        std::string                     name;
        float                           shininess;
        float                           opacity;
        
        std::shared_ptr<Texture2D>      boundedDiffuseTexture;
        std::shared_ptr<Texture2D>      boundedSpecularTexture;
        std::shared_ptr<Texture2D>      boundedNormalTexture;
        std::shared_ptr<Texture2D>      boundedHeightTexture;
	};
	
}
