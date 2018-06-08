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

	/**
     * Baza dla wszystkich klas posiadających zasoby zarówno w buforze karty graficznej jak i głównej pamięci RAM komputera.
     * Zapewnia wspólny interfejs dla pochodnych, w którym następuje komunikacja z API OpenGL.
     * Jej funkcjonalność, choć stanowi część wewnętrznych funkcjonalności biblioteki, nie jest dostępna dla końcowego użytkownika korzystającego z biblioteki.
     */
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

    public:

        /**
         * Metoda służąca do sprawdzenia jaka klasa specyficzna jest wykorzystana.
         * @return zwraca nazwę
         */
        virtual std::string 	get_class		() const = 0;
		/**
         * Informuje czy dany zasób jest załadowany do karty graficznej
         */
        virtual bool    		is_loaded   	() const final;

	protected:
        
		 						Resource		();
		virtual void    		load  			() final;
		virtual void 			unload 			() final;
        virtual void            _load           () = 0;
        virtual void            _unload         () = 0;
		virtual         		~Resource   	();
	
    private:
        
				bool			isLoaded;
    };
    
}
