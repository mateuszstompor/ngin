//
//  drawable.hpp
//  ngin
//
//  Created by Mateusz Stompór on 04/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "../resources/resource.hpp"
#include "geometry.hpp"
#include "material.hpp"
#include "../utils/ogl/proxyOGL.hpp"
#include "../rendering/reusableGeometry.hpp"

namespace ms {
	
	class Drawable : public Resource {
		
	public:
		
		friend class Loader;
		
                                                Drawable            	();
                                                Drawable                (Drawable const &) = delete;
                                                ~Drawable               () = default;
        
        Drawable &                              operator =              (Drawable const &) = delete;
        
        std::string 	                        get_class	            () const override;
        virtual void			                draw            		();
        
        static  std::unique_ptr<Drawable>       get_quad                ();
        
        math::mat4                              transformation;
        
        std::shared_ptr<Geometry>               boundedGeometry;
        std::weak_ptr<Material>                 boundedMaterial;
        std::weak_ptr<Texture2D>                boundedDiffuseTexture;
        std::weak_ptr<Texture2D>                boundedSpecularTexture;
        std::weak_ptr<Texture2D>                boundedNormalTexture;
        std::weak_ptr<Texture2D>                boundedHeightTexture;
        
	private:
        
                void                            _load                   () override;
                void                            _unload                 () override;
        virtual void                            use                     ();
		GLuint                                  vertexArray;
		
	};
	
}
