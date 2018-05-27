//
//  resource.h
//  ngin
//
//  Created by Mateusz Stompór on 02/03/2018.
//  Copyright © 2018 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <iostream>
#include <string>

#include "glMath.h"

namespace ms {

	class ResourceCoordinator;

    class Resource {

        friend class ResourceCoordinator;
        friend class DeferredRender;
        friend class NGin;
        friend class LightSourcesRender;
        friend class PostprocessRender;
        friend class Render;
        friend class ForwardRender;
        friend class Renderbuffer;
        friend class Framebuffer;
        
	protected:
        
		 						Resource		();
		virtual void    		load  			() final;
		virtual void 			unload 			() final;
		virtual bool    		is_loaded   	() const final;
        virtual void            _load            () = 0;
        virtual void            _unload            () = 0;
		virtual std::string 	get_class		() const = 0;
		virtual         		~Resource   	();
	
    private:
        
				bool			isLoaded;
    };
    
}
