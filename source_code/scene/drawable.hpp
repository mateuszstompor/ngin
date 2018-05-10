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
		
        friend class Render;
        friend class NGin;
        friend class LightSourcesRender;
        friend class DLShadowRender;
        friend class ForwardRender;
        friend class DeferredRender;
        friend class PostprocessDrawer;
        
                                                Drawable                (std::shared_ptr<Geometry> geometry = nullptr, std::shared_ptr<Material> material = nullptr);
                                                Drawable                (Drawable const &) = delete;

												~Drawable               () = default;
        
        Drawable &                              operator =              (Drawable const &) = delete;
        
        std::string 	                        get_class	            () const override;
        
        constexpr math::mat4 const &            get_transformation      () const { return transformation; }
        constexpr math::mat4 &                  get_transformation      () { return transformation; }
        void                                    bind_geometry           (std::shared_ptr<Geometry> geometry);
        void                                    bind_material           (std::shared_ptr<Material> material);
        Material *                              get_material            ();
        Geometry *                              get_geometry            ();
        void                                    set_shading             (bool shouldBeShaded);
        constexpr bool                          is_shaded               () const { return isShaded; }
        
    private:
        bool                                    can_be_drawn            () const;
                void                            _load                   () override;
                void                            _unload                 () override;
        virtual void                            use                     ();
        virtual void                            draw                    ();
        static  std::unique_ptr<Drawable>       get_quad                ();
        bool                                    invalidated;
        std::shared_ptr<Geometry>               boundedGeometry;
        std::shared_ptr<Material>               boundedMaterial;
        bool                                    isShaded;
		math::mat4                              transformation;
        GLuint                                  vertexArray;
		
	};
	
}
