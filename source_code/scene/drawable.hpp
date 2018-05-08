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
#include "../utils/proxyOGL.hpp"
#include "../rendering/reusableGeometry.hpp"

namespace ms {
	
	class Drawable : public Resource {
		
	public:
		
		friend class Loader;
		
                                                Drawable                (std::shared_ptr<Geometry> geometry = nullptr, std::shared_ptr<Material> material = nullptr);
                                                Drawable                (Drawable const &) = delete;
												Drawable                (Drawable &&) = delete;

												~Drawable               () = default;
        
        Drawable &                              operator =              (Drawable const &) = delete;
		Drawable &                              operator =              (Drawable &&) = delete;
        
        std::string 	                        get_class	            () const override;
        virtual void			                draw            		();
        
        static  std::unique_ptr<Drawable>       get_quad                ();
        constexpr math::mat4 const &            get_transformation      () const { return transformation; }
        constexpr math::mat4 &                  get_transformation      () { return transformation; }
        void                                    bind_geometry           (std::shared_ptr<Geometry> geometry);
        void                                    bind_material           (std::shared_ptr<Material> material);
        Material *                              get_material            ();
        Geometry *                              get_geometry            ();
        constexpr bool                          is_invalidated          () const { return invalidated; }
        
    private:
        
                void                            _load                   () override;
                void                            _unload                 () override;
        virtual void                            use                     ();
        bool                                    invalidated;
        std::shared_ptr<Geometry>               boundedGeometry;
        std::shared_ptr<Material>               boundedMaterial;
		math::mat4                              transformation;
        GLuint                                  vertexArray;
		
	};
	
}
